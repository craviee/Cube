#include "layerssolver.h"

#include <iostream>

LayersSolver::LayersSolver(int* rotationsNumber, std::shared_ptr<Cube> cube)
 : Solver{rotationsNumber, cube}{ std::cout << "rotationsNumber: " << rotationsNumber << std::endl; }
void LayersSolver::solve(SolverStep step)
{
    switch (step)
    {
        case SolverStep::CROSS: cross(); break;
        case SolverStep::ALL: break;
    }    
}

void LayersSolver::cross()
{
    front8Cross();
    front2Cross();
    front6Cross();
    front4Cross();
}

void LayersSolver::front8Cross()
{
    if(cube->squares["up8"].getColor() != Color::BLUE || cube->squares["front2"].getColor() != Color::WHITE)
    {
        if(cube->squares["front2"].getColor() == Color::BLUE && cube->squares["up8"].getColor() == Color::WHITE)
        {
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front6"].getColor() == Color::BLUE && cube->squares["right4"].getColor() == Color::WHITE)
        {
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front6"].getColor() == Color::WHITE && cube->squares["right4"].getColor() == Color::BLUE)
        {
            cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front4"].getColor() == Color::BLUE && cube->squares["left6"].getColor() == Color::WHITE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front4"].getColor() == Color::WHITE && cube->squares["left6"].getColor() == Color::BLUE)
        {
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front8"].getColor() == Color::WHITE && cube->squares["down2"].getColor() == Color::BLUE)
        {
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front8"].getColor() == Color::BLUE && cube->squares["down2"].getColor() == Color::WHITE)
        {
            cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right2"].getColor() == Color::WHITE && cube->squares["up6"].getColor() == Color::BLUE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right2"].getColor() == Color::BLUE && cube->squares["up6"].getColor() == Color::WHITE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left2"].getColor() == Color::WHITE && cube->squares["up4"].getColor() == Color::BLUE)
        {
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left2"].getColor() == Color::BLUE && cube->squares["up4"].getColor() == Color::WHITE)
        {
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right8"].getColor() == Color::WHITE && cube->squares["down6"].getColor() == Color::BLUE)
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right8"].getColor() == Color::BLUE && cube->squares["down6"].getColor() == Color::WHITE)
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left8"].getColor() == Color::WHITE && cube->squares["down4"].getColor() == Color::BLUE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left8"].getColor() == Color::BLUE && cube->squares["down4"].getColor() == Color::WHITE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back8"].getColor() == Color::WHITE && cube->squares["up2"].getColor() == Color::BLUE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back8"].getColor() == Color::BLUE && cube->squares["up2"].getColor() == Color::WHITE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back6"].getColor() == Color::WHITE && cube->squares["right6"].getColor() == Color::BLUE)
        {
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back6"].getColor() == Color::BLUE && cube->squares["right6"].getColor() == Color::WHITE)
        {
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back4"].getColor() == Color::WHITE && cube->squares["left4"].getColor() == Color::BLUE)
        {
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back4"].getColor() == Color::BLUE && cube->squares["left4"].getColor() == Color::WHITE)
        {
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back2"].getColor() == Color::WHITE && cube->squares["down8"].getColor() == Color::BLUE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back2"].getColor() == Color::BLUE && cube->squares["down8"].getColor() == Color::WHITE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
    }
}

void LayersSolver::front6Cross()
{
    if(cube->squares["up6"].getColor() != Color::BLUE || cube->squares["right2"].getColor() != Color::RED)
    {
        if(cube->squares["up6"].getColor() == Color::RED && cube->squares["right2"].getColor() == Color::BLUE)
        {
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right6"].getColor() == Color::BLUE && cube->squares["back6"].getColor() == Color::RED)
        {
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right6"].getColor() == Color::RED && cube->squares["back6"].getColor() == Color::BLUE)
        {
            cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right4"].getColor() == Color::BLUE && cube->squares["front6"].getColor() == Color::RED)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right4"].getColor() == Color::RED && cube->squares["front6"].getColor() == Color::BLUE)
        {
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right8"].getColor() == Color::RED && cube->squares["down6"].getColor() == Color::BLUE)
        {
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right8"].getColor() == Color::BLUE && cube->squares["down6"].getColor() == Color::RED)
        {
            cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back8"].getColor() == Color::RED && cube->squares["up2"].getColor() == Color::BLUE)
        {
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back8"].getColor() == Color::BLUE && cube->squares["up2"].getColor() == Color::RED)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front2"].getColor() == Color::RED && cube->squares["up8"].getColor() == Color::BLUE)
        {
            cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front2"].getColor() == Color::BLUE && cube->squares["up8"].getColor() == Color::RED)
        {
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back2"].getColor() == Color::RED && cube->squares["down8"].getColor() == Color::BLUE)
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back2"].getColor() == Color::BLUE && cube->squares["down8"].getColor() == Color::RED)
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front8"].getColor() == Color::RED && cube->squares["down2"].getColor() == Color::BLUE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front8"].getColor() == Color::BLUE && cube->squares["down2"].getColor() == Color::RED)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left2"].getColor() == Color::RED && cube->squares["up4"].getColor() == Color::BLUE)
        {
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left2"].getColor() == Color::BLUE && cube->squares["up4"].getColor() == Color::RED)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left4"].getColor() == Color::RED && cube->squares["back4"].getColor() == Color::BLUE)
        {
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left4"].getColor() == Color::BLUE && cube->squares["back4"].getColor() == Color::RED)
        {
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left6"].getColor() == Color::RED && cube->squares["front4"].getColor() == Color::BLUE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left6"].getColor() == Color::BLUE && cube->squares["front4"].getColor() == Color::RED)
        {
            cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left8"].getColor() == Color::RED && cube->squares["down4"].getColor() == Color::BLUE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left8"].getColor() == Color::BLUE && cube->squares["down4"].getColor() == Color::RED)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
    }
}

void LayersSolver::front4Cross()
{
    if(cube->squares["up4"].getColor() != Color::BLUE || cube->squares["left2"].getColor() != Color::RED)
    {
        if(cube->squares["left2"].getColor() == Color::BLUE && cube->squares["up4"].getColor() == Color::ORANGE)
        {
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left6"].getColor() == Color::BLUE && cube->squares["front4"].getColor() == Color::ORANGE)
        {
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left6"].getColor() == Color::ORANGE && cube->squares["front4"].getColor() == Color::BLUE)
        {
            cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left4"].getColor() == Color::BLUE && cube->squares["back4"].getColor() == Color::ORANGE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left4"].getColor() == Color::ORANGE && cube->squares["back4"].getColor() == Color::BLUE)
        {
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left8"].getColor() == Color::ORANGE && cube->squares["down4"].getColor() == Color::BLUE)
        {
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left8"].getColor() == Color::BLUE && cube->squares["down4"].getColor() == Color::ORANGE)
        {
            cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front2"].getColor() == Color::ORANGE && cube->squares["up8"].getColor() == Color::BLUE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front2"].getColor() == Color::BLUE && cube->squares["up8"].getColor() == Color::ORANGE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back8"].getColor() == Color::ORANGE && cube->squares["up2"].getColor() == Color::BLUE)
        {
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back8"].getColor() == Color::BLUE && cube->squares["up2"].getColor() == Color::ORANGE)
        {
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front8"].getColor() == Color::ORANGE && cube->squares["down2"].getColor() == Color::BLUE)
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front8"].getColor() == Color::BLUE && cube->squares["down2"].getColor() == Color::ORANGE)
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back2"].getColor() == Color::ORANGE && cube->squares["down8"].getColor() == Color::BLUE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back2"].getColor() == Color::BLUE && cube->squares["down8"].getColor() == Color::ORANGE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right2"].getColor() == Color::ORANGE && cube->squares["up6"].getColor() == Color::BLUE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right2"].getColor() == Color::BLUE && cube->squares["up6"].getColor() == Color::ORANGE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right4"].getColor() == Color::ORANGE && cube->squares["front6"].getColor() == Color::BLUE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right4"].getColor() == Color::BLUE && cube->squares["front6"].getColor() == Color::ORANGE)
        {
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right6"].getColor() == Color::ORANGE && cube->squares["back6"].getColor() == Color::BLUE)
        {
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right6"].getColor() == Color::BLUE && cube->squares["back6"].getColor() == Color::ORANGE)
        {
            cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right8"].getColor() == Color::ORANGE && cube->squares["down6"].getColor() == Color::BLUE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right8"].getColor() == Color::BLUE && cube->squares["down6"].getColor() == Color::ORANGE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
    }
}

void LayersSolver::front2Cross()
{
    //if(cube->squares["up2"].getColor() != Color::BLUE || cube->squares["back8"].getColor() != Color::YELLOW)
    //{
        if(cube->squares["up2"].getColor() == Color::YELLOW && cube->squares["back8"].getColor() == Color::BLUE)
        {
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back4"].getColor() == Color::BLUE && cube->squares["left4"].getColor() == Color::YELLOW)
        {
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back4"].getColor() == Color::YELLOW && cube->squares["left4"].getColor() == Color::BLUE)
        {
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1); 
        }
        else if(cube->squares["back6"].getColor() == Color::BLUE && cube->squares["right6"].getColor() == Color::YELLOW)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back6"].getColor() == Color::YELLOW && cube->squares["right6"].getColor() == Color::BLUE)
        {
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back2"].getColor() == Color::YELLOW && cube->squares["down8"].getColor() == Color::BLUE)
        {
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back2"].getColor() == Color::BLUE && cube->squares["down8"].getColor() == Color::YELLOW)
        {
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left2"].getColor() == Color::YELLOW && cube->squares["up4"].getColor() == Color::BLUE)
        {
            cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left2"].getColor() == Color::BLUE && cube->squares["up4"].getColor() == Color::YELLOW)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right2"].getColor() == Color::YELLOW && cube->squares["up6"].getColor() == Color::BLUE)
        {
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right2"].getColor() == Color::BLUE && cube->squares["up6"].getColor() == Color::YELLOW)
        {
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left8"].getColor() == Color::YELLOW && cube->squares["down4"].getColor() == Color::BLUE)
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["left8"].getColor() == Color::BLUE && cube->squares["down4"].getColor() == Color::YELLOW)
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right8"].getColor() == Color::YELLOW && cube->squares["down6"].getColor() == Color::BLUE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["right8"].getColor() == Color::BLUE && cube->squares["down6"].getColor() == Color::YELLOW)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front2"].getColor() == Color::YELLOW && cube->squares["up8"].getColor() == Color::BLUE)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front2"].getColor() == Color::BLUE && cube->squares["up8"].getColor() == Color::YELLOW)
        {
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front4"].getColor() == Color::YELLOW && cube->squares["left6"].getColor() == Color::BLUE)
        {
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front4"].getColor() == Color::BLUE && cube->squares["left6"].getColor() == Color::YELLOW)
        {
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front6"].getColor() == Color::YELLOW && cube->squares["right4"].getColor() == Color::BLUE)
        {
            cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front6"].getColor() == Color::BLUE && cube->squares["right4"].getColor() == Color::YELLOW)
        {
            cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front8"].getColor() == Color::YELLOW && cube->squares["down2"].getColor() == Color::BLUE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front8"].getColor() == Color::BLUE && cube->squares["down2"].getColor() == Color::YELLOW)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateUA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateU(); notify(*this, (*rotationsNumber)+1);
        }
    //}
}
