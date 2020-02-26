#include "layerssolver.h"

#include <iostream>

LayersSolver::LayersSolver(int* rotationsNumber, std::shared_ptr<Cube> cube)
 : Solver{rotationsNumber, cube}{ }

void LayersSolver::solve(SolverStep step)
{
    switch (step)
    {
        case SolverStep::CROSS: cross(); break;
        case SolverStep::FIRST_LAYER: firstLayer(); break;
        case SolverStep::SECOND_LAYER: secondLayer(); break;
        case SolverStep::DOWN_CROSS: downCross(); break;
        case SolverStep::BACK_EDGES: backEdges(); break;
        case SolverStep::ALL: break;
    }    
}

void LayersSolver::cross()
{
    up8Cross();
    up2Cross();
    up6Cross();
    up4Cross();
}

void LayersSolver::firstLayer()
{
    up9FirstLayer();
    up7FirstLayer();
    up3FirstLayer();
    up1FirstLayer();
    //Sometimes it is needed to execute twice
    //Does not affect if the first layer is already completed
    front9FirstLayer();
    front7FirstLayer();
    back3FirstLayer();
    back1FirstLayer();
}

void LayersSolver::secondLayer()
{
    initiateSecondLayer();
    rightSecondLayer();
    frontSecondLayer();
    backSecondLayer();
    leftSecondLayer();
    //Sometimes it is needed to execute twice
    //Does not affect if the first layer is already completed
    rightSecondLayer();
    frontSecondLayer();
    backSecondLayer();
    leftSecondLayer();
}

