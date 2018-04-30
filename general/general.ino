#include <arduino.h>
#include <Incoming.h>

Incoming incoming;
bool command_ready = false;

char _char = -1;

const long interval = 1000; //interval between sensor commands sent.
unsigned long previous_millis=0;
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
    check_durations();
}

void send_sensor_data()
{
    Serial.print("{s:a:5}\n");
}

void check_durations()
{
    unsigned long current_millis = millis();

    if (current_millis - previous_millis >= interval)
    {
        previous_millis = current_millis;
        send_sensor_data();
    }
}



void read_incoming()
{
    _char = Serial.read();
    if (_char > -1)
    {
        Serial.print("incoming: ");
        Serial.println(_char);
        command_ready = incoming.update(_char);
        if (command_ready)
        {
            Serial.println("command ready");
            Serial.println(incoming.command);
            Serial.println(incoming.parameter1);
            Serial.println(incoming.parameter2);
            Serial.println(incoming.parameter3);
            Serial.println(incoming.parameter4);
        }
    }
}
