#pragma once

#include <memory>
#include "rotator.h"
#include "enums.h"
#include "observable.h"
#include "microcontroller.h"
#include "cube.h"

class Solver : public Observable<Solver>
{
    public:
        virtual void solve(SolverStep step){}
    protected:
        Solver(int* rotationsNumber, std::shared_ptr<Cube> cube, std::shared_ptr<Microcontroller> microcontroller)
         : rotationsNumber{rotationsNumber}, cube{cube}, microcontroller{microcontroller} {}
        std::shared_ptr<Cube> cube;
        std::shared_ptr<Microcontroller> microcontroller;
        int* rotationsNumber;
};
