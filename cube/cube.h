#pragma once

#include <map>
#include <string>
#include "rotator.h"
#include "square.h"

class Cube
{
    public:
        Cube(std::map<std::string, Square> squares) : squares{squares}, rotator{squares}{}
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
