#pragma once

#include "solver.h"

class LayersSolver : public Solver
{
   public:
       LayersSolver(int& rotationsNumber, std::shared_ptr<Cube> cube);
       void solve(SolverStep step = SolverStep::ALL) override;
    private:
        void cross();
        void front8Cross();
        void front2Cross();
        void front4Cross();
        void front6Cross();
};