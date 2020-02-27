#include "layerssolver.h"

LayersSolver::LayersSolver(int* rotationsNumber, std::shared_ptr<Cube> cube
    , std::shared_ptr<Microcontroller> microcontroller)
    : Solver{rotationsNumber, cube, microcontroller}{ }

void LayersSolver::solve(SolverStep step)
{
    switch (step)
    {
        case SolverStep::CROSS: isSimulation = true; cross(); break;
        case SolverStep::FIRST_LAYER: isSimulation = true; firstLayer(); break;
        case SolverStep::SECOND_LAYER: isSimulation = true; secondLayer(); break;
        case SolverStep::DOWN_CROSS: isSimulation = true; downCross(); break;
        case SolverStep::DOWN_EDGES: isSimulation = true; downEdges(); break;
        case SolverStep::PLACE_DOWN_CORNERS: isSimulation = true; placeDownCorners(); break;
        case SolverStep::TURN_DOWN_CORNERS: isSimulation = true; turnDownCorners(); break;
        case SolverStep::COMPLETE: isSimulation = true; completeSolve(); break;
        case SolverStep::CROSS_MICROCONTROLLER: isSimulation = false; cross(); break;
        case SolverStep::FIRST_LAYER_MICROCONTROLLER: isSimulation = false; firstLayer(); break;
        case SolverStep::SECOND_LAYER_MICROCONTROLLER: isSimulation = false; secondLayer(); break;
        case SolverStep::DOWN_CROSS_MICROCONTROLLER: isSimulation = false; downCross(); break;
        case SolverStep::DOWN_EDGES_MICROCONTROLLER: isSimulation = false; downEdges(); break;
        case SolverStep::PLACE_DOWN_CORNERS_MICROCONTROLLER: isSimulation = false; placeDownCorners(); break;
        case SolverStep::TURN_DOWN_CORNERS_MICROCONTROLLER: isSimulation = false; turnDownCorners(); break;
        case SolverStep::COMPLETE_MICROCONTROLLER: isSimulation = false; completeSolve(); break;
    }
}

void LayersSolver::handlePreRotation(Command command)
{
    if(!isSimulation)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(command);
        else
            throw std::error_condition();
    }
    notify(*this, (*rotationsNumber)+1);
}

void LayersSolver::cross()
{
    while(cube->squares["up2"].getColor() != Color::BLUE ||
        cube->squares["up4"].getColor() != Color::BLUE ||
        cube->squares["up6"].getColor() != Color::BLUE ||
        cube->squares["up8"].getColor() != Color::BLUE ||
        cube->squares["back8"].getColor() != Color::YELLOW ||
        cube->squares["left2"].getColor() != Color::ORANGE ||
        cube->squares["right2"].getColor() != Color::RED ||
        cube->squares["front2"].getColor() != Color::WHITE)
    {
        up8Cross();
        up2Cross();
        up6Cross();
        up4Cross();
    }
}

void LayersSolver::firstLayer()
{
    while(cube->squares["up1"].getColor() != Color::BLUE || cube->squares["up3"].getColor() != Color::BLUE ||
        cube->squares["up7"].getColor() != Color::BLUE || cube->squares["up9"].getColor() != Color::BLUE ||
        cube->squares["front1"].getColor() != Color::WHITE || cube->squares["front3"].getColor() != Color::WHITE ||
        cube->squares["left1"].getColor() != Color::ORANGE || cube->squares["left3"].getColor() != Color::ORANGE ||
        cube->squares["right1"].getColor() != Color::RED || cube->squares["right3"].getColor() != Color::RED ||
        cube->squares["back7"].getColor() != Color::YELLOW || cube->squares["back9"].getColor() != Color::YELLOW)
    {
        up3FirstLayer();
        up1FirstLayer();
        up9FirstLayer();
        up7FirstLayer();
        front9FirstLayer();
        front7FirstLayer();
        back3FirstLayer();
        back1FirstLayer();        
    }
}

void LayersSolver::secondLayer()
{
    int turn = 0;
    while(cube->squares["front4"].getColor() != Color::WHITE ||
        cube->squares["front6"].getColor() != Color::WHITE ||
        cube->squares["left4"].getColor() != Color::ORANGE ||
        cube->squares["left6"].getColor() != Color::ORANGE ||
        cube->squares["right4"].getColor() != Color::RED ||
        cube->squares["right6"].getColor() != Color::RED ||
        cube->squares["back4"].getColor() != Color::YELLOW ||
        cube->squares["back6"].getColor() != Color::YELLOW)
    {
        if(turn==0 && fixBackSecondLayer()){}
        else if(turn==1 && fixFrontSecondLayer()){}
        else if(turn==2 && fixRightSecondLayer()){}
        else if(turn==3 && fixLeftSecondLayer()){}
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
        turn = (turn+1)%4;
    }
}

