#include "Arduino.h"
#include "Incoming.h"

// Incoming data is in form of a {a:1:2:3:4}
// "a" is the actual command
// the rest are optional parameters accompanied with that command.

Incoming::Incoming()
{
    command = -1;
    // char parameter1[2];
    // char parameter2[2];
    // char parameter3[2];
    // char parameter4[2];
    _incoming_state = 0;
    _val_index = 1;
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
        // Serial.print("state: ");
        // Serial.println(_incoming_state);
        // Serial.println(sizeof(parameter1));
        switch (_incoming_state)
        {

        case 0: //reset
            // Serial.println("state = 0");
            command = -1;
            for (int i=0;i<sizeof(parameter1);++i){
                parameter1[i] = (char)0;
            }
            for (int i=0;i<sizeof(parameter2);++i){
                parameter2[i] = (char)0;
            }
            for (int i=0;i<sizeof(parameter3);++i){
                parameter3[i] = (char)0;
            }
            for (int i=0;i<sizeof(parameter4);++i){
                parameter4[i] = (char)0;
            }
            

            if (character == '{')
            {
                _incoming_state = 1;
                _val_index = 0;
            }
            break;

        case 1: //command collect
            // Serial.println("state = 1");
            if (character == ':')
            {
                _incoming_state = 2;
                _val_index = 0;
            }
            else
            {
                command = character;
            }
            break;

        case 2: //collect parameter 1
            if (character == ':')
            {
                _incoming_state = 3;
                _val_index = 0;
            }
            else
            {
                parameter1[_val_index] = character;
                _val_index += 1;
                //Serial.println(parameter1);
            }
            break;

        case 3: //collect parameter 2
            //Serial.println(parameter1);
            if (character == ':')
            {
                _incoming_state = 4;
                _val_index = 0;
            }
            else
            {
                parameter2[_val_index] = character;
                _val_index += 1;
            }
            break;

        case 4: //collect parameter 3

            parameter3[_val_index] = character;
            _val_index += 1;
            break;
        }
        // return false to indicate parsing is not ready.
        return false;
    }
}
