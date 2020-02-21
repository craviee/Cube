#pragma once

#include <map>
#include <string>
#include "rotator.h"
#include "square.h"

#define SQUARES_PER_FACE 9

class Cube
{
    public:
        Cube(std::map<std::string, Square> squares) : squares{squares}, rotator{squares}{}
        void initialize();
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
        Rotator rotator;
};
