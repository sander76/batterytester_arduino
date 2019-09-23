/*
    Incoming.h - parsing incoming serial data
*/
#ifndef Incoming_h
#define Incoming_h

#include "Arduino.h"

class Incoming
{
    public:
        Incoming();
        
        boolean update(char character);
        char command;
        char parameter1[3];
        char parameter2[3];
        char parameter3[3];
        char parameter4[3];
    private:
        int _incoming_state;
        int _val_index;
};

#endif
