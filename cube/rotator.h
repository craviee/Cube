#pragma once

#include "square.h"
#include "face.h"
#include "direction.h"

class Rotator
{
    friend class Cube;

    private:
        Rotator(std::map<std::string, Square> squares) : squares{squares}{}
        void rotateDA();
        void rotateUA();
        void rotate(std::string square1, std::string square2, std::string square3, std::string square4);
        void rotateFace(Face face, Direction direction);
        std::string enum2str(Face face);
        std::map<std::string, Square> squares;
};
