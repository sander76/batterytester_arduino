// This switches a relays on/off for a defined amount of time.

#include <arduino.h>
#include <Incoming.h>
#include <helpers.h>

const char identity[] = "Relay_actor";
const char version_nr[] = "1.1";

Incoming incoming;
char received_char = -1;

// listen for incoming serial data in the form of: {a:2:1} or {a:2:4}
// activate pin 2 for 1 second.
// activate pin 2 for 4 seconds

const int nr_of_pins = 4;                             // Number of pins where digital out is connected to.
const int out_pins[nr_of_pins] = {4, 5, 6, 7};        // Pin numbers.
unsigned long durations[nr_of_pins] = {0, 0, 0, 0};   // Durations of each pin.
unsigned long start_times[nr_of_pins] = {0, 0, 0, 0}; // Start times of each pin.

const int relay_on = HIGH; // For the relay to switch on it needs LOW in.
const int relay_off = LOW; // For relay to switch off it nees HIGH in.

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  // Blink(LED_BUILTIN,relay_switch);

  for (int i = 0; i < nr_of_pins; i++)
  {
    //Serial.print("Configuring pin: ");
    //Serial.println(out_pins[i]);
    // By default pins are HIGH. Relays are off.
    pinMode(out_pins[i], OUTPUT);
    digitalWrite(out_pins[i], relay_off);
  }

  Serial.begin(115200);
  while (!Serial)
    ;
  send_identity(identity, version_nr);
}

int parse_int(char value[])
{
  // Convert slice of array [start_idx:_length] to int.
  //char digits[] = {value, 0};

  int val = atoi(value);
  //Serial.println(val);
  return val;
}

int get_pin_index(int pin_nr)
{
  // Check to see whether the requested pin is actually in the allowed pin array.
  for (int i = 0; i < nr_of_pins; i++)
  {
    if (pin_nr == out_pins[i])
    {
      return i;
    }
  }
  return -1;
}

void activate()
{
  int pin_idx = get_pin_index(parse_int(incoming.parameter1));
  unsigned long int duration = 1000 * (parse_int(incoming.parameter2));
  Serial.print("Activate: ");
  Serial.println(pin_idx);
  Serial.print("Duration :");
  Serial.println(duration);

  digitalWrite(out_pins[pin_idx], relay_on);
  durations[pin_idx] = duration;
  start_times[pin_idx] = millis();
}

void turnoff(int pin_index)
{
  Serial.print("Deactivate: ");
  Serial.println(pin_index);
  digitalWrite(out_pins[pin_index], relay_off);
  durations[pin_index] = 0;
}

void check_durations()
{
  unsigned long current_millis = millis();
  for (int i = 0; i < nr_of_pins; i++)
  {
    if (durations[i] > 0)
    {
      if (current_millis - start_times[i] >= durations[i])
      {
        turnoff(i);
      }
    }
  }
}

void loop()
{
  check_durations();
  read_incoming();
}

bool command_finished = false;

void parse_string()
{
  switch (incoming.command)
  {
  case 'a': // Activate command
    //
    //unsigned long duration = (parse_int(incoming.parameter2));
    activate();

    break;
  case 'i': // Return version info.
    send_identity(identity, version_nr);
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
