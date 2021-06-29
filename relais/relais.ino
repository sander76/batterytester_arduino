// This switches a relays on or off or activates it for a certain amount of time.
// To switch a relais on or off send data in the form of: {r:4:0} or {r:4:1}
// r = relais identity
// 2 = arduino pin number.
// 1 = on, 0 == off.
//
// To activate a pin_nr for a certain amount of time, send data in the form of: {a:2:10} or {a:2:500}
// activate pin 2 for 10 milliseconds.
// activate pin 2 for 500 milliseconds.
//

#include <arduino.h>
#include <Incoming.h>
#include <helpers.h>
#include <arduino-timer.h>

auto timer = timer_create_default();

const char identity[] = "Relay_on_off_switch";
const char version_nr[] = "2.0";

Incoming incoming;

char received_char = -1;
const int nr_of_pins = 8; // Number of pins where digital out is connected to.
const int out_pins[nr_of_pins] = { 1, 2, 3, 4, 5, 6, 7, 8 };

const int relay_on = HIGH; // For the relay to switch on it needs LOW in.
const int relay_off = LOW; // For relay to switch off it nees HIGH in.

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  // Blink(LED_BUILTIN,relay_switch);

  for (int i = 0; i < nr_of_pins; i++)
  {
    pinMode(out_pins[i], OUTPUT);
    digitalWrite(out_pins[i], relay_off);
  }

  Serial.begin(115200);
  while (!Serial)
    ;
  send_identity(identity, version_nr);
}

void loop()
{
  timer.tick();
  read_incoming();
}

int parse_int(char value[])
{
  int val = atoi(value);
  return val;
}

void relais_switch()
{ 

  int pin_nr = parse_int(incoming.parameter1);
  int on_off = parse_int(incoming.parameter2);
  Serial.print("pin nr ");
  Serial.print(pin_nr);
  Serial.print(" ");
  
  if (on_off == 1)
  {
    Serial.println("on");
    digitalWrite(pin_nr, relay_on);
  }
  else
  {
    Serial.println("off");
    digitalWrite(pin_nr, relay_off);
  }
}

void activate()
{
  int pin_nr = parse_int(incoming.parameter1);
  int duration = parse_int(incoming.parameter2);
  if (!verify_pin(pin_nr))
  {
    Serial.print("Pin nr not allowed: ");
    Serial.print(pin_nr);
    return;
  }
  Serial.print("Activating pin nr: ");
  Serial.print(pin_nr);
  Serial.print(" for: ");
  Serial.print(duration);
  Serial.println("ms.");

  press_button(pin_nr, duration);
}

void press_button(int pin_nr, int duration_ms)
{
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(pin_nr, HIGH);
  timer.in(duration_ms, turnoff, pin_nr);
}

bool turnoff(const int btn)
{

  Serial.print("Deactivate pin: ");
  Serial.println(btn);

  digitalWrite(btn, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  return false;
}

bool verify_pin(int pin_nr)
{

  for (int i = 0; i < nr_of_pins; i++)
  {
    if (out_pins[i] == pin_nr) {
      return true;
    }
  }
  return false;
}

void parse_string()
{
//  Serial.write("parsing string\n");
  switch (incoming.command)
  {
  case 'a': // activate command
    activate();
  case 'r': // relais on or off
    //
    //unsigned long duration = (parse_int(incoming.parameter2));
//    Serial.write("relais identity\n");
    relais_switch();

    break;
  case 'i': // Return version info.
    send_identity(identity, version_nr);
    break;
  }
}

bool command_finished = false;

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
