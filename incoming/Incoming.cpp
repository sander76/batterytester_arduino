#include "Arduino.h"
#include "Incoming.h"

Incoming::Incoming()
{
    char command = -1;
    char parameter1 = -1;
    char parameter2 = -1;
    char parameter3 = -1;
    char parameter4 = -1;
    int _incoming_state=0;
}

boolean Incoming::update(char character)
{
    if (character == '}')
    {
        //Serial.println("end of command received.");
        // end of incoming command.
        // interpret and execute command.
        _incoming_state = 0;
        return true;
    }
    else
    {
        switch (_incoming_state)
        {
        case 0: //reset
            // Serial.println("state = 0");
            command = -1;
            parameter1 = -1;
            parameter2 = -1;
            parameter3 = -1;
            if (character == '{')
            {
                _incoming_state = 1;
            }
            break;

        case 1: //command collect
            // Serial.println("state = 1");
            if (character == ':')
            {
                _incoming_state = 2;
            }
            else
            {
                command = character;
            }
            break;

        case 2: //collect parameter 1
            // Serial.println("state = 2");
            if (character == ':')
            {
                _incoming_state = 3;
            }
            else
            {
                parameter1 = character;
            }
            break;

        case 3: //collect parameter 2
            if (character == ':')
            {
                _incoming_state = 4;
            }
            else
            {
                parameter2 = character;
            }
            break;

        case 4: //collect parameter 3
            parameter3 = character;
            break;
        }
        // return false to indicate parsing is not ready.
        return false;
    }
}
