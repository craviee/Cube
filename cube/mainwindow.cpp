#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "identification.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <qthread.h>
#include <fstream>
#include <string>
#include <QTime>
#include <vector>
#include <sstream>
#include <QProcess>
#include <QCamera>
#include <QMessageBox>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVBoxLayout>
#include <QCameraInfo>
#include <QFileDialog>
#include <QBuffer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <QElapsedTimer>

void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}
QSerialPort *arduino = new QSerialPort;
static const quint16 arduino_uno_vendor_id = 9025;
static const quint16 arduino_uno_product_id = 67;
QString arduino_port_name = "";
bool arduino_is_available = false;
bool CalibrationOn = false;
QCamera *mCamera;
std::vector<double> calibrateVector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
QCameraViewfinder *mCameraViewFinder;
QCameraImageCapture *mCameraImageCapture;
int lado = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupSquares();
    cube = std::make_unique<Cube>(squares);
    calibrator = std::make_unique<ColorCalibrator>(squares);
    reader = std::make_unique<ColorReader>(squares, calibrator->configValues);
    cube->initialize();
    setRotationsNumber(0);
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

void MainWindow::showDialog(std::string message)
{
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(message));
    msgBox.exec();
}

//TODO: VALIDATES THE MODEL BEFORE EVERY ALGORITHM

