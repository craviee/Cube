#include "rotator.h"

void Rotator::rotateDA()
{
    rotate("front7", "left7", "back3", "right7");
    rotate("front8", "left8", "back2", "right8");
    rotate("front9", "left9", "back1", "right9");
    rotateFace(Face::DOWN, Direction::ANTI_CLOCKWISE);
}

void Rotator::rotate(std::string square1, std::string square2, std::string square3, std::string square4)
{
    Color auxRotationColor1,auxRotationColor2,
    auxRotationColor3, auxRotationColor4;

    auxRotationColor1 = squares[square1].getColor();
    auxRotationColor2 = squares[square2].getColor();
    auxRotationColor3 = squares[square3].getColor();
    auxRotationColor4 = squares[square4].getColor();
    squares[square2].setColor(auxRotationColor1);
    squares[square3].setColor(auxRotationColor2);
    squares[square4].setColor(auxRotationColor3);
    squares[square1].setColor(auxRotationColor4);
}

std::string Rotator::enum2str(Face face)
{
    switch (face)
    {
        case Face::UP: return "up";
        case Face::DOWN: return "down";
        case Face::FRONT: return "front";
        case Face::BACK: return "back";
        case Face::LEFT: return "left";
        case Face::RIGHT: return "right";
        default: return "error";
    }
}

void Rotator::rotateFace(Face face, Direction direction)
{
    Color auxRotationColor1,auxRotationColor2, auxRotationColor3,
    auxRotationColor4, auxRotationColor5, auxRotationColor6,
    auxRotationColor7, auxRotationColor8;
    std::string faceStr = enum2str(face);

    auxRotationColor1 = squares[faceStr + "1"].getColor();
    auxRotationColor2 = squares[faceStr + "2"].getColor();
    auxRotationColor3 = squares[faceStr + "3"].getColor();
    auxRotationColor4 = squares[faceStr + "4"].getColor();
    auxRotationColor5 = squares[faceStr + "6"].getColor();
    auxRotationColor6 = squares[faceStr + "7"].getColor();
    auxRotationColor7 = squares[faceStr + "8"].getColor();
    auxRotationColor8 = squares[faceStr + "9"].getColor();

    if(direction == Direction::CLOCKWISE)
    {

    }
    else if(direction == Direction::ANTI_CLOCKWISE)
    {
        squares[faceStr + "7"].setColor(auxRotationColor1);
        squares[faceStr + "4"].setColor(auxRotationColor2);
        squares[faceStr + "1"].setColor(auxRotationColor3);
        squares[faceStr + "8"].setColor(auxRotationColor4);
        squares[faceStr + "2"].setColor(auxRotationColor5);
        squares[faceStr + "9"].setColor(auxRotationColor6);
        squares[faceStr + "6"].setColor(auxRotationColor7);
        squares[faceStr + "3"].setColor(auxRotationColor8);
    }
}
