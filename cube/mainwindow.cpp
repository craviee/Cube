#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupSquares();
    setRotationsNumber(0);
    rotator = std::make_shared<Rotator>(squares);
    cube = std::make_shared<Cube>(squares, rotator);
    layersSolver = std::make_shared<LayersSolver>(&rotationsNumber, cube);
    //optimalSolver = std::make_shared<OptimalSolver>(rotationsNumber);
    layersSolver->subscribe(this);
    //optimalSolver->subscribe(this);
    microcontroller = std::make_shared<Arduino>();
    calibrator = std::make_unique<ColorCalibrator>(squares, microcontroller);
    reader = std::make_unique<ColorReader>(squares, calibrator->configValues, microcontroller);
    cube->initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSquares()
{
    squares["front1"]= Square{ui->front1Button};
    squares["front2"]= Square{ui->front2Button};
    squares["front3"]= Square{ui->front3Button};
    squares["front4"]= Square{ui->front4Button};
    squares["front5"]= Square{ui->front5Button};
    squares["front6"]= Square{ui->front6Button};
    squares["front7"]= Square{ui->front7Button};
    squares["front8"]= Square{ui->front8Button};
    squares["front9"]= Square{ui->front9Button};
    squares["back1"]= Square{ui->back1Button};
    squares["back2"]= Square{ui->back2Button};
    squares["back3"]= Square{ui->back3Button};
    squares["back4"]= Square{ui->back4Button};
    squares["back5"]= Square{ui->back5Button};
    squares["back6"]= Square{ui->back6Button};
    squares["back7"]= Square{ui->back7Button};
    squares["back8"]= Square{ui->back8Button};
    squares["back9"]= Square{ui->back9Button};
    squares["up1"]= Square{ui->up1Button};
    squares["up2"]= Square{ui->up2Button};
    squares["up3"]= Square{ui->up3Button};
    squares["up4"]= Square{ui->up4Button};
    squares["up5"]= Square{ui->up5Button};
    squares["up6"]= Square{ui->up6Button};
    squares["up7"]= Square{ui->up7Button};
    squares["up8"]= Square{ui->up8Button};
    squares["up9"]= Square{ui->up9Button};
    squares["down1"]= Square{ui->down1Button};
    squares["down2"]= Square{ui->down2Button};
    squares["down3"]= Square{ui->down3Button};
    squares["down4"]= Square{ui->down4Button};
    squares["down5"]= Square{ui->down5Button};
    squares["down6"]= Square{ui->down6Button};
    squares["down7"]= Square{ui->down7Button};
    squares["down8"]= Square{ui->down8Button};
    squares["down9"]= Square{ui->down9Button};
    squares["left1"]= Square{ui->left1Button};
    squares["left2"]= Square{ui->left2Button};
    squares["left3"]= Square{ui->left3Button};
    squares["left4"]= Square{ui->left4Button};
    squares["left5"]= Square{ui->left5Button};
    squares["left6"]= Square{ui->left6Button};
    squares["left7"]= Square{ui->left7Button};
    squares["left8"]= Square{ui->left8Button};
    squares["left9"]= Square{ui->left9Button};
    squares["right1"]= Square{ui->right1Button};
    squares["right2"]= Square{ui->right2Button};
    squares["right3"]= Square{ui->right3Button};
    squares["right4"]= Square{ui->right4Button};
    squares["right5"]= Square{ui->right5Button};
    squares["right6"]= Square{ui->right6Button};
    squares["right7"]= Square{ui->right7Button};
    squares["right8"]= Square{ui->right8Button};
    squares["right9"]= Square{ui->right9Button};
}

void MainWindow::setRotationsNumber(int rotationsNumber)
{
    this->rotationsNumber = rotationsNumber;
    ui->rotationsNumberTextBox->setPlainText(QString::number(rotationsNumber));
}

void MainWindow::changeColor(QPushButton *button)
{
    if(Utils::getColor(button) == Color::BLUE)
        Utils::setColor(Color::WHITE, button);
    else if(Utils::getColor(button) == Color::WHITE)
        Utils::setColor(Color::ORANGE, button);
    else if(Utils::getColor(button) == Color::ORANGE)
        Utils::setColor(Color::RED, button);
    else if(Utils::getColor(button) == Color::RED)
        Utils::setColor(Color::GREEN, button);
    else if(Utils::getColor(button) == Color::GREEN)
        Utils::setColor(Color::YELLOW, button);
    else if(Utils::getColor(button) == Color::YELLOW)
        Utils::setColor(Color::BLUE, button);
}

