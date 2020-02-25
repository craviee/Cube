#pragma once

class Microcontroller
{
    public:
        virtual void runCommand(const char* command){}
        virtual ~Microcontroller(){}
    protected:
        Microcontroller(){}
};
