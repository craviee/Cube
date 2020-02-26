#include "layerssolver.h"

LayersSolver::LayersSolver(int& rotationsNumber, std::shared_ptr<Cube> cube)
 : Solver{rotationsNumber, cube}{}

void LayersSolver::solve(SolverStep step)
{

}

void LayersSolver::cross()
{
    // while(ui->up2Button->styleSheet() != ui->up5Button->styleSheet() || ui->back8Button->styleSheet() != ui->back5Button->styleSheet() ||
    // ui->up4Button->styleSheet() != ui->up5Button->styleSheet() || ui->left2Button->styleSheet() != ui->left5Button->styleSheet() ||
    // ui->up6Button->styleSheet() != ui->up5Button->styleSheet() || ui->right2Button->styleSheet() != ui->right5Button->styleSheet() ||
    // ui->up8Button->styleSheet() != ui->up5Button->styleSheet() || ui->front2Button->styleSheet() != ui->front5Button->styleSheet() )
    // {

        front8Cross();
        front2Cross();
        front6Cross();
        front4Cross();

        // CruzSul();
        // CruzNorte();
        // CruzLeste();
        // CruzOeste();
    // }
}

void LayersSolver::front8Cross()
{
    if(cube->squares["front2"].getColor() == Color::BLUE && cube->squares["up8"].getColor() == Color::WHITE)
    {
        cube->rotateF();
        cube->rotateUA();
        cube->rotateR();
        cube->rotateU();
    }
    // else if(ui->front6Button->styleSheet() == ui->up5Button->styleSheet() && ui->right4Button->styleSheet() == ui->front5Button->styleSheet())
    // {
    //     cube->rotateUA();
    //     cube->rotateR();
    //     cube->rotateU();
    // }
    // else if(ui->front6Button->styleSheet() == ui->front5Button->styleSheet() && ui->right4Button->styleSheet() == ui->up5Button->styleSheet())
    // {
    //     cube->rotateFA();
    // }
    // else if(ui->front4Button->styleSheet() == ui->up5Button->styleSheet() && ui->left6Button->styleSheet() == ui->front5Button->styleSheet())
    // {
    //     cube->rotateU();
    //     cube->rotateLA();
    //     cube->rotateUA();
    // }
    // else if(ui->front4Button->styleSheet() == ui->front5Button->styleSheet() && ui->left6Button->styleSheet() == ui->up5Button->styleSheet())
    // {
    //     cube->rotateF();
    // }
    // else if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
    // {
    //     cube->rotateF();
    //     cube->rotateF();
    // }
    // else if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down2Button->styleSheet() == ui->front5Button->styleSheet())
    // {
    //     cube->rotateFA();
    //     cube->rotateUA();
    //     cube->rotateR();
    //     cube->rotateU();
    // }
    // else if(ui->right2Button->styleSheet() == ui->front5Button->styleSheet() && ui->up6Button->styleSheet() == ui->up5Button->styleSheet())
    // {
    //     cube->rotateU();
    // }
    // else if(ui->right2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up6Button->styleSheet() == ui->front5Button->styleSheet())
    // {
    //     cube->rotateU();
    //     cube->rotateF();
    //     cube->rotateUA();
    //     cube->rotateR();
    //     cube->rotateU();
    // }
    // else if(ui->left2Button->styleSheet() == ui->front5Button->styleSheet() && ui->up4Button->styleSheet() == ui->up5Button->styleSheet())
    // {
    //     cube->rotateUA();
    // }
    // else if(ui->left2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up4Button->styleSheet() == ui->front5Button->styleSheet())
    // {
    //     cube->rotateUA();
    //     cube->rotateF();
    //     cube->rotateUA();
    //     cube->rotateR();
    //     cube->rotateU();
    // }
    // else if(ui->right8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
    // {
    //     cube->rotateDA();
    //     cube->rotateF();
    //     cube->rotateF();
    // }
    // else if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down6Button->styleSheet() == ui->front5Button->styleSheet())
    // {
    //     cube->rotateDA();
    //     cube->rotateFA();
    //     cube->rotateUA();
    //     cube->rotateR();
    //     cube->rotateU();
    // }
    // else if(ui->left8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
    // {
    //     cube->rotateD();
    //     cube->rotateF();
    //     cube->rotateF();
    // }
    // else if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down4Button->styleSheet() == ui->front5Button->styleSheet())
    // {
    //     cube->rotateD();
    //     cube->rotateFA();
    //     cube->rotateUA();
    //     cube->rotateR();
    //     cube->rotateU();
    // }
    // else if(ui->back8Button->styleSheet() == ui->front5Button->styleSheet() && ui->up2Button->styleSheet() == ui->up5Button->styleSheet())
    // {
    //     cube->rotateU();
    //     cube->rotateU();
    // }
    // else if(ui->back8Button->styleSheet() == ui->up5Button->styleSheet() && ui->up2Button->styleSheet() == ui->front5Button->styleSheet())
    // {
    //     cube->rotateU();
    //     cube->rotateU();
    //     cube->rotateF();
    //     cube->rotateUA();
    //     cube->rotateR();
    //     cube->rotateU();
    // }
    // else if(ui->back6Button->styleSheet() == ui->front5Button->styleSheet() && ui->right6Button->styleSheet() == ui->up5Button->styleSheet())
    // {
    //     cube->rotateB();
    //     cube->rotateU();
    //     cube->rotateU();
    // }
    // else if(ui->back6Button->styleSheet() == ui->up5Button->styleSheet() && ui->right6Button->styleSheet() == ui->front5Button->styleSheet())
    // {
    //     cube->rotateB();
    //     cube->rotateU();
    //     cube->rotateU();
    //     cube->rotateF();
    //     cube->rotateUA();
    //     cube->rotateR();
    //     cube->rotateU();
    // }
    // else if(ui->back4Button->styleSheet() == ui->front5Button->styleSheet() && ui->left4Button->styleSheet() == ui->up5Button->styleSheet())
    // {
    //     cube->rotateBA();
    //     cube->rotateU();
    //     cube->rotateU();
    // }
    // else if(ui->back4Button->styleSheet() == ui->up5Button->styleSheet() && ui->left4Button->styleSheet() == ui->front5Button->styleSheet())
    // {
    //     cube->rotateBA();
    //     cube->rotateU();
    //     cube->rotateU();
    //     cube->rotateF();
    //     cube->rotateUA();
    //     cube->rotateR();
    //     cube->rotateU();
    // }
    // else if(ui->back2Button->styleSheet() == ui->front5Button->styleSheet() && ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
    // {
    //     cube->rotateD();
    //     cube->rotateD();
    //     cube->rotateF();
    //     cube->rotateF();
    // }
    // else if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet() && ui->down8Button->styleSheet() == ui->front5Button->styleSheet())
    // {
    //     cube->rotateD();
    //     cube->rotateD();
    //     cube->rotateFA();
    //     cube->rotateUA();
    //     cube->rotateR();
    //     cube->rotateU();
    // }
}

void LayersSolver::front2Cross()
{

}

void LayersSolver::front6Cross()
{

}

void LayersSolver::front4Cross()
{

}

