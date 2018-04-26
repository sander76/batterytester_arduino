#include <arduino.h>
#include <Incoming.h>

Incoming incoming;
bool command_ready = false;

char _char=-1;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }
}

void loop()
{
    read_incoming();
}

int bytes_read = 0;

void read_incoming()
{
    _char = Serial.read();
    if (_char > -1)
    {
        Serial.print("incoming: ");
        Serial.println(_char);
        command_ready = incoming.update(_char);
        if (command_ready){
            Serial.println("command ready");
            Serial.println(incoming.command);
            Serial.println(incoming.parameter1);
            Serial.println(incoming.parameter2);
            Serial.println(incoming.parameter3);
            Serial.println(incoming.parameter4);
            
        }
    }
}

