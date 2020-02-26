#pragma once

#include <QRandomGenerator>

#include "square.h"
#include "enums.h"
#include "utils.h"

class Rotator
{
    public:
        Rotator(std::map<std::string, Square> squares) : squares{squares}{}
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
        void randomRotations(int numberRotations);
    private:
        void rotate(std::string square1, std::string square2, std::string square3, std::string square4);
        void rotateFace(Face face, Direction direction);
        std::map<std::string, Square> squares;
};
