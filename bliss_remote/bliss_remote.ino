// This activates a digital pin for a certain amount of milliseconds.
// listen for incoming serial data in the form of: {a:2:10} or {a:2:500}
// activate pin 2 for 10 milliseconds.
// activate pin 2 for 500 milliseconds.
//

#include <Incoming.h>
#include <helpers.h>
#include <arduino-timer.h>

auto timer = timer_create_default();

const char identity[] = "bliss remote";
const char version_nr[] = "1.0";

Incoming incoming;
char received_char = -1;

const int nr_of_buttons = 8;
const int all_buttons[nr_of_buttons] = { 1, 2, 3, 4, 5, 6, 7, 8 };


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  // wait for serial communication to start.
  Serial.begin(115200);
  // while (!Serial)
  //   ;

  for (int i = 0; i < nr_of_buttons; i++)
  {

    pinMode(all_buttons[i], OUTPUT);
    digitalWrite(all_buttons[i], HIGH);
  }


  send_identity(identity, version_nr);
}


void loop()
{
  // check_durations();
  timer.tick();
  read_incoming();
}

int parse_int(char value[])
{
  // Convert slice of array [start_idx:_length] to int.
  //char digits[] = {value, 0};

  int val = atoi(value);
  return val;
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

bool turnoff(const int btn)
{

  Serial.print("Deactivate pin: ");
  Serial.println(btn);

  digitalWrite(btn, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  return false;
}

bool verify_pin(int pin_nr)
{

  for (int i = 0; i < nr_of_buttons; i++)
  {
    if (all_buttons[i] == pin_nr) {
      return true;
    }
  }
  return false;
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

void press_button(int pin_nr, int duration_ms)
{
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(pin_nr, LOW);
  timer.in(duration_ms, turnoff, pin_nr);
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
