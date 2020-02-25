#pragma once

#include "solver.h"

class LayersSolver : public Solver
{
   public:
       LayersSolver(int& rotationsNumber);
       void solve() override;
       void solve(SolverStep step) override;
};
