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
        char parameter1[2];
        char parameter2[2];
        char parameter3[2];
        char parameter4[2];
    private:
        int _incoming_state;
};

#endif