void LayersSolver::downCross()
{
    if(cube->squares["down2"].getColor() != Color::GREEN && cube->squares["down4"].getColor() != Color::GREEN &&
            cube->squares["down6"].getColor() != Color::GREEN && cube->squares["down8"].getColor() != Color::GREEN)
    {
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    } 
    if(cube->squares["down6"].getColor() != Color::GREEN && cube->squares["down8"].getColor() != Color::GREEN)
    {
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
    if((cube->squares["down2"].getColor() == Color::GREEN &&
        cube->squares["down8"].getColor() == Color::GREEN) &&
        cube->squares["down4"].getColor() != Color::GREEN &&
        cube->squares["down6"].getColor() != Color::GREEN)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
    }
    if(cube->squares["down2"].getColor() != Color::GREEN || cube->squares["down4"].getColor() != Color::GREEN ||
            cube->squares["down6"].getColor() != Color::GREEN || cube->squares["down8"].getColor() != Color::GREEN)
    {
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::backEdges()
{
    placeBackEdges();
    placeBackEdges();
    placeBackEdges();
}

void LayersSolver::up8Cross()
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

void LayersSolver::up6Cross()
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

void LayersSolver::up4Cross()
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

void LayersSolver::up2Cross()
{
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
}

void LayersSolver::up9FirstLayer()
{
    if(cube->squares["up9"].getColor() == Color::BLUE)
    {
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        if(cube->squares["front7"].getColor() == Color::WHITE) 
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front7"].getColor() == Color::ORANGE) 
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front7"].getColor() == Color::YELLOW) 
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        }
    }
    else if(cube->squares["front3"].getColor() == Color::BLUE)
    {
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        if(cube->squares["back3"].getColor() == Color::WHITE)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back3"].getColor() == Color::ORANGE)
        {
            cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back3"].getColor() == Color::RED)
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["back3"].getColor() == Color::YELLOW)  
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        }
    }
    else if(cube->squares["right1"].getColor() == Color::BLUE)
    {
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        if(cube->squares["front9"].getColor() == Color::WHITE) 
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateF(); notify(*this, (*rotationsNumber)+1);
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front9"].getColor() == Color::ORANGE) 
        {
            cube->rotateL(); notify(*this, (*rotationsNumber)+1);
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front9"].getColor() == Color::RED)
        {
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        }
        else if(cube->squares["front9"].getColor() == Color::YELLOW)
        {
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        }
    }
}

void LayersSolver::up7FirstLayer()
{
    if(cube->squares["up7"].getColor() == Color::WHITE && cube->squares["front1"].getColor() == Color::ORANGE && cube->squares["left3"].getColor() == Color::BLUE)
    {
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up7"].getColor() == Color::ORANGE && cube->squares["front1"].getColor() == Color::BLUE && cube->squares["left3"].getColor() == Color::WHITE)
    {
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up7"].getColor() == Color::BLUE && cube->squares["front1"].getColor() == Color::RED && cube->squares["left3"].getColor() == Color::WHITE)
    {
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up7"].getColor() == Color::RED && cube->squares["front1"].getColor() == Color::WHITE && cube->squares["left3"].getColor() == Color::BLUE)
    {
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up7"].getColor() == Color::WHITE && cube->squares["front1"].getColor() == Color::BLUE && cube->squares["left3"].getColor() == Color::RED)
    {
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up7"].getColor() == Color::BLUE && cube->squares["front1"].getColor() == Color::YELLOW && cube->squares["left3"].getColor() == Color::RED)
    {
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up7"].getColor() == Color::RED && cube->squares["front1"].getColor() == Color::BLUE && cube->squares["left3"].getColor() == Color::YELLOW)
    {
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up7"].getColor() == Color::YELLOW && cube->squares["front1"].getColor() == Color::RED && cube->squares["left3"].getColor() == Color::BLUE)
    {
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up7"].getColor() == Color::BLUE && cube->squares["front1"].getColor() == Color::ORANGE && cube->squares["left3"].getColor() == Color::YELLOW)
    {
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up7"].getColor() == Color::ORANGE && cube->squares["front1"].getColor() == Color::YELLOW && cube->squares["left3"].getColor() == Color::BLUE)
    {
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up7"].getColor() == Color::YELLOW && cube->squares["front1"].getColor() == Color::BLUE && cube->squares["left3"].getColor() == Color::ORANGE)
    {
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::up3FirstLayer()
{
    if(cube->squares["up3"].getColor() == Color::RED && cube->squares["right3"].getColor() == Color::YELLOW && cube->squares["back9"].getColor() == Color::BLUE)
    {
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up3"].getColor() == Color::YELLOW && cube->squares["right3"].getColor() == Color::BLUE && cube->squares["back9"].getColor() == Color::RED)
    {
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up3"].getColor() == Color::BLUE && cube->squares["right3"].getColor() == Color::WHITE && cube->squares["back9"].getColor() == Color::RED)
    {
            cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateB(); notify(*this, (*rotationsNumber)+1);
            cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
            cube->rotateD(); notify(*this, (*rotationsNumber)+1);
            cube->rotateR(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up3"].getColor() == Color::RED && cube->squares["right3"].getColor() == Color::BLUE && cube->squares["back9"].getColor() == Color::WHITE)
    {
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up3"].getColor() == Color::WHITE && cube->squares["right3"].getColor() == Color::RED && cube->squares["back9"].getColor() == Color::BLUE)
    {
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up3"].getColor() == Color::BLUE && cube->squares["right3"].getColor() == Color::ORANGE && cube->squares["back9"].getColor() == Color::WHITE)
    {
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up3"].getColor() == Color::ORANGE && cube->squares["right3"].getColor() == Color::WHITE && cube->squares["back9"].getColor() == Color::BLUE)
    {
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up3"].getColor() == Color::WHITE && cube->squares["right3"].getColor() == Color::BLUE && cube->squares["back9"].getColor() == Color::ORANGE)
    {
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up3"].getColor() == Color::BLUE && cube->squares["right3"].getColor() == Color::YELLOW && cube->squares["back9"].getColor() == Color::ORANGE)
    {
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up3"].getColor() == Color::ORANGE && cube->squares["right3"].getColor() == Color::BLUE && cube->squares["back9"].getColor() == Color::YELLOW)
    {
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up3"].getColor() == Color::YELLOW && cube->squares["right3"].getColor() == Color::ORANGE && cube->squares["back9"].getColor() == Color::BLUE)
    {
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::up1FirstLayer()
{
    if(cube->squares["up1"].getColor() == Color::ORANGE && cube->squares["left1"].getColor() == Color::YELLOW && cube->squares["back7"].getColor() == Color::BLUE)
    {
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up1"].getColor() == Color::YELLOW && cube->squares["left1"].getColor() == Color::BLUE && cube->squares["back7"].getColor() == Color::ORANGE)
    {
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up1"].getColor() == Color::BLUE && cube->squares["left1"].getColor() == Color::RED && cube->squares["back7"].getColor() == Color::WHITE)
    {
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up1"].getColor() == Color::RED && cube->squares["left1"].getColor() == Color::WHITE && cube->squares["back7"].getColor() == Color::BLUE)
    {
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up1"].getColor() == Color::WHITE && cube->squares["left1"].getColor() == Color::BLUE && cube->squares["back7"].getColor() == Color::RED)
    {
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up1"].getColor() == Color::BLUE && cube->squares["left1"].getColor() == Color::WHITE && cube->squares["back7"].getColor() == Color::ORANGE)
    {
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up1"].getColor() == Color::WHITE && cube->squares["left1"].getColor() == Color::ORANGE && cube->squares["back7"].getColor() == Color::BLUE)
    {
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up1"].getColor() == Color::ORANGE && cube->squares["left1"].getColor() == Color::BLUE && cube->squares["back7"].getColor() == Color::WHITE)
    {
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up1"].getColor() == Color::BLUE && cube->squares["left1"].getColor() == Color::YELLOW && cube->squares["back7"].getColor() == Color::RED)
    {
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up1"].getColor() == Color::YELLOW && cube->squares["left1"].getColor() == Color::RED && cube->squares["back7"].getColor() == Color::BLUE)
    {
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["up1"].getColor() == Color::RED && cube->squares["left1"].getColor() == Color::BLUE && cube->squares["back7"].getColor() == Color::YELLOW)
    {
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::front9FirstLayer()
{
    if(cube->squares["front9"].getColor() == Color::WHITE && cube->squares["down3"].getColor() == Color::RED && cube->squares["right7"].getColor() == Color::BLUE)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front9"].getColor() == Color::BLUE && cube->squares["down3"].getColor() == Color::WHITE && cube->squares["right7"].getColor() == Color::RED)
    {
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front9"].getColor() == Color::RED && cube->squares["down3"].getColor() == Color::BLUE && cube->squares["right7"].getColor() == Color::WHITE)
    {
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front9"].getColor() == Color::WHITE && cube->squares["down3"].getColor() == Color::BLUE && cube->squares["right7"].getColor() == Color::ORANGE)
    {
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front9"].getColor() == Color::BLUE && cube->squares["down3"].getColor() == Color::ORANGE && cube->squares["right7"].getColor() == Color::WHITE)
    {
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front9"].getColor() == Color::ORANGE && cube->squares["down3"].getColor() == Color::WHITE && cube->squares["right7"].getColor() == Color::BLUE)
    {
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front9"].getColor() == Color::ORANGE && cube->squares["down3"].getColor() == Color::BLUE && cube->squares["right7"].getColor() == Color::YELLOW)
    {
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front9"].getColor() == Color::BLUE && cube->squares["down3"].getColor() == Color::YELLOW && cube->squares["right7"].getColor() == Color::ORANGE)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front9"].getColor() == Color::YELLOW && cube->squares["down3"].getColor() == Color::ORANGE && cube->squares["right7"].getColor() == Color::BLUE)
    {
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front9"].getColor() == Color::YELLOW && cube->squares["down3"].getColor() == Color::BLUE && cube->squares["right7"].getColor() == Color::RED)
    {
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front9"].getColor() == Color::BLUE && cube->squares["down3"].getColor() == Color::RED && cube->squares["right7"].getColor() == Color::YELLOW)
    {
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front9"].getColor() == Color::RED && cube->squares["down3"].getColor() == Color::YELLOW && cube->squares["right7"].getColor() == Color::BLUE)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::front7FirstLayer()
{
    if(cube->squares["front7"].getColor() == Color::WHITE && cube->squares["down1"].getColor() == Color::BLUE && cube->squares["left9"].getColor() == Color::RED)
    {
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front7"].getColor() == Color::BLUE && cube->squares["down1"].getColor() == Color::RED && cube->squares["left9"].getColor() == Color::WHITE)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front7"].getColor() == Color::RED && cube->squares["down1"].getColor() == Color::WHITE && cube->squares["left9"].getColor() == Color::BLUE)
    {
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front7"].getColor() == Color::BLUE && cube->squares["down1"].getColor() == Color::WHITE && cube->squares["left9"].getColor() == Color::ORANGE)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front7"].getColor() == Color::WHITE && cube->squares["down1"].getColor() == Color::ORANGE && cube->squares["left9"].getColor() == Color::BLUE)
    {
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front7"].getColor() == Color::ORANGE && cube->squares["down1"].getColor() == Color::BLUE && cube->squares["left9"].getColor() == Color::WHITE)
    {
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front7"].getColor() == Color::YELLOW && cube->squares["down1"].getColor() == Color::BLUE && cube->squares["left9"].getColor() == Color::ORANGE)
    {
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front7"].getColor() == Color::BLUE && cube->squares["down1"].getColor() == Color::ORANGE && cube->squares["left9"].getColor() == Color::YELLOW)
    {
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front7"].getColor() == Color::ORANGE && cube->squares["down1"].getColor() == Color::YELLOW && cube->squares["left9"].getColor() == Color::BLUE)
    {
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front7"].getColor() == Color::YELLOW && cube->squares["down1"].getColor() == Color::RED && cube->squares["left9"].getColor() == Color::BLUE)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front7"].getColor() == Color::RED && cube->squares["down1"].getColor() == Color::BLUE && cube->squares["left9"].getColor() == Color::YELLOW)
    {
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front7"].getColor() == Color::BLUE && cube->squares["down1"].getColor() == Color::YELLOW && cube->squares["left9"].getColor() == Color::RED)
    {
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::back3FirstLayer()
{
    if(cube->squares["back3"].getColor() == Color::WHITE && cube->squares["down9"].getColor() == Color::BLUE && cube->squares["right9"].getColor() == Color::RED)
    {
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back3"].getColor() == Color::BLUE && cube->squares["down9"].getColor() == Color::RED && cube->squares["right9"].getColor() == Color::WHITE)
    {
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back3"].getColor() == Color::RED && cube->squares["down9"].getColor() == Color::WHITE && cube->squares["right9"].getColor() == Color::BLUE)
    {
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back3"].getColor() == Color::BLUE && cube->squares["down9"].getColor() == Color::WHITE && cube->squares["right9"].getColor() == Color::ORANGE)
    {
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back3"].getColor() == Color::WHITE && cube->squares["down9"].getColor() == Color::ORANGE && cube->squares["right9"].getColor() == Color::BLUE)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back3"].getColor() == Color::ORANGE && cube->squares["down9"].getColor() == Color::BLUE && cube->squares["right9"].getColor() == Color::WHITE)
    {
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back3"].getColor() == Color::YELLOW && cube->squares["down9"].getColor() == Color::BLUE && cube->squares["right9"].getColor() == Color::ORANGE)
    {
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back3"].getColor() == Color::BLUE && cube->squares["down9"].getColor() == Color::ORANGE && cube->squares["right9"].getColor() == Color::YELLOW)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back3"].getColor() == Color::ORANGE && cube->squares["down9"].getColor() == Color::YELLOW && cube->squares["right9"].getColor() == Color::BLUE)
    {
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back3"].getColor() == Color::YELLOW && cube->squares["down9"].getColor() == Color::RED && cube->squares["right9"].getColor() == Color::BLUE)
    {
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back3"].getColor() == Color::RED && cube->squares["down9"].getColor() == Color::BLUE && cube->squares["right9"].getColor() == Color::YELLOW)
    {
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back3"].getColor() == Color::BLUE && cube->squares["down9"].getColor() == Color::YELLOW && cube->squares["right9"].getColor() == Color::RED)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::back1FirstLayer()
{
    if(cube->squares["back1"].getColor() == Color::WHITE && cube->squares["down7"].getColor() == Color::RED && cube->squares["left7"].getColor() == Color::BLUE)
    {
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back1"].getColor() == Color::BLUE && cube->squares["down7"].getColor() == Color::WHITE && cube->squares["left7"].getColor() == Color::RED)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back1"].getColor() == Color::RED && cube->squares["down7"].getColor() == Color::BLUE && cube->squares["left7"].getColor() == Color::WHITE)
    {
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back1"].getColor() == Color::WHITE && cube->squares["down7"].getColor() == Color::BLUE && cube->squares["left7"].getColor() == Color::ORANGE)
    {
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back1"].getColor() == Color::BLUE && cube->squares["down7"].getColor() == Color::ORANGE && cube->squares["left7"].getColor() == Color::WHITE)
    {
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back1"].getColor() == Color::ORANGE && cube->squares["down7"].getColor() == Color::WHITE && cube->squares["left7"].getColor() == Color::BLUE)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back1"].getColor() == Color::ORANGE && cube->squares["down7"].getColor() == Color::BLUE && cube->squares["left7"].getColor() == Color::YELLOW)
    {
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back1"].getColor() == Color::BLUE && cube->squares["down7"].getColor() == Color::YELLOW && cube->squares["left7"].getColor() == Color::ORANGE)
    {
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back1"].getColor() == Color::YELLOW && cube->squares["down7"].getColor() == Color::ORANGE && cube->squares["left7"].getColor() == Color::BLUE)
    {
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back1"].getColor() == Color::YELLOW && cube->squares["down7"].getColor() == Color::BLUE && cube->squares["left7"].getColor() == Color::RED)
    {
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back1"].getColor() == Color::BLUE && cube->squares["down7"].getColor() == Color::RED && cube->squares["left7"].getColor() == Color::YELLOW)
    {
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back1"].getColor() == Color::RED && cube->squares["down7"].getColor() == Color::YELLOW && cube->squares["left7"].getColor() == Color::BLUE)
    {
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::initiateSecondLayer()
{
    if(!cube->isComplete() && cube->squares["back2"].getColor() == Color::YELLOW && cube->squares["down8"].getColor() == Color::GREEN)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
    }
    else if(!cube->isComplete() && cube->squares["back2"].getColor() == Color::YELLOW && cube->squares["down8"].getColor() == Color::GREEN)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["right8"].getColor() == Color::YELLOW && cube->squares["down6"].getColor() == Color::GREEN)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["right8"].getColor() == Color::YELLOW && cube->squares["down6"].getColor() == Color::GREEN)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["left8"].getColor() == Color::YELLOW && cube->squares["down4"].getColor() == Color::GREEN)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["left8"].getColor() == Color::YELLOW && cube->squares["down4"].getColor() == Color::GREEN)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front8"].getColor() == Color::YELLOW && cube->squares["down2"].getColor() == Color::GREEN)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front8"].getColor() == Color::YELLOW && cube->squares["down2"].getColor() == Color::GREEN)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back6"].getColor() == Color::RED && cube->squares["right6"].getColor() == Color::YELLOW)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back4"].getColor() == Color::ORANGE && cube->squares["left4"].getColor() == Color::YELLOW)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(!cube->isComplete())
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::frontSecondLayer()
{
    if(cube->squares["front8"].getColor() == Color::WHITE && cube->squares["down2"].getColor() == Color::ORANGE)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front8"].getColor() == Color::WHITE && cube->squares["down2"].getColor() == Color::RED)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["left8"].getColor() == Color::WHITE && cube->squares["down4"].getColor() == Color::ORANGE)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["left8"].getColor() == Color::WHITE && cube->squares["down4"].getColor() == Color::RED)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["right8"].getColor() == Color::WHITE && cube->squares["down6"].getColor() == Color::ORANGE)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["right8"].getColor() == Color::WHITE && cube->squares["down6"].getColor() == Color::RED)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back2"].getColor() == Color::WHITE && cube->squares["down8"].getColor() == Color::ORANGE)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back2"].getColor() == Color::WHITE && cube->squares["down8"].getColor() == Color::RED)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::leftSecondLayer()
{
    if(cube->squares["left8"].getColor() == Color::ORANGE && cube->squares["down4"].getColor() == Color::YELLOW)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["left8"].getColor() == Color::ORANGE && cube->squares["down4"].getColor() == Color::WHITE)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back2"].getColor() == Color::ORANGE && cube->squares["down8"].getColor() == Color::YELLOW)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back2"].getColor() == Color::ORANGE && cube->squares["down8"].getColor() == Color::WHITE)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front8"].getColor() == Color::ORANGE && cube->squares["down2"].getColor() == Color::YELLOW)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front8"].getColor() == Color::ORANGE && cube->squares["down2"].getColor() == Color::WHITE)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["right8"].getColor() == Color::ORANGE && cube->squares["down6"].getColor() == Color::YELLOW)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["right8"].getColor() == Color::ORANGE && cube->squares["down6"].getColor() == Color::WHITE)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::rightSecondLayer()
{
    if(cube->squares["right8"].getColor() == Color::RED && cube->squares["down6"].getColor() == Color::WHITE)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["right8"].getColor() == Color::RED && cube->squares["down6"].getColor() == Color::YELLOW)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front8"].getColor() == Color::RED && cube->squares["down2"].getColor() == Color::WHITE)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front8"].getColor() == Color::RED && cube->squares["down2"].getColor() == Color::YELLOW)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back2"].getColor() == Color::RED && cube->squares["down8"].getColor() == Color::WHITE)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back2"].getColor() == Color::RED && cube->squares["down8"].getColor() == Color::YELLOW)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["left8"].getColor() == Color::RED && cube->squares["down4"].getColor() == Color::WHITE)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateF();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateFA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["left8"].getColor() == Color::RED && cube->squares["down4"].getColor() == Color::YELLOW)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::backSecondLayer()
{
    if(cube->squares["back2"].getColor() == Color::YELLOW && cube->squares["down8"].getColor() == Color::RED)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back2"].getColor() == Color::YELLOW && cube->squares["down8"].getColor() == Color::ORANGE)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["right8"].getColor() == Color::YELLOW && cube->squares["down6"].getColor() == Color::RED)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["right8"].getColor() == Color::YELLOW && cube->squares["down6"].getColor() == Color::ORANGE)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["left8"].getColor() == Color::YELLOW && cube->squares["down4"].getColor() == Color::RED)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["left8"].getColor() == Color::YELLOW && cube->squares["down4"].getColor() == Color::ORANGE)
    {
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front8"].getColor() == Color::YELLOW && cube->squares["down2"].getColor() == Color::RED)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateR();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateRA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front8"].getColor() == Color::YELLOW && cube->squares["down2"].getColor() == Color::ORANGE)
    {
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateLA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateL();  notify(*this, (*rotationsNumber)+1);
        cube->rotateD();  notify(*this, (*rotationsNumber)+1);
        cube->rotateB();  notify(*this, (*rotationsNumber)+1);
        cube->rotateDA();  notify(*this, (*rotationsNumber)+1);
        cube->rotateBA();  notify(*this, (*rotationsNumber)+1);
    }
}

void LayersSolver::placeBackEdges()
{
    if(cube->squares["front8"].getColor() != Color::WHITE && cube->squares["left8"].getColor() != Color::ORANGE)
    {
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateL(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateLA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back2"].getColor() != cube->squares["back2"].getColor() && cube->squares["left8"].getColor() != Color::ORANGE)
    {
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateB(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateBA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["back2"].getColor() != Color::YELLOW && cube->squares["right8"].getColor() != Color::RED)
    {
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateR(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateRA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
    }
    else if(cube->squares["front8"].getColor() != cube->squares["front2"].getColor() && cube->squares["right8"].getColor() != Color::RED)
    {
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateD(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateF(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateFA(); notify(*this, (*rotationsNumber)+1);
        cube->rotateDA(); notify(*this, (*rotationsNumber)+1);
    }
}