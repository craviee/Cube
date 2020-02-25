#pragma once

#include "enums.h"

class Microcontroller
{
    public:
        virtual void runCommand(Command command){}
        virtual ~Microcontroller(){}
        virtual bool isAvailable(){ return false; }
    protected:
        Microcontroller(){}
};
