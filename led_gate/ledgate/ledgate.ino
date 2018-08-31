/*
  Breakbeam sensor code.
  Allows for 4 breakbeam sensors to be connected to digital pins 4 to 7.
  Output is [sensor_number]:[value]
  
  Return sensor value = {s:4:1} or {s:4:0}
  meaning: 
  type = (s)sensor
  name = 4
  value =1 (true/open) or 0 (false/closed)
*/

#include <arduino.h>
#include <Incoming.h>
#include <helpers.h>

// Pin 13: Arduino has an LED connected on pin 13
// Pin 11: Teensy 2.0 has the LED on pin 11
// Pin  6: Teensy++ 2.0 has the LED on pin 6
// Pin 13: Teensy 3.0 has the LED on pin 13

#define SENSOR_7_PIN 7
#define SENSOR_6_PIN 6
#define SENSOR_5_PIN 5
#define SENSOR_4_PIN 4

const char identity[] = "Led_gate";
const char version_nr[] = "1.0";

// variables wont change
const long led_interval = 200; // led blink duration [ms]

// variables will change:
int sensor_4_state = 0, last_4_state = 0;       // variable for reading the pushbutton status
int sensor_5_state = 0, last_5_state = 0;
int sensor_6_state = 0, last_6_state = 0;
int sensor_7_state = 0, last_7_state = 0;

int led_state = LOW;
unsigned long previousMillis = 0;

Incoming incoming;
char received_char = -1;
bool command_finished = false;

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  //Blink(LED_BUILTIN,led_gate);

  
  // initialize the sensor pins as an input:
  pinMode(SENSOR_7_PIN, INPUT);
  pinMode(SENSOR_6_PIN, INPUT);
  pinMode(SENSOR_5_PIN, INPUT);
  pinMode(SENSOR_4_PIN, INPUT);

  digitalWrite(SENSOR_7_PIN, HIGH); // turn on the pullup
  digitalWrite(SENSOR_6_PIN, HIGH); // turn on the pullup
  digitalWrite(SENSOR_5_PIN, HIGH); // turn on the pullup
  digitalWrite(SENSOR_4_PIN, HIGH); // turn on the pullup
  
  Serial.begin(115200);
  while(!Serial){
    ;
  }
  
  send_identity(identity,version_nr);
  // initialize the LED pin as an output:
  
}


// void send_identity(){
//   Serial.print("{i:n:");
//   Serial.print(identity);
//   Serial.print(":v:");
//   Serial.print(version_nr)
//   Serial.println("}");
// }

void enable_led() {
  previousMillis = millis();
  led_state = HIGH;
  digitalWrite(LED_BUILTIN, led_state);
}

void check_led() {
  if (led_state == HIGH) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= led_interval) {
      led_state = LOW;
      digitalWrite(LED_BUILTIN, led_state);
    }
  }
}

void state_changed(String pin_name, String pin_value) {
  Serial.print("{s:");
  Serial.print(pin_name);
  Serial.print(":");
  Serial.print(pin_value);
  Serial.print("}\n");
  enable_led();
}

int check_pin(String pin_name, int new_state, int old_state) {
  if (new_state && !old_state) {
    // unbroken
    state_changed(pin_name, "1");
  }
  if (!new_state && old_state) {
    // broken
    state_changed(pin_name, "0");
  }
  return new_state;
}

void loop() {
  // read the state of the pushbutton value:
  last_4_state = check_pin("4", digitalRead(SENSOR_4_PIN), last_4_state);
  last_5_state = check_pin("5", digitalRead(SENSOR_5_PIN), last_5_state);
  last_6_state = check_pin("6", digitalRead(SENSOR_6_PIN), last_6_state);
  last_7_state = check_pin("7", digitalRead(SENSOR_7_PIN), last_7_state);
  check_led();
  read_incoming();
}

void parse_string()
{
  switch (incoming.command)
  {
  case 'i': // Return version info.
    send_identity(identity,version_nr);
    break;
  }
}

void read_incoming()
{
  received_char = Serial.read();
  if (received_char > -1)
  {
    command_finished = incoming.update(received_char);
    if (command_finished)
    {
      parse_string();
    }
  }
}
