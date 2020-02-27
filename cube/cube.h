#pragma once

#include <map>
#include <string>
#include "rotator.h"
#include "square.h"
#include "utils.h"
#include "constants.h"

class Cube
{
    friend class LayersSolver;
    friend class OptimalSolver;

    public:
        Cube(std::map<std::string, Square> squares, std::shared_ptr<Rotator> rotator);
        void initialize();
        void randomize();
        bool isValid();
        bool isComplete();
        void rotateD();
        void rotateU();
        void rotateF();
        void rotateB();
        void rotateL();
        void rotateR();
        void rotateDA();
        void rotateUA();
        void rotateFA();
        void rotateBA();
        void rotateLA();
        void rotateRA();
    private:
        std::map<std::string, Square> squares;
        std::shared_ptr<Rotator> rotator;
};
