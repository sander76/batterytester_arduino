// This activates a digital pin for a certain amount of milliseconds.
// listen for incoming serial data in the form of: {a:2:10} or {a:2:500}
// activate pin 2 for 10 milliseconds.
// activate pin 2 for 500 milliseconds.
// 
// Special pin is D12. It is defined as the HomeMaticIP system button.


#include <Incoming.h>
#include <helpers.h>
#include <arduino-timer.h>

auto timer = timer_create_default();

const char identity[] = "hmip-remote";
const char version_nr[] = "1.0";

Incoming incoming;
char received_char = -1;



struct Button
{
  int button_nr;
  int arduino_pin;
};

const int nr_of_buttons = 3;
Button all_buttons[nr_of_buttons] = { {1,2},{2,3},{0,12} };


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  // wait for serial communication to start.
  Serial.begin(115200);
  // while (!Serial)
  //   ;

  for (int i = 0; i < nr_of_buttons; i++)
  {

    pinMode(all_buttons[i].arduino_pin, OUTPUT);
    digitalWrite(all_buttons[i].arduino_pin, LOW);
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

Button get_button(int button_nr)
// Return the button struct based on the button nr.
{
  for (int i = 0; i < nr_of_buttons; i++)
  {
    if (button_nr == all_buttons[i].button_nr)
    {
      return all_buttons[i];
    }
  }
}

void activate()
{
  Button btn = get_button(parse_int(incoming.parameter1));
  int duration = parse_int(incoming.parameter2);
  Serial.print("Activating button: ");
  Serial.print(btn.button_nr);
  Serial.print(" Arduino pin: ");
  Serial.println(btn.arduino_pin);

  press_button(btn, duration);

}

bool turnoff(const int btn)
{

  Serial.print("Deactivate pin: ");
  Serial.println(btn);

  digitalWrite(btn, LOW);
  digitalWrite(LED_BUILTIN, LOW);
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

void press_button(Button btn, int duration_ms)
{
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(btn.arduino_pin, HIGH);
  timer.in(duration_ms, turnoff, btn.arduino_pin);
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


