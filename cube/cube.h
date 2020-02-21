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
    private:
        std::map<std::string, Square> squares;
        Rotator rotator;
};
