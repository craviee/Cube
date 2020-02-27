#include "rotator.h"

void Rotator::rotateD()
{
    rotate("right7", "back3", "left7", "front7");
    rotate("right8", "back2", "left8", "front8");
    rotate("right9", "back1", "left9", "front9");
    rotateFace(Face::DOWN, Direction::CLOCKWISE);
}

void Rotator::rotateU()
{

    rotate("right1", "front1", "left1", "back9");
    rotate("right2", "front2", "left2", "back8");
    rotate("right3", "front3", "left3", "back7");
    rotateFace(Face::UP, Direction::CLOCKWISE);
}

void Rotator::rotateF()
{
    rotate("left9", "up7", "right1", "down3");
    rotate("left6", "up8", "right4", "down2");
    rotate("left3", "up9", "right7", "down1");
    rotateFace(Face::FRONT, Direction::CLOCKWISE);
}

void Rotator::rotateB()
{
    rotate("right3", "up1", "left7", "down9");
    rotate("right6", "up2", "left4", "down8");
    rotate("right9", "up3", "left1", "down7");
    rotateFace(Face::BACK, Direction::CLOCKWISE);
}

void Rotator::rotateL()
{
    rotate("up1", "front1", "down1", "back1");
    rotate("up4", "front4", "down4", "back4");
    rotate("up7", "front7", "down7", "back7");
    rotateFace(Face::LEFT, Direction::CLOCKWISE);
}

void Rotator::rotateR()
{
    rotate("down3", "front3", "up3", "back3");
    rotate("down6", "front6", "up6", "back6");
    rotate("down9", "front9", "up9", "back9");
    rotateFace(Face::RIGHT, Direction::CLOCKWISE);
}

void Rotator::rotateDA()
{
    rotate("front7", "left7", "back3", "right7");
    rotate("front8", "left8", "back2", "right8");
    rotate("front9", "left9", "back1", "right9");
    rotateFace(Face::DOWN, Direction::ANTI_CLOCKWISE);
}

void Rotator::rotateUA()
{
    rotate("front1", "right1", "back9", "left1");
    rotate("front2", "right2", "back8", "left2");
    rotate("front3", "right3", "back7", "left3");
    rotateFace(Face::UP, Direction::ANTI_CLOCKWISE);
}

void Rotator::rotateFA()
{
    rotate("up7", "left9", "down3", "right1");
    rotate("up8", "left6", "down2", "right4");
    rotate("up9", "left3", "down1", "right7");
    rotateFace(Face::FRONT, Direction::ANTI_CLOCKWISE);
}

void Rotator::rotateBA()
{
    rotate("up1", "right3", "down9", "left7");
    rotate("up2", "right6", "down8", "left4");
    rotate("up3", "right9", "down7", "left1");
    rotateFace(Face::BACK, Direction::ANTI_CLOCKWISE);
}

void Rotator::rotateLA()
{
    rotate("front1", "up1", "back1", "down1");
    rotate("front4", "up4", "back4", "down4");
    rotate("front7", "up7", "back7", "down7");
    rotateFace(Face::LEFT, Direction::ANTI_CLOCKWISE);
}

void Rotator::rotateRA()
{
    rotate("front3", "down3", "back3", "up3");
    rotate("front6", "down6", "back6", "up6");
    rotate("front9", "down9", "back9", "up9");
    rotateFace(Face::RIGHT, Direction::ANTI_CLOCKWISE);
}

void Rotator::randomRotations(int numberRotations)
{
    int rotation;

    for(int i = 0; i < numberRotations; i++)
    {
        rotation = QRandomGenerator::system()->bounded(0, 12);
        switch (rotation)
        {
            case 0: rotateU(); break;
            case 1: rotateUA(); break;
            case 2: rotateD(); break;
            case 3: rotateDA(); break;
            case 4: rotateF(); break;
            case 5: rotateFA(); break;
            case 6: rotateB(); break;
            case 7: rotateBA(); break;
            case 8: rotateL(); break;
            case 9: rotateLA(); break;
            case 10: rotateR(); break;
            case 11: rotateRA(); break;
        }
    }
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

void Rotator::rotateFace(Face face, Direction direction)
{
    Color auxRotationColor1,auxRotationColor2, auxRotationColor3,
    auxRotationColor4, auxRotationColor6, auxRotationColor7,
    auxRotationColor8, auxRotationColor9;
    std::string faceStr = Utils::face2str(face);

    auxRotationColor1 = squares[faceStr + "1"].getColor();
    auxRotationColor2 = squares[faceStr + "2"].getColor();
    auxRotationColor3 = squares[faceStr + "3"].getColor();
    auxRotationColor4 = squares[faceStr + "4"].getColor();
    // 5 is always fixed
    auxRotationColor6 = squares[faceStr + "6"].getColor();
    auxRotationColor7 = squares[faceStr + "7"].getColor();
    auxRotationColor8 = squares[faceStr + "8"].getColor();
    auxRotationColor9 = squares[faceStr + "9"].getColor();

    if(direction == Direction::CLOCKWISE)
    {
        squares[faceStr + "1"].setColor(auxRotationColor7);
        squares[faceStr + "2"].setColor(auxRotationColor4);
        squares[faceStr + "3"].setColor(auxRotationColor1);
        squares[faceStr + "4"].setColor(auxRotationColor8);
        squares[faceStr + "6"].setColor(auxRotationColor2);
        squares[faceStr + "7"].setColor(auxRotationColor9);
        squares[faceStr + "8"].setColor(auxRotationColor6);
        squares[faceStr + "9"].setColor(auxRotationColor3);
    }
    else if(direction == Direction::ANTI_CLOCKWISE)
    {
        squares[faceStr + "7"].setColor(auxRotationColor1);
        squares[faceStr + "4"].setColor(auxRotationColor2);
        squares[faceStr + "1"].setColor(auxRotationColor3);
        squares[faceStr + "8"].setColor(auxRotationColor4);
        squares[faceStr + "2"].setColor(auxRotationColor6);
        squares[faceStr + "9"].setColor(auxRotationColor7);
        squares[faceStr + "6"].setColor(auxRotationColor8);
        squares[faceStr + "3"].setColor(auxRotationColor9);
    }
}
