// Change outgoing to the new protocol.
// {s:v:2.34:a:0.33}
// s: sensor identifier
// v: voltage
// <float> voltage value
// a: amps
// <float> amps value.

#include <arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Incoming.h>
#include <helpers.h>

#define LEDPIN 13

Adafruit_INA219 ina219;

unsigned long PREVMILLIS = 0;
unsigned long MILLISDIF = 0;
const int SLOWINTERVAL = 60000;
const int FASTINTERVAL = 1000;
int INTERVAL = FASTINTERVAL;


// 0 means low speed voltage testing. 1 means high speed voltage testing.
int volt_speed = 0;

Incoming incoming;

const char identity[] = "Volts_Amps_sensor";
const char version_nr[] = "1.0";


void setup() {
  Serial.begin(115200);
  while(!Serial){
    ;
  }
  // put your setup code here, to run once:
  pinMode(LEDPIN, OUTPUT);

  send_identity(identity,version_nr);
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();
  //Serial.println("Measuring voltage and current with INA219 ...");
}

void loop() {
  check_volt_amps();
  read_incoming();
  }

void check_volt_amps(){
  // checking battery periodically.
  unsigned long currentMillis = millis();
  if (currentMillis - PREVMILLIS > INTERVAL) {
    PREVMILLIS = currentMillis;
    digitalWrite(LEDPIN, HIGH);
    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_mA = 0;
    float loadvoltage = 0;

    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    loadvoltage = busvoltage + (shuntvoltage / 1000);

    Serial.print("{s:v:");
    Serial.print(loadvoltage);
    Serial.print(":a:");
    Serial.print(current_mA);
    Serial.print("}\n");
    digitalWrite(LEDPIN,LOW);
  }
}

void parse_string(){
switch (incoming.command)
  {
  case 'i': // Return version info.
    send_identity(identity,version_nr);
    break;
  }
}

void read_incoming()
{
  char received_char = Serial.read();
  if (received_char > -1)
  {
    bool command_finished = incoming.update(received_char);
    if (command_finished)
    {
      parse_string();
    }
  }
}