void MainWindow::onUpdate(Solver& source, int number){ setRotationsNumber(number); }

void MainWindow::solve(std::string s)
{
    if(s == "2F")
    {
        cube->rotateF();
        cube->rotateF();
    }
    else if( s == "F")
        cube->rotateF();
    else if ( s == "F'" )
        cube->rotateFA();
    else if ( s == "2T" )
    {
        cube->rotateR();
        cube->rotateR();
    }
    else if ( s == "T" )
        cube->rotateR();
    else if ( s == "T'" )
        cube->rotateRA();
    else if ( s == "2D" )
    {
        cube->rotateL();
        cube->rotateL();
    }
    else if ( s == "D" )
        cube->rotateL();
    else if ( s == "D'" )
        cube->rotateLA();
    else if ( s == "2L" )
    {
        cube->rotateU();
        cube->rotateU();
    }
    else if ( s == "L" )
        cube->rotateU();
    else if ( s == "L'" )
        cube->rotateUA();
    else if ( s == "2R" )
    {
        cube->rotateD();
        cube->rotateD();
    }
    else if ( s == "R" )
        cube->rotateD();
    else if ( s == "R'" )
        cube->rotateDA();
    else if ( s == "2B" )
    {
        cube->rotateB();
        cube->rotateB();
    }
    else if ( s == "B" )
        cube->rotateB();
    else if ( s == "B'" )
        cube->rotateBA();
    else
        std::cout << "String não reconhecida solve()" << std::endl;
}

void MainWindow::on_randomButton_clicked()
{
    ui->randomButton->setEnabled(false);
    cube->randomize();
    setRotationsNumber(0);
    ui->randomButton->setEnabled(true);
}

void MainWindow::on_restartButton_clicked() { cube->initialize(); setRotationsNumber(0); }

void MainWindow::on_toggleModeButton_clicked()
{
    mode = mode == Mode::SIMULATION ? Mode::ROBOT : Mode::SIMULATION;
    ui->modeTextBox->setPlainText(QString(mode == Mode::SIMULATION ? "SIMULATION" : "ROBOT"));
}

void MainWindow::on_crossButton_clicked()
{
    if(cube->isValid()) layersSolver->solve(SolverStep::CROSS);
    else Utils::showDialog(std::string("Error: The cube has more than 9 squares of the same color."));
}

void MainWindow::on_firstLayerCornersButton_clicked()
{
    if(cube->isValid()) layersSolver->solve(SolverStep::FIRST_LAYER);
    else Utils::showDialog(std::string("Error: The cube has more than 9 squares of the same color."));
}

void MainWindow::on_secondLayerButton_clicked()
{
    if(cube->isValid()) layersSolver->solve(SolverStep::SECOND_LAYER);
    else Utils::showDialog(std::string("Error: The cube has more than 9 squares of the same color."));
}

void MainWindow::on_downCrossButton_clicked()
{
    if(cube->isValid()) layersSolver->solve(SolverStep::DOWN_CROSS);
    else Utils::showDialog(std::string("Error: The cube has more than 9 squares of the same color."));
}

void MainWindow::on_downEdgesButton_clicked()
{
    if(cube->isValid()) layersSolver->solve(SolverStep::DOWN_EDGES);
    else Utils::showDialog(std::string("Error: The cube has more than 9 squares of the same color."));
}

void MainWindow::on_placeDownCornersButton_clicked()
{
    if(cube->isValid()) layersSolver->solve(SolverStep::PLACE_DOWN_CORNERS);
    else Utils::showDialog(std::string("Error: The cube has more than 9 squares of the same color."));
}

void MainWindow::on_turnDownCornersButton_clicked()
{
    if(cube->isValid()) layersSolver->solve(SolverStep::TURN_DOWN_CORNERS);
    else Utils::showDialog(std::string("Error: The cube has more than 9 squares of the same color."));
}

void MainWindow::on_solveLanesButton_clicked()
{
    if(cube->isValid()) layersSolver->solve();
    else Utils::showDialog(std::string("Error: The cube has more than 9 squares of the same color."));
}

