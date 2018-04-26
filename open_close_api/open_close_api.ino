#include <arduino.h>

// listen for incoming serial data in the form of: a:2:1 or a:2:4
// activate pin 2 for 1 second.
// activate pin 2 for 4 seconds
const int command_pos = 0; // index position in protocol for command type.
const int pin_nr_pos = 2; // index postion for pin nr.
const int duration_start_pos = 4; // index position for duration indication.

const int read_buffer = 9;
char _buffer[read_buffer]; // Buffer for incoming serial data.

const int nr_of_pins = 4; // Number of pins where digital out is connected to.
const int out_pins[nr_of_pins] = {4, 5, 6, 7}; // Pin numbers.
unsigned long durations[nr_of_pins] = {0, 0, 0, 0}; // Durations of each pin.
unsigned long start_times[nr_of_pins] = {0, 0, 0, 0}; // Start times of each pin.

const int relay_on = LOW; // For the relay to switch on it needs LOW in.
const int relay_off = HIGH; // For relay to switch off it nees HIGH in.

void setup()
{

  Serial.begin(115200);
  while(!Serial);
  for (int i = 0; i < nr_of_pins; i++)
  {
    //Serial.print("Configuring pin: ");
    //Serial.println(out_pins[i]);
    // By default pins are HIGH. Relays are off.
    pinMode(out_pins[i], OUTPUT);
    digitalWrite(out_pins[i], relay_off);
  }
}

int parse_int(char _buffer[], int start_idx, int _length)
{
  // Convert slice of array [start_idx:_length] to int.
  char digits[_length + 1];

  for (int i = 0; i < _length; i++)
  {
    digits[i] = _buffer[i + start_idx];
  }
  digits[_length] = 0;
  int val = atoi(digits);
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

void activate(int pin_idx, unsigned long duration)
{
  Serial.print("Activate: ");
  Serial.println(pin_idx);
  Serial.print("Duration :");
  Serial.println(duration);

  digitalWrite(out_pins[pin_idx], relay_on);
  durations[pin_idx] = duration;
  start_times[pin_idx] = millis();
}

void parse_string()
{
  Serial.print("Buffer: ");
  Serial.println(_buffer);
  char command = _buffer[command_pos];
  int pin_nr = parse_int(_buffer, pin_nr_pos, 1);
  
  // Duration in milliseconds.
  unsigned long duration = 1000 * (parse_int(_buffer, duration_start_pos, (strlen(_buffer) - duration_start_pos)));

  Serial.print("command: ");
  Serial.println(command);
  Serial.print("pin nr: ");
  Serial.println(pin_nr);
  Serial.print("duration [ms]: ");
  Serial.println(duration);

  if (command == 'a')
  {
    Serial.println("Command a detected <activate>");
    int pin_index = get_pin_index(pin_nr);
    if (pin_index > -1)
    {
      Serial.println("allowed pin nr.");
      activate(pin_index, duration);
    }
    else
    {
      Serial.println("Pin nr not allowed.");
    }
  }
}

void turnoff(int pin_index)
{
  digitalWrite(out_pins[pin_index], relay_off);
  durations[pin_index]=0;
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

int bytes_read = 0;

void read_incoming()
{
  if (Serial.available())
  {
    Serial.println("### Checking for serial input. ###");
    bytes_read = Serial.readBytesUntil('\n', _buffer, read_buffer);
    if (bytes_read > 0)
    {
      parse_string();
      memset(_buffer, 0, sizeof(_buffer));
    }
    bytes_read = 0;
  }
}
