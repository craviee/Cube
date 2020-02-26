#include "cube.h"

Cube::Cube(std::map<std::string, Square> squares, std::shared_ptr<Rotator> rotator)
    : squares{squares}, rotator{rotator}{}

void Cube::initialize()
{
    for(int square = 1; square <= SQUARES_PER_FACE; square++)
    {
        squares["front" + std::to_string(square)].setColor(Color::WHITE);
        squares["back" + std::to_string(square)].setColor(Color::YELLOW);
        squares["up" + std::to_string(square)].setColor(Color::BLUE);
        squares["down" + std::to_string(square)].setColor(Color::GREEN);
        squares["left" + std::to_string(square)].setColor(Color::ORANGE);
        squares["right" + std::to_string(square)].setColor(Color::RED);
    }
}

void Cube::randomize() { rotator->randomRotations(RANDOM_ROTATIONS); }

bool Cube::isValid()
{
    std::map<Color, int> colorsNumber =
    {{Color::BLUE, 0},
    {Color::RED, 0},
    {Color::GREEN, 0},
    {Color::ORANGE, 0},
    {Color::WHITE, 0},
    {Color::YELLOW, 0}};

    std::vector<Face> faces = 
    { Face::BACK, Face::FRONT, Face::UP, Face::DOWN, Face::RIGHT, Face::LEFT };

    for(size_t faceIndex = 0; faceIndex < faces.size(); faceIndex++)
    {
        for(int square = 1; square <= SQUARES_PER_FACE; square++)
        {
            colorsNumber[squares[Utils::face2str(faces[faceIndex]) + std::to_string(square)].getColor()]++;
        }
    }
    for(auto it=colorsNumber.begin(); it!=colorsNumber.end(); ++it)
        if(it->second != 9)
            return false;
    return true;
}

bool Cube::isComplete()
{
    for(int square = 1; square <= SQUARES_PER_FACE; square++)
    {
        if (squares["front" + std::to_string(square)].getColor() != Color::WHITE) return false;
        if (squares["back" + std::to_string(square)].getColor() != Color::YELLOW) return false;
        if (squares["up" + std::to_string(square)].getColor() != Color::BLUE) return false;
        if (squares["down" + std::to_string(square)].getColor() != Color::GREEN) return false;
        if (squares["left" + std::to_string(square)].getColor() != Color::ORANGE) return false;
        if (squares["right" + std::to_string(square)].getColor() != Color::RED) return false;
    }
    return true;
}

// The reason why repaint() is called is to trigger the whole UI to repaint itself
// causing the smoth sensation of rotation
void Cube::rotateD() { rotator->rotateD(); squares["front1"].repaint(); }
void Cube::rotateU() { rotator->rotateU(); squares["front1"].repaint(); }
void Cube::rotateF() { rotator->rotateF(); squares["front1"].repaint(); }
void Cube::rotateB() { rotator->rotateB(); squares["front1"].repaint(); }
void Cube::rotateL() { rotator->rotateL(); squares["front1"].repaint(); }
void Cube::rotateR() { rotator->rotateR(); squares["front1"].repaint(); }
void Cube::rotateDA() { rotator->rotateDA(); squares["front1"].repaint(); }
void Cube::rotateUA() { rotator->rotateUA(); squares["front1"].repaint(); }
void Cube::rotateFA() { rotator->rotateFA(); squares["front1"].repaint(); }
void Cube::rotateBA() { rotator->rotateBA(); squares["front1"].repaint(); }
void Cube::rotateLA() { rotator->rotateLA(); squares["front1"].repaint(); }
void Cube::rotateRA() { rotator->rotateRA(); squares["front1"].repaint(); }