void MainWindow::on_solveOptimalButton_clicked()
{
    // QString input = "";
    // input += color(ui->right3Button->styleSheet());
    // input += color(ui->right6Button->styleSheet());
    // input += color(ui->right9Button->styleSheet());
    // input += color(ui->right2Button->styleSheet());
    // input += color(ui->right8Button->styleSheet());
    // input += color(ui->right1Button->styleSheet());
    // input += color(ui->right4Button->styleSheet());
    // input += color(ui->right7Button->styleSheet());
    // input += color(ui->up3Button->styleSheet());
    // input += color(ui->up6Button->styleSheet());
    // input += color(ui->up9Button->styleSheet());
    // input += color(ui->front3Button->styleSheet());
    // input += color(ui->front6Button->styleSheet());
    // input += color(ui->front9Button->styleSheet());
    // input += color(ui->down3Button->styleSheet());
    // input += color(ui->down6Button->styleSheet());
    // input += color(ui->down9Button->styleSheet());
    // input += color(ui->back3Button->styleSheet());
    // input += color(ui->back6Button->styleSheet());
    // input += color(ui->back9Button->styleSheet());
    // input += color(ui->up2Button->styleSheet());
    // input += color(ui->up8Button->styleSheet());
    // input += color(ui->front2Button->styleSheet());
    // input += color(ui->front8Button->styleSheet());
    // input += color(ui->down2Button->styleSheet());
    // input += color(ui->down8Button->styleSheet());
    // input += color(ui->back2Button->styleSheet());
    // input += color(ui->back8Button->styleSheet());
    // input += color(ui->up1Button->styleSheet());
    // input += color(ui->up4Button->styleSheet());
    // input += color(ui->up7Button->styleSheet());
    // input += color(ui->front1Button->styleSheet());
    // input += color(ui->front4Button->styleSheet());
    // input += color(ui->front7Button->styleSheet());
    // input += color(ui->down1Button->styleSheet());
    // input += color(ui->down4Button->styleSheet());
    // input += color(ui->down7Button->styleSheet());
    // input += color(ui->back1Button->styleSheet());
    // input += color(ui->back4Button->styleSheet());
    // input += color(ui->back7Button->styleSheet());
    // input += color(ui->left3Button->styleSheet());
    // input += color(ui->left6Button->styleSheet());
    // input += color(ui->left9Button->styleSheet());
    // input += color(ui->left2Button->styleSheet());
    // input += color(ui->left8Button->styleSheet());
    // input += color(ui->left1Button->styleSheet());
    // input += color(ui->left4Button->styleSheet());
    // input += color(ui->left7Button->styleSheet());
    // std::cout << input.toStdString() << std::endl;
    // QString program = "/home/lain/Desktop/TCC/backup_note/Rubiks-Cube-Solver/format.sh";
    // QStringList arguments;
    // QProcess *myProcess = new QProcess(this);
    // myProcess->start(program,(QStringList) arguments << input );
    
    // std::string line;
    // QString entrada;
    // if (myProcess->waitForFinished())
    // {
    //     myProcess->close();
    //     std::ifstream myfile ("format.txt");
    
    //      if (myfile.is_open())
    //       {
    
    //         while ( std::getline (myfile,line ))
    //         {
    //             std::cout << "size: " << line.size() << std::endl;
    //             if(line.size() == 120)
    //                entrada = QString::fromStdString(line);
    
    //           std::cout << line << '\n';
    //         }
    //         std::cout << "size: " << line.size() << std::endl;
    
    //         myfile.close();
    
    //       }
    //      std::cout << "começa a executar o run" << std::endl;
    // }
    // program = "/home/lain/Desktop/TCC/backup_note/Rubiks-Cube-Solver/run.sh";
    // QStringList arguments2;
    // QProcess *myProcess2 = new QProcess();
    // myProcess2->start(program,(QStringList) arguments2 << entrada );
    // std::cout << "mandou o run do solver" << std::endl;
    // std::ifstream myfile2 ("result.txt");
    // myProcess2->write("exit\n\r");
    // if (myProcess2->waitForFinished(-1))
    // {
    // std::cout << "run ficou pronto run do solver" << std::endl;

    //  if (myfile2.is_open())
    //   {
    //     while ( std::getline (myfile2,line ))
    //     {
    //         if(line.size() == 1 || line.size() == 2)
    //         {
    //             solve(line);
    //         }
    //       std::cout << line << '\n';
    //       std::cout << "size: " << line.size() << std::endl;
    //     }
    //     myfile2.close();
    //   }
    // }
}

