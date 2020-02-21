#include "cube.h"

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

void Cube::randomize() { rotator.randomRotations(RANDOM_ROTATIONS); }

// The reason why repaint() is called is to trigger the whole UI to repaint itself
// causing the smoth sensation of rotation
void Cube::rotateD() { rotator.rotateD(); squares["front1"].repaint(); }
void Cube::rotateU() { rotator.rotateU(); squares["front1"].repaint(); }
void Cube::rotateF() { rotator.rotateF(); squares["front1"].repaint(); }
void Cube::rotateB() { rotator.rotateB(); squares["front1"].repaint(); }
void Cube::rotateL() { rotator.rotateL(); squares["front1"].repaint(); }
void Cube::rotateR() { rotator.rotateR(); squares["front1"].repaint(); }
void Cube::rotateDA() { rotator.rotateDA(); squares["front1"].repaint(); }
void Cube::rotateUA() { rotator.rotateUA(); squares["front1"].repaint(); }
void Cube::rotateFA() { rotator.rotateFA(); squares["front1"].repaint(); }
void Cube::rotateBA() { rotator.rotateBA(); squares["front1"].repaint(); }
void Cube::rotateLA() { rotator.rotateLA(); squares["front1"].repaint(); }
void Cube::rotateRA() { rotator.rotateRA(); squares["front1"].repaint(); }
