#pragma once

#include "enums.h"
#include "observable.h"

class Solver : public Observable<Solver>
{
    public:
        virtual void solve(){}
        virtual void solve(SolverStep step){}
    protected:
        Solver(int& rotationsNumber) : rotationsNumber{rotationsNumber}{}
        int rotationsNumber;
};