void LayersSolver::downCross()
{
    while(cube->squares["down2"].getColor() != Color::GREEN ||
        cube->squares["down4"].getColor() != Color::GREEN ||
        cube->squares["down6"].getColor() != Color::GREEN ||
        cube->squares["down8"].getColor() != Color::GREEN)
    {
        if(cube->squares["down2"].getColor() != Color::GREEN &&
            cube->squares["down4"].getColor() != Color::GREEN &&
            cube->squares["down6"].getColor() != Color::GREEN &&
            cube->squares["down8"].getColor() != Color::GREEN)
        {
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        while((cube->squares["down2"].getColor() != Color::GREEN || cube->squares["down4"].getColor() != Color::GREEN) &&
            (cube->squares["down2"].getColor() != Color::GREEN || cube->squares["down8"].getColor() != Color::GREEN) &&
            (cube->squares["down4"].getColor() != Color::GREEN || cube->squares["down6"].getColor() != Color::GREEN))
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        }
        if(cube->squares["down6"].getColor() != Color::GREEN &&
            cube->squares["down8"].getColor() != Color::GREEN)
        {
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        
        if((cube->squares["down2"].getColor() == Color::GREEN &&
            cube->squares["down8"].getColor() == Color::GREEN) &&
            cube->squares["down4"].getColor() != Color::GREEN &&
            cube->squares["down6"].getColor() != Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        }
        if(cube->squares["down2"].getColor() != Color::GREEN ||
            cube->squares["down4"].getColor() != Color::GREEN ||
            cube->squares["down6"].getColor() != Color::GREEN ||
            cube->squares["down8"].getColor() != Color::GREEN)
        {
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
    }
}

void LayersSolver::downEdges()
{
    while(cube->squares["down2"].getColor() != Color::GREEN ||
        cube->squares["down4"].getColor() != Color::GREEN ||
        cube->squares["down6"].getColor() != Color::GREEN ||
        cube->squares["down8"].getColor() != Color::GREEN ||
        cube->squares["front8"].getColor() != Color::WHITE ||
        cube->squares["left8"].getColor() != Color::ORANGE ||
        cube->squares["right8"].getColor() != Color::RED ||
        cube->squares["back2"].getColor() != Color::YELLOW)
    {
        if(cube->squares["front8"].getColor() != Color::WHITE &&
            cube->squares["left8"].getColor() != Color::ORANGE &&
            cube->squares["right8"].getColor() != Color::RED &&
            cube->squares["back2"].getColor() != Color::YELLOW)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        }
        if(cube->squares["front8"].getColor() != Color::WHITE && cube->squares["left8"].getColor() != Color::ORANGE)
        {
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        }
        else if(cube->squares["back2"].getColor() != cube->squares["back2"].getColor() && cube->squares["left8"].getColor() != Color::ORANGE)
        {
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        }
        else if(cube->squares["back2"].getColor() != Color::YELLOW && cube->squares["right8"].getColor() != Color::RED)
        {
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        }
        else if(cube->squares["front8"].getColor() != cube->squares["front2"].getColor() && cube->squares["right8"].getColor() != Color::RED)
        {
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        }
        else
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        }
    }

}

void LayersSolver::placeDownCorners()
{
    while(!isCornerFrontDownRightPlaced() ||
        !isCornerFrontDownLeftPlaced() ||
        !isCornerBackDownRightPlaced() ||
        !isCornerBackDownLeftPlaced())
    {
        if(isCornerFrontDownLeftPlaced())
        {
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        }
        else if(isCornerFrontDownRightPlaced())
        {
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        }
        else if(isCornerBackDownLeftPlaced())
        {
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        }
        else if(isCornerBackDownRightPlaced())
        {
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        }
        else
        {
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        }
    }
}

