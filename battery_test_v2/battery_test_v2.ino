#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;

unsigned long PREVMILLIS = 0;
unsigned long MILLISDIF = 0;
const int SLOWINTERVAL = 60000;
const int FASTINTERVAL = 1000;
int INTERVAL = FASTINTERVAL;
const int inputPin = 4;
const int led = 13;
int val = HIGH;
int prev_val = HIGH;
float INA[] = {0.0, 0.0};
// 0 means low speed voltage testing. 1 means high speed voltage testing.
int volt_speed = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(inputPin, INPUT);
  pinMode(led, OUTPUT);

  //setup the serial port
  Serial.begin(115200);

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

void get_ina() {
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  INA[0] = loadvoltage;
  INA[1] = current_mA;
}

void loop() {

  // checking serial input
  while (Serial.available() > 0) {
    // reads input from the serial port and defines the volt read speed.
    volt_speed = Serial.parseInt();
    if (volt_speed == 1) {
      INTERVAL = FASTINTERVAL;
    }
    else {
      INTERVAL = SLOWINTERVAL;
    }
    if (Serial.read() == '\n') {
    }
  }


  // checking battery periodically.
  unsigned long currentMillis = millis();
  MILLISDIF = currentMillis - PREVMILLIS;
  if (MILLISDIF > INTERVAL) {
    PREVMILLIS = currentMillis;
    get_ina();
    Serial.print("v;");
    Serial.print(INA[0]);
    Serial.print(";");
    Serial.print(INA[1]);
    Serial.print("\n");
  }

  // check the ir sensor and print out the result.
  val = digitalRead(inputPin);
  if (val != prev_val) {
    digitalWrite(led, val);
    Serial.print("i;");
    if (val == HIGH) {
      Serial.print("0");
    }
    else {
      Serial.print("1");
    }
    Serial.print('\n');
    prev_val = val;
  }
}
