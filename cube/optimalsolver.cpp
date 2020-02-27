#include "optimalsolver.h"

OptimalSolver::OptimalSolver(int* rotationsNumber, std::shared_ptr<Cube> cube
    , std::shared_ptr<Microcontroller> microcontroller)
    : Solver{rotationsNumber, cube, microcontroller}{ }

QString OptimalSolver::color2qstring(Color color)
{
    if (color == Color::BLUE) return QString("b");
    if (color == Color::RED) return QString("r");
    if (color == Color::WHITE) return QString("w");
    if (color == Color::ORANGE) return QString("o");
    if (color == Color::YELLOW) return QString("y");
    if (color == Color::GREEN) return QString("g");
}

void OptimalSolver::handlePreRotation(Command command)
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

QString OptimalSolver::getFormatedQString()
{
    QString qstring = "";
    qstring += color2qstring(cube->squares["right3"].getColor());
    qstring += color2qstring(cube->squares["right6"].getColor());
    qstring += color2qstring(cube->squares["right9"].getColor());
    qstring += color2qstring(cube->squares["right2"].getColor());
    qstring += color2qstring(cube->squares["right8"].getColor());
    qstring += color2qstring(cube->squares["right1"].getColor());
    qstring += color2qstring(cube->squares["right4"].getColor());
    qstring += color2qstring(cube->squares["right7"].getColor());
    qstring += color2qstring(cube->squares["up3"].getColor());
    qstring += color2qstring(cube->squares["up6"].getColor());
    qstring += color2qstring(cube->squares["up9"].getColor());
    qstring += color2qstring(cube->squares["front3"].getColor());
    qstring += color2qstring(cube->squares["front6"].getColor());
    qstring += color2qstring(cube->squares["front9"].getColor());
    qstring += color2qstring(cube->squares["down3"].getColor());
    qstring += color2qstring(cube->squares["down6"].getColor());
    qstring += color2qstring(cube->squares["down9"].getColor());
    qstring += color2qstring(cube->squares["back3"].getColor());
    qstring += color2qstring(cube->squares["back6"].getColor());
    qstring += color2qstring(cube->squares["back9"].getColor());
    qstring += color2qstring(cube->squares["up2"].getColor());
    qstring += color2qstring(cube->squares["up8"].getColor());
    qstring += color2qstring(cube->squares["front2"].getColor());
    qstring += color2qstring(cube->squares["front8"].getColor());
    qstring += color2qstring(cube->squares["down2"].getColor());
    qstring += color2qstring(cube->squares["down8"].getColor());
    qstring += color2qstring(cube->squares["back2"].getColor());
    qstring += color2qstring(cube->squares["back8"].getColor());
    qstring += color2qstring(cube->squares["up1"].getColor());
    qstring += color2qstring(cube->squares["up4"].getColor());
    qstring += color2qstring(cube->squares["up7"].getColor());
    qstring += color2qstring(cube->squares["front1"].getColor());
    qstring += color2qstring(cube->squares["front4"].getColor());
    qstring += color2qstring(cube->squares["front7"].getColor());
    qstring += color2qstring(cube->squares["down1"].getColor());
    qstring += color2qstring(cube->squares["down4"].getColor());
    qstring += color2qstring(cube->squares["down7"].getColor());
    qstring += color2qstring(cube->squares["back1"].getColor());
    qstring += color2qstring(cube->squares["back4"].getColor());
    qstring += color2qstring(cube->squares["back7"].getColor());
    qstring += color2qstring(cube->squares["left3"].getColor());
    qstring += color2qstring(cube->squares["left6"].getColor());
    qstring += color2qstring(cube->squares["left9"].getColor());
    qstring += color2qstring(cube->squares["left2"].getColor());
    qstring += color2qstring(cube->squares["left8"].getColor());
    qstring += color2qstring(cube->squares["left1"].getColor());
    qstring += color2qstring(cube->squares["left4"].getColor());
    qstring += color2qstring(cube->squares["left7"].getColor());
    return qstring;
}

void OptimalSolver::solve(SolverStep step)
{
    if(step == SolverStep::COMPLETE || step == SolverStep::COMPLETE_MICROCONTROLLER)
    {
        isSimulation = (step == SolverStep::COMPLETE ? true : false);

        QString formatterInput = getFormatedQString();
        QString solverInput;
        QStringList formatterArguments;
        QStringList solverArguments;
        std::string lineReaded;
        auto formatterProcess = std::make_unique<QProcess>();
        auto solverProcess = std::make_unique<QProcess>();

        formatterProcess->start(FORMATTER,(QStringList) formatterArguments << formatterInput );
        
        if (formatterProcess->waitForFinished())
        {
            formatterProcess->close();
            std::ifstream formatterFile (FORMATTER_OUTPUT);
            if (formatterFile.is_open())
            {
                while ( std::getline (formatterFile,lineReaded ))
                {
                    if(lineReaded.size() == FORMATTER_SIZE)
                        solverInput = QString::fromStdString(lineReaded);
                }
                formatterFile.close();
            }
        }
        
        solverProcess->start(SOLVER,(QStringList) solverArguments << solverInput );
        solverProcess->write("exit\n\r");

        if (solverProcess->waitForFinished(-1))
        {
            std::ifstream solverFile (SOLVER_OUPUT);
            if (solverFile.is_open())
            {
                while ( std::getline (solverFile,lineReaded ))
                {
                    // One or Two rotations for each line i.e B, B', 2B
                    if(lineReaded.size() == 1 || lineReaded.size() == 2)
                    {
                        processResult(lineReaded);
                    }
                }
                solverFile.close();
            }
        }
    }
}

void OptimalSolver::processResult(std::string rotation)
{
    if(rotation == "2F")
    {
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
        handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();
    }
    else if ( rotation == "2T" )
    {
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
        handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();
    }
    else if ( rotation == "2D" )
    {
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
        handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();
    }
    else if ( rotation == "2L" )
    {
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
        handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();
    }
    else if ( rotation == "2R" )
    {
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
        handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();
    }
    else if ( rotation == "2B" )
    {
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
        handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();
    }
    else if ( rotation == "F") {handlePreRotation(Command::DO_F_ROTATION); cube->rotateF();}
    else if ( rotation == "F'" ) {handlePreRotation(Command::DO_FA_ROTATION); cube->rotateFA();}
    else if ( rotation == "T" ) {handlePreRotation(Command::DO_R_ROTATION); cube->rotateR();}
    else if ( rotation == "T'" ) {handlePreRotation(Command::DO_RA_ROTATION); cube->rotateRA();}
    else if ( rotation == "D" ) {handlePreRotation(Command::DO_L_ROTATION); cube->rotateL();}
    else if ( rotation == "D'" ) {handlePreRotation(Command::DO_LA_ROTATION); cube->rotateLA();}
    else if ( rotation == "L" ) {handlePreRotation(Command::DO_U_ROTATION); cube->rotateU();}
    else if ( rotation == "L'" ) {handlePreRotation(Command::DO_UA_ROTATION); cube->rotateUA();}
    else if ( rotation == "R" ) {handlePreRotation(Command::DO_D_ROTATION); cube->rotateD();}
    else if ( rotation == "R'" ) {handlePreRotation(Command::DO_DA_ROTATION); cube->rotateDA();}
    else if ( rotation == "B" ) {handlePreRotation(Command::DO_B_ROTATION); cube->rotateB();}
    else if ( rotation == "B'" ) {handlePreRotation(Command::DO_BA_ROTATION); cube->rotateBA();}
}