void MainWindow::CruzSul()
{
    if(ui->up8Button->styleSheet() != ui->up5Button->styleSheet() || ui->front2Button->styleSheet() != ui->front5Button->styleSheet())
    {
        if(ui->front2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up8Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateUA();
            cube->rotateR();
            cube->rotateU();
        }
        else if(ui->front6Button->styleSheet() == ui->up5Button->styleSheet() && ui->right4Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateUA();
            cube->rotateR();
            cube->rotateU();
        }
        else if(ui->front6Button->styleSheet() == ui->front5Button->styleSheet() && ui->right4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateFA();
        }
        else if(ui->front4Button->styleSheet() == ui->up5Button->styleSheet() && ui->left6Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateLA();
            cube->rotateUA();
        }
        else if(ui->front4Button->styleSheet() == ui->front5Button->styleSheet() && ui->left6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateF();
        }
        else if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateF();
        }
        else if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down2Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateUA();
            cube->rotateR();
            cube->rotateU();
        }
        else if(ui->right2Button->styleSheet() == ui->front5Button->styleSheet() && ui->up6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateU();
        }
        else if(ui->right2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up6Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateF();
            cube->rotateUA();
            cube->rotateR();
            cube->rotateU();
        }
        else if(ui->left2Button->styleSheet() == ui->front5Button->styleSheet() && ui->up4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateUA();
        }
        else if(ui->left2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up4Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateUA();
            cube->rotateF();
            cube->rotateUA();
            cube->rotateR();
            cube->rotateU();
        }
        else if(ui->right8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateF();
            cube->rotateF();
        }
        else if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down6Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateUA();
            cube->rotateR();
            cube->rotateU();
        }
        else if(ui->left8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateF();
            cube->rotateF();
        }
        else if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down4Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateFA();
            cube->rotateUA();
            cube->rotateR();
            cube->rotateU();
        }
        else if(ui->back8Button->styleSheet() == ui->front5Button->styleSheet() && ui->up2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();
        }
        else if(ui->back8Button->styleSheet() == ui->up5Button->styleSheet() && ui->up2Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();
            cube->rotateF();
            cube->rotateUA();
            cube->rotateR();
            cube->rotateU();
        }
        else if(ui->back6Button->styleSheet() == ui->front5Button->styleSheet() && ui->right6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateU();
            cube->rotateU();
        }
        else if(ui->back6Button->styleSheet() == ui->up5Button->styleSheet() && ui->right6Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateU();
            cube->rotateU();
            cube->rotateF();
            cube->rotateUA();
            cube->rotateR();
            cube->rotateU();
        }
        else if(ui->back4Button->styleSheet() == ui->front5Button->styleSheet() && ui->left4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateBA();
            cube->rotateU();
            cube->rotateU();
        }
        else if(ui->back4Button->styleSheet() == ui->up5Button->styleSheet() && ui->left4Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateBA();
            cube->rotateU();
            cube->rotateU();
            cube->rotateF();
            cube->rotateUA();
            cube->rotateR();
            cube->rotateU();
        }
        else if(ui->back2Button->styleSheet() == ui->front5Button->styleSheet() && ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateF();
            cube->rotateF();
        }
        else if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet() && ui->down8Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateFA();
            cube->rotateUA();
            cube->rotateR();
            cube->rotateU();
        }
    }
}
void MainWindow::CruzLeste()
{
    if(ui->up6Button->styleSheet() != ui->up5Button->styleSheet() || ui->right2Button->styleSheet() != ui->right5Button->styleSheet())
    {
        if(ui->up6Button->styleSheet() == ui->right5Button->styleSheet() && ui->right2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();

            
        }
        else if(ui->right6Button->styleSheet() == ui->up5Button->styleSheet() && ui->back6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();

            
        }
        else if(ui->right6Button->styleSheet() == ui->right5Button->styleSheet() && ui->back6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateRA();

            
        }
        else if(ui->right4Button->styleSheet() == ui->up5Button->styleSheet() && ui->front6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateFA();
            cube->rotateUA();

            
        }
        else if(ui->right4Button->styleSheet() == ui->right5Button->styleSheet() && ui->front6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateR();

            
        }
        else if(ui->right8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateR();

            
        }
        else if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateRA();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();

            
        }
        else if(ui->back8Button->styleSheet() == ui->right5Button->styleSheet() && ui->up2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateBA();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();

            
        }
        else if(ui->back8Button->styleSheet() == ui->up5Button->styleSheet() && ui->up2Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateR();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();

            
        }
        else if(ui->front2Button->styleSheet() == ui->right5Button->styleSheet() && ui->up8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateU();
            cube->rotateF();
            cube->rotateUA();

            
        }
        else if(ui->front2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up8Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateR();

            
        }
        else if(ui->back2Button->styleSheet() == ui->right5Button->styleSheet() && ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateR();
            cube->rotateR();

            
        }
        else if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet() && ui->down8Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();

            
        }
        else if(ui->front8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateR();
            cube->rotateR();

            
        }
        else if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down2Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateRA();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();

            
        }
        else if(ui->left2Button->styleSheet() == ui->right5Button->styleSheet() && ui->up4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateL();
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateR();

            
        }
        else if(ui->left2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();
            cube->rotateR();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();

            
        }
        else if(ui->left4Button->styleSheet() == ui->right5Button->styleSheet() && ui->back4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateU();
            cube->rotateU();

            
        }
        else if(ui->left4Button->styleSheet() == ui->up5Button->styleSheet() && ui->back4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateU();
            cube->rotateU();
            cube->rotateR();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();

            
        }
        else if(ui->left6Button->styleSheet() == ui->right5Button->styleSheet() && ui->front4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            
            cube->rotateU();
            cube->rotateU();
            cube->rotateLA();
            cube->rotateUA();
            cube->rotateUA();

            
        }
        else if(ui->left6Button->styleSheet() == ui->up5Button->styleSheet() && ui->front4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateLA();
            cube->rotateU();
            cube->rotateU();
            cube->rotateR();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();

            
        }
        else if(ui->left8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateR();

            
        }
        else if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateRA();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();

            
        }
    }
}
void MainWindow::CruzOeste()
{
    if(ui->up4Button->styleSheet() != ui->up5Button->styleSheet() || ui->left2Button->styleSheet() != ui->right5Button->styleSheet())
    {
        if(ui->left2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();

            
        }
        else if(ui->left6Button->styleSheet() == ui->up5Button->styleSheet() && ui->front4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();

            
        }
        else if(ui->left6Button->styleSheet() == ui->left5Button->styleSheet() && ui->front4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateLA();

            
        }
        else if(ui->left4Button->styleSheet() == ui->up5Button->styleSheet() && ui->back4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateBA();
            cube->rotateUA();

            
        }
        else if(ui->left4Button->styleSheet() == ui->left5Button->styleSheet() && ui->back4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();

            
        }
        else if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateL();

            
        }
        else if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateLA();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();

            
        }
        else if(ui->front2Button->styleSheet() == ui->left5Button->styleSheet() && ui->up8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateU();

            
        }
        else if(ui->front2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up8Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateL();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();

            
        }
        else if(ui->back8Button->styleSheet() == ui->left5Button->styleSheet() && ui->up2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateUA();

            
        }
        else if(ui->back8Button->styleSheet() == ui->up5Button->styleSheet() && ui->up2Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateUA();
            cube->rotateL();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();

            
        }
        else if(ui->front8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateL();
            cube->rotateL();

            
        }
        else if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down2Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();

            
        }
        else if(ui->back2Button->styleSheet() == ui->left5Button->styleSheet() && ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateL();
            cube->rotateL();

            
        }
        else if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet() && ui->down8Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateLA();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();

            
        }
        else if(ui->right2Button->styleSheet() == ui->left5Button->styleSheet() && ui->up6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();

            
        }
        else if(ui->right2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();
            cube->rotateL();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();

            
        }
        else if(ui->right4Button->styleSheet() == ui->left5Button->styleSheet() && ui->front6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();
            cube->rotateR();
            cube->rotateUA();
            cube->rotateUA();

            
        }
        else if(ui->right4Button->styleSheet() == ui->up5Button->styleSheet() && ui->front6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateU();
            cube->rotateU();
            cube->rotateL();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();

            
        }
        else if(ui->right6Button->styleSheet() == ui->left5Button->styleSheet() && ui->back6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateD();
            cube->rotateD();
            cube->rotateL();
            cube->rotateL();

            
        }
        else if(ui->right6Button->styleSheet() == ui->up5Button->styleSheet() && ui->back6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateRA();
            cube->rotateU();
            cube->rotateU();
            cube->rotateL();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();

            
        }
        else if(ui->right8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateL();
            cube->rotateL();

            
        }
        else if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();

            
        }
    }
}
void MainWindow::CruzNorte()
{
    if(ui->up2Button->styleSheet() != ui->up5Button->styleSheet() || ui->back8Button->styleSheet() != ui->back5Button->styleSheet())
    {
        if(ui->up2Button->styleSheet() == ui->back5Button->styleSheet() && ui->back8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();

            
        }
        else if(ui->back4Button->styleSheet() == ui->up5Button->styleSheet() && ui->left4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();

            
        }
        else if(ui->back4Button->styleSheet() == ui->back5Button->styleSheet() && ui->left4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateBA();

            
        }
        else if(ui->back6Button->styleSheet() == ui->up5Button->styleSheet() && ui->right6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateRA();
            cube->rotateUA();

            
        }
        else if(ui->back6Button->styleSheet() == ui->back5Button->styleSheet() && ui->right6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateB();

            
        }
        else if(ui->back2Button->styleSheet() == ui->back5Button->styleSheet() && ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateB();

            
        }
        else if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet() && ui->down8Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateBA();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();

            
        }
        else if(ui->left2Button->styleSheet() == ui->back5Button->styleSheet() && ui->up4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateLA();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();

            
        }
        else if(ui->left2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateB();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();

            
        }
        else if(ui->right2Button->styleSheet() == ui->back5Button->styleSheet() && ui->up6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateU();
            cube->rotateRA();
            cube->rotateUA();

            
        }
        else if(ui->right2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateUA();
            cube->rotateB();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();

            
        }
        else if(ui->left8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateB();
            cube->rotateB();

            
        }
        else if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();

            
        }
        else if(ui->right8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateB();
            cube->rotateB();

            
        }
        else if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateBA();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();

            
        }
        else if(ui->front2Button->styleSheet() == ui->back5Button->styleSheet() && ui->up8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();

            
        }
        else if(ui->front2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up8Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();
            cube->rotateB();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();

            
        }
        else if(ui->front4Button->styleSheet() == ui->back5Button->styleSheet() && ui->left6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateU();
            cube->rotateU();

            
        }
        else if(ui->front4Button->styleSheet() == ui->up5Button->styleSheet() && ui->left6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateU();
            cube->rotateU();
            cube->rotateB();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();

            
        }
        else if(ui->front6Button->styleSheet() == ui->back5Button->styleSheet() && ui->right4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateU();
            cube->rotateU();

            
        }
        else if(ui->front6Button->styleSheet() == ui->up5Button->styleSheet() && ui->right4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateU();
            cube->rotateU();
            cube->rotateB();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();

            
        }
        else if(ui->front8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateF();
            cube->rotateF();
            cube->rotateBA();
            cube->rotateBA();

            
        }
        else if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down2Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();

            
        }
    }
}
void MainWindow::QuinaSudeste()
{
    if(ui->up9Button->styleSheet() != ui->up5Button->styleSheet() || ui->front3Button->styleSheet() != ui->front5Button->styleSheet() || ui->right1Button->styleSheet() != ui->right5Button->styleSheet())
    {
        if(ui->up9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateR();
            if(ui->front7Button->styleSheet() == ui->front5Button->styleSheet()) 
            {
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
            }
            else if(ui->front7Button->styleSheet() == ui->left5Button->styleSheet()) 
            {
                cube->rotateDA();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateD();
                cube->rotateL();
            }
            else if(ui->front7Button->styleSheet() == ui->back5Button->styleSheet()) 
            {
                cube->rotateD();
                cube->rotateBA();
                cube->rotateD();
                cube->rotateB();
            }
        }
        else if(ui->front3Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateD();
            cube->rotateFA();
            if(ui->back3Button->styleSheet() == ui->front5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
            }
            else if(ui->back3Button->styleSheet() == ui->left5Button->styleSheet())
            {
                cube->rotateLA();
                cube->rotateD();
                cube->rotateL();
            }
            else if(ui->back3Button->styleSheet() == ui->right5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateD();
                cube->rotateR();
            }
            else if(ui->back3Button->styleSheet() == ui->back5Button->styleSheet())  
            {
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateD();
                cube->rotateB();
            }
        }
        else if(ui->right1Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
            if(ui->front9Button->styleSheet() == ui->front5Button->styleSheet()) 
            {
                cube->rotateD();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
            }
            else if(ui->front9Button->styleSheet() == ui->left5Button->styleSheet()) 
            {
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
            }
            else if(ui->front9Button->styleSheet() == ui->right5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateD();
                cube->rotateR();
                cube->rotateDA();
                cube->rotateRA();
            }
            else if(ui->front9Button->styleSheet() == ui->back5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateB();
                cube->rotateDA();
                cube->rotateBA();
            }
        }
    }
}
void MainWindow::QuinaSudoeste()
{
    if(ui->up7Button->styleSheet() != ui->up5Button->styleSheet() || ui->front1Button->styleSheet() != ui->front5Button->styleSheet() || ui->left3Button->styleSheet() != ui->left5Button->styleSheet())
    {
        if(ui->up7Button->styleSheet() == ui->front5Button->styleSheet() && ui->front1Button->styleSheet() == ui->left5Button->styleSheet() && ui->left3Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
        }
        else if(ui->up7Button->styleSheet() == ui->left5Button->styleSheet() && ui->front1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left3Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateDA();
            cube->rotateF();
            cube->rotateD();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
        }
        else if(ui->up7Button->styleSheet() == ui->up5Button->styleSheet() && ui->front1Button->styleSheet() == ui->right5Button->styleSheet() && ui->left3Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->up7Button->styleSheet() == ui->right5Button->styleSheet() && ui->front1Button->styleSheet() == ui->front5Button->styleSheet() && ui->left3Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
        }
        else if(ui->up7Button->styleSheet() == ui->front5Button->styleSheet() && ui->front1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left3Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateDA();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->up7Button->styleSheet() == ui->up5Button->styleSheet() && ui->front1Button->styleSheet() == ui->back5Button->styleSheet() && ui->left3Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
        else if(ui->up7Button->styleSheet() == ui->right5Button->styleSheet() && ui->front1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left3Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateDA();
            cube->rotateF();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
        else if(ui->up7Button->styleSheet() == ui->back5Button->styleSheet() && ui->front1Button->styleSheet() == ui->right5Button->styleSheet() && ui->left3Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->up7Button->styleSheet() == ui->up5Button->styleSheet() && ui->front1Button->styleSheet() == ui->left5Button->styleSheet() && ui->left3Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
        }
        else if(ui->up7Button->styleSheet() == ui->left5Button->styleSheet() && ui->front1Button->styleSheet() == ui->back5Button->styleSheet() && ui->left3Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
        }
        else if(ui->up7Button->styleSheet() == ui->back5Button->styleSheet() && ui->front1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left3Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateDA();
            cube->rotateF();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
    }
}
void MainWindow::QuinaNordeste()
{
    if(ui->up3Button->styleSheet() != ui->up5Button->styleSheet() || ui->right3Button->styleSheet() != ui->right5Button->styleSheet() || ui->back9Button->styleSheet() != ui->back5Button->styleSheet())
    {
        if(ui->up3Button->styleSheet() == ui->right5Button->styleSheet() && ui->right3Button->styleSheet() == ui->back5Button->styleSheet() && ui->back9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateB();
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
        else if(ui->up3Button->styleSheet() == ui->back5Button->styleSheet() && ui->right3Button->styleSheet() == ui->up5Button->styleSheet() && ui->back9Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateD();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->up3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right3Button->styleSheet() == ui->front5Button->styleSheet() && ui->back9Button->styleSheet() == ui->right5Button->styleSheet())
        {
             cube->rotateBA();
             cube->rotateDA();
             cube->rotateB();
             cube->rotateDA();
             cube->rotateRA();
             cube->rotateD();
             cube->rotateR();
        }
        else if(ui->up3Button->styleSheet() == ui->right5Button->styleSheet() && ui->right3Button->styleSheet() == ui->up5Button->styleSheet() && ui->back9Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateD();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
        }
        else if(ui->up3Button->styleSheet() == ui->front5Button->styleSheet() && ui->right3Button->styleSheet() == ui->right5Button->styleSheet() && ui->back9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateB();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->up3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right3Button->styleSheet() == ui->left5Button->styleSheet() && ui->back9Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
        }
        else if(ui->up3Button->styleSheet() == ui->left5Button->styleSheet() && ui->right3Button->styleSheet() == ui->front5Button->styleSheet() && ui->back9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateB();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
        }
        else if(ui->up3Button->styleSheet() == ui->front5Button->styleSheet() && ui->right3Button->styleSheet() == ui->up5Button->styleSheet() && ui->back9Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateD();
            cube->rotateRA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
        }
        else if(ui->up3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right3Button->styleSheet() == ui->back5Button->styleSheet() && ui->back9Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateD();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else if(ui->up3Button->styleSheet() == ui->left5Button->styleSheet() && ui->right3Button->styleSheet() == ui->up5Button->styleSheet() && ui->back9Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateRA();
        }
        else if(ui->up3Button->styleSheet() == ui->back5Button->styleSheet() && ui->right3Button->styleSheet() == ui->left5Button->styleSheet() && ui->back9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
    }
}
void MainWindow::QuinaNoroeste()
{
    if(ui->up1Button->styleSheet() != ui->up5Button->styleSheet() || ui->left1Button->styleSheet() != ui->left5Button->styleSheet() || ui->back7Button->styleSheet() != ui->back5Button->styleSheet())
    {
        if(ui->up1Button->styleSheet() == ui->left5Button->styleSheet() && ui->left1Button->styleSheet() == ui->back5Button->styleSheet() && ui->back7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
        }
        else if(ui->up1Button->styleSheet() == ui->back5Button->styleSheet() && ui->left1Button->styleSheet() == ui->up5Button->styleSheet() && ui->back7Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateL();
            cube->rotateD();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else if(ui->up1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left1Button->styleSheet() == ui->right5Button->styleSheet() && ui->back7Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->up1Button->styleSheet() == ui->right5Button->styleSheet() && ui->left1Button->styleSheet() == ui->front5Button->styleSheet() && ui->back7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
        }
        else if(ui->up1Button->styleSheet() == ui->front5Button->styleSheet() && ui->left1Button->styleSheet() == ui->up5Button->styleSheet() && ui->back7Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateL();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->up1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left1Button->styleSheet() == ui->front5Button->styleSheet() && ui->back7Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
        }
        else if(ui->up1Button->styleSheet() == ui->front5Button->styleSheet() && ui->left1Button->styleSheet() == ui->left5Button->styleSheet() && ui->back7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
        }
        else if(ui->up1Button->styleSheet() == ui->left5Button->styleSheet() && ui->left1Button->styleSheet() == ui->up5Button->styleSheet() && ui->back7Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateLA();
            cube->rotateD();
            cube->rotateD();
            cube->rotateL();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
        }
        else if(ui->up1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left1Button->styleSheet() == ui->back5Button->styleSheet() && ui->back7Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->up1Button->styleSheet() == ui->back5Button->styleSheet() && ui->left1Button->styleSheet() == ui->right5Button->styleSheet() && ui->back7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->up1Button->styleSheet() == ui->right5Button->styleSheet() && ui->left1Button->styleSheet() == ui->up5Button->styleSheet() && ui->back7Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateL();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
    }
}
void MainWindow::SudesteBaixo()
{
    if(ui->front9Button->styleSheet() == ui->up5Button->styleSheet() || ui->down3Button->styleSheet() == ui->up5Button->styleSheet() || ui->right7Button->styleSheet() == ui->up5Button->styleSheet())
    {
        if(ui->front9Button->styleSheet() == ui->front5Button->styleSheet() && ui->down3Button->styleSheet() == ui->right5Button->styleSheet() && ui->right7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->front9Button->styleSheet() == ui->up5Button->styleSheet() && ui->down3Button->styleSheet() == ui->front5Button->styleSheet() && ui->right7Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
        }
        else if(ui->front9Button->styleSheet() == ui->right5Button->styleSheet() && ui->down3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right7Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
        }
        else if(ui->front9Button->styleSheet() == ui->front5Button->styleSheet() && ui->down3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right7Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
        }
        else if(ui->front9Button->styleSheet() == ui->up5Button->styleSheet() && ui->down3Button->styleSheet() == ui->left5Button->styleSheet() && ui->right7Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
        }
        else if(ui->front9Button->styleSheet() == ui->left5Button->styleSheet() && ui->down3Button->styleSheet() == ui->front5Button->styleSheet() && ui->right7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
        }
        else if(ui->front9Button->styleSheet() == ui->left5Button->styleSheet() && ui->down3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right7Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
        }
        else if(ui->front9Button->styleSheet() == ui->up5Button->styleSheet() && ui->down3Button->styleSheet() == ui->back5Button->styleSheet() && ui->right7Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
        }
        else if(ui->front9Button->styleSheet() == ui->back5Button->styleSheet() && ui->down3Button->styleSheet() == ui->left5Button->styleSheet() && ui->right7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else if(ui->front9Button->styleSheet() == ui->back5Button->styleSheet() && ui->down3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right7Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
        else if(ui->front9Button->styleSheet() == ui->up5Button->styleSheet() && ui->down3Button->styleSheet() == ui->right5Button->styleSheet() && ui->right7Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->front9Button->styleSheet() == ui->right5Button->styleSheet() && ui->down3Button->styleSheet() == ui->back5Button->styleSheet() && ui->right7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
    }
}
void MainWindow::SudoesteBaixo()
{
    if(ui->front7Button->styleSheet() == ui->up5Button->styleSheet() || ui->down1Button->styleSheet() == ui->up5Button->styleSheet() || ui->left9Button->styleSheet() == ui->up5Button->styleSheet())
    {
        if(ui->front7Button->styleSheet() == ui->front5Button->styleSheet() && ui->down1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left9Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->front7Button->styleSheet() == ui->up5Button->styleSheet() && ui->down1Button->styleSheet() == ui->right5Button->styleSheet() && ui->left9Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->front7Button->styleSheet() == ui->right5Button->styleSheet() && ui->down1Button->styleSheet() == ui->front5Button->styleSheet() && ui->left9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
        }
        else if(ui->front7Button->styleSheet() == ui->up5Button->styleSheet() && ui->down1Button->styleSheet() == ui->front5Button->styleSheet() && ui->left9Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
        }
        else if(ui->front7Button->styleSheet() == ui->front5Button->styleSheet() && ui->down1Button->styleSheet() == ui->left5Button->styleSheet() && ui->left9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
        }
        else if(ui->front7Button->styleSheet() == ui->left5Button->styleSheet() && ui->down1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left9Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
        }
        else if(ui->front7Button->styleSheet() == ui->back5Button->styleSheet() && ui->down1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left9Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
        }
        else if(ui->front7Button->styleSheet() == ui->up5Button->styleSheet() && ui->down1Button->styleSheet() == ui->left5Button->styleSheet() && ui->left9Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else if(ui->front7Button->styleSheet() == ui->left5Button->styleSheet() && ui->down1Button->styleSheet() == ui->back5Button->styleSheet() && ui->left9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
        }
        else if(ui->front7Button->styleSheet() == ui->back5Button->styleSheet() && ui->down1Button->styleSheet() == ui->right5Button->styleSheet() && ui->left9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->front7Button->styleSheet() == ui->right5Button->styleSheet() && ui->down1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left9Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
        else if(ui->front7Button->styleSheet() == ui->up5Button->styleSheet() && ui->down1Button->styleSheet() == ui->back5Button->styleSheet() && ui->left9Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
    }
}
void MainWindow::frontSecond()
{
    if(ui->front4Button->styleSheet() != ui->front5Button->styleSheet() || ui->front6Button->styleSheet() != ui->front5Button->styleSheet() ||
            ui->left6Button->styleSheet() != ui->left5Button->styleSheet() || ui->right4Button->styleSheet() != ui->right5Button->styleSheet())
    {
        if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down2Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
        }
        else if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down2Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->left8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
        }
        else if(ui->left8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->right8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
        }
        else if(ui->right8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->back2Button->styleSheet() == ui->front5Button->styleSheet() && ui->down8Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
        }
        else if(ui->back2Button->styleSheet() == ui->front5Button->styleSheet() && ui->down8Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
    }
}
void MainWindow::leftSecond()
{
    if(ui->left4Button->styleSheet() != ui->left5Button->styleSheet() || ui->left6Button->styleSheet() != ui->left5Button->styleSheet() ||
            ui->front4Button->styleSheet() != ui->front5Button->styleSheet() || ui->back4Button->styleSheet() != ui->back5Button->styleSheet())
    {
        if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();

            
        }
        else if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down4Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();

            
        }
        else if(ui->back2Button->styleSheet() == ui->left5Button->styleSheet() && ui->down8Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();

            
        }
        else if(ui->back2Button->styleSheet() == ui->left5Button->styleSheet() && ui->down8Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();

            
        }
        else if(ui->front8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down2Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();

            
        }
        else if(ui->front8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down2Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();

            
        }
        else if(ui->right8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();

            
        }
        else if(ui->right8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down6Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();

            
        }
    }
}
void MainWindow::rightSecond()
{
    if(ui->right4Button->styleSheet() != ui->right5Button->styleSheet() || ui->right6Button->styleSheet() != ui->right5Button->styleSheet() ||
            ui->back6Button->styleSheet() != ui->back5Button->styleSheet() || ui->front6Button->styleSheet() != ui->front5Button->styleSheet())
    {
        if(ui->right8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down6Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
        }
        else if(ui->right8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
        else if(ui->front8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down2Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
        }
        else if(ui->front8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down2Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
        else if(ui->back2Button->styleSheet() == ui->right5Button->styleSheet() && ui->down8Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
        }
        else if(ui->back2Button->styleSheet() == ui->right5Button->styleSheet() && ui->down8Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
        else if(ui->left8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down4Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
        }
        else if(ui->left8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
    }
}
void MainWindow::backSecond()
{
    if(ui->back4Button->styleSheet() != ui->back5Button->styleSheet() || ui->back6Button->styleSheet() != ui->back5Button->styleSheet() ||
            ui->left4Button->styleSheet() != ui->left5Button->styleSheet() || ui->right6Button->styleSheet() != ui->right5Button->styleSheet())
    {
        if(ui->back2Button->styleSheet() == ui->back5Button->styleSheet() && ui->down8Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->back2Button->styleSheet() == ui->back5Button->styleSheet() && ui->down8Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else if(ui->right8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->right8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else if(ui->left8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->left8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else if(ui->front8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down2Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->front8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down2Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
    }
}
void MainWindow::NordesteBaixo()
{
    if(ui->back3Button->styleSheet() == ui->up5Button->styleSheet() || ui->down9Button->styleSheet() == ui->up5Button->styleSheet() || ui->right9Button->styleSheet() == ui->up5Button->styleSheet())
    {
       if(ui->back3Button->styleSheet() == ui->front5Button->styleSheet() && ui->down9Button->styleSheet() == ui->up5Button->styleSheet() && ui->right9Button->styleSheet() == ui->right5Button->styleSheet())
       {
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
       }
       else if(ui->back3Button->styleSheet() == ui->up5Button->styleSheet() && ui->down9Button->styleSheet() == ui->right5Button->styleSheet() && ui->right9Button->styleSheet() == ui->front5Button->styleSheet())
       {
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
       }
       else if(ui->back3Button->styleSheet() == ui->right5Button->styleSheet() && ui->down9Button->styleSheet() == ui->front5Button->styleSheet() && ui->right9Button->styleSheet() == ui->up5Button->styleSheet())
       {
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
       }
       else if(ui->back3Button->styleSheet() == ui->up5Button->styleSheet() && ui->down9Button->styleSheet() == ui->front5Button->styleSheet() && ui->right9Button->styleSheet() == ui->left5Button->styleSheet())
       {
            cube->rotateDA();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
       }
       else if(ui->back3Button->styleSheet() == ui->front5Button->styleSheet() && ui->down9Button->styleSheet() == ui->left5Button->styleSheet() && ui->right9Button->styleSheet() == ui->up5Button->styleSheet())
       {
            cube->rotateD();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
       }
       else if(ui->back3Button->styleSheet() == ui->left5Button->styleSheet() && ui->down9Button->styleSheet() == ui->up5Button->styleSheet() && ui->right9Button->styleSheet() == ui->front5Button->styleSheet())
       {
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
       }
       else if(ui->back3Button->styleSheet() == ui->back5Button->styleSheet() && ui->down9Button->styleSheet() == ui->up5Button->styleSheet() && ui->right9Button->styleSheet() == ui->left5Button->styleSheet())
       {
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
       }
       else if(ui->back3Button->styleSheet() == ui->up5Button->styleSheet() && ui->down9Button->styleSheet() == ui->left5Button->styleSheet() && ui->right9Button->styleSheet() == ui->back5Button->styleSheet())
       {
            cube->rotateD();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
       }
       else if(ui->back3Button->styleSheet() == ui->left5Button->styleSheet() && ui->down9Button->styleSheet() == ui->back5Button->styleSheet() && ui->right9Button->styleSheet() == ui->up5Button->styleSheet())
       {
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
       }
       else if(ui->back3Button->styleSheet() == ui->back5Button->styleSheet() && ui->down9Button->styleSheet() == ui->right5Button->styleSheet() && ui->right9Button->styleSheet() == ui->up5Button->styleSheet())
       {
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
       }
       else if(ui->back3Button->styleSheet() == ui->right5Button->styleSheet() && ui->down9Button->styleSheet() == ui->up5Button->styleSheet() && ui->right9Button->styleSheet() == ui->back5Button->styleSheet())
       {
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
       }
       else if(ui->back3Button->styleSheet() == ui->up5Button->styleSheet() && ui->down9Button->styleSheet() == ui->back5Button->styleSheet() && ui->right9Button->styleSheet() == ui->right5Button->styleSheet())
       {
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
       }
    }
}
void MainWindow::NoroesteBaixo()
{
    if(ui->back1Button->styleSheet() == ui->up5Button->styleSheet() || ui->down7Button->styleSheet() == ui->up5Button->styleSheet() || ui->left7Button->styleSheet() == ui->up5Button->styleSheet())
    {
        if(ui->back1Button->styleSheet() == ui->front5Button->styleSheet() && ui->down7Button->styleSheet() == ui->right5Button->styleSheet() && ui->left7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->back1Button->styleSheet() == ui->up5Button->styleSheet() && ui->down7Button->styleSheet() == ui->front5Button->styleSheet() && ui->left7Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();
        }
        else if(ui->back1Button->styleSheet() == ui->right5Button->styleSheet() && ui->down7Button->styleSheet() == ui->up5Button->styleSheet() && ui->left7Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
        }
        else if(ui->back1Button->styleSheet() == ui->front5Button->styleSheet() && ui->down7Button->styleSheet() == ui->up5Button->styleSheet() && ui->left7Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
        }
        else if(ui->back1Button->styleSheet() == ui->up5Button->styleSheet() && ui->down7Button->styleSheet() == ui->left5Button->styleSheet() && ui->left7Button->styleSheet() == ui->front5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();
        }
        else if(ui->back1Button->styleSheet() == ui->left5Button->styleSheet() && ui->down7Button->styleSheet() == ui->front5Button->styleSheet() && ui->left7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
        }
        else if(ui->back1Button->styleSheet() == ui->left5Button->styleSheet() && ui->down7Button->styleSheet() == ui->up5Button->styleSheet() && ui->left7Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
        }
        else if(ui->back1Button->styleSheet() == ui->up5Button->styleSheet() && ui->down7Button->styleSheet() == ui->back5Button->styleSheet() && ui->left7Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
        }
        else if(ui->back1Button->styleSheet() == ui->back5Button->styleSheet() && ui->down7Button->styleSheet() == ui->left5Button->styleSheet() && ui->left7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else if(ui->back1Button->styleSheet() == ui->back5Button->styleSheet() && ui->down7Button->styleSheet() == ui->up5Button->styleSheet() && ui->left7Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->back1Button->styleSheet() == ui->up5Button->styleSheet() && ui->down7Button->styleSheet() == ui->right5Button->styleSheet() && ui->left7Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->back1Button->styleSheet() == ui->right5Button->styleSheet() && ui->down7Button->styleSheet() == ui->back5Button->styleSheet() && ui->left7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
        }
    }
}
void MainWindow::debugaFront()
{
    if(ui->front4Button->styleSheet() != ui->front5Button->styleSheet() || ui->front6Button->styleSheet() != ui->front5Button->styleSheet() ||
                ui->left6Button->styleSheet() != ui->left5Button->styleSheet() || ui->right4Button->styleSheet() != ui->right5Button->styleSheet())
        {
            if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
            }
            else if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateD();
                cube->rotateR();
                cube->rotateD();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
            }
            else if(ui->left8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateD();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
            }
            else if(ui->left8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateD();
                cube->rotateR();
                cube->rotateD();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
            }
            else if(ui->right8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateD();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
            }
            else if(ui->right8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateD();
                cube->rotateR();
                cube->rotateD();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
            }
            else if(ui->back2Button->styleSheet() == ui->front5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateD();
                cube->rotateD();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
            }
            else if(ui->back2Button->styleSheet() == ui->front5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateD();
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateD();
                cube->rotateR();
                cube->rotateD();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
            }
            else if(ui->front4Button->styleSheet() == ui->left5Button->styleSheet() && ui->left6Button->styleSheet() == ui->front5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
            }
            else if(ui->front6Button->styleSheet() == ui->right5Button->styleSheet() && ui->right4Button->styleSheet() == ui->front5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateD();
                cube->rotateR();
                cube->rotateD();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
            }
            else
            {
                cube->rotateD();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
            }
        }
}
void MainWindow::debugaLeft()
{
    if(ui->left4Button->styleSheet() != ui->left5Button->styleSheet() || ui->left6Button->styleSheet() != ui->left5Button->styleSheet() ||
                ui->front4Button->styleSheet() != ui->front5Button->styleSheet() || ui->back4Button->styleSheet() != ui->back5Button->styleSheet())
        {
            if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateB();
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateD();
                cube->rotateL();
            }
            else if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
                cube->rotateD();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
            }
            else if(ui->back2Button->styleSheet() == ui->left5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateD();
                cube->rotateB();
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateD();
                cube->rotateL();
            }
            else if(ui->back2Button->styleSheet() == ui->left5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
                cube->rotateD();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
            }
            else if(ui->front8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateD();
                cube->rotateB();
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateD();
                cube->rotateL();
            }
            else if(ui->front8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
                cube->rotateD();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
            }
            else if(ui->right8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateD();
                cube->rotateD();
                cube->rotateB();
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateD();
                cube->rotateL();
            }
            else if(ui->right8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateD();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
                cube->rotateD();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
            }
            else if(ui->left4Button->styleSheet() == ui->down5Button->styleSheet() && ui->down4Button->styleSheet() == ui->left5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateB();
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateD();
                cube->rotateL();
            }
            else if(ui->left6Button->styleSheet() == ui->front5Button->styleSheet() && ui->front4Button->styleSheet() == ui->left5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateD();
                cube->rotateF();
                cube->rotateD();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
            }
            else
            {
                cube->rotateD();
                cube->rotateB();
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateD();
                cube->rotateL();
            }
        }
}
void MainWindow::debugaRight()
{
    if(ui->right4Button->styleSheet() != ui->right5Button->styleSheet() || ui->right6Button->styleSheet() != ui->right5Button->styleSheet() ||
                ui->back6Button->styleSheet() != ui->back5Button->styleSheet() || ui->front6Button->styleSheet() != ui->front5Button->styleSheet())
        {
            if(ui->right8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateD();
                cube->rotateR();
            }
            else if(ui->right8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateD();
                cube->rotateB();
                cube->rotateD();
                cube->rotateR();
                cube->rotateDA();
                cube->rotateRA();
            }
            else if(ui->front8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateD();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateD();
                cube->rotateR();
            }
            else if(ui->front8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateD();
                cube->rotateB();
                cube->rotateD();
                cube->rotateR();
                cube->rotateDA();
                cube->rotateRA();
            }
            else if(ui->back2Button->styleSheet() == ui->right5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateD();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateD();
                cube->rotateR();
            }
            else if(ui->back2Button->styleSheet() == ui->right5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateD();
                cube->rotateB();
                cube->rotateD();
                cube->rotateR();
                cube->rotateDA();
                cube->rotateRA();
            }
            else if(ui->left8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateD();
                cube->rotateD();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateD();
                cube->rotateR();
            }
            else if(ui->left8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateD();
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateD();
                cube->rotateB();
                cube->rotateD();
                cube->rotateR();
                cube->rotateDA();
                cube->rotateRA();
            }
            else if(ui->right4Button->styleSheet() == ui->front5Button->styleSheet() && ui->front6Button->styleSheet() == ui->right5Button->styleSheet())
            {
                cube->rotateD();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateD();
                cube->rotateR();
            }
            else if(ui->right6Button->styleSheet() == ui->down5Button->styleSheet() && ui->down6Button->styleSheet() == ui->right5Button->styleSheet())
            {
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateD();
                cube->rotateB();
                cube->rotateD();
                cube->rotateR();
                cube->rotateDA();
                cube->rotateRA();
            }
            else
            {
                cube->rotateD();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateD();
                cube->rotateR();
            }
        }
}
void MainWindow::debugaBack()
{
    if(ui->back4Button->styleSheet() != ui->back5Button->styleSheet() || ui->back6Button->styleSheet() != ui->back5Button->styleSheet() ||
                ui->left4Button->styleSheet() != ui->left5Button->styleSheet() || ui->right6Button->styleSheet() != ui->right5Button->styleSheet())
    {
        if(ui->back2Button->styleSheet() == ui->back5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->back2Button->styleSheet() == ui->back5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else if(ui->right8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->right8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else if(ui->left8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->left8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else if(ui->front8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->front8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else if(ui->back6Button->styleSheet() == ui->right5Button->styleSheet() && ui->right6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
        else if(ui->back4Button->styleSheet() == ui->left5Button->styleSheet() && ui->left4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
        }
        else
        {
            cube->rotateD();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();
        }
    }
}
bool MainWindow::sudestecorreto()
{
    if((ui->front9Button->styleSheet() == ui->front5Button->styleSheet() || ui->front9Button->styleSheet() == ui->down5Button->styleSheet() || ui->front9Button->styleSheet() == ui->right5Button->styleSheet()) &&
            (ui->down3Button->styleSheet() == ui->front5Button->styleSheet() || ui->down3Button->styleSheet() == ui->down5Button->styleSheet() || ui->down3Button->styleSheet() == ui->right5Button->styleSheet()) &&
            (ui->right7Button->styleSheet() == ui->front5Button->styleSheet() || ui->right7Button->styleSheet() == ui->down5Button->styleSheet() || ui->right7Button->styleSheet() == ui->right5Button->styleSheet()))
        return true;
    else
        return false;
}
bool MainWindow::sudoestecorreto()
{
    if((ui->front7Button->styleSheet() == ui->front5Button->styleSheet() || ui->front7Button->styleSheet() == ui->down5Button->styleSheet() || ui->front7Button->styleSheet() == ui->left5Button->styleSheet()) &&
            (ui->down1Button->styleSheet() == ui->front5Button->styleSheet() || ui->down1Button->styleSheet() == ui->down5Button->styleSheet() || ui->down1Button->styleSheet() == ui->left5Button->styleSheet()) &&
            (ui->left9Button->styleSheet() == ui->front5Button->styleSheet() || ui->left9Button->styleSheet() == ui->down5Button->styleSheet() || ui->left9Button->styleSheet() == ui->left5Button->styleSheet()))
        return true;
    else
        return false;
}
bool MainWindow::nordestecorreto()
{
    if((ui->back3Button->styleSheet() == ui->back5Button->styleSheet() || ui->back3Button->styleSheet() == ui->down5Button->styleSheet() || ui->back3Button->styleSheet() == ui->right5Button->styleSheet()) &&
                    (ui->down9Button->styleSheet() == ui->back5Button->styleSheet() || ui->down9Button->styleSheet() == ui->down5Button->styleSheet() || ui->down9Button->styleSheet() == ui->right5Button->styleSheet()) &&
                    (ui->right9Button->styleSheet() == ui->back5Button->styleSheet() || ui->right9Button->styleSheet() == ui->down5Button->styleSheet() || ui->right9Button->styleSheet() == ui->right5Button->styleSheet()))
        return true;
    else
        return false;
}
bool MainWindow::noroestecorreto()
{
    if((ui->back1Button->styleSheet() == ui->back5Button->styleSheet() || ui->back1Button->styleSheet() == ui->down5Button->styleSheet() || ui->back1Button->styleSheet() == ui->left5Button->styleSheet()) &&
            (ui->down7Button->styleSheet() == ui->back5Button->styleSheet() || ui->down7Button->styleSheet() == ui->down5Button->styleSheet() || ui->down7Button->styleSheet() == ui->left5Button->styleSheet()) &&
            (ui->left7Button->styleSheet() == ui->back5Button->styleSheet() || ui->left7Button->styleSheet() == ui->down5Button->styleSheet() || ui->left7Button->styleSheet() == ui->left5Button->styleSheet()))
        return true;
    else
        return false;
}
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

void MainWindow::on_randomButton_clicked() { cube->randomize(); }

void MainWindow::on_restartButton_clicked() { cube->initialize(); setRotationsNumber(0); }

void MainWindow::on_toggleModeButton_clicked()
{
    mode = mode == Mode::SIMULATION ? Mode::ROBOT : Mode::SIMULATION;
    ui->modeTextBox->setPlainText(QString(mode == Mode::SIMULATION ? "SIMULATION" : "ROBOT"));
}

void MainWindow::on_crossButton_clicked()
{
    while(ui->up2Button->styleSheet() != ui->up5Button->styleSheet() || ui->back8Button->styleSheet() != ui->back5Button->styleSheet() ||
          ui->up4Button->styleSheet() != ui->up5Button->styleSheet() || ui->left2Button->styleSheet() != ui->left5Button->styleSheet() ||
          ui->up6Button->styleSheet() != ui->up5Button->styleSheet() || ui->right2Button->styleSheet() != ui->right5Button->styleSheet() ||
          ui->up8Button->styleSheet() != ui->up5Button->styleSheet() || ui->front2Button->styleSheet() != ui->front5Button->styleSheet() )
    {
        CruzSul();
        CruzNorte();
        
        
        CruzLeste();
        CruzOeste();
     }
    
    
    
    
    
    
}

void MainWindow::on_firstLayerCornersButton_clicked()
{
    while(ui->up1Button->styleSheet() != ui->up5Button->styleSheet() || ui->back7Button->styleSheet() != ui->back5Button->styleSheet() || ui->left1Button->styleSheet() != ui->left5Button->styleSheet()
          || ui->up3Button->styleSheet() != ui->up5Button->styleSheet() || ui->back9Button->styleSheet() != ui->back5Button->styleSheet() || ui->right3Button->styleSheet() != ui->right5Button->styleSheet()
          || ui->up7Button->styleSheet() != ui->up5Button->styleSheet() || ui->front1Button->styleSheet() != ui->front5Button->styleSheet() || ui->left3Button->styleSheet() != ui->left5Button->styleSheet()
          || ui->up9Button->styleSheet() != ui->up5Button->styleSheet() || ui->front3Button->styleSheet() != ui->front5Button->styleSheet() || ui->right1Button->styleSheet() != ui->right5Button->styleSheet())
    {
        QuinaNordeste();
        QuinaNoroeste();
        QuinaSudeste();
        QuinaSudoeste();
        SudesteBaixo();
        SudoesteBaixo();
        NordesteBaixo();
        NoroesteBaixo();
    }
}

void MainWindow::on_secondLayerButton_clicked()
{
    int j=0;
    int k=0;
    while(ui->front4Button->styleSheet() != ui->front5Button->styleSheet() || ui->front6Button->styleSheet() != ui->front5Button->styleSheet() ||
          ui->left4Button->styleSheet() != ui->left5Button->styleSheet() || ui->left6Button->styleSheet() != ui->left5Button->styleSheet() ||
          ui->right4Button->styleSheet() != ui->right5Button->styleSheet() || ui->right6Button->styleSheet() != ui->right5Button->styleSheet() ||
          ui->back4Button->styleSheet() != ui->back5Button->styleSheet() || ui->back6Button->styleSheet() != ui->back5Button->styleSheet())
    {
        
        
            rightSecond();
        
            frontSecond();
        
            backSecond();
        
            leftSecond();
        j++;
        if(j==25)
        {
            k = qrand()%4;
            if(k == 0)
                debugaBack();
            else if(k == 1)
                debugaFront();
            else if(k == 2)
                debugaLeft();
            else if(k == 3)
                debugaRight();
            j = 0;
        }       
    }
}

void MainWindow::on_downCrossButton_clicked()
{
    if(ui->down2Button->styleSheet() != ui->down5Button->styleSheet() && ui->down4Button->styleSheet() != ui->down5Button->styleSheet() &&
            ui->down6Button->styleSheet() != ui->down5Button->styleSheet() && ui->down8Button->styleSheet() != ui->down5Button->styleSheet())
    {
        cube->rotateF();
        cube->rotateL();
        cube->rotateD();
        cube->rotateLA();
        cube->rotateDA();
        cube->rotateFA();
    }
    while((ui->down2Button->styleSheet() != ui->down5Button->styleSheet() || ui->down4Button->styleSheet() != ui->down5Button->styleSheet()) &&
          (ui->down2Button->styleSheet() != ui->down5Button->styleSheet() || ui->down8Button->styleSheet() != ui->down5Button->styleSheet()) &&
           (ui->down4Button->styleSheet() != ui->down5Button->styleSheet() || ui->down6Button->styleSheet() != ui->down5Button->styleSheet()))
        cube->rotateD();
    if(ui->down6Button->styleSheet() != ui->down5Button->styleSheet() && ui->down8Button->styleSheet() != ui->down5Button->styleSheet())
    {
        cube->rotateF();
        cube->rotateL();
        cube->rotateD();
        cube->rotateLA();
        cube->rotateDA();
        cube->rotateFA();
    }
    
    if((ui->down2Button->styleSheet() == ui->down5Button->styleSheet() &&
        ui->down8Button->styleSheet() == ui->down5Button->styleSheet()) &&
            ui->down4Button->styleSheet() != ui->down5Button->styleSheet() &&
            ui->down6Button->styleSheet() != ui->down5Button->styleSheet())
        cube->rotateD();
    if(ui->down2Button->styleSheet() != ui->down5Button->styleSheet() || ui->down4Button->styleSheet() != ui->down5Button->styleSheet() ||
            ui->down6Button->styleSheet() != ui->down5Button->styleSheet() || ui->down8Button->styleSheet() != ui->down5Button->styleSheet())
    {
        cube->rotateF();
        cube->rotateL();
        cube->rotateD();
        cube->rotateLA();
        cube->rotateDA();
        cube->rotateFA();
    }
}

void MainWindow::on_backEdgesButton_clicked()
{
    int i;
    int j=0;
    while(ui->front8Button->styleSheet() != ui->front5Button->styleSheet() || ui->left8Button->styleSheet() != ui->left5Button->styleSheet() ||
          ui->back2Button->styleSheet() != ui->back5Button->styleSheet() || ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
    {
        j++;
        if (j%30==0)
            cube->rotateDA();
        if(ui->front8Button->styleSheet() != ui->front5Button->styleSheet() || ui->left8Button->styleSheet() != ui->left5Button->styleSheet() ||
                ui->back2Button->styleSheet() != ui->back5Button->styleSheet() || ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
        {
            i = 0;
            if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet())
                i++;
            if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet())
                i++;
            if(ui->right8Button->styleSheet() == ui->right5Button->styleSheet())
                i++;
            if(ui->back2Button->styleSheet() == ui->back5Button->styleSheet())
                i++;
            if(i == 0)
            {
                cube->rotateD();
                cube->rotateD();
            }
            if(ui->front8Button->styleSheet() != ui->front5Button->styleSheet() && ui->left8Button->styleSheet() != ui->left5Button->styleSheet())
            {
                cube->rotateL();
                cube->rotateD();
                cube->rotateD();
                cube->rotateLA();
                cube->rotateDA();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateDA();
            }
            else if(ui->back2Button->styleSheet() != ui->back2Button->styleSheet() && ui->left8Button->styleSheet() != ui->left5Button->styleSheet())
            {
                cube->rotateB();
                cube->rotateD();
                cube->rotateD();
                cube->rotateBA();
                cube->rotateDA();
                cube->rotateB();
                cube->rotateDA();
                cube->rotateBA();
                cube->rotateDA();
            }
            else if(ui->back2Button->styleSheet() != ui->back5Button->styleSheet() && ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
            {
                cube->rotateR();
                cube->rotateD();
                cube->rotateD();
                cube->rotateRA();
                cube->rotateDA();
                cube->rotateR();
                cube->rotateDA();
                cube->rotateRA();
                cube->rotateDA();
            }
            else if(ui->front8Button->styleSheet() != ui->front2Button->styleSheet() && ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
            {
                cube->rotateF();
                cube->rotateD();
                cube->rotateD();
                cube->rotateFA();
                cube->rotateDA();
                cube->rotateF();
                cube->rotateDA();
                cube->rotateFA();
                cube->rotateDA();
            }
            else
                cube->rotateDA();
        }
    }
    /*int i = 0;
    while(ui->front8Button->styleSheet() != ui->front5Button->styleSheet() || ui->left8Button->styleSheet() != ui->left5Button->styleSheet() ||
          ui->back2Button->styleSheet() != ui->back5Button->styleSheet() || ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
    {
        if(ui->front8Button->styleSheet() != ui->front5Button->styleSheet() || ui->left8Button->styleSheet() != ui->left5Button->styleSheet() ||
                ui->back2Button->styleSheet() != ui->back5Button->styleSheet() || ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
        {
            i = 0;
            if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet())
                i++;
            if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet())
                i++;
            if(ui->right8Button->styleSheet() == ui->right5Button->styleSheet())
                i++;
            if(ui->back2Button->styleSheet() == ui->back5Button->styleSheet())
                i++;
            if(i == 1)
            {
                cube->rotateD();
            }
            else if (i == 1)
            {
                cube->rotateL();
                cube->rotateD();
                cube->rotateD();
                cube->rotateLA();
                cube->rotateDA();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
                cube->rotateDA();
            }
            if(i == 2)
            {
                if(ui->front8Button->styleSheet() != ui->front5Button->styleSheet() && ui->left8Button->styleSheet() != ui->left5Button->styleSheet())
                {
                    cube->rotateL();
                    cube->rotateD();
                    cube->rotateD();
                    cube->rotateLA();
                    cube->rotateDA();
                    cube->rotateL();
                    cube->rotateDA();
                    cube->rotateLA();
                    cube->rotateDA();
                }
                else if(ui->back2Button->styleSheet() != ui->back2Button->styleSheet() && ui->left8Button->styleSheet() != ui->left5Button->styleSheet())
                {
                    cube->rotateF();
                    cube->rotateD();
                    cube->rotateD();
                    cube->rotateFA();
                    cube->rotateDA();
                    cube->rotateF();
                    cube->rotateDA();
                    cube->rotateFA();
                    cube->rotateDA();
                }
                else if(ui->back2Button->styleSheet() != ui->back5Button->styleSheet() && ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
                {
                    cube->rotateL();
                    cube->rotateD();
                    cube->rotateD();
                    cube->rotateLA();
                    cube->rotateDA();
                    cube->rotateL();
                    cube->rotateDA();
                    cube->rotateLA();
                    cube->rotateDA();
                }
                else if(ui->front8Button->styleSheet() != ui->front2Button->styleSheet() && ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
                {
                    cube->rotateF();
                    cube->rotateD();
                    cube->rotateD();
                    cube->rotateFA();
                    cube->rotateDA();
                    cube->rotateF();
                    cube->rotateDA();
                    cube->rotateFA();
                    cube->rotateDA();
                }
                else
                {
                    cube->rotateL();
                    cube->rotateD();
                    cube->rotateD();
                    cube->rotateLA();
                    cube->rotateDA();
                    cube->rotateL();
                    cube->rotateDA();
                    cube->rotateLA();
                    cube->rotateDA();
                }
            }
            else
            {
                cube->rotateD();
                cube->rotateD();
            }
            if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet() && ui->right8Button->styleSheet() == ui->right5Button->styleSheet())
            {
                cube->rotateL();
                cube->rotateD();
                cube->rotateD();
                cube->rotateLA();
                cube->rotateDA();
                cube->rotateL();
                cube->rotateDA();
                cube->rotateLA();
            }
        }
        if(ui->front8Button->styleSheet() != ui->front5Button->styleSheet() && ui->left8Button->styleSheet() != ui->left5Button->styleSheet() &&
                ui->back2Button->styleSheet() != ui->back5Button->styleSheet() && ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateL();
            cube->rotateD();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateD();
        }
    }*/
}

void MainWindow::on_placeDownEdgesButton_clicked()
{
    while(!sudestecorreto() || !sudoestecorreto() || !nordestecorreto() || !noroestecorreto())
    {
        if(sudoestecorreto())
        {
            cube->rotateRA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateR();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateDA();

            
        }
        else if(sudestecorreto())
        {
            cube->rotateBA();
            cube->rotateD();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateB();
            cube->rotateD();
            cube->rotateFA();
            cube->rotateDA();

            
        }
        else if(noroestecorreto())
        {
            cube->rotateFA();
            cube->rotateD();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateF();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateDA();

            
        }
        else if(nordestecorreto())
        {
            cube->rotateRA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateR();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateDA();

            
        }
        else
        {
            cube->rotateRA();
            cube->rotateD();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateR();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateDA();

            
        }
    }
}

void MainWindow::on_turnDownEdgesButton_clicked()
{
    while(ui->front1Button->styleSheet() != ui->front5Button->styleSheet() || ui->front2Button->styleSheet() != ui->front5Button->styleSheet() || ui->front3Button->styleSheet() != ui->front5Button->styleSheet() ||
          ui->front4Button->styleSheet() != ui->front5Button->styleSheet() || ui->front6Button->styleSheet() != ui->front5Button->styleSheet() || ui->front7Button->styleSheet() != ui->front5Button->styleSheet() ||
          ui->front8Button->styleSheet() != ui->front5Button->styleSheet() || ui->front9Button->styleSheet() != ui->front5Button->styleSheet() ||
          ui->left1Button->styleSheet() != ui->left5Button->styleSheet() || ui->left2Button->styleSheet() != ui->left5Button->styleSheet() || ui->left3Button->styleSheet() != ui->left5Button->styleSheet() ||
          ui->left4Button->styleSheet() != ui->left5Button->styleSheet() || ui->left6Button->styleSheet() != ui->left5Button->styleSheet() || ui->left7Button->styleSheet() != ui->left5Button->styleSheet() ||
          ui->left8Button->styleSheet() != ui->left5Button->styleSheet() || ui->left9Button->styleSheet() != ui->left5Button->styleSheet() ||
          ui->right1Button->styleSheet() != ui->right5Button->styleSheet() || ui->right2Button->styleSheet() != ui->right5Button->styleSheet() || ui->right3Button->styleSheet() != ui->right5Button->styleSheet() ||
          ui->right4Button->styleSheet() != ui->right5Button->styleSheet() || ui->right6Button->styleSheet() != ui->right5Button->styleSheet() || ui->right7Button->styleSheet() != ui->right5Button->styleSheet() ||
          ui->right8Button->styleSheet() != ui->right5Button->styleSheet() || ui->right9Button->styleSheet() != ui->right5Button->styleSheet() ||
          ui->back1Button->styleSheet() != ui->back5Button->styleSheet() || ui->back2Button->styleSheet() != ui->back5Button->styleSheet() || ui->back3Button->styleSheet() != ui->back5Button->styleSheet() ||
          ui->back4Button->styleSheet() != ui->back5Button->styleSheet() || ui->back6Button->styleSheet() != ui->back5Button->styleSheet() || ui->back7Button->styleSheet() != ui->back5Button->styleSheet() ||
          ui->back8Button->styleSheet() != ui->back5Button->styleSheet() || ui->back9Button->styleSheet() != ui->back5Button->styleSheet() ||
          ui->up1Button->styleSheet() != ui->up5Button->styleSheet() || ui->up2Button->styleSheet() != ui->up5Button->styleSheet() || ui->up3Button->styleSheet() != ui->up5Button->styleSheet() ||
          ui->up4Button->styleSheet() != ui->up5Button->styleSheet() || ui->up6Button->styleSheet() != ui->up5Button->styleSheet() || ui->up7Button->styleSheet() != ui->up5Button->styleSheet() ||
          ui->up8Button->styleSheet() != ui->up5Button->styleSheet() || ui->up9Button->styleSheet() != ui->up5Button->styleSheet() ||
          ui->down1Button->styleSheet() != ui->down5Button->styleSheet() || ui->down2Button->styleSheet() != ui->down5Button->styleSheet() || ui->down3Button->styleSheet() != ui->down5Button->styleSheet() ||
          ui->down4Button->styleSheet() != ui->down5Button->styleSheet() || ui->down6Button->styleSheet() != ui->down5Button->styleSheet() || ui->down7Button->styleSheet() != ui->down5Button->styleSheet() ||
          ui->down8Button->styleSheet() != ui->down5Button->styleSheet() || ui->down9Button->styleSheet() != ui->down5Button->styleSheet())
    {
        if((ui->front7Button->styleSheet() != ui->front5Button->styleSheet() || ui->left9Button->styleSheet() != ui->left5Button->styleSheet() || ui->down1Button->styleSheet() != ui->down5Button->styleSheet()) &&
                (ui->back1Button->styleSheet() != ui->back5Button->styleSheet() || ui->left7Button->styleSheet() != ui->left5Button->styleSheet() || ui->down7Button->styleSheet() != ui->down7Button->styleSheet()))
        {
            cube->rotateL();
            cube->rotateD();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateD();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();

            
        }
        else if((ui->back1Button->styleSheet() != ui->back5Button->styleSheet() || ui->left7Button->styleSheet() != ui->left5Button->styleSheet() || ui->down7Button->styleSheet() != ui->down7Button->styleSheet()) &&
                (ui->back3Button->styleSheet() != ui->back5Button->styleSheet() || ui->right9Button->styleSheet() != ui->right5Button->styleSheet() || ui->down9Button->styleSheet() != ui->down7Button->styleSheet()))
        {
            cube->rotateB();
            cube->rotateD();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateD();
            cube->rotateF();
            cube->rotateD();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();

            
        }
        else if((ui->back3Button->styleSheet() != ui->back5Button->styleSheet() || ui->right9Button->styleSheet() != ui->right5Button->styleSheet() || ui->down9Button->styleSheet() != ui->down7Button->styleSheet()) &&
                (ui->front9Button->styleSheet() != ui->front5Button->styleSheet() || ui->right7Button->styleSheet() != ui->right5Button->styleSheet() || ui->down3Button->styleSheet() != ui->down7Button->styleSheet()))
        {
            cube->rotateR();
            cube->rotateD();
            cube->rotateD();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateD();
            cube->rotateL();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();

            
        }
        else if((ui->front9Button->styleSheet() != ui->front5Button->styleSheet() || ui->right7Button->styleSheet() != ui->right5Button->styleSheet() || ui->down3Button->styleSheet() != ui->down7Button->styleSheet()) &&
                (ui->front7Button->styleSheet() != ui->front5Button->styleSheet() || ui->left9Button->styleSheet() != ui->left5Button->styleSheet() || ui->down1Button->styleSheet() != ui->down5Button->styleSheet()))
        {
            cube->rotateF();
            cube->rotateD();
            cube->rotateD();
            cube->rotateFA();
            cube->rotateDA();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateD();
            cube->rotateB();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();

            
        }
        else if(ui->front7Button->styleSheet() != ui->front5Button->styleSheet() || ui->left9Button->styleSheet() != ui->left5Button->styleSheet() || ui->down1Button->styleSheet() != ui->down5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateD();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateDA();
            cube->rotateL();
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateD();
            cube->rotateRA();
            cube->rotateD();
            cube->rotateR();

            
        }
        else if(ui->back1Button->styleSheet() != ui->back5Button->styleSheet() || ui->left7Button->styleSheet() != ui->left5Button->styleSheet() || ui->down7Button->styleSheet() != ui->down7Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateD();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateDA();
            cube->rotateB();
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateD();
            cube->rotateF();
            cube->rotateD();
            cube->rotateFA();
            cube->rotateD();
            cube->rotateF();

            
        }
        else if(ui->back3Button->styleSheet() != ui->back5Button->styleSheet() || ui->right9Button->styleSheet() != ui->right5Button->styleSheet() || ui->down9Button->styleSheet() != ui->down7Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateD();
            cube->rotateD();
            cube->rotateRA();
            cube->rotateDA();
            cube->rotateR();
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateD();
            cube->rotateL();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateD();
            cube->rotateL();

            
        }
        else if(ui->front9Button->styleSheet() != ui->front5Button->styleSheet() || ui->right7Button->styleSheet() != ui->right5Button->styleSheet() || ui->down3Button->styleSheet() != ui->down7Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateD();
            cube->rotateD();
            cube->rotateFA();
            cube->rotateDA();
            cube->rotateF();
            cube->rotateDA();
            cube->rotateFA();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateD();
            cube->rotateB();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateD();
            cube->rotateB();

            
        }
    }
}

void MainWindow::on_solveLanesButton_clicked()
{
    on_crossButton_clicked();
    on_firstLayerCornersButton_clicked();
    on_secondLayerButton_clicked();
    on_downCrossButton_clicked();
    on_backEdgesButton_clicked();
    on_placeDownEdgesButton_clicked();
    on_turnDownEdgesButton_clicked();
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
        showDialog(std::string("Calibration done with success."));
    }
    else showDialog(std::string("Error: The Calibration only works on ROBOT mode."));
}

void MainWindow::on_readColorsButton_clicked()
{
    if(mode == Mode::ROBOT)
    {
        reader->read();
        if(cube->isValid()) showDialog(std::string("Reading done with success."));
        else showDialog(std::string("Error: The new cube have more than 9 squares of the same color."));
    }
    else showDialog(std::string("Error: The Reading only works on ROBOT mode."));
}

void MainWindow::on_rotationUButton_clicked() { cube->rotateU(); setRotationsNumber(rotationsNumber+1); }
void MainWindow::on_rotationUAButton_clicked() { cube->rotateUA(); setRotationsNumber(rotationsNumber+1); }
void MainWindow::on_rotationDButton_clicked() { cube->rotateD(); setRotationsNumber(rotationsNumber+1); }
void MainWindow::on_rotationDAButton_clicked() { cube->rotateDA(); setRotationsNumber(rotationsNumber+1); }
void MainWindow::on_rotationRButton_clicked() { cube->rotateR(); setRotationsNumber(rotationsNumber+1); }
void MainWindow::on_rotationRAButton_clicked() { cube->rotateRA(); setRotationsNumber(rotationsNumber+1); }
void MainWindow::on_rotationLButton_clicked() { cube->rotateL(); setRotationsNumber(rotationsNumber+1); }
void MainWindow::on_rotationLAButton_clicked() { cube->rotateLA(); setRotationsNumber(rotationsNumber+1); }
void MainWindow::on_rotationFButton_clicked() { cube->rotateF(); setRotationsNumber(rotationsNumber+1); }
void MainWindow::on_rotationFAButton_clicked() { cube->rotateFA(); setRotationsNumber(rotationsNumber+1); }
void MainWindow::on_rotationBButton_clicked() { cube->rotateB(); setRotationsNumber(rotationsNumber+1); }
void MainWindow::on_rotationBAButton_clicked() { cube->rotateBA(); setRotationsNumber(rotationsNumber+1); }

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
