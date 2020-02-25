#pragma once

#include "microcontroller.h"

class Arduino : public Microcontroller
{
    public:
        Arduino();
        void runCommand(const char* command) override;
};
