#pragma once

#include <stdexcept>
#include "solver.h"

class LayersSolver : public Solver
{
    public:
        LayersSolver(int* rotationsNumber, std::shared_ptr<Cube> cube, std::shared_ptr<Microcontroller> microcontroller);
        void solve(SolverStep step) override;
    private:
        bool isSimulation;

        // Execute Microcontroller if needed and update the counter of rotations
        void handlePreRotation(Command command);

        void cross();
        void firstLayer();
        void secondLayer();
        void downCross();
        void downEdges();
        void placeDownCorners();
        void turnDownCorners();
        void completeSolve();
        void up8Cross();
        void up2Cross();
        void up4Cross();
        void up6Cross();
        void up9FirstLayer();
        void up7FirstLayer();
        void up3FirstLayer();
        void up1FirstLayer();
        void front9FirstLayer();
        void front7FirstLayer();
        void back3FirstLayer();
        void back1FirstLayer();
        void frontSecondLayer();
        void leftSecondLayer();
        void rightSecondLayer();
        void backSecondLayer();
        bool fixBackSecondLayer();
        bool fixLeftSecondLayer();
        bool fixRightSecondLayer();
        bool fixFrontSecondLayer();
        bool isCornerFrontDownRightPlaced();
        bool isCornerFrontDownLeftPlaced();
        bool isCornerBackDownRightPlaced();
        bool isCornerBackDownLeftPlaced();
};
