#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "squares.cpp"
#include "config.h";
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
int simulacao = 1;
int del = 2;
QSerialPort *arduino = new QSerialPort;
static const quint16 arduino_uno_vendor_id = 9025;
static const quint16 arduino_uno_product_id = 67;
QString arduino_port_name = "";
bool arduino_is_available = false;
bool CalibrationOn = false;
QString ult = "";
QCamera *mCamera;
config conf;
config aux;
std::vector<double> calibrateVector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
QCameraViewfinder *mCameraViewFinder;
QCameraImageCapture *mCameraImageCapture;
QVBoxLayout *mLayout;
int lado = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupSquares();
    cube = std::make_unique<Cube>(squares);
    cube->initialize();
    conf.readCalibrateColors(&conf);
    setRotationsNumber(0);
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
            if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
                if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
                    if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
                        arduino_port_name = "ttyACM1";
                        std::cout << arduino_port_name.toStdString() << std::endl;
                        arduino_is_available = true;
                        std::cout << "arduino_is_available = true" << std::endl;
                    }
                }
            }
        }
        if(arduino_is_available){
            
            arduino->setPortName(arduino_port_name);
            arduino->setBaudRate(QSerialPort::Baud9600);
            arduino->setDataBits(QSerialPort::Data8);
            arduino->setParity(QSerialPort::NoParity);
            arduino->setStopBits(QSerialPort::OneStop);
            arduino->setFlowControl(QSerialPort::NoFlowControl);
            if(arduino->open(QSerialPort::WriteOnly))
                std::cout << "arduino aberto" << std::endl;
            else
                std::cout << "arduino não aberto" << std:: endl;
        }else{
            
            std::cout << "Arduido não está ativo" << std::endl;
        }
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
    if(button->styleSheet() == ui->up5Button->styleSheet())
        button->setStyleSheet("background-color: white");
    else if(button->styleSheet() == ui->front5Button->styleSheet())
        button->setStyleSheet("background-color: orange");
    else if(button->styleSheet() == ui->left5Button->styleSheet())
        button->setStyleSheet("background-color: red");
    else if(button->styleSheet() == ui->right5Button->styleSheet())
        button->setStyleSheet("background-color: green");
    else if(button->styleSheet() == ui->down5Button->styleSheet())
        button->setStyleSheet("background-color: yellow");
    else if(button->styleSheet() == ui->back5Button->styleSheet())
        button->setStyleSheet("background-color: blue");
}

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
            ult = "1L";
            
        }
        else if(ui->right6Button->styleSheet() == ui->up5Button->styleSheet() && ui->back6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();
            ult = "2L";
            
        }
        else if(ui->right6Button->styleSheet() == ui->right5Button->styleSheet() && ui->back6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateRA();
            ult = "3L";
            
        }
        else if(ui->right4Button->styleSheet() == ui->up5Button->styleSheet() && ui->front6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateFA();
            cube->rotateUA();
            ult = "4L";
            
        }
        else if(ui->right4Button->styleSheet() == ui->right5Button->styleSheet() && ui->front6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateR();
            ult = "5L";
            
        }
        else if(ui->right8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateR();
            ult = "6L";
            
        }
        else if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateRA();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();
            ult = "7L";
            
        }
        else if(ui->back8Button->styleSheet() == ui->right5Button->styleSheet() && ui->up2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateBA();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();
            ult = "8L";
            
        }
        else if(ui->back8Button->styleSheet() == ui->up5Button->styleSheet() && ui->up2Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateR();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();
            ult = "9L";
            
        }
        else if(ui->front2Button->styleSheet() == ui->right5Button->styleSheet() && ui->up8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateU();
            cube->rotateF();
            cube->rotateUA();
            ult = "10L";
            
        }
        else if(ui->front2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up8Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateR();
            ult = "11L";
            
        }
        else if(ui->back2Button->styleSheet() == ui->right5Button->styleSheet() && ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateR();
            cube->rotateR();
            ult = "12L";
            
        }
        else if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet() && ui->down8Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateRA();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();
            ult = "13L";
            
        }
        else if(ui->front8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateR();
            cube->rotateR();
            ult = "14L";
            
        }
        else if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down2Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateRA();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();
            ult = "15L";
            
        }
        else if(ui->left2Button->styleSheet() == ui->right5Button->styleSheet() && ui->up4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateL();
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateR();
            ult = "16L";
            
        }
        else if(ui->left2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();
            cube->rotateR();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();
            ult = "17L";
            
        }
        else if(ui->left4Button->styleSheet() == ui->right5Button->styleSheet() && ui->back4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateU();
            cube->rotateU();
            ult = "18L";
            
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
            ult = "19L";
            
        }
        else if(ui->left6Button->styleSheet() == ui->right5Button->styleSheet() && ui->front4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            
            cube->rotateU();
            cube->rotateU();
            cube->rotateLA();
            cube->rotateUA();
            cube->rotateUA();
            ult = "20L";
            
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
            ult = "21L";
            
        }
        else if(ui->left8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateR();
            cube->rotateR();
            ult = "22L";
            
        }
        else if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateRA();
            cube->rotateUA();
            cube->rotateB();
            cube->rotateU();
            ult = "23L";
            
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
            ult = "1O";
            
        }
        else if(ui->left6Button->styleSheet() == ui->up5Button->styleSheet() && ui->front4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();
            ult = "2O";
            
        }
        else if(ui->left6Button->styleSheet() == ui->left5Button->styleSheet() && ui->front4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateLA();
            ult = "3O";
            
        }
        else if(ui->left4Button->styleSheet() == ui->up5Button->styleSheet() && ui->back4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateBA();
            cube->rotateUA();
            ult = "4O";
            
        }
        else if(ui->left4Button->styleSheet() == ui->left5Button->styleSheet() && ui->back4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();
            ult = "5O";
            
        }
        else if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateL();
            cube->rotateL();
            ult = "6O";
            
        }
        else if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateLA();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();
            ult = "7O";
            
        }
        else if(ui->front2Button->styleSheet() == ui->left5Button->styleSheet() && ui->up8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateU();
            ult = "8O";
            
        }
        else if(ui->front2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up8Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateL();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();
            ult = "9O";
            
        }
        else if(ui->back8Button->styleSheet() == ui->left5Button->styleSheet() && ui->up2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateUA();
            ult = "10O";
            
        }
        else if(ui->back8Button->styleSheet() == ui->up5Button->styleSheet() && ui->up2Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateUA();
            cube->rotateL();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();
            ult = "11O";
            
        }
        else if(ui->front8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateL();
            cube->rotateL();
            ult = "12O";
            
        }
        else if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down2Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateLA();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();
            ult = "13O";
            
        }
        else if(ui->back2Button->styleSheet() == ui->left5Button->styleSheet() && ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateL();
            cube->rotateL();
            ult = "14O";
            
        }
        else if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet() && ui->down8Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateLA();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();
            ult = "15O";
            
        }
        else if(ui->right2Button->styleSheet() == ui->left5Button->styleSheet() && ui->up6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();
            ult = "16O";
            
        }
        else if(ui->right2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();
            cube->rotateL();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();
            ult = "17O";
            
        }
        else if(ui->right4Button->styleSheet() == ui->left5Button->styleSheet() && ui->front6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();
            cube->rotateR();
            cube->rotateUA();
            cube->rotateUA();
            ult = "18O";
            
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
            ult = "19O";
            
        }
        else if(ui->right6Button->styleSheet() == ui->left5Button->styleSheet() && ui->back6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateD();
            cube->rotateD();
            cube->rotateL();
            cube->rotateL();
            ult = "20O";
            
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
            ult = "21O";
            
        }
        else if(ui->right8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateL();
            cube->rotateL();
            ult = "22O";
            
        }
        else if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateLA();
            cube->rotateUA();
            cube->rotateF();
            cube->rotateU();
            ult = "23O";
            
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
            ult = "1N";
            
        }
        else if(ui->back4Button->styleSheet() == ui->up5Button->styleSheet() && ui->left4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();
            ult = "2N";
            
        }
        else if(ui->back4Button->styleSheet() == ui->back5Button->styleSheet() && ui->left4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateBA();
            ult = "3N";
            
        }
        else if(ui->back6Button->styleSheet() == ui->up5Button->styleSheet() && ui->right6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateRA();
            cube->rotateUA();
            ult = "4N";
            
        }
        else if(ui->back6Button->styleSheet() == ui->back5Button->styleSheet() && ui->right6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateB();
            ult = "5N";
            
        }
        else if(ui->back2Button->styleSheet() == ui->back5Button->styleSheet() && ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateB();
            cube->rotateB();
            ult = "6N";
            
        }
        else if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet() && ui->down8Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateBA();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();
            ult = "7N";
            
        }
        else if(ui->left2Button->styleSheet() == ui->back5Button->styleSheet() && ui->up4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateLA();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();
            ult = "8N";
            
        }
        else if(ui->left2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateB();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();
            ult = "9N";
            
        }
        else if(ui->right2Button->styleSheet() == ui->back5Button->styleSheet() && ui->up6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateR();
            cube->rotateU();
            cube->rotateRA();
            cube->rotateUA();
            ult = "10N";
            
        }
        else if(ui->right2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateUA();
            cube->rotateB();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();
            ult = "11N";
            
        }
        else if(ui->left8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateB();
            cube->rotateB();
            ult = "12N";
            
        }
        else if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateDA();
            cube->rotateBA();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();
            ult = "13N";
            
        }
        else if(ui->right8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateB();
            cube->rotateB();
            ult = "14N";
            
        }
        else if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateBA();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();
            ult = "15N";
            
        }
        else if(ui->front2Button->styleSheet() == ui->back5Button->styleSheet() && ui->up8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();
            ult = "16N";
            
        }
        else if(ui->front2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up8Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateU();
            cube->rotateU();
            cube->rotateB();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();
            ult = "17N";
            
        }
        else if(ui->front4Button->styleSheet() == ui->back5Button->styleSheet() && ui->left6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateF();
            cube->rotateU();
            cube->rotateU();
            ult = "18N";
            
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
            ult = "19N";
            
        }
        else if(ui->front6Button->styleSheet() == ui->back5Button->styleSheet() && ui->right4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateFA();
            cube->rotateU();
            cube->rotateU();
            ult = "20N";
            
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
            ult = "21N";
            
        }
        else if(ui->front8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateF();
            cube->rotateF();
            cube->rotateBA();
            cube->rotateBA();
            ult = "22N";
            
        }
        else if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down2Button->styleSheet() == ui->back5Button->styleSheet())
        {
            cube->rotateD();
            cube->rotateD();
            cube->rotateBA();
            cube->rotateUA();
            cube->rotateL();
            cube->rotateU();
            ult = "23N";
            
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
            ult = "1LL";
            
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
            ult = "2LL";
            
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
            ult = "3LL";
            
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
            ult = "4LL";
            
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
            ult = "5LL";
            
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
            ult = "6LL";
            
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
            ult = "7LL";
            
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
            ult = "8LL";
            
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
QString MainWindow::color(QString cor)
{
    if (cor.toStdString() == "background-color: blue")
        return QString("b");
    else if (cor.toStdString() == "background-color: red")
        return QString("r");
    else if (cor.toStdString() == "background-color: white")
        return QString("w");
    else if (cor.toStdString() == "background-color: orange")
        return QString("o");
    else if (cor.toStdString() == "background-color: yellow")
        return QString("y");
    else
        return QString("g");
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
void MainWindow::camera()
{
    mCamera->start();
    
    
    
    
}
void MainWindow::setModel(std::vector<std::string> styles, int c)
{
    if(c == 0)
    {
        ui->up1Button->setStyleSheet(QString::fromStdString(styles[0]));
        ui->up2Button->setStyleSheet(QString::fromStdString(styles[1]));
        ui->up3Button->setStyleSheet(QString::fromStdString(styles[2]));
        ui->up4Button->setStyleSheet(QString::fromStdString(styles[3]));
        ui->up5Button->setStyleSheet(QString::fromStdString(styles[4]));
        ui->up6Button->setStyleSheet(QString::fromStdString(styles[5]));
        ui->up7Button->setStyleSheet(QString::fromStdString(styles[6]));
        ui->up8Button->setStyleSheet(QString::fromStdString(styles[7]));
        ui->up9Button->setStyleSheet(QString::fromStdString(styles[8]));
    }
    else if(c == 1)
    {
        ui->front1Button->setStyleSheet(QString::fromStdString(styles[0]));
        ui->front2Button->setStyleSheet(QString::fromStdString(styles[1]));
        ui->front3Button->setStyleSheet(QString::fromStdString(styles[2]));
        ui->front4Button->setStyleSheet(QString::fromStdString(styles[3]));
        ui->front5Button->setStyleSheet(QString::fromStdString(styles[4]));
        ui->front6Button->setStyleSheet(QString::fromStdString(styles[5]));
        ui->front7Button->setStyleSheet(QString::fromStdString(styles[6]));
        ui->front8Button->setStyleSheet(QString::fromStdString(styles[7]));
        ui->front9Button->setStyleSheet(QString::fromStdString(styles[8]));
    }
    else if(c == 2)
    {
        ui->down1Button->setStyleSheet(QString::fromStdString(styles[0]));
        ui->down2Button->setStyleSheet(QString::fromStdString(styles[1]));
        ui->down3Button->setStyleSheet(QString::fromStdString(styles[2]));
        ui->down4Button->setStyleSheet(QString::fromStdString(styles[3]));
        ui->down5Button->setStyleSheet(QString::fromStdString(styles[4]));
        ui->down6Button->setStyleSheet(QString::fromStdString(styles[5]));
        ui->down7Button->setStyleSheet(QString::fromStdString(styles[6]));
        ui->down8Button->setStyleSheet(QString::fromStdString(styles[7]));
        ui->down9Button->setStyleSheet(QString::fromStdString(styles[8]));
    }
    else if(c == 3)
    {
        ui->back1Button->setStyleSheet(QString::fromStdString(styles[0]));
        ui->back2Button->setStyleSheet(QString::fromStdString(styles[1]));
        ui->back3Button->setStyleSheet(QString::fromStdString(styles[2]));
        ui->back4Button->setStyleSheet(QString::fromStdString(styles[3]));
        ui->back5Button->setStyleSheet(QString::fromStdString(styles[4]));
        ui->back6Button->setStyleSheet(QString::fromStdString(styles[5]));
        ui->back7Button->setStyleSheet(QString::fromStdString(styles[6]));
        ui->back8Button->setStyleSheet(QString::fromStdString(styles[7]));
        ui->back9Button->setStyleSheet(QString::fromStdString(styles[8]));
    }
    else if(c == 5)
    {
        ui->left1Button->setStyleSheet(QString::fromStdString(styles[0]));
        ui->left2Button->setStyleSheet(QString::fromStdString(styles[1]));
        ui->left3Button->setStyleSheet(QString::fromStdString(styles[2]));
        ui->left4Button->setStyleSheet(QString::fromStdString(styles[3]));
        ui->left5Button->setStyleSheet(QString::fromStdString(styles[4]));
        ui->left6Button->setStyleSheet(QString::fromStdString(styles[5]));
        ui->left7Button->setStyleSheet(QString::fromStdString(styles[6]));
        ui->left8Button->setStyleSheet(QString::fromStdString(styles[7]));
        ui->left9Button->setStyleSheet(QString::fromStdString(styles[8]));
    }
    else if(c == 4)
    {
        ui->right1Button->setStyleSheet(QString::fromStdString(styles[0]));
        ui->right2Button->setStyleSheet(QString::fromStdString(styles[1]));
        ui->right3Button->setStyleSheet(QString::fromStdString(styles[2]));
        ui->right4Button->setStyleSheet(QString::fromStdString(styles[3]));
        ui->right5Button->setStyleSheet(QString::fromStdString(styles[4]));
        ui->right6Button->setStyleSheet(QString::fromStdString(styles[5]));
        ui->right7Button->setStyleSheet(QString::fromStdString(styles[6]));
        ui->right8Button->setStyleSheet(QString::fromStdString(styles[7]));
        ui->right9Button->setStyleSheet(QString::fromStdString(styles[8]));
    }
}
bool MainWindow::verificaCoerencia()
{
    std::vector<QString> numberOfColors;
    numberOfColors.push_back(ui->up1Button->styleSheet());
    numberOfColors.push_back(ui->up2Button->styleSheet());
    numberOfColors.push_back(ui->up3Button->styleSheet());
    numberOfColors.push_back(ui->up4Button->styleSheet());
    numberOfColors.push_back(ui->up5Button->styleSheet());
    numberOfColors.push_back(ui->up6Button->styleSheet());
    numberOfColors.push_back(ui->up7Button->styleSheet());
    numberOfColors.push_back(ui->up8Button->styleSheet());
    numberOfColors.push_back(ui->up9Button->styleSheet());
    numberOfColors.push_back(ui->front1Button->styleSheet());
    numberOfColors.push_back(ui->front2Button->styleSheet());
    numberOfColors.push_back(ui->front3Button->styleSheet());
    numberOfColors.push_back(ui->front4Button->styleSheet());
    numberOfColors.push_back(ui->front5Button->styleSheet());
    numberOfColors.push_back(ui->front6Button->styleSheet());
    numberOfColors.push_back(ui->front7Button->styleSheet());
    numberOfColors.push_back(ui->front8Button->styleSheet());
    numberOfColors.push_back(ui->front9Button->styleSheet());
    numberOfColors.push_back(ui->down1Button->styleSheet());
    numberOfColors.push_back(ui->down2Button->styleSheet());
    numberOfColors.push_back(ui->down3Button->styleSheet());
    numberOfColors.push_back(ui->down4Button->styleSheet());
    numberOfColors.push_back(ui->down5Button->styleSheet());
    numberOfColors.push_back(ui->down6Button->styleSheet());
    numberOfColors.push_back(ui->down7Button->styleSheet());
    numberOfColors.push_back(ui->down8Button->styleSheet());
    numberOfColors.push_back(ui->down9Button->styleSheet());
    numberOfColors.push_back(ui->back1Button->styleSheet());
    numberOfColors.push_back(ui->back2Button->styleSheet());
    numberOfColors.push_back(ui->back3Button->styleSheet());
    numberOfColors.push_back(ui->back4Button->styleSheet());
    numberOfColors.push_back(ui->back5Button->styleSheet());
    numberOfColors.push_back(ui->back6Button->styleSheet());
    numberOfColors.push_back(ui->back7Button->styleSheet());
    numberOfColors.push_back(ui->back8Button->styleSheet());
    numberOfColors.push_back(ui->back9Button->styleSheet());
    numberOfColors.push_back(ui->left1Button->styleSheet());
    numberOfColors.push_back(ui->left2Button->styleSheet());
    numberOfColors.push_back(ui->left3Button->styleSheet());
    numberOfColors.push_back(ui->left4Button->styleSheet());
    numberOfColors.push_back(ui->left5Button->styleSheet());
    numberOfColors.push_back(ui->left6Button->styleSheet());
    numberOfColors.push_back(ui->left7Button->styleSheet());
    numberOfColors.push_back(ui->left8Button->styleSheet());
    numberOfColors.push_back(ui->left9Button->styleSheet());
    numberOfColors.push_back(ui->right1Button->styleSheet());
    numberOfColors.push_back(ui->right2Button->styleSheet());
    numberOfColors.push_back(ui->right3Button->styleSheet());
    numberOfColors.push_back(ui->right4Button->styleSheet());
    numberOfColors.push_back(ui->right5Button->styleSheet());
    numberOfColors.push_back(ui->right6Button->styleSheet());
    numberOfColors.push_back(ui->right7Button->styleSheet());
    numberOfColors.push_back(ui->right8Button->styleSheet());
    numberOfColors.push_back(ui->right9Button->styleSheet());
    int b = std::count(numberOfColors.begin(), numberOfColors.end(), QString("background-color: blue"));
    int w = std::count(numberOfColors.begin(), numberOfColors.end(), QString("background-color: white"));
    int o = std::count(numberOfColors.begin(), numberOfColors.end(), QString("background-color: orange"));
    int r = std::count(numberOfColors.begin(), numberOfColors.end(), QString("background-color: red"));
    int y = std::count(numberOfColors.begin(), numberOfColors.end(), QString("background-color: yellow"));
    int g = std::count(numberOfColors.begin(), numberOfColors.end(), QString("background-color: green"));
    std::cout << "b = " << b << std::endl;
    std::cout << "w = " << w << std::endl;
    std::cout << "o = " << o << std::endl;
    std::cout << "r = " << r << std::endl;
    std::cout << "y = " << y << std::endl;
    std::cout << "g = " << g << std::endl;
    if(b == 9 &&
       w == b &&
       o == w &&
       r == o &&
       y == r &&
       g == y)
    {
        std::cout << "Configuração de Cubo Válida!" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Configuração de Cubo Inválida!" << std::endl;
        return false;
    }
}
void MainWindow::takePictureAndProcess(std::vector<char> coresFaceAtual)
{
    double R=0,G=0,B=0;
    cv::VideoCapture cam(1);
    cv::Mat pic1;
    cv::Mat pic2;
    std::vector <std::string> styles;
    while (!cam.isOpened())
    {
            std::cout << "Failed to make connection to cam" << std::endl;
            cam.open(1);
    }
    cam >> pic1;
    cv::flip(pic1,pic1,-1);
    vector<vector<Point> > squares;
    for(int i = 0; i < 100; i++)
    {
        delay(1000);
        std::cout << lado << " " << i  << "/100" << std::endl;
        while (!cam.isOpened())
        {
                
                
                cam.open(1);
        }
        cam >> pic1;
        cv::flip(pic1,pic1,-1);
        while(squares.empty() || squares.size() != 9)
        {
            
            imshow("antes find", pic1);
            findSquares(pic1, squares);
            cam >> pic1;
            cv::flip(pic1,pic1,-1);
        }
        cam.~VideoCapture();
        std::vector<double> faceColors = drawSquares(pic1, squares, styles, conf);
        for(int i = 0 ; i < coresFaceAtual.size(); i++)
        {
            if(coresFaceAtual[i] == 'G')
            {
                calibrateVector[0] += faceColors[i*3];
                calibrateVector[1] += faceColors[i*3+1];
                calibrateVector[2] += faceColors[i*3+2];
            }
            else if(coresFaceAtual[i] == 'R')
            {
                calibrateVector[3] += faceColors[i*3];
                calibrateVector[4] += faceColors[i*3+1];
                calibrateVector[5] += faceColors[i*3+2];
            }
            else if(coresFaceAtual[i] == 'B')
            {
                calibrateVector[6] += faceColors[i*3];
                calibrateVector[7] += faceColors[i*3+1];
                calibrateVector[8] += faceColors[i*3+2];
            }
            else if(coresFaceAtual[i] == 'O')
            {
                calibrateVector[9] += faceColors[i*3];
                calibrateVector[10] += faceColors[i*3+1];
                calibrateVector[11] += faceColors[i*3+2];
            }
            else if(coresFaceAtual[i] == 'Y')
            {
                calibrateVector[12] += faceColors[i*3];
                calibrateVector[13] += faceColors[i*3+1];
                calibrateVector[14] += faceColors[i*3+2];
            }
            else if(coresFaceAtual[i] == 'W')
            {
                calibrateVector[15] += faceColors[i*3];
                calibrateVector[16] += faceColors[i*3+1];
                calibrateVector[17] += faceColors[i*3+2];
            }
        }
        squares.clear();
    }
}


void MainWindow::on_randomButton_clicked()
{
    
    int i;
    int random2;
    int random = 20;
    int aux = simulacao;
    random += qrand()%31;
    simulacao = 1;
    for(i = 0; i < random; i++)
    {
        random2 = qrand()%12;
        if(random2 == 0)
            cube->rotateU();
        else if(random2 == 1)
            cube->rotateUA();
        else if(random2 == 2)
            cube->rotateD();
        else if(random2 == 3)
            cube->rotateDA();
        else if(random2 == 4)
            cube->rotateR();
        else if(random2 == 5)
            cube->rotateRA();
        else if(random2 == 6)
            cube->rotateL();
        else if(random2 == 7)
            cube->rotateLA();
        else if(random2 == 8)
            cube->rotateF();
        else if(random2 == 9)
            cube->rotateFA();
        else if(random2 == 10)
            cube->rotateB();
        else if(random2 == 11)
            cube->rotateBA();
    }
    simulacao = aux;
    rotationsNumber = 0;
    ui->rotationsNumberTextBox->setPlainText(QString::number(rotationsNumber));
}

void MainWindow::on_restartButton_clicked()
{
    cube->initialize();
    setRotationsNumber(0);
}

void MainWindow::on_toggleModeButton_clicked()
{
    if(simulacao == 1)
    {
        ui->modeTextBox->setPlainText(QString("Arduino"));
        simulacao = 0;
    }
    else if(simulacao == 0)
    {
        ui->modeTextBox->setPlainText(QString("Simulation"));
        simulacao = 1;
    }
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
            ult = "1SS";
            
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
            ult = "2SS";
            
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
            ult = "3SS";
            
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
            ult = "4SS";
            
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
            ult = "5SS";
            
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
            ult = "1FF";
            
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
            ult = "2FF";
            
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
            ult = "3FF";
            
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
            ult = "4FF";
            
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
            ult = "5FF";
            
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
            ult = "6FF";
            
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
            ult = "7FF";
            
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
            ult = "8FF";
            
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
    QString input = "";
    input += color(ui->right3Button->styleSheet());
    input += color(ui->right6Button->styleSheet());
    input += color(ui->right9Button->styleSheet());
    input += color(ui->right2Button->styleSheet());
    input += color(ui->right8Button->styleSheet());
    input += color(ui->right1Button->styleSheet());
    input += color(ui->right4Button->styleSheet());
    input += color(ui->right7Button->styleSheet());
    input += color(ui->up3Button->styleSheet());
    input += color(ui->up6Button->styleSheet());
    input += color(ui->up9Button->styleSheet());
    input += color(ui->front3Button->styleSheet());
    input += color(ui->front6Button->styleSheet());
    input += color(ui->front9Button->styleSheet());
    input += color(ui->down3Button->styleSheet());
    input += color(ui->down6Button->styleSheet());
    input += color(ui->down9Button->styleSheet());
    input += color(ui->back3Button->styleSheet());
    input += color(ui->back6Button->styleSheet());
    input += color(ui->back9Button->styleSheet());
    input += color(ui->up2Button->styleSheet());
    input += color(ui->up8Button->styleSheet());
    input += color(ui->front2Button->styleSheet());
    input += color(ui->front8Button->styleSheet());
    input += color(ui->down2Button->styleSheet());
    input += color(ui->down8Button->styleSheet());
    input += color(ui->back2Button->styleSheet());
    input += color(ui->back8Button->styleSheet());
    input += color(ui->up1Button->styleSheet());
    input += color(ui->up4Button->styleSheet());
    input += color(ui->up7Button->styleSheet());
    input += color(ui->front1Button->styleSheet());
    input += color(ui->front4Button->styleSheet());
    input += color(ui->front7Button->styleSheet());
    input += color(ui->down1Button->styleSheet());
    input += color(ui->down4Button->styleSheet());
    input += color(ui->down7Button->styleSheet());
    input += color(ui->back1Button->styleSheet());
    input += color(ui->back4Button->styleSheet());
    input += color(ui->back7Button->styleSheet());
    input += color(ui->left3Button->styleSheet());
    input += color(ui->left6Button->styleSheet());
    input += color(ui->left9Button->styleSheet());
    input += color(ui->left2Button->styleSheet());
    input += color(ui->left8Button->styleSheet());
    input += color(ui->left1Button->styleSheet());
    input += color(ui->left4Button->styleSheet());
    input += color(ui->left7Button->styleSheet());
    std::cout << input.toStdString() << std::endl;
    QString program = "/home/lain/Desktop/TCC/backup_note/Rubiks-Cube-Solver/format.sh";
    QStringList arguments;
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program,(QStringList) arguments << input );
    
    std::string line;
    QString entrada;
    if (myProcess->waitForFinished())
    {
        myProcess->close();
        std::ifstream myfile ("format.txt");
    
         if (myfile.is_open())
          {
    
            while ( std::getline (myfile,line ))
            {
                std::cout << "size: " << line.size() << std::endl;
                if(line.size() == 120)
                   entrada = QString::fromStdString(line);
    
              std::cout << line << '\n';
            }
            std::cout << "size: " << line.size() << std::endl;
    
            myfile.close();
    
          }
         std::cout << "começa a executar o run" << std::endl;
    }
    program = "/home/lain/Desktop/TCC/backup_note/Rubiks-Cube-Solver/run.sh";
    QStringList arguments2;
    QProcess *myProcess2 = new QProcess();
    myProcess2->start(program,(QStringList) arguments2 << entrada );
   std::cout << "mandou o run do solver" << std::endl;
    myProcess2->write("exit\n\r");
    if (myProcess2->waitForFinished(-1))
    {
    std::cout << "run ficou pronto run do solver" << std::endl;
    std::ifstream myfile2 ("result.txt");
     if (myfile2.is_open())
      {
        while ( std::getline (myfile2,line ))
        {
            if(line.size() == 1 || line.size() == 2)
            {
                solve(line);
            }
          std::cout << line << '\n';
          std::cout << "size: " << line.size() << std::endl;
        }
        myfile2.close();
      }
    }
}

void MainWindow::on_colorCalibrationButton_clicked()
{
    if(!simulacao)
    {
        double R=0,G=0,B=0;
        lado = 0;
        cv::VideoCapture cam(1);
        cv::Mat pic1;
        cv::Mat pic2;
        std::vector <std::string> styles;

        while (!cam.isOpened())
        {
                std::cout << "3" << std::endl;
                std::cout << "Failed to make connection to cam" << std::endl;
                cam.open(1);
        }
        vector<vector<Point> > squares;
        while (!cam.isOpened())
        {
                
                
                cam.open(0);
        }
        cam >> pic1;
        cv::flip(pic1,pic1,-1);
        const char* wndname = "Original";
        imshow(wndname, pic1);
        while(squares.empty() || squares.size() != 9)
        {
            findSquares(pic1, squares);
            cam >> pic1;
            cv::flip(pic1,pic1,-1);
        }
        cam.~VideoCapture();
        std::cout << "debug lado" << std::endl;
        drawSquares(pic1, squares, styles, conf);
        squares.clear();
    }
}

void MainWindow::on_readColorsButton_clicked()
{
    if(simulacao==0)
    {
        try
        {
            double R=0,G=0,B=0;
            lado = 0;
            cv::VideoCapture cam(1);
            cv::Mat pic1;
            cv::Mat pic2;
            std::vector <std::string> styles;
            for(lado = 0; lado < 6; lado++)
            {
                while (!cam.isOpened())
                {
                        std::cout << "3" << std::endl;
                        std::cout << "Failed to make connection to cam" << std::endl;
                        cam.open(1);
                }
                vector<vector<Point> > squares;
                while (!cam.isOpened())
                {
                        
                        
                        cam.open(0);
                }
                cam >> pic1;
                cv::flip(pic1,pic1,-1);
                while(squares.empty() || squares.size() != 9)
                {
                    findSquares(pic1, squares);
                    cam >> pic1;
                    cv::flip(pic1,pic1,-1);
                }
                cam.~VideoCapture();
                std::cout << "debug lado" << std::endl;
                drawSquares(pic1, squares, styles, conf);
                squares.clear();
                setModel(styles, lado);
                styles.clear();
                std::string comando_aux = "0" + std::to_string(lado+1);
                char* comando = new char[comando_aux.length()];
                strcpy(comando, comando_aux.c_str());
                if(arduino->isWritable())
                {
                    arduino->write(comando);
                    if(lado+1 == 1 || lado+1 == 2 || lado+1 == 3)
                        delay(5000);
                    else if(lado+1 == 4)
                        delay(9000);
                    else if(lado+1 == 5)
                        delay(7000);
                    else if(lado+1 == 6)
                        delay(12000);
                    std::cout << comando << std::endl;
                }
                else
                {
                     std::cout << "Couldn't write to serial!" << std::endl;
                }
            }
            if(lado == 6)
            {
                lado = 0;
                if(verificaCoerencia())
                    QMessageBox::information(this,tr("Sucesso"),tr("A configuração do cubo é factível."));
                else
                {
                    QMessageBox::information(this,tr("Erro"),tr("Houve erro na identificação de cores\nSerá realizada uma nova leitura."));
                    on_readColorsButton_clicked();
                }
            }
            std::cout << "R: " << R << std::endl;
            std::cout << "G: " << G << std::endl;
            std::cout << "B: " << B << std::endl;
        }
        catch(...)
        {
            std::exception_ptr p = std::current_exception();
            std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;
        }
    }
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