void LayersSolver::turnDownCorners()
{
    while(!cube->isComplete())
    {
        if((cube->squares["front7"].getColor() != Color::WHITE || cube->squares["left9"].getColor() != Color::ORANGE || cube->squares["down1"].getColor() != Color::GREEN) &&
                (cube->squares["back1"].getColor() != Color::YELLOW || cube->squares["left7"].getColor() != Color::ORANGE || cube->squares["down7"].getColor() != cube->squares["down7"].getColor()))
        {
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if((cube->squares["back1"].getColor() != Color::YELLOW || cube->squares["left7"].getColor() != Color::ORANGE || cube->squares["down7"].getColor() != cube->squares["down7"].getColor()) &&
                (cube->squares["back3"].getColor() != Color::YELLOW || cube->squares["right9"].getColor() != Color::RED || cube->squares["down9"].getColor() != cube->squares["down7"].getColor()))
        {
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if((cube->squares["back3"].getColor() != Color::YELLOW || cube->squares["right9"].getColor() != Color::RED || cube->squares["down9"].getColor() != cube->squares["down7"].getColor()) &&
                (cube->squares["front9"].getColor() != Color::WHITE || cube->squares["right7"].getColor() != Color::RED || cube->squares["down3"].getColor() != cube->squares["down7"].getColor()))
        {
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if((cube->squares["front9"].getColor() != Color::WHITE || cube->squares["right7"].getColor() != Color::RED || cube->squares["down3"].getColor() != cube->squares["down7"].getColor()) &&
                (cube->squares["front7"].getColor() != Color::WHITE || cube->squares["left9"].getColor() != Color::ORANGE || cube->squares["down1"].getColor() != Color::GREEN))
        {
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["front7"].getColor() != Color::WHITE || cube->squares["left9"].getColor() != Color::ORANGE || cube->squares["down1"].getColor() != Color::GREEN)
        {
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["back1"].getColor() != Color::YELLOW || cube->squares["left7"].getColor() != Color::ORANGE || cube->squares["down7"].getColor() != cube->squares["down7"].getColor())
        {
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["back3"].getColor() != Color::YELLOW || cube->squares["right9"].getColor() != Color::RED || cube->squares["down9"].getColor() != cube->squares["down7"].getColor())
        {
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["front9"].getColor() != Color::WHITE || cube->squares["right7"].getColor() != Color::RED || cube->squares["down3"].getColor() != cube->squares["down7"].getColor())
        {
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
    }
}

void LayersSolver::completeSolve()
{
    cross();
    firstLayer();
    secondLayer();
    downCross();
    downEdges();
    placeDownCorners();
    turnDownCorners();
}

void LayersSolver::up8Cross()
{
    if(cube->squares["front2"].getColor() == Color::BLUE && cube->squares["up8"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front6"].getColor() == Color::BLUE && cube->squares["right4"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front6"].getColor() == Color::WHITE && cube->squares["right4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
    }
    else if(cube->squares["front4"].getColor() == Color::BLUE && cube->squares["left6"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
    }
    else if(cube->squares["front4"].getColor() == Color::WHITE && cube->squares["left6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
    }
    else if(cube->squares["front8"].getColor() == Color::WHITE && cube->squares["down2"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
    }
    else if(cube->squares["front8"].getColor() == Color::BLUE && cube->squares["down2"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["right2"].getColor() == Color::WHITE && cube->squares["up6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["right2"].getColor() == Color::BLUE && cube->squares["up6"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["left2"].getColor() == Color::WHITE && cube->squares["up4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
    }
    else if(cube->squares["left2"].getColor() == Color::BLUE && cube->squares["up4"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["right8"].getColor() == Color::WHITE && cube->squares["down6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
    }
    else if(cube->squares["right8"].getColor() == Color::BLUE && cube->squares["down6"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["left8"].getColor() == Color::WHITE && cube->squares["down4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
    }
    else if(cube->squares["left8"].getColor() == Color::BLUE && cube->squares["down4"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back8"].getColor() == Color::WHITE && cube->squares["up2"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back8"].getColor() == Color::BLUE && cube->squares["up2"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back6"].getColor() == Color::WHITE && cube->squares["right6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back6"].getColor() == Color::BLUE && cube->squares["right6"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back4"].getColor() == Color::WHITE && cube->squares["left4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back4"].getColor() == Color::BLUE && cube->squares["left4"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back2"].getColor() == Color::WHITE && cube->squares["down8"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
    }
    else if(cube->squares["back2"].getColor() == Color::BLUE && cube->squares["down8"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
}

void LayersSolver::up6Cross()
{
    if(cube->squares["up6"].getColor() == Color::RED && cube->squares["right2"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["right6"].getColor() == Color::BLUE && cube->squares["back6"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["right6"].getColor() == Color::RED && cube->squares["back6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
    }
    else if(cube->squares["right4"].getColor() == Color::BLUE && cube->squares["front6"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
    }
    else if(cube->squares["right4"].getColor() == Color::RED && cube->squares["front6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
    }
    else if(cube->squares["right8"].getColor() == Color::RED && cube->squares["down6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
    }
    else if(cube->squares["right8"].getColor() == Color::BLUE && cube->squares["down6"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back8"].getColor() == Color::RED && cube->squares["up2"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back8"].getColor() == Color::BLUE && cube->squares["up2"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front2"].getColor() == Color::RED && cube->squares["up8"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
    }
    else if(cube->squares["front2"].getColor() == Color::BLUE && cube->squares["up8"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
    }
    else if(cube->squares["back2"].getColor() == Color::RED && cube->squares["down8"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
    }
    else if(cube->squares["back2"].getColor() == Color::BLUE && cube->squares["down8"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front8"].getColor() == Color::RED && cube->squares["down2"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
    }
    else if(cube->squares["front8"].getColor() == Color::BLUE && cube->squares["down2"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["left2"].getColor() == Color::RED && cube->squares["up4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
    }
    else if(cube->squares["left2"].getColor() == Color::BLUE && cube->squares["up4"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["left4"].getColor() == Color::RED && cube->squares["back4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["left4"].getColor() == Color::BLUE && cube->squares["back4"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["left6"].getColor() == Color::RED && cube->squares["front4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
    }
    else if(cube->squares["left6"].getColor() == Color::BLUE && cube->squares["front4"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["left8"].getColor() == Color::RED && cube->squares["down4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
    }
    else if(cube->squares["left8"].getColor() == Color::BLUE && cube->squares["down4"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
}

void LayersSolver::up4Cross()
{
    if(cube->squares["left2"].getColor() == Color::BLUE && cube->squares["up4"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["left6"].getColor() == Color::BLUE && cube->squares["front4"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["left6"].getColor() == Color::ORANGE && cube->squares["front4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
    }
    else if(cube->squares["left4"].getColor() == Color::BLUE && cube->squares["back4"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
    }
    else if(cube->squares["left4"].getColor() == Color::ORANGE && cube->squares["back4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
    }
    else if(cube->squares["left8"].getColor() == Color::ORANGE && cube->squares["down4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
    }
    else if(cube->squares["left8"].getColor() == Color::BLUE && cube->squares["down4"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front2"].getColor() == Color::ORANGE && cube->squares["up8"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front2"].getColor() == Color::BLUE && cube->squares["up8"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back8"].getColor() == Color::ORANGE && cube->squares["up2"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
    }
    else if(cube->squares["back8"].getColor() == Color::BLUE && cube->squares["up2"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front8"].getColor() == Color::ORANGE && cube->squares["down2"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
    }
    else if(cube->squares["front8"].getColor() == Color::BLUE && cube->squares["down2"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back2"].getColor() == Color::ORANGE && cube->squares["down8"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
    }
    else if(cube->squares["back2"].getColor() == Color::BLUE && cube->squares["down8"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["right2"].getColor() == Color::ORANGE && cube->squares["up6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["right2"].getColor() == Color::BLUE && cube->squares["up6"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["right4"].getColor() == Color::ORANGE && cube->squares["front6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
    }
    else if(cube->squares["right4"].getColor() == Color::BLUE && cube->squares["front6"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["right6"].getColor() == Color::ORANGE && cube->squares["back6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
    }
    else if(cube->squares["right6"].getColor() == Color::BLUE && cube->squares["back6"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["right8"].getColor() == Color::ORANGE && cube->squares["down6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
    }
    else if(cube->squares["right8"].getColor() == Color::BLUE && cube->squares["down6"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
}

void LayersSolver::up2Cross()
{
    if(cube->squares["up2"].getColor() == Color::YELLOW && cube->squares["back8"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back4"].getColor() == Color::BLUE && cube->squares["left4"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["back4"].getColor() == Color::YELLOW && cube->squares["left4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
    }
    else if(cube->squares["back6"].getColor() == Color::BLUE && cube->squares["right6"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
    }
    else if(cube->squares["back6"].getColor() == Color::YELLOW && cube->squares["right6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
    }
    else if(cube->squares["back2"].getColor() == Color::YELLOW && cube->squares["down8"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
    }
    else if(cube->squares["back2"].getColor() == Color::BLUE && cube->squares["down8"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["left2"].getColor() == Color::YELLOW && cube->squares["up4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["left2"].getColor() == Color::BLUE && cube->squares["up4"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["right2"].getColor() == Color::YELLOW && cube->squares["up6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
    }
    else if(cube->squares["right2"].getColor() == Color::BLUE && cube->squares["up6"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["left8"].getColor() == Color::YELLOW && cube->squares["down4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
    }
    else if(cube->squares["left8"].getColor() == Color::BLUE && cube->squares["down4"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["right8"].getColor() == Color::YELLOW && cube->squares["down6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
    }
    else if(cube->squares["right8"].getColor() == Color::BLUE && cube->squares["down6"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front2"].getColor() == Color::YELLOW && cube->squares["up8"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front2"].getColor() == Color::BLUE && cube->squares["up8"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front4"].getColor() == Color::YELLOW && cube->squares["left6"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front4"].getColor() == Color::BLUE && cube->squares["left6"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front6"].getColor() == Color::YELLOW && cube->squares["right4"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front6"].getColor() == Color::BLUE && cube->squares["right4"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if(cube->squares["front8"].getColor() == Color::YELLOW && cube->squares["down2"].getColor() == Color::BLUE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
    }
    else if(cube->squares["front8"].getColor() == Color::BLUE && cube->squares["down2"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
}

void LayersSolver::up9FirstLayer()
{
    if(cube->squares["up9"].getColor() != Color::BLUE || cube->squares["front3"].getColor() != Color::WHITE || cube->squares["right1"].getColor() != Color::RED)
    {
        if(cube->squares["up9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            if(cube->squares["front7"].getColor() == Color::WHITE) 
            {
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            }
            else if(cube->squares["front7"].getColor() == Color::ORANGE) 
            {
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            }
            else if(cube->squares["front7"].getColor() == Color::YELLOW) 
            {
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            }
        }
        else if(cube->squares["front3"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            if(cube->squares["back3"].getColor() == Color::WHITE)
            {
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            }
            else if(cube->squares["back3"].getColor() == Color::ORANGE)
            {
                handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            }
            else if(cube->squares["back3"].getColor() == Color::RED)
            {
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            }
            else if(cube->squares["back3"].getColor() == Color::YELLOW)  
            {
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            }
        }
        else if(cube->squares["right1"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            if(cube->squares["front9"].getColor() == Color::WHITE) 
            {
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            }
            else if(cube->squares["front9"].getColor() == Color::ORANGE) 
            {
                handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            }
            else if(cube->squares["front9"].getColor() == Color::RED)
            {
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            }
            else if(cube->squares["front9"].getColor() == Color::YELLOW)
            {
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            }
        }
    }
}

void LayersSolver::up7FirstLayer()
{
    if(cube->squares["up7"].getColor() != Color::BLUE ||
        cube->squares["front1"].getColor() != Color::WHITE ||
        cube->squares["left3"].getColor() != Color::ORANGE)
    {
        if(cube->squares["up7"].getColor() == Color::WHITE && cube->squares["front1"].getColor() == Color::ORANGE && cube->squares["left3"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["up7"].getColor() == Color::ORANGE && cube->squares["front1"].getColor() == Color::BLUE && cube->squares["left3"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["up7"].getColor() == Color::BLUE && cube->squares["front1"].getColor() == Color::RED && cube->squares["left3"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["up7"].getColor() == Color::RED && cube->squares["front1"].getColor() == Color::WHITE && cube->squares["left3"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["up7"].getColor() == Color::WHITE && cube->squares["front1"].getColor() == Color::BLUE && cube->squares["left3"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["up7"].getColor() == Color::BLUE && cube->squares["front1"].getColor() == Color::YELLOW && cube->squares["left3"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
        else if(cube->squares["up7"].getColor() == Color::RED && cube->squares["front1"].getColor() == Color::BLUE && cube->squares["left3"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
        else if(cube->squares["up7"].getColor() == Color::YELLOW && cube->squares["front1"].getColor() == Color::RED && cube->squares["left3"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["up7"].getColor() == Color::BLUE && cube->squares["front1"].getColor() == Color::ORANGE && cube->squares["left3"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["up7"].getColor() == Color::ORANGE && cube->squares["front1"].getColor() == Color::YELLOW && cube->squares["left3"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["up7"].getColor() == Color::YELLOW && cube->squares["front1"].getColor() == Color::BLUE && cube->squares["left3"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
    }
}

void LayersSolver::up3FirstLayer()
{
    if(cube->squares["up3"].getColor() != Color::BLUE || 
        cube->squares["right3"].getColor() != Color::RED ||
        cube->squares["back9"].getColor() != Color::YELLOW)
    {
        if(cube->squares["up3"].getColor() == Color::RED && cube->squares["right3"].getColor() == Color::YELLOW && cube->squares["back9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
        else if(cube->squares["up3"].getColor() == Color::YELLOW && cube->squares["right3"].getColor() == Color::BLUE && cube->squares["back9"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["up3"].getColor() == Color::BLUE && cube->squares["right3"].getColor() == Color::WHITE && cube->squares["back9"].getColor() == Color::RED)
        {
                handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
                handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
                handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
                handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
                handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["up3"].getColor() == Color::RED && cube->squares["right3"].getColor() == Color::BLUE && cube->squares["back9"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["up3"].getColor() == Color::WHITE && cube->squares["right3"].getColor() == Color::RED && cube->squares["back9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["up3"].getColor() == Color::BLUE && cube->squares["right3"].getColor() == Color::ORANGE && cube->squares["back9"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["up3"].getColor() == Color::ORANGE && cube->squares["right3"].getColor() == Color::WHITE && cube->squares["back9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["up3"].getColor() == Color::WHITE && cube->squares["right3"].getColor() == Color::BLUE && cube->squares["back9"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["up3"].getColor() == Color::BLUE && cube->squares["right3"].getColor() == Color::YELLOW && cube->squares["back9"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
        else if(cube->squares["up3"].getColor() == Color::ORANGE && cube->squares["right3"].getColor() == Color::BLUE && cube->squares["back9"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
        else if(cube->squares["up3"].getColor() == Color::YELLOW && cube->squares["right3"].getColor() == Color::ORANGE && cube->squares["back9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
    }
}

void LayersSolver::up1FirstLayer()
{
    if(cube->squares["up1"].getColor() != Color::BLUE ||
        cube->squares["left1"].getColor() != Color::ORANGE ||
        cube->squares["back7"].getColor() != Color::YELLOW)
    {
        if(cube->squares["up1"].getColor() == Color::ORANGE && cube->squares["left1"].getColor() == Color::YELLOW && cube->squares["back7"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["up1"].getColor() == Color::YELLOW && cube->squares["left1"].getColor() == Color::BLUE && cube->squares["back7"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
        else if(cube->squares["up1"].getColor() == Color::BLUE && cube->squares["left1"].getColor() == Color::RED && cube->squares["back7"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["up1"].getColor() == Color::RED && cube->squares["left1"].getColor() == Color::WHITE && cube->squares["back7"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["up1"].getColor() == Color::WHITE && cube->squares["left1"].getColor() == Color::BLUE && cube->squares["back7"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["up1"].getColor() == Color::BLUE && cube->squares["left1"].getColor() == Color::WHITE && cube->squares["back7"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["up1"].getColor() == Color::WHITE && cube->squares["left1"].getColor() == Color::ORANGE && cube->squares["back7"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["up1"].getColor() == Color::ORANGE && cube->squares["left1"].getColor() == Color::BLUE && cube->squares["back7"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["up1"].getColor() == Color::BLUE && cube->squares["left1"].getColor() == Color::YELLOW && cube->squares["back7"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["up1"].getColor() == Color::YELLOW && cube->squares["left1"].getColor() == Color::RED && cube->squares["back7"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["up1"].getColor() == Color::RED && cube->squares["left1"].getColor() == Color::BLUE && cube->squares["back7"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
    }
}

void LayersSolver::front9FirstLayer()
{
    if(cube->squares["front9"].getColor() == Color::BLUE ||
        cube->squares["down3"].getColor() == Color::BLUE ||
        cube->squares["right7"].getColor() == Color::BLUE)
    {
        if(cube->squares["front9"].getColor() == Color::WHITE && cube->squares["down3"].getColor() == Color::RED && cube->squares["right7"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["front9"].getColor() == Color::BLUE && cube->squares["down3"].getColor() == Color::WHITE && cube->squares["right7"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["front9"].getColor() == Color::RED && cube->squares["down3"].getColor() == Color::BLUE && cube->squares["right7"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["front9"].getColor() == Color::WHITE && cube->squares["down3"].getColor() == Color::BLUE && cube->squares["right7"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["front9"].getColor() == Color::BLUE && cube->squares["down3"].getColor() == Color::ORANGE && cube->squares["right7"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["front9"].getColor() == Color::ORANGE && cube->squares["down3"].getColor() == Color::WHITE && cube->squares["right7"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["front9"].getColor() == Color::ORANGE && cube->squares["down3"].getColor() == Color::BLUE && cube->squares["right7"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["front9"].getColor() == Color::BLUE && cube->squares["down3"].getColor() == Color::YELLOW && cube->squares["right7"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["front9"].getColor() == Color::YELLOW && cube->squares["down3"].getColor() == Color::ORANGE && cube->squares["right7"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
        else if(cube->squares["front9"].getColor() == Color::YELLOW && cube->squares["down3"].getColor() == Color::BLUE && cube->squares["right7"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
        else if(cube->squares["front9"].getColor() == Color::BLUE && cube->squares["down3"].getColor() == Color::RED && cube->squares["right7"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["front9"].getColor() == Color::RED && cube->squares["down3"].getColor() == Color::YELLOW && cube->squares["right7"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
    }
}

void LayersSolver::front7FirstLayer()
{
    if(cube->squares["front7"].getColor() == Color::BLUE ||
        cube->squares["down1"].getColor() == Color::BLUE ||
        cube->squares["left9"].getColor() == Color::BLUE)
    {
        if(cube->squares["front7"].getColor() == Color::WHITE && cube->squares["down1"].getColor() == Color::BLUE && cube->squares["left9"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["front7"].getColor() == Color::BLUE && cube->squares["down1"].getColor() == Color::RED && cube->squares["left9"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["front7"].getColor() == Color::RED && cube->squares["down1"].getColor() == Color::WHITE && cube->squares["left9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["front7"].getColor() == Color::BLUE && cube->squares["down1"].getColor() == Color::WHITE && cube->squares["left9"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["front7"].getColor() == Color::WHITE && cube->squares["down1"].getColor() == Color::ORANGE && cube->squares["left9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["front7"].getColor() == Color::ORANGE && cube->squares["down1"].getColor() == Color::BLUE && cube->squares["left9"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["front7"].getColor() == Color::YELLOW && cube->squares["down1"].getColor() == Color::BLUE && cube->squares["left9"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["front7"].getColor() == Color::BLUE && cube->squares["down1"].getColor() == Color::ORANGE && cube->squares["left9"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
        else if(cube->squares["front7"].getColor() == Color::ORANGE && cube->squares["down1"].getColor() == Color::YELLOW && cube->squares["left9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["front7"].getColor() == Color::YELLOW && cube->squares["down1"].getColor() == Color::RED && cube->squares["left9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["front7"].getColor() == Color::RED && cube->squares["down1"].getColor() == Color::BLUE && cube->squares["left9"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
        else if(cube->squares["front7"].getColor() == Color::BLUE && cube->squares["down1"].getColor() == Color::YELLOW && cube->squares["left9"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
    }
}

void LayersSolver::back3FirstLayer()
{
    if(cube->squares["back3"].getColor() == Color::BLUE ||
        cube->squares["down9"].getColor() == Color::BLUE ||
        cube->squares["right9"].getColor() == Color::BLUE)
    {
        if(cube->squares["back3"].getColor() == Color::WHITE && cube->squares["down9"].getColor() == Color::BLUE && cube->squares["right9"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["back3"].getColor() == Color::BLUE && cube->squares["down9"].getColor() == Color::RED && cube->squares["right9"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["back3"].getColor() == Color::RED && cube->squares["down9"].getColor() == Color::WHITE && cube->squares["right9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["back3"].getColor() == Color::BLUE && cube->squares["down9"].getColor() == Color::WHITE && cube->squares["right9"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["back3"].getColor() == Color::WHITE && cube->squares["down9"].getColor() == Color::ORANGE && cube->squares["right9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["back3"].getColor() == Color::ORANGE && cube->squares["down9"].getColor() == Color::BLUE && cube->squares["right9"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["back3"].getColor() == Color::YELLOW && cube->squares["down9"].getColor() == Color::BLUE && cube->squares["right9"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
        else if(cube->squares["back3"].getColor() == Color::BLUE && cube->squares["down9"].getColor() == Color::ORANGE && cube->squares["right9"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
        else if(cube->squares["back3"].getColor() == Color::ORANGE && cube->squares["down9"].getColor() == Color::YELLOW && cube->squares["right9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["back3"].getColor() == Color::YELLOW && cube->squares["down9"].getColor() == Color::RED && cube->squares["right9"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["back3"].getColor() == Color::RED && cube->squares["down9"].getColor() == Color::BLUE && cube->squares["right9"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["back3"].getColor() == Color::BLUE && cube->squares["down9"].getColor() == Color::YELLOW && cube->squares["right9"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
    }
}

void LayersSolver::back1FirstLayer()
{
    if(cube->squares["back1"].getColor() == Color::BLUE ||
        cube->squares["down7"].getColor() == Color::BLUE ||
        cube->squares["left7"].getColor() == Color::BLUE)
    {
        if(cube->squares["back1"].getColor() == Color::WHITE && cube->squares["down7"].getColor() == Color::RED && cube->squares["left7"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["back1"].getColor() == Color::BLUE && cube->squares["down7"].getColor() == Color::WHITE && cube->squares["left7"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["back1"].getColor() == Color::RED && cube->squares["down7"].getColor() == Color::BLUE && cube->squares["left7"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["back1"].getColor() == Color::WHITE && cube->squares["down7"].getColor() == Color::BLUE && cube->squares["left7"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["back1"].getColor() == Color::BLUE && cube->squares["down7"].getColor() == Color::ORANGE && cube->squares["left7"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["back1"].getColor() == Color::ORANGE && cube->squares["down7"].getColor() == Color::WHITE && cube->squares["left7"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["back1"].getColor() == Color::ORANGE && cube->squares["down7"].getColor() == Color::BLUE && cube->squares["left7"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["back1"].getColor() == Color::BLUE && cube->squares["down7"].getColor() == Color::YELLOW && cube->squares["left7"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["back1"].getColor() == Color::YELLOW && cube->squares["down7"].getColor() == Color::ORANGE && cube->squares["left7"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
        else if(cube->squares["back1"].getColor() == Color::YELLOW && cube->squares["down7"].getColor() == Color::BLUE && cube->squares["left7"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["back1"].getColor() == Color::BLUE && cube->squares["down7"].getColor() == Color::RED && cube->squares["left7"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["back1"].getColor() == Color::RED && cube->squares["down7"].getColor() == Color::YELLOW && cube->squares["left7"].getColor() == Color::BLUE)
        {
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
    }
}

void LayersSolver::frontSecondLayer()
{
    if(cube->squares["front8"].getColor() == Color::WHITE && cube->squares["down2"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
    }
    else if(cube->squares["front8"].getColor() == Color::WHITE && cube->squares["down2"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
    }
    else if(cube->squares["left8"].getColor() == Color::WHITE && cube->squares["down4"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
    }
    else if(cube->squares["left8"].getColor() == Color::WHITE && cube->squares["down4"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
    }
    else if(cube->squares["right8"].getColor() == Color::WHITE && cube->squares["down6"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
    }
    else if(cube->squares["right8"].getColor() == Color::WHITE && cube->squares["down6"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
    }
    else if(cube->squares["back2"].getColor() == Color::WHITE && cube->squares["down8"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
    }
    else if(cube->squares["back2"].getColor() == Color::WHITE && cube->squares["down8"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
    }
}

void LayersSolver::leftSecondLayer()
{
    if(cube->squares["left8"].getColor() == Color::ORANGE && cube->squares["down4"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
    }
    else if(cube->squares["left8"].getColor() == Color::ORANGE && cube->squares["down4"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
    }
    else if(cube->squares["back2"].getColor() == Color::ORANGE && cube->squares["down8"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
    }
    else if(cube->squares["back2"].getColor() == Color::ORANGE && cube->squares["down8"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
    }
    else if(cube->squares["front8"].getColor() == Color::ORANGE && cube->squares["down2"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
    }
    else if(cube->squares["front8"].getColor() == Color::ORANGE && cube->squares["down2"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
    }
    else if(cube->squares["right8"].getColor() == Color::ORANGE && cube->squares["down6"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
    }
    else if(cube->squares["right8"].getColor() == Color::ORANGE && cube->squares["down6"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
    }
}

void LayersSolver::rightSecondLayer()
{
    if(cube->squares["right8"].getColor() == Color::RED && cube->squares["down6"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
    }
    else if(cube->squares["right8"].getColor() == Color::RED && cube->squares["down6"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
    }
    else if(cube->squares["front8"].getColor() == Color::RED && cube->squares["down2"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
    }
    else if(cube->squares["front8"].getColor() == Color::RED && cube->squares["down2"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
    }
    else if(cube->squares["back2"].getColor() == Color::RED && cube->squares["down8"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
    }
    else if(cube->squares["back2"].getColor() == Color::RED && cube->squares["down8"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
    }
    else if(cube->squares["left8"].getColor() == Color::RED && cube->squares["down4"].getColor() == Color::WHITE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
    }
    else if(cube->squares["left8"].getColor() == Color::RED && cube->squares["down4"].getColor() == Color::YELLOW)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
    }
}

void LayersSolver::backSecondLayer()
{
    if(cube->squares["back2"].getColor() == Color::YELLOW && cube->squares["down8"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
    }
    else if(cube->squares["back2"].getColor() == Color::YELLOW && cube->squares["down8"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
    }
    else if(cube->squares["right8"].getColor() == Color::YELLOW && cube->squares["down6"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
    }
    else if(cube->squares["right8"].getColor() == Color::YELLOW && cube->squares["down6"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
    }
    else if(cube->squares["left8"].getColor() == Color::YELLOW && cube->squares["down4"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
    }
    else if(cube->squares["left8"].getColor() == Color::YELLOW && cube->squares["down4"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
    }
    else if(cube->squares["front8"].getColor() == Color::YELLOW && cube->squares["down2"].getColor() == Color::RED)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
    }
    else if(cube->squares["front8"].getColor() == Color::YELLOW && cube->squares["down2"].getColor() == Color::ORANGE)
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
        handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
    }
}

bool LayersSolver::fixBackSecondLayer()
{
    if(cube->squares["back4"].getColor() != Color::YELLOW ||
        cube->squares["back6"].getColor() != Color::YELLOW)
    {
        if(!cube->isComplete() && cube->squares["back2"].getColor() == Color::YELLOW && cube->squares["down8"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(!cube->isComplete() && cube->squares["back2"].getColor() == Color::YELLOW && cube->squares["down8"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
        else if(cube->squares["right8"].getColor() == Color::YELLOW && cube->squares["down6"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["right8"].getColor() == Color::YELLOW && cube->squares["down6"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
        else if(cube->squares["left8"].getColor() == Color::YELLOW && cube->squares["down4"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["left8"].getColor() == Color::YELLOW && cube->squares["down4"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
        else if(cube->squares["front8"].getColor() == Color::YELLOW && cube->squares["down2"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["front8"].getColor() == Color::YELLOW && cube->squares["down2"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
        else if(cube->squares["back6"].getColor() == Color::RED && cube->squares["right6"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        else if(cube->squares["back4"].getColor() == Color::ORANGE && cube->squares["left4"].getColor() == Color::YELLOW)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
        }
        else if(!cube->isComplete())
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        }
        return true;
    }
    return false;
}

bool LayersSolver::fixLeftSecondLayer()
{
    if(cube->squares["left4"].getColor() != Color::ORANGE ||
        cube->squares["left6"].getColor() != Color::ORANGE)
    {
        if(cube->squares["left8"].getColor() == Color::ORANGE && cube->squares["down4"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["left8"].getColor() == Color::ORANGE && cube->squares["down4"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["back2"].getColor() == Color::ORANGE && cube->squares["down8"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["back2"].getColor() == Color::ORANGE && cube->squares["down8"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["front8"].getColor() == Color::ORANGE && cube->squares["down2"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["front8"].getColor() == Color::ORANGE && cube->squares["down2"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["right8"].getColor() == Color::ORANGE && cube->squares["down6"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["right8"].getColor() == Color::ORANGE && cube->squares["down6"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else if(cube->squares["left4"].getColor() == Color::GREEN && cube->squares["down4"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        else if(cube->squares["left6"].getColor() == Color::WHITE && cube->squares["front4"].getColor() == Color::ORANGE)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
        }
        else
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        }
        return true;
    }
    return false;
}

bool LayersSolver::fixRightSecondLayer()
{
    if(cube->squares["right4"].getColor() != Color::ORANGE ||
        cube->squares["right6"].getColor() != Color::ORANGE)
    {
        if(cube->squares["right8"].getColor() == Color::RED && cube->squares["down6"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["right8"].getColor() == Color::RED && cube->squares["down6"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
        else if(cube->squares["front8"].getColor() == Color::RED && cube->squares["down2"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["front8"].getColor() == Color::RED && cube->squares["down2"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
        else if(cube->squares["back2"].getColor() == Color::RED && cube->squares["down8"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["back2"].getColor() == Color::RED && cube->squares["down8"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
        else if(cube->squares["left8"].getColor() == Color::RED && cube->squares["down4"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["left8"].getColor() == Color::RED && cube->squares["down4"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
        else if(cube->squares["right4"].getColor() == Color::WHITE && cube->squares["front6"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        else if(cube->squares["right6"].getColor() == Color::GREEN && cube->squares["down6"].getColor() == Color::RED)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
        }
        else
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        }
        return true;
    }
    return false;
}

bool LayersSolver::fixFrontSecondLayer()
{
    if(cube->squares["front4"].getColor() != Color::WHITE ||
        cube->squares["front6"].getColor() != Color::WHITE)
    {
        if(cube->squares["front8"].getColor() == Color::WHITE && cube->squares["down2"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["front8"].getColor() == Color::WHITE && cube->squares["down2"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["left8"].getColor() == Color::WHITE && cube->squares["down4"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["left8"].getColor() == Color::WHITE && cube->squares["down4"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["right8"].getColor() == Color::WHITE && cube->squares["down6"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["right8"].getColor() == Color::WHITE && cube->squares["down6"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["back2"].getColor() == Color::WHITE && cube->squares["down8"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["back2"].getColor() == Color::WHITE && cube->squares["down8"].getColor() == Color::GREEN)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else if(cube->squares["front4"].getColor() == Color::ORANGE && cube->squares["left6"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        else if(cube->squares["front6"].getColor() == Color::RED && cube->squares["right4"].getColor() == Color::WHITE)
        {
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
        }
        else
        {
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();
            handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();
            handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();
            handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
            handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        }
        return true;   
    }
    return false;
}

bool LayersSolver::isCornerFrontDownRightPlaced()
{
    if((cube->squares["front9"].getColor() == Color::WHITE || cube->squares["front9"].getColor() == Color::GREEN || cube->squares["front9"].getColor() == Color::RED) &&
            (cube->squares["down3"].getColor() == Color::WHITE || cube->squares["down3"].getColor() == Color::GREEN || cube->squares["down3"].getColor() == Color::RED) &&
            (cube->squares["right7"].getColor() == Color::WHITE || cube->squares["right7"].getColor() == Color::GREEN || cube->squares["right7"].getColor() == Color::RED))
        return true;
    return false;
}

bool LayersSolver::isCornerFrontDownLeftPlaced()
{
    if((cube->squares["front7"].getColor() == Color::WHITE || cube->squares["front7"].getColor() == Color::GREEN || cube->squares["front7"].getColor() == Color::ORANGE) &&
        (cube->squares["down1"].getColor() == Color::WHITE || cube->squares["down1"].getColor() == Color::GREEN || cube->squares["down1"].getColor() == Color::ORANGE) &&
        (cube->squares["left9"].getColor() == Color::WHITE || cube->squares["left9"].getColor() == Color::GREEN || cube->squares["left9"].getColor() == Color::ORANGE))
        return true;
    return false;
}

bool LayersSolver::isCornerBackDownRightPlaced()
{
    if((cube->squares["back3"].getColor() == Color::YELLOW || cube->squares["back3"].getColor() == Color::GREEN || cube->squares["back3"].getColor() == Color::RED) &&
        (cube->squares["down9"].getColor() == Color::YELLOW || cube->squares["down9"].getColor() == Color::GREEN || cube->squares["down9"].getColor() == Color::RED) &&
        (cube->squares["right9"].getColor() == Color::YELLOW || cube->squares["right9"].getColor() == Color::GREEN || cube->squares["right9"].getColor() == Color::RED))
        return true;
    return false;
}

bool LayersSolver::isCornerBackDownLeftPlaced()
{
    if((cube->squares["back1"].getColor() == Color::YELLOW || cube->squares["back1"].getColor() == Color::GREEN || cube->squares["back1"].getColor() == Color::ORANGE) &&
        (cube->squares["down7"].getColor() == Color::YELLOW || cube->squares["down7"].getColor() == Color::GREEN || cube->squares["down7"].getColor() == Color::ORANGE) &&
        (cube->squares["left7"].getColor() == Color::YELLOW || cube->squares["left7"].getColor() == Color::GREEN || cube->squares["left7"].getColor() == Color::ORANGE))
        return true;
    return false;
}
