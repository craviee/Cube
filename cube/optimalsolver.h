#pragma once

#include <QString>
#include <QProcess>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include "solver.h"
#include "enums.h"

#define FORMATTER "../Rubiks-Cube-Solver/format.sh"
#define FORMATTER_SIZE 120
#define FORMATTER_OUTPUT "../Rubiks-Cube-Solver/format.txt"
#define SOLVER "../Rubiks-Cube-Solver/run.sh"
#define SOLVER_OUPUT "../Rubiks-Cube-Solver/result.txt"

class OptimalSolver : public Solver
{
    public:
        OptimalSolver(int* rotationsNumber, std::shared_ptr<Cube> cube, std::shared_ptr<Microcontroller> microcontroller);
        void solve(SolverStep step) override;
    private:

        // Execute Microcontroller if needed and update the counter of rotations
        void handlePreRotation(Command command);

        QString color2qstring(Color color);
        QString getFormatedQString();
        void processResult(std::string rotation);
        bool isSimulation;
};