void MainWindow::on_colorCalibrationButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        calibrator->calibrate();
        Utils::showDialog(std::string("Calibration done with success."));
    }
    else Utils::showDialog(std::string("Error: The Calibration only works on ROBOT mode."));
}

void MainWindow::on_readColorsButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        reader->read();
        if(cube->isValid()) Utils::showDialog(std::string("Reading done with success."));
        else Utils::showDialog(std::string("Error: The new cube has more than 9 squares of the same color."));
    }
    else Utils::showDialog(std::string("Error: The Reading only works on ROBOT mode."));
}

void MainWindow::on_rotationUButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(Command::DO_U_ROTATION);
        else
        {
            Utils::showDialog("Microcontroller is not available.");
            return;
        }
    }
    cube->rotateU();
    setRotationsNumber(rotationsNumber+1);
}

void MainWindow::on_rotationUAButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(Command::DO_UA_ROTATION);
        else
        {
            Utils::showDialog("Microcontroller is not available.");
            return;
        }
    }
    cube->rotateUA();
    setRotationsNumber(rotationsNumber+1);
}

void MainWindow::on_rotationDButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(Command::DO_D_ROTATION);
        else
        {
            Utils::showDialog("Microcontroller is not available.");
            return;
        }
    }
    cube->rotateD();
    setRotationsNumber(rotationsNumber+1);
}

void MainWindow::on_rotationDAButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(Command::DO_DA_ROTATION);
        else
        {
            Utils::showDialog("Microcontroller is not available.");
            return;
        }
    }
    cube->rotateDA();
    setRotationsNumber(rotationsNumber+1);
}

void MainWindow::on_rotationRButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(Command::DO_R_ROTATION);
        else
        {
            Utils::showDialog("Microcontroller is not available.");
            return;
        }
    }
    cube->rotateR();
    setRotationsNumber(rotationsNumber+1);
}

void MainWindow::on_rotationRAButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(Command::DO_RA_ROTATION);
        else
        {
            Utils::showDialog("Microcontroller is not available.");
            return;
        }
    }
    cube->rotateRA();
    setRotationsNumber(rotationsNumber+1);
}

void MainWindow::on_rotationLButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(Command::DO_L_ROTATION);
        else
        {
            Utils::showDialog("Microcontroller is not available.");
            return;
        }
    }
    cube->rotateL();
    setRotationsNumber(rotationsNumber+1);
}

void MainWindow::on_rotationLAButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(Command::DO_LA_ROTATION);
        else
        {
            Utils::showDialog("Microcontroller is not available.");
            return;
        }
    }
    cube->rotateLA();
    setRotationsNumber(rotationsNumber+1);
}

void MainWindow::on_rotationFButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(Command::DO_F_ROTATION);
        else
        {
            Utils::showDialog("Microcontroller is not available.");
            return;
        }
    }
    cube->rotateF();
    setRotationsNumber(rotationsNumber+1);
}

void MainWindow::on_rotationFAButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(Command::DO_FA_ROTATION);
        else
        {
            Utils::showDialog("Microcontroller is not available.");
            return;
        }
    }
    cube->rotateFA();
    setRotationsNumber(rotationsNumber+1);
}

void MainWindow::on_rotationBButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(Command::DO_B_ROTATION);
        else
        {
            Utils::showDialog("Microcontroller is not available.");
            return;
        }
    }
    cube->rotateB();
    setRotationsNumber(rotationsNumber+1);
}

void MainWindow::on_rotationBAButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        if(microcontroller->isAvailable())
            microcontroller->runCommand(Command::DO_BA_ROTATION);
        else
        {
            Utils::showDialog("Microcontroller is not available.");
            return;
        }
    }
    cube->rotateBA();
    setRotationsNumber(rotationsNumber+1);
}

void MainWindow::on_up9Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_front3Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_right1Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_left3Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_front1Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_up7Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_up3Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_right3Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_back9Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_up1Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_left1Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_back7Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_front9Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_right7Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_down3Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_front7Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_left9Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_down1Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_down9Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_right9Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_back3Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_back1Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_down7Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_left7Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_back8Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_back2Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_back6Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_back4Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_down8Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_down6Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_down4Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_down2Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_front8Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_front4Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_front6Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_front2Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_left8Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_left4Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_left2Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_left6Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_right4Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_right8Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_right6Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_right2Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_up8Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_up4Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_up6Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
void MainWindow::on_up2Button_clicked() { changeColor(qobject_cast<QPushButton*>(sender())); }
