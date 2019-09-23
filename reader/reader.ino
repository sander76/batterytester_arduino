#include <arduino.h>
#include <Incoming.h>
#include <helpers.h>

char received_char = -1;
Incoming incoming;
bool command_finished = false;
void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
}

void loop()
{

    read_incoming();
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

void parse_string()
{
    Serial.println(incoming.command);
    Serial.print("parameter 1: ");
    Serial.println(incoming.parameter1);
    Serial.print("parameter 2: ");
    Serial.println(incoming.parameter2);
    Serial.print("parameter 3: ");
    Serial.println(incoming.parameter3);
}
