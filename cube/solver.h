#pragma once

#include <memory>
#include "rotator.h"
#include "enums.h"
#include "observable.h"
#include "cube.h"

class Solver : public Observable<Solver>
{
    public:
        virtual void solve(SolverStep step = SolverStep::ALL){}
    protected:
        Solver(int* rotationsNumber, std::shared_ptr<Cube> cube)
         : rotationsNumber{rotationsNumber}, cube{cube}{}
        std::shared_ptr<Cube> cube;
        int* rotationsNumber;
};
