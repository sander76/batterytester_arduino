#include <arduino.h>

char command = -1;
char parameter1 = -1;
char parameter2 = -1;
char parameter3 = -1;

char incoming;
int incoming_state = 0;

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
    incoming = Serial.read();
    if (incoming > -1)
    {
        Serial.print("incoming: ");
        Serial.println(incoming);
        state(incoming);
    }
}

void state(char incoming)
{
    if (incoming == '}')
    {
        Serial.println("end of command received.");
        execute();
        // end of incoming command.
        // interpret and execute command.
        incoming_state = 0;
    }
    else
    {
        switch (incoming_state)
        {
        case 0: //reset
            Serial.println("state = 0");
            command = -1;
            parameter1 = -1;
            parameter2 = -1;
            parameter3 = -1;
            if (incoming == '{')
            {
                incoming_state = 1;
            }
            break;

        case 1: //command collect
            Serial.println("state = 1");
            if (incoming == ':')
            {
                incoming_state = 2;
            }
            else
            {
                command = incoming;
            }
            break;

        case 2: //collect parameter 1
            Serial.println("state = 2");
            if (incoming == ':')
            {
                incoming_state = 3;
            }
            else
            {
                parameter1 = incoming;
            }
            break;

        case 3: //collect parameter 2
            if (incoming == ':')
            {
                incoming_state = 4;
            }
            else
            {
                parameter2 = incoming;
            }
            break;

        case 4: //collect parameter 3
            parameter3 = incoming;
            break;
        }
    }
}

void execute()
{
    Serial.println("executing command");
    Serial.println(command);
    Serial.println(parameter1);
    Serial.println(parameter2);
    Serial.println(parameter3);
}