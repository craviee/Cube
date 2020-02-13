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
int movimentos = 0;
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
    completeCube();
    conf.readCalibrateColors(&conf);
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
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
            rotateU();
        else if(random2 == 1)
            rotateUA();
        else if(random2 == 2)
            rotateD();
        else if(random2 == 3)
            rotateDA();
        else if(random2 == 4)
            rotateR();
        else if(random2 == 5)
            rotateRA();
        else if(random2 == 6)
            rotateL();
        else if(random2 == 7)
            rotateLA();
        else if(random2 == 8)
            rotateF();
        else if(random2 == 9)
            rotateFA();
        else if(random2 == 10)
            rotateB();
        else if(random2 == 11)
            rotateBA();
    }
    simulacao = aux;
    movimentos = 0;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateU()
{
    if(!simulacao)
    {
                delay(35000*del);
    }
    QString auxF1 = ui->front1Button->styleSheet();
    QString auxF2 = ui->front2Button->styleSheet();
    QString auxF3 = ui->front3Button->styleSheet();
    QString auxL1 = ui->left1Button->styleSheet();
    QString auxL2 = ui->left2Button->styleSheet();
    QString auxL3 = ui->left3Button->styleSheet();
    QString auxR1 = ui->right1Button->styleSheet();
    QString auxR2 = ui->right2Button->styleSheet();
    QString auxR3 = ui->right3Button->styleSheet();
    QString auxB7 = ui->back7Button->styleSheet();
    QString auxB8 = ui->back8Button->styleSheet();
    QString auxB9 = ui->back9Button->styleSheet();
    QString auxU1 = ui->up1Button->styleSheet();
    QString auxU2 = ui->up2Button->styleSheet();
    QString auxU3 = ui->up3Button->styleSheet();
    QString auxU4 = ui->up4Button->styleSheet();
    QString auxU6 = ui->up6Button->styleSheet();
    QString auxU7 = ui->up7Button->styleSheet();
    QString auxU8 = ui->up8Button->styleSheet();
    QString auxU9 = ui->up9Button->styleSheet();
    ui->front1Button->setStyleSheet(auxR1);
    ui->front2Button->setStyleSheet(auxR2);
    ui->front3Button->setStyleSheet(auxR3);
    ui->left1Button->setStyleSheet(auxF1);
    ui->left2Button->setStyleSheet(auxF2);
    ui->left3Button->setStyleSheet(auxF3);
    ui->right1Button->setStyleSheet(auxB9);
    ui->right2Button->setStyleSheet(auxB8);
    ui->right3Button->setStyleSheet(auxB7);
    ui->back9Button->setStyleSheet(auxL1);
    ui->back8Button->setStyleSheet(auxL2);
    ui->back7Button->setStyleSheet(auxL3);
    ui->up1Button->setStyleSheet(auxU7);
    ui->up2Button->setStyleSheet(auxU4);
    ui->up3Button->setStyleSheet(auxU1);
    ui->up4Button->setStyleSheet(auxU8);
    ui->up6Button->setStyleSheet(auxU2);
    ui->up7Button->setStyleSheet(auxU9);
    ui->up8Button->setStyleSheet(auxU6);
    ui->up9Button->setStyleSheet(auxU3);
    ui->up5Button->repaint();
    movimentos++;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateUA()
{
    if(!simulacao)
    {
                delay(20000*del);
    }
    QString auxF1 = ui->front1Button->styleSheet();
    QString auxF2 = ui->front2Button->styleSheet();
    QString auxF3 = ui->front3Button->styleSheet();
    QString auxL1 = ui->left1Button->styleSheet();
    QString auxL2 = ui->left2Button->styleSheet();
    QString auxL3 = ui->left3Button->styleSheet();
    QString auxR1 = ui->right1Button->styleSheet();
    QString auxR2 = ui->right2Button->styleSheet();
    QString auxR3 = ui->right3Button->styleSheet();
    QString auxB7 = ui->back7Button->styleSheet();
    QString auxB8 = ui->back8Button->styleSheet();
    QString auxB9 = ui->back9Button->styleSheet();
    QString auxU1 = ui->up1Button->styleSheet();
    QString auxU2 = ui->up2Button->styleSheet();
    QString auxU3 = ui->up3Button->styleSheet();
    QString auxU4 = ui->up4Button->styleSheet();
    QString auxU6 = ui->up6Button->styleSheet();
    QString auxU7 = ui->up7Button->styleSheet();
    QString auxU8 = ui->up8Button->styleSheet();
    QString auxU9 = ui->up9Button->styleSheet();
    ui->front1Button->setStyleSheet(auxL1);
    ui->front2Button->setStyleSheet(auxL2);
    ui->front3Button->setStyleSheet(auxL3);
    ui->left1Button->setStyleSheet(auxB9);
    ui->left2Button->setStyleSheet(auxB8);
    ui->left3Button->setStyleSheet(auxB7);
    ui->right1Button->setStyleSheet(auxF1);
    ui->right2Button->setStyleSheet(auxF2);
    ui->right3Button->setStyleSheet(auxF3);
    ui->back9Button->setStyleSheet(auxR1);
    ui->back8Button->setStyleSheet(auxR2);
    ui->back7Button->setStyleSheet(auxR3);
    ui->up1Button->setStyleSheet(auxU3);
    ui->up2Button->setStyleSheet(auxU6);
    ui->up3Button->setStyleSheet(auxU9);
    ui->up4Button->setStyleSheet(auxU2);
    ui->up6Button->setStyleSheet(auxU8);
    ui->up7Button->setStyleSheet(auxU1);
    ui->up8Button->setStyleSheet(auxU4);
    ui->up9Button->setStyleSheet(auxU7);
    ui->up5Button->repaint();
    movimentos++;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateD()
{
    if(!simulacao)
    {
                delay(31000*del);
    }
    QString auxF7 = ui->front7Button->styleSheet();
    QString auxF8 = ui->front8Button->styleSheet();
    QString auxF9 = ui->front9Button->styleSheet();
    QString auxL7 = ui->left7Button->styleSheet();
    QString auxL8 = ui->left8Button->styleSheet();
    QString auxL9 = ui->left9Button->styleSheet();
    QString auxR7 = ui->right7Button->styleSheet();
    QString auxR8 = ui->right8Button->styleSheet();
    QString auxR9 = ui->right9Button->styleSheet();
    QString auxB1 = ui->back1Button->styleSheet();
    QString auxB2 = ui->back2Button->styleSheet();
    QString auxB3 = ui->back3Button->styleSheet();
    QString auxD1 = ui->down1Button->styleSheet();
    QString auxD2 = ui->down2Button->styleSheet();
    QString auxD3 = ui->down3Button->styleSheet();
    QString auxD4 = ui->down4Button->styleSheet();
    QString auxD6 = ui->down6Button->styleSheet();
    QString auxD7 = ui->down7Button->styleSheet();
    QString auxD8 = ui->down8Button->styleSheet();
    QString auxD9 = ui->down9Button->styleSheet();
    ui->front7Button->setStyleSheet(auxL7);
    ui->front8Button->setStyleSheet(auxL8);
    ui->front9Button->setStyleSheet(auxL9);
    ui->left7Button->setStyleSheet(auxB3);
    ui->left8Button->setStyleSheet(auxB2);
    ui->left9Button->setStyleSheet(auxB1);
    ui->right7Button->setStyleSheet(auxF7);
    ui->right8Button->setStyleSheet(auxF8);
    ui->right9Button->setStyleSheet(auxF9);
    ui->back3Button->setStyleSheet(auxR7);
    ui->back2Button->setStyleSheet(auxR8);
    ui->back1Button->setStyleSheet(auxR9);
    ui->down1Button->setStyleSheet(auxD7);
    ui->down2Button->setStyleSheet(auxD4);
    ui->down3Button->setStyleSheet(auxD1);
    ui->down4Button->setStyleSheet(auxD8);
    ui->down6Button->setStyleSheet(auxD2);
    ui->down7Button->setStyleSheet(auxD9);
    ui->down8Button->setStyleSheet(auxD6);
    ui->down9Button->setStyleSheet(auxD3);
    ui->down5Button->repaint();
    movimentos++;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateDA()
{
    if(!simulacao)
    {
                delay(20000*del);
    }
    QString auxF7 = ui->front7Button->styleSheet();
    QString auxF8 = ui->front8Button->styleSheet();
    QString auxF9 = ui->front9Button->styleSheet();
    QString auxL7 = ui->left7Button->styleSheet();
    QString auxL8 = ui->left8Button->styleSheet();
    QString auxL9 = ui->left9Button->styleSheet();
    QString auxR7 = ui->right7Button->styleSheet();
    QString auxR8 = ui->right8Button->styleSheet();
    QString auxR9 = ui->right9Button->styleSheet();
    QString auxB1 = ui->back1Button->styleSheet();
    QString auxB2 = ui->back2Button->styleSheet();
    QString auxB3 = ui->back3Button->styleSheet();
    QString auxD1 = ui->down1Button->styleSheet();
    QString auxD2 = ui->down2Button->styleSheet();
    QString auxD3 = ui->down3Button->styleSheet();
    QString auxD4 = ui->down4Button->styleSheet();
    QString auxD6 = ui->down6Button->styleSheet();
    QString auxD7 = ui->down7Button->styleSheet();
    QString auxD8 = ui->down8Button->styleSheet();
    QString auxD9 = ui->down9Button->styleSheet();
    ui->front7Button->setStyleSheet(auxR7);
    ui->front8Button->setStyleSheet(auxR8);
    ui->front9Button->setStyleSheet(auxR9);
    ui->left7Button->setStyleSheet(auxF7);
    ui->left8Button->setStyleSheet(auxF8);
    ui->left9Button->setStyleSheet(auxF9);
    ui->right7Button->setStyleSheet(auxB3);
    ui->right8Button->setStyleSheet(auxB2);
    ui->right9Button->setStyleSheet(auxB1);
    ui->back3Button->setStyleSheet(auxL7);
    ui->back2Button->setStyleSheet(auxL8);
    ui->back1Button->setStyleSheet(auxL9);
    ui->down1Button->setStyleSheet(auxD3);
    ui->down2Button->setStyleSheet(auxD6);
    ui->down3Button->setStyleSheet(auxD9);
    ui->down4Button->setStyleSheet(auxD2);
    ui->down6Button->setStyleSheet(auxD8);
    ui->down7Button->setStyleSheet(auxD1);
    ui->down8Button->setStyleSheet(auxD4);
    ui->down9Button->setStyleSheet(auxD7);
    ui->down5Button->repaint();
    movimentos++;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateR()
{
    if(!simulacao)
    {
                delay(39000*del);
    }
    QString auxF3 = ui->front3Button->styleSheet();
    QString auxF6 = ui->front6Button->styleSheet();
    QString auxF9 = ui->front9Button->styleSheet();
    QString auxU3 = ui->up3Button->styleSheet();
    QString auxU6 = ui->up6Button->styleSheet();
    QString auxU9 = ui->up9Button->styleSheet();
    QString auxB3 = ui->back3Button->styleSheet();
    QString auxB6 = ui->back6Button->styleSheet();
    QString auxB9 = ui->back9Button->styleSheet();
    QString auxD3 = ui->down3Button->styleSheet();
    QString auxD6 = ui->down6Button->styleSheet();
    QString auxD9 = ui->down9Button->styleSheet();
    QString auxR1 = ui->right1Button->styleSheet();
    QString auxR2 = ui->right2Button->styleSheet();
    QString auxR3 = ui->right3Button->styleSheet();
    QString auxR4 = ui->right4Button->styleSheet();
    QString auxR6 = ui->right6Button->styleSheet();
    QString auxR7 = ui->right7Button->styleSheet();
    QString auxR8 = ui->right8Button->styleSheet();
    QString auxR9 = ui->right9Button->styleSheet();
    ui->front3Button->setStyleSheet(auxD3);
    ui->front6Button->setStyleSheet(auxD6);
    ui->front9Button->setStyleSheet(auxD9);
    ui->up3Button->setStyleSheet(auxF3);
    ui->up6Button->setStyleSheet(auxF6);
    ui->up9Button->setStyleSheet(auxF9);
    ui->back3Button->setStyleSheet(auxU3);
    ui->back6Button->setStyleSheet(auxU6);
    ui->back9Button->setStyleSheet(auxU9);
    ui->down3Button->setStyleSheet(auxB3);
    ui->down6Button->setStyleSheet(auxB6);
    ui->down9Button->setStyleSheet(auxB9);
    ui->right1Button->setStyleSheet(auxR7);
    ui->right2Button->setStyleSheet(auxR4);
    ui->right3Button->setStyleSheet(auxR1);
    ui->right4Button->setStyleSheet(auxR8);
    ui->right6Button->setStyleSheet(auxR2);
    ui->right7Button->setStyleSheet(auxR9);
    ui->right8Button->setStyleSheet(auxR6);
    ui->right9Button->setStyleSheet(auxR3);
    ui->right5Button->repaint();
    movimentos++;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateRA()
{
    if(!simulacao)
    {
                delay(29000*del);
    }
    QString auxF3 = ui->front3Button->styleSheet();
    QString auxF6 = ui->front6Button->styleSheet();
    QString auxF9 = ui->front9Button->styleSheet();
    QString auxU3 = ui->up3Button->styleSheet();
    QString auxU6 = ui->up6Button->styleSheet();
    QString auxU9 = ui->up9Button->styleSheet();
    QString auxB3 = ui->back3Button->styleSheet();
    QString auxB6 = ui->back6Button->styleSheet();
    QString auxB9 = ui->back9Button->styleSheet();
    QString auxD3 = ui->down3Button->styleSheet();
    QString auxD6 = ui->down6Button->styleSheet();
    QString auxD9 = ui->down9Button->styleSheet();
    QString auxR1 = ui->right1Button->styleSheet();
    QString auxR2 = ui->right2Button->styleSheet();
    QString auxR3 = ui->right3Button->styleSheet();
    QString auxR4 = ui->right4Button->styleSheet();
    QString auxR6 = ui->right6Button->styleSheet();
    QString auxR7 = ui->right7Button->styleSheet();
    QString auxR8 = ui->right8Button->styleSheet();
    QString auxR9 = ui->right9Button->styleSheet();
    ui->front3Button->setStyleSheet(auxU3);
    ui->front6Button->setStyleSheet(auxU6);
    ui->front9Button->setStyleSheet(auxU9);
    ui->up3Button->setStyleSheet(auxB3);
    ui->up6Button->setStyleSheet(auxB6);
    ui->up9Button->setStyleSheet(auxB9);
    ui->back3Button->setStyleSheet(auxD3);
    ui->back6Button->setStyleSheet(auxD6);
    ui->back9Button->setStyleSheet(auxD9);
    ui->down3Button->setStyleSheet(auxF3);
    ui->down6Button->setStyleSheet(auxF6);
    ui->down9Button->setStyleSheet(auxF9);
    ui->right1Button->setStyleSheet(auxR3);
    ui->right2Button->setStyleSheet(auxR6);
    ui->right3Button->setStyleSheet(auxR9);
    ui->right4Button->setStyleSheet(auxR2);
    ui->right6Button->setStyleSheet(auxR8);
    ui->right7Button->setStyleSheet(auxR1);
    ui->right8Button->setStyleSheet(auxR4);
    ui->right9Button->setStyleSheet(auxR7);
    ui->right5Button->repaint();
    movimentos++;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateL()
{
    if(!simulacao)
    {
                delay(39000*del);
    }
    QString auxF1 = ui->front1Button->styleSheet();
    QString auxF4 = ui->front4Button->styleSheet();
    QString auxF7 = ui->front7Button->styleSheet();
    QString auxU1 = ui->up1Button->styleSheet();
    QString auxU4 = ui->up4Button->styleSheet();
    QString auxU7 = ui->up7Button->styleSheet();
    QString auxB1 = ui->back1Button->styleSheet();
    QString auxB4 = ui->back4Button->styleSheet();
    QString auxB7 = ui->back7Button->styleSheet();
    QString auxD1 = ui->down1Button->styleSheet();
    QString auxD4 = ui->down4Button->styleSheet();
    QString auxD7 = ui->down7Button->styleSheet();
    QString auxL1 = ui->left1Button->styleSheet();
    QString auxL2 = ui->left2Button->styleSheet();
    QString auxL3 = ui->left3Button->styleSheet();
    QString auxL4 = ui->left4Button->styleSheet();
    QString auxL6 = ui->left6Button->styleSheet();
    QString auxL7 = ui->left7Button->styleSheet();
    QString auxL8 = ui->left8Button->styleSheet();
    QString auxL9 = ui->left9Button->styleSheet();
    ui->front1Button->setStyleSheet(auxU1);
    ui->front4Button->setStyleSheet(auxU4);
    ui->front7Button->setStyleSheet(auxU7);
    ui->up1Button->setStyleSheet(auxB1);
    ui->up4Button->setStyleSheet(auxB4);
    ui->up7Button->setStyleSheet(auxB7);
    ui->back1Button->setStyleSheet(auxD1);
    ui->back4Button->setStyleSheet(auxD4);
    ui->back7Button->setStyleSheet(auxD7);
    ui->down1Button->setStyleSheet(auxF1);
    ui->down4Button->setStyleSheet(auxF4);
    ui->down7Button->setStyleSheet(auxF7);
    ui->left1Button->setStyleSheet(auxL7);
    ui->left2Button->setStyleSheet(auxL4);
    ui->left3Button->setStyleSheet(auxL1);
    ui->left4Button->setStyleSheet(auxL8);
    ui->left6Button->setStyleSheet(auxL2);
    ui->left7Button->setStyleSheet(auxL9);
    ui->left8Button->setStyleSheet(auxL6);
    ui->left9Button->setStyleSheet(auxL3);
    ui->left5Button->repaint();
    movimentos++;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateLA()
{
    if(!simulacao)
    {
                delay(21000*del);
    }
    QString auxF1 = ui->front1Button->styleSheet();
    QString auxF4 = ui->front4Button->styleSheet();
    QString auxF7 = ui->front7Button->styleSheet();
    QString auxU1 = ui->up1Button->styleSheet();
    QString auxU4 = ui->up4Button->styleSheet();
    QString auxU7 = ui->up7Button->styleSheet();
    QString auxB1 = ui->back1Button->styleSheet();
    QString auxB4 = ui->back4Button->styleSheet();
    QString auxB7 = ui->back7Button->styleSheet();
    QString auxD1 = ui->down1Button->styleSheet();
    QString auxD4 = ui->down4Button->styleSheet();
    QString auxD7 = ui->down7Button->styleSheet();
    QString auxL1 = ui->left1Button->styleSheet();
    QString auxL2 = ui->left2Button->styleSheet();
    QString auxL3 = ui->left3Button->styleSheet();
    QString auxL4 = ui->left4Button->styleSheet();
    QString auxL6 = ui->left6Button->styleSheet();
    QString auxL7 = ui->left7Button->styleSheet();
    QString auxL8 = ui->left8Button->styleSheet();
    QString auxL9 = ui->left9Button->styleSheet();
    ui->front1Button->setStyleSheet(auxD1);
    ui->front4Button->setStyleSheet(auxD4);
    ui->front7Button->setStyleSheet(auxD7);
    ui->up1Button->setStyleSheet(auxF1);
    ui->up4Button->setStyleSheet(auxF4);
    ui->up7Button->setStyleSheet(auxF7);
    ui->back1Button->setStyleSheet(auxU1);
    ui->back4Button->setStyleSheet(auxU4);
    ui->back7Button->setStyleSheet(auxU7);
    ui->down1Button->setStyleSheet(auxB1);
    ui->down4Button->setStyleSheet(auxB4);
    ui->down7Button->setStyleSheet(auxB7);
    ui->left1Button->setStyleSheet(auxL3);
    ui->left2Button->setStyleSheet(auxL6);
    ui->left3Button->setStyleSheet(auxL9);
    ui->left4Button->setStyleSheet(auxL2);
    ui->left6Button->setStyleSheet(auxL8);
    ui->left7Button->setStyleSheet(auxL1);
    ui->left8Button->setStyleSheet(auxL4);
    ui->left9Button->setStyleSheet(auxL7);
    ui->left5Button->repaint();
    movimentos++;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateF()
{
    if(!simulacao)
    {
                delay(35000*del);
    }
    QString auxU7 = ui->up7Button->styleSheet();
    QString auxU8 = ui->up8Button->styleSheet();
    QString auxU9 = ui->up9Button->styleSheet();
    QString auxR1 = ui->right1Button->styleSheet();
    QString auxR4 = ui->right4Button->styleSheet();
    QString auxR7 = ui->right7Button->styleSheet();
    QString auxD1 = ui->down1Button->styleSheet();
    QString auxD2 = ui->down2Button->styleSheet();
    QString auxD3 = ui->down3Button->styleSheet();
    QString auxL3 = ui->left3Button->styleSheet();
    QString auxL6 = ui->left6Button->styleSheet();
    QString auxL9 = ui->left9Button->styleSheet();
    QString auxF1 = ui->front1Button->styleSheet();
    QString auxF2 = ui->front2Button->styleSheet();
    QString auxF3 = ui->front3Button->styleSheet();
    QString auxF4 = ui->front4Button->styleSheet();
    QString auxF6 = ui->front6Button->styleSheet();
    QString auxF7 = ui->front7Button->styleSheet();
    QString auxF8 = ui->front8Button->styleSheet();
    QString auxF9 = ui->front9Button->styleSheet();
    ui->up7Button->setStyleSheet(auxL9);
    ui->up8Button->setStyleSheet(auxL6);
    ui->up9Button->setStyleSheet(auxL3);
    ui->right1Button->setStyleSheet(auxU7);
    ui->right4Button->setStyleSheet(auxU8);
    ui->right7Button->setStyleSheet(auxU9);
    ui->down1Button->setStyleSheet(auxR7);
    ui->down2Button->setStyleSheet(auxR4);
    ui->down3Button->setStyleSheet(auxR1);
    ui->left3Button->setStyleSheet(auxD1);
    ui->left6Button->setStyleSheet(auxD2);
    ui->left9Button->setStyleSheet(auxD3);
    ui->front1Button->setStyleSheet(auxF7);
    ui->front2Button->setStyleSheet(auxF4);
    ui->front3Button->setStyleSheet(auxF1);
    ui->front4Button->setStyleSheet(auxF8);
    ui->front6Button->setStyleSheet(auxF2);
    ui->front7Button->setStyleSheet(auxF9);
    ui->front8Button->setStyleSheet(auxF6);
    ui->front9Button->setStyleSheet(auxF3);
    ui->front5Button->repaint();
    movimentos++;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateFA()
{
    if(!simulacao)
    {
                delay(20000*del);
    }
    QString auxU7 = ui->up7Button->styleSheet();
    QString auxU8 = ui->up8Button->styleSheet();
    QString auxU9 = ui->up9Button->styleSheet();
    QString auxR1 = ui->right1Button->styleSheet();
    QString auxR4 = ui->right4Button->styleSheet();
    QString auxR7 = ui->right7Button->styleSheet();
    QString auxD1 = ui->down1Button->styleSheet();
    QString auxD2 = ui->down2Button->styleSheet();
    QString auxD3 = ui->down3Button->styleSheet();
    QString auxL3 = ui->left3Button->styleSheet();
    QString auxL6 = ui->left6Button->styleSheet();
    QString auxL9 = ui->left9Button->styleSheet();
    QString auxF1 = ui->front1Button->styleSheet();
    QString auxF2 = ui->front2Button->styleSheet();
    QString auxF3 = ui->front3Button->styleSheet();
    QString auxF4 = ui->front4Button->styleSheet();
    QString auxF6 = ui->front6Button->styleSheet();
    QString auxF7 = ui->front7Button->styleSheet();
    QString auxF8 = ui->front8Button->styleSheet();
    QString auxF9 = ui->front9Button->styleSheet();
    ui->left9Button->setStyleSheet(auxU7);
    ui->left6Button->setStyleSheet(auxU8);
    ui->left3Button->setStyleSheet(auxU9);
    ui->up7Button->setStyleSheet(auxR1);
    ui->up8Button->setStyleSheet(auxR4);
    ui->up9Button->setStyleSheet(auxR7);
    ui->right7Button->setStyleSheet(auxD1);
    ui->right4Button->setStyleSheet(auxD2);
    ui->right1Button->setStyleSheet(auxD3);
    ui->down1Button->setStyleSheet(auxL3);
    ui->down2Button->setStyleSheet(auxL6);
    ui->down3Button->setStyleSheet(auxL9);
    ui->front1Button->setStyleSheet(auxF3);
    ui->front2Button->setStyleSheet(auxF6);
    ui->front3Button->setStyleSheet(auxF9);
    ui->front4Button->setStyleSheet(auxF2);
    ui->front6Button->setStyleSheet(auxF8);
    ui->front7Button->setStyleSheet(auxF1);
    ui->front8Button->setStyleSheet(auxF4);
    ui->front9Button->setStyleSheet(auxF7);
    ui->front5Button->repaint();
    movimentos++;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateB()
{
    if(!simulacao)
    {
                delay(27000*del);
    }
    QString auxU1 = ui->up1Button->styleSheet();
    QString auxU2 = ui->up2Button->styleSheet();
    QString auxU3 = ui->up3Button->styleSheet();
    QString auxR3 = ui->right3Button->styleSheet();
    QString auxR6 = ui->right6Button->styleSheet();
    QString auxR9 = ui->right9Button->styleSheet();
    QString auxD7 = ui->down7Button->styleSheet();
    QString auxD8 = ui->down8Button->styleSheet();
    QString auxD9 = ui->down9Button->styleSheet();
    QString auxL1 = ui->left1Button->styleSheet();
    QString auxL4 = ui->left4Button->styleSheet();
    QString auxL7 = ui->left7Button->styleSheet();
    QString auxB1 = ui->back1Button->styleSheet();
    QString auxB2 = ui->back2Button->styleSheet();
    QString auxB3 = ui->back3Button->styleSheet();
    QString auxB4 = ui->back4Button->styleSheet();
    QString auxB6 = ui->back6Button->styleSheet();
    QString auxB7 = ui->back7Button->styleSheet();
    QString auxB8 = ui->back8Button->styleSheet();
    QString auxB9 = ui->back9Button->styleSheet();
    ui->up1Button->setStyleSheet(auxR3);
    ui->up2Button->setStyleSheet(auxR6);
    ui->up3Button->setStyleSheet(auxR9);
    ui->right3Button->setStyleSheet(auxD9);
    ui->right6Button->setStyleSheet(auxD8);
    ui->right9Button->setStyleSheet(auxD7);
    ui->down7Button->setStyleSheet(auxL1);
    ui->down8Button->setStyleSheet(auxL4);
    ui->down9Button->setStyleSheet(auxL7);
    ui->left1Button->setStyleSheet(auxU3);
    ui->left4Button->setStyleSheet(auxU2);
    ui->left7Button->setStyleSheet(auxU1);
    ui->back1Button->setStyleSheet(auxB7);
    ui->back2Button->setStyleSheet(auxB4);
    ui->back3Button->setStyleSheet(auxB1);
    ui->back4Button->setStyleSheet(auxB8);
    ui->back6Button->setStyleSheet(auxB2);
    ui->back7Button->setStyleSheet(auxB9);
    ui->back8Button->setStyleSheet(auxB6);
    ui->back9Button->setStyleSheet(auxB3);
    ui->back5Button->repaint();
    movimentos++;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateBA()
{
    if(!simulacao)
    {
                delay(19000*del);
    }
    QString auxU1 = ui->up1Button->styleSheet();
    QString auxU2 = ui->up2Button->styleSheet();
    QString auxU3 = ui->up3Button->styleSheet();
    QString auxR3 = ui->right3Button->styleSheet();
    QString auxR6 = ui->right6Button->styleSheet();
    QString auxR9 = ui->right9Button->styleSheet();
    QString auxD7 = ui->down7Button->styleSheet();
    QString auxD8 = ui->down8Button->styleSheet();
    QString auxD9 = ui->down9Button->styleSheet();
    QString auxL1 = ui->left1Button->styleSheet();
    QString auxL4 = ui->left4Button->styleSheet();
    QString auxL7 = ui->left7Button->styleSheet();
    QString auxB1 = ui->back1Button->styleSheet();
    QString auxB2 = ui->back2Button->styleSheet();
    QString auxB3 = ui->back3Button->styleSheet();
    QString auxB4 = ui->back4Button->styleSheet();
    QString auxB6 = ui->back6Button->styleSheet();
    QString auxB7 = ui->back7Button->styleSheet();
    QString auxB8 = ui->back8Button->styleSheet();
    QString auxB9 = ui->back9Button->styleSheet();
    ui->up1Button->setStyleSheet(auxL7);
    ui->up2Button->setStyleSheet(auxL4);
    ui->up3Button->setStyleSheet(auxL1);
    ui->right3Button->setStyleSheet(auxU1);
    ui->right6Button->setStyleSheet(auxU2);
    ui->right9Button->setStyleSheet(auxU3);
    ui->down7Button->setStyleSheet(auxR9);
    ui->down8Button->setStyleSheet(auxR6);
    ui->down9Button->setStyleSheet(auxR3);
    ui->left1Button->setStyleSheet(auxD7);
    ui->left4Button->setStyleSheet(auxD8);
    ui->left7Button->setStyleSheet(auxD9);
    ui->back1Button->setStyleSheet(auxB3);
    ui->back2Button->setStyleSheet(auxB6);
    ui->back3Button->setStyleSheet(auxB9);
    ui->back4Button->setStyleSheet(auxB2);
    ui->back6Button->setStyleSheet(auxB8);
    ui->back7Button->setStyleSheet(auxB1);
    ui->back8Button->setStyleSheet(auxB4);
    ui->back9Button->setStyleSheet(auxB7);
    ui->back5Button->repaint();
    movimentos++;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::completeCube()
{
    
    ui->up1Button->setStyleSheet("background-color: blue");
    ui->up2Button->setStyleSheet("background-color: blue");
    ui->up3Button->setStyleSheet("background-color: blue");
    ui->up4Button->setStyleSheet("background-color: blue");
    ui->up5Button->setStyleSheet("background-color: blue");
    ui->up6Button->setStyleSheet("background-color: blue");
    ui->up7Button->setStyleSheet("background-color: blue");
    ui->up8Button->setStyleSheet("background-color: blue");
    ui->up9Button->setStyleSheet("background-color: blue");
    ui->front1Button->setStyleSheet("background-color: white");
    ui->front2Button->setStyleSheet("background-color: white");
    ui->front3Button->setStyleSheet("background-color: white");
    ui->front4Button->setStyleSheet("background-color: white");
    ui->front5Button->setStyleSheet("background-color: white");
    ui->front6Button->setStyleSheet("background-color: white");
    ui->front7Button->setStyleSheet("background-color: white");
    ui->front8Button->setStyleSheet("background-color: white");
    ui->front9Button->setStyleSheet("background-color: white");
    ui->left1Button->setStyleSheet("background-color: orange");
    ui->left2Button->setStyleSheet("background-color: orange");
    ui->left3Button->setStyleSheet("background-color: orange");
    ui->left4Button->setStyleSheet("background-color: orange");
    ui->left5Button->setStyleSheet("background-color: orange");
    ui->left6Button->setStyleSheet("background-color: orange");
    ui->left7Button->setStyleSheet("background-color: orange");
    ui->left8Button->setStyleSheet("background-color: orange");
    ui->left9Button->setStyleSheet("background-color: orange");
    ui->right1Button->setStyleSheet("background-color: red");
    ui->right2Button->setStyleSheet("background-color: red");
    ui->right3Button->setStyleSheet("background-color: red");
    ui->right4Button->setStyleSheet("background-color: red");
    ui->right5Button->setStyleSheet("background-color: red");
    ui->right6Button->setStyleSheet("background-color: red");
    ui->right7Button->setStyleSheet("background-color: red");
    ui->right8Button->setStyleSheet("background-color: red");
    ui->right9Button->setStyleSheet("background-color: red");
    ui->down1Button->setStyleSheet("background-color: green");
    ui->down2Button->setStyleSheet("background-color: green");
    ui->down3Button->setStyleSheet("background-color: green");
    ui->down4Button->setStyleSheet("background-color: green");
    ui->down5Button->setStyleSheet("background-color: green");
    ui->down6Button->setStyleSheet("background-color: green");
    ui->down7Button->setStyleSheet("background-color: green");
    ui->down8Button->setStyleSheet("background-color: green");
    ui->down9Button->setStyleSheet("background-color: green");
    ui->back1Button->setStyleSheet("background-color: yellow");
    ui->back2Button->setStyleSheet("background-color: yellow");
    ui->back3Button->setStyleSheet("background-color: yellow");
    ui->back4Button->setStyleSheet("background-color: yellow");
    ui->back5Button->setStyleSheet("background-color: yellow");
    ui->back6Button->setStyleSheet("background-color: yellow");
    ui->back7Button->setStyleSheet("background-color: yellow");
    ui->back8Button->setStyleSheet("background-color: yellow");
    ui->back9Button->setStyleSheet("background-color: yellow");
    
    /*
    ui->up5Button->setStyleSheet("background-color: blue");
    ui->down5Button->setStyleSheet("background-color: green");
    ui->right5Button->setStyleSheet("background-color: red");
    ui->front5Button->setStyleSheet("background-color: white");
    ui->left5Button->setStyleSheet("background-color: orange");
    ui->back5Button->setStyleSheet("background-color: yellow");
    ui->back1Button->setStyleSheet("background-color: blue");
    ui->down7Button->setStyleSheet("background-color: blue");
    ui->left7Button->setStyleSheet("background-color: blue");
    */
}
void MainWindow::on_rotationUButton_clicked()
{
   rotateU();
}
void MainWindow::on_rotationUAButton_clicked()
{
    rotateUA();
}
void MainWindow::on_rotationDButton_clicked()
{
    rotateD();
}
void MainWindow::on_rotationDAButton_clicked()
{
   rotateDA();
}
void MainWindow::on_rotationRButton_clicked()
{
    rotateR();
}
void MainWindow::on_rotationRAButton_clicked()
{
    rotateRA();
}
void MainWindow::on_rotationLButton_clicked()
{
    rotateL();
}
void MainWindow::on_rotationLAButton_clicked()
{
    rotateLA();
}
void MainWindow::on_rotationFButton_clicked()
{
    rotateF();
}
void MainWindow::on_rotationFAButton_clicked()
{
    rotateFA();
}
void MainWindow::on_rotationBButton_clicked()
{
    rotateB();
}
void MainWindow::on_rotationBAButton_clicked()
{
    rotateBA();
}
void MainWindow::CruzSul()
{
    if(ui->up8Button->styleSheet() != ui->up5Button->styleSheet() || ui->front2Button->styleSheet() != ui->front5Button->styleSheet())
    {
        if(ui->front2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up8Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateF();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->front6Button->styleSheet() == ui->up5Button->styleSheet() && ui->right4Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->front6Button->styleSheet() == ui->front5Button->styleSheet() && ui->right4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateFA();
        }
        else if(ui->front4Button->styleSheet() == ui->up5Button->styleSheet() && ui->left6Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateU();
            rotateLA();
            rotateUA();
        }
        else if(ui->front4Button->styleSheet() == ui->front5Button->styleSheet() && ui->left6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateF();
        }
        else if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateF();
            rotateF();
        }
        else if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down2Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateFA();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->right2Button->styleSheet() == ui->front5Button->styleSheet() && ui->up6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateU();
        }
        else if(ui->right2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up6Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateU();
            rotateF();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->left2Button->styleSheet() == ui->front5Button->styleSheet() && ui->up4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateUA();
        }
        else if(ui->left2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up4Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateUA();
            rotateF();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->right8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateDA();
            rotateF();
            rotateF();
        }
        else if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down6Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateDA();
            rotateFA();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->left8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateF();
            rotateF();
        }
        else if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down4Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateD();
            rotateFA();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->back8Button->styleSheet() == ui->front5Button->styleSheet() && ui->up2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateU();
            rotateU();
        }
        else if(ui->back8Button->styleSheet() == ui->up5Button->styleSheet() && ui->up2Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateU();
            rotateU();
            rotateF();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->back6Button->styleSheet() == ui->front5Button->styleSheet() && ui->right6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateB();
            rotateU();
            rotateU();
        }
        else if(ui->back6Button->styleSheet() == ui->up5Button->styleSheet() && ui->right6Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateB();
            rotateU();
            rotateU();
            rotateF();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->back4Button->styleSheet() == ui->front5Button->styleSheet() && ui->left4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateBA();
            rotateU();
            rotateU();
        }
        else if(ui->back4Button->styleSheet() == ui->up5Button->styleSheet() && ui->left4Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateBA();
            rotateU();
            rotateU();
            rotateF();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->back2Button->styleSheet() == ui->front5Button->styleSheet() && ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateF();
            rotateF();
        }
        else if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet() && ui->down8Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateFA();
            rotateUA();
            rotateR();
            rotateU();
        }
    }
}
void MainWindow::CruzLeste()
{
    if(ui->up6Button->styleSheet() != ui->up5Button->styleSheet() || ui->right2Button->styleSheet() != ui->right5Button->styleSheet())
    {
        if(ui->up6Button->styleSheet() == ui->right5Button->styleSheet() && ui->right2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateR();
            rotateUA();
            rotateB();
            rotateU();
            ult = "1L";
            
        }
        else if(ui->right6Button->styleSheet() == ui->up5Button->styleSheet() && ui->back6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateUA();
            rotateB();
            rotateU();
            ult = "2L";
            
        }
        else if(ui->right6Button->styleSheet() == ui->right5Button->styleSheet() && ui->back6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateRA();
            ult = "3L";
            
        }
        else if(ui->right4Button->styleSheet() == ui->up5Button->styleSheet() && ui->front6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateU();
            rotateFA();
            rotateUA();
            ult = "4L";
            
        }
        else if(ui->right4Button->styleSheet() == ui->right5Button->styleSheet() && ui->front6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateR();
            ult = "5L";
            
        }
        else if(ui->right8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateR();
            rotateR();
            ult = "6L";
            
        }
        else if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateRA();
            rotateUA();
            rotateB();
            rotateU();
            ult = "7L";
            
        }
        else if(ui->back8Button->styleSheet() == ui->right5Button->styleSheet() && ui->up2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateBA();
            rotateUA();
            rotateB();
            rotateU();
            ult = "8L";
            
        }
        else if(ui->back8Button->styleSheet() == ui->up5Button->styleSheet() && ui->up2Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateU();
            rotateR();
            rotateUA();
            rotateB();
            rotateU();
            ult = "9L";
            
        }
        else if(ui->front2Button->styleSheet() == ui->right5Button->styleSheet() && ui->up8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateFA();
            rotateU();
            rotateF();
            rotateUA();
            ult = "10L";
            
        }
        else if(ui->front2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up8Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateF();
            rotateR();
            ult = "11L";
            
        }
        else if(ui->back2Button->styleSheet() == ui->right5Button->styleSheet() && ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateDA();
            rotateR();
            rotateR();
            ult = "12L";
            
        }
        else if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet() && ui->down8Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateDA();
            rotateRA();
            rotateUA();
            rotateB();
            rotateU();
            ult = "13L";
            
        }
        else if(ui->front8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateR();
            rotateR();
            ult = "14L";
            
        }
        else if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down2Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateD();
            rotateRA();
            rotateUA();
            rotateB();
            rotateU();
            ult = "15L";
            
        }
        else if(ui->left2Button->styleSheet() == ui->right5Button->styleSheet() && ui->up4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateL();
            rotateL();
            rotateD();
            rotateD();
            rotateR();
            rotateR();
            ult = "16L";
            
        }
        else if(ui->left2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateU();
            rotateU();
            rotateR();
            rotateUA();
            rotateB();
            rotateU();
            ult = "17L";
            
        }
        else if(ui->left4Button->styleSheet() == ui->right5Button->styleSheet() && ui->back4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateL();
            rotateU();
            rotateU();
            ult = "18L";
            
        }
        else if(ui->left4Button->styleSheet() == ui->up5Button->styleSheet() && ui->back4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateL();
            rotateU();
            rotateU();
            rotateR();
            rotateUA();
            rotateB();
            rotateU();
            ult = "19L";
            
        }
        else if(ui->left6Button->styleSheet() == ui->right5Button->styleSheet() && ui->front4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            
            rotateU();
            rotateU();
            rotateLA();
            rotateUA();
            rotateUA();
            ult = "20L";
            
        }
        else if(ui->left6Button->styleSheet() == ui->up5Button->styleSheet() && ui->front4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateLA();
            rotateU();
            rotateU();
            rotateR();
            rotateUA();
            rotateB();
            rotateU();
            ult = "21L";
            
        }
        else if(ui->left8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateR();
            rotateR();
            ult = "22L";
            
        }
        else if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateRA();
            rotateUA();
            rotateB();
            rotateU();
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
            rotateL();
            rotateUA();
            rotateF();
            rotateU();
            ult = "1O";
            
        }
        else if(ui->left6Button->styleSheet() == ui->up5Button->styleSheet() && ui->front4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateUA();
            rotateF();
            rotateU();
            ult = "2O";
            
        }
        else if(ui->left6Button->styleSheet() == ui->left5Button->styleSheet() && ui->front4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateLA();
            ult = "3O";
            
        }
        else if(ui->left4Button->styleSheet() == ui->up5Button->styleSheet() && ui->back4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateU();
            rotateBA();
            rotateUA();
            ult = "4O";
            
        }
        else if(ui->left4Button->styleSheet() == ui->left5Button->styleSheet() && ui->back4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateL();
            ult = "5O";
            
        }
        else if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateL();
            rotateL();
            ult = "6O";
            
        }
        else if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateLA();
            rotateUA();
            rotateF();
            rotateU();
            ult = "7O";
            
        }
        else if(ui->front2Button->styleSheet() == ui->left5Button->styleSheet() && ui->up8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateU();
            ult = "8O";
            
        }
        else if(ui->front2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up8Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateU();
            rotateL();
            rotateUA();
            rotateF();
            rotateU();
            ult = "9O";
            
        }
        else if(ui->back8Button->styleSheet() == ui->left5Button->styleSheet() && ui->up2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateUA();
            ult = "10O";
            
        }
        else if(ui->back8Button->styleSheet() == ui->up5Button->styleSheet() && ui->up2Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateUA();
            rotateL();
            rotateUA();
            rotateF();
            rotateU();
            ult = "11O";
            
        }
        else if(ui->front8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateDA();
            rotateL();
            rotateL();
            ult = "12O";
            
        }
        else if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down2Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateDA();
            rotateLA();
            rotateUA();
            rotateF();
            rotateU();
            ult = "13O";
            
        }
        else if(ui->back2Button->styleSheet() == ui->left5Button->styleSheet() && ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateL();
            rotateL();
            ult = "14O";
            
        }
        else if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet() && ui->down8Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateD();
            rotateLA();
            rotateUA();
            rotateF();
            rotateU();
            ult = "15O";
            
        }
        else if(ui->right2Button->styleSheet() == ui->left5Button->styleSheet() && ui->up6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateU();
            rotateU();
            ult = "16O";
            
        }
        else if(ui->right2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateU();
            rotateU();
            rotateL();
            rotateUA();
            rotateF();
            rotateU();
            ult = "17O";
            
        }
        else if(ui->right4Button->styleSheet() == ui->left5Button->styleSheet() && ui->front6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateU();
            rotateU();
            rotateR();
            rotateUA();
            rotateUA();
            ult = "18O";
            
        }
        else if(ui->right4Button->styleSheet() == ui->up5Button->styleSheet() && ui->front6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateR();
            rotateU();
            rotateU();
            rotateL();
            rotateUA();
            rotateF();
            rotateU();
            ult = "19O";
            
        }
        else if(ui->right6Button->styleSheet() == ui->left5Button->styleSheet() && ui->back6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateR();
            rotateD();
            rotateD();
            rotateL();
            rotateL();
            ult = "20O";
            
        }
        else if(ui->right6Button->styleSheet() == ui->up5Button->styleSheet() && ui->back6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateRA();
            rotateU();
            rotateU();
            rotateL();
            rotateUA();
            rotateF();
            rotateU();
            ult = "21O";
            
        }
        else if(ui->right8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateL();
            rotateL();
            ult = "22O";
            
        }
        else if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateLA();
            rotateUA();
            rotateF();
            rotateU();
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
            rotateB();
            rotateUA();
            rotateL();
            rotateU();
            ult = "1N";
            
        }
        else if(ui->back4Button->styleSheet() == ui->up5Button->styleSheet() && ui->left4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateUA();
            rotateL();
            rotateU();
            ult = "2N";
            
        }
        else if(ui->back4Button->styleSheet() == ui->back5Button->styleSheet() && ui->left4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateBA();
            ult = "3N";
            
        }
        else if(ui->back6Button->styleSheet() == ui->up5Button->styleSheet() && ui->right6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateU();
            rotateRA();
            rotateUA();
            ult = "4N";
            
        }
        else if(ui->back6Button->styleSheet() == ui->back5Button->styleSheet() && ui->right6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateB();
            ult = "5N";
            
        }
        else if(ui->back2Button->styleSheet() == ui->back5Button->styleSheet() && ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateB();
            rotateB();
            ult = "6N";
            
        }
        else if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet() && ui->down8Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateBA();
            rotateUA();
            rotateL();
            rotateU();
            ult = "7N";
            
        }
        else if(ui->left2Button->styleSheet() == ui->back5Button->styleSheet() && ui->up4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateLA();
            rotateUA();
            rotateL();
            rotateU();
            ult = "8N";
            
        }
        else if(ui->left2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateU();
            rotateB();
            rotateUA();
            rotateL();
            rotateU();
            ult = "9N";
            
        }
        else if(ui->right2Button->styleSheet() == ui->back5Button->styleSheet() && ui->up6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateR();
            rotateU();
            rotateRA();
            rotateUA();
            ult = "10N";
            
        }
        else if(ui->right2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateUA();
            rotateB();
            rotateUA();
            rotateL();
            rotateU();
            ult = "11N";
            
        }
        else if(ui->left8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateDA();
            rotateB();
            rotateB();
            ult = "12N";
            
        }
        else if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateDA();
            rotateBA();
            rotateUA();
            rotateL();
            rotateU();
            ult = "13N";
            
        }
        else if(ui->right8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateB();
            rotateB();
            ult = "14N";
            
        }
        else if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateD();
            rotateBA();
            rotateUA();
            rotateL();
            rotateU();
            ult = "15N";
            
        }
        else if(ui->front2Button->styleSheet() == ui->back5Button->styleSheet() && ui->up8Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateU();
            rotateU();
            ult = "16N";
            
        }
        else if(ui->front2Button->styleSheet() == ui->up5Button->styleSheet() && ui->up8Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateU();
            rotateU();
            rotateB();
            rotateUA();
            rotateL();
            rotateU();
            ult = "17N";
            
        }
        else if(ui->front4Button->styleSheet() == ui->back5Button->styleSheet() && ui->left6Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateF();
            rotateU();
            rotateU();
            ult = "18N";
            
        }
        else if(ui->front4Button->styleSheet() == ui->up5Button->styleSheet() && ui->left6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateF();
            rotateU();
            rotateU();
            rotateB();
            rotateUA();
            rotateL();
            rotateU();
            ult = "19N";
            
        }
        else if(ui->front6Button->styleSheet() == ui->back5Button->styleSheet() && ui->right4Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateFA();
            rotateU();
            rotateU();
            ult = "20N";
            
        }
        else if(ui->front6Button->styleSheet() == ui->up5Button->styleSheet() && ui->right4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateFA();
            rotateU();
            rotateU();
            rotateB();
            rotateUA();
            rotateL();
            rotateU();
            ult = "21N";
            
        }
        else if(ui->front8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateF();
            rotateF();
            rotateBA();
            rotateBA();
            ult = "22N";
            
        }
        else if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet() && ui->down2Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateBA();
            rotateUA();
            rotateL();
            rotateU();
            ult = "23N";
            
        }
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
void MainWindow::on_restartButton_clicked()
{
    completeCube();
    movimentos = 0;
    ui->rotationsNumberTextBox->setPlainText(QString::number(movimentos));
}
void MainWindow::QuinaSudeste()
{
    if(ui->up9Button->styleSheet() != ui->up5Button->styleSheet() || ui->front3Button->styleSheet() != ui->front5Button->styleSheet() || ui->right1Button->styleSheet() != ui->right5Button->styleSheet())
    {
        if(ui->up9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateRA();
            rotateDA();
            rotateR();
            if(ui->front7Button->styleSheet() == ui->front5Button->styleSheet()) 
            {
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
            }
            else if(ui->front7Button->styleSheet() == ui->left5Button->styleSheet()) 
            {
                rotateDA();
                rotateDA();
                rotateLA();
                rotateD();
                rotateL();
            }
            else if(ui->front7Button->styleSheet() == ui->back5Button->styleSheet()) 
            {
                rotateD();
                rotateBA();
                rotateD();
                rotateB();
            }
        }
        else if(ui->front3Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateF();
            rotateD();
            rotateFA();
            if(ui->back3Button->styleSheet() == ui->front5Button->styleSheet())
            {
                rotateD();
                rotateFA();
                rotateD();
                rotateF();
            }
            else if(ui->back3Button->styleSheet() == ui->left5Button->styleSheet())
            {
                rotateLA();
                rotateD();
                rotateL();
            }
            else if(ui->back3Button->styleSheet() == ui->right5Button->styleSheet())
            {
                rotateDA();
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
            }
            else if(ui->back3Button->styleSheet() == ui->back5Button->styleSheet())  
            {
                rotateDA();
                rotateBA();
                rotateD();
                rotateB();
            }
        }
        else if(ui->right1Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateF();
            rotateDA();
            rotateFA();
            if(ui->front9Button->styleSheet() == ui->front5Button->styleSheet()) 
            {
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
            }
            else if(ui->front9Button->styleSheet() == ui->left5Button->styleSheet()) 
            {
                rotateL();
                rotateDA();
                rotateLA();
            }
            else if(ui->front9Button->styleSheet() == ui->right5Button->styleSheet())
            {
                rotateD();
                rotateD();
                rotateR();
                rotateDA();
                rotateRA();
            }
            else if(ui->front9Button->styleSheet() == ui->back5Button->styleSheet())
            {
                rotateDA();
                rotateB();
                rotateDA();
                rotateBA();
            }
        }
    }
}
void MainWindow::on_up9Button_clicked()
{
    if(ui->up9Button->styleSheet() == ui->up5Button->styleSheet())
        ui->up9Button->setStyleSheet("background-color: white");
    else if(ui->up9Button->styleSheet() == ui->front5Button->styleSheet())
        ui->up9Button->setStyleSheet("background-color: orange");
    else if(ui->up9Button->styleSheet() == ui->left5Button->styleSheet())
        ui->up9Button->setStyleSheet("background-color: red");
    else if(ui->up9Button->styleSheet() == ui->right5Button->styleSheet())
        ui->up9Button->setStyleSheet("background-color: green");
    else if(ui->up9Button->styleSheet() == ui->down5Button->styleSheet())
        ui->up9Button->setStyleSheet("background-color: yellow");
    else if(ui->up9Button->styleSheet() == ui->back5Button->styleSheet())
        ui->up9Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_front3Button_clicked()
{
    if(ui->front3Button->styleSheet() == ui->up5Button->styleSheet())
        ui->front3Button->setStyleSheet("background-color: white");
    else if(ui->front3Button->styleSheet() == ui->front5Button->styleSheet())
        ui->front3Button->setStyleSheet("background-color: orange");
    else if(ui->front3Button->styleSheet() == ui->left5Button->styleSheet())
        ui->front3Button->setStyleSheet("background-color: red");
    else if(ui->front3Button->styleSheet() == ui->right5Button->styleSheet())
        ui->front3Button->setStyleSheet("background-color: green");
    else if(ui->front3Button->styleSheet() == ui->down5Button->styleSheet())
        ui->front3Button->setStyleSheet("background-color: yellow");
    else if(ui->front3Button->styleSheet() == ui->back5Button->styleSheet())
        ui->front3Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_right1Button_clicked()
{
    if(ui->right1Button->styleSheet() == ui->up5Button->styleSheet())
        ui->right1Button->setStyleSheet("background-color: white");
    else if(ui->right1Button->styleSheet() == ui->front5Button->styleSheet())
        ui->right1Button->setStyleSheet("background-color: orange");
    else if(ui->right1Button->styleSheet() == ui->left5Button->styleSheet())
        ui->right1Button->setStyleSheet("background-color: red");
    else if(ui->right1Button->styleSheet() == ui->right5Button->styleSheet())
        ui->right1Button->setStyleSheet("background-color: green");
    else if(ui->right1Button->styleSheet() == ui->down5Button->styleSheet())
        ui->right1Button->setStyleSheet("background-color: yellow");
    else if(ui->right1Button->styleSheet() == ui->back5Button->styleSheet())
        ui->right1Button->setStyleSheet("background-color: blue");
}
void MainWindow::QuinaSudoeste()
{
    if(ui->up7Button->styleSheet() != ui->up5Button->styleSheet() || ui->front1Button->styleSheet() != ui->front5Button->styleSheet() || ui->left3Button->styleSheet() != ui->left5Button->styleSheet())
    {
        if(ui->up7Button->styleSheet() == ui->front5Button->styleSheet() && ui->front1Button->styleSheet() == ui->left5Button->styleSheet() && ui->left3Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateL();
            rotateD();
            rotateLA();
            rotateDA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->up7Button->styleSheet() == ui->left5Button->styleSheet() && ui->front1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left3Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateFA();
            rotateDA();
            rotateF();
            rotateD();
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->up7Button->styleSheet() == ui->up5Button->styleSheet() && ui->front1Button->styleSheet() == ui->right5Button->styleSheet() && ui->left3Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateL();
            rotateD();
            rotateLA();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->up7Button->styleSheet() == ui->right5Button->styleSheet() && ui->front1Button->styleSheet() == ui->front5Button->styleSheet() && ui->left3Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateL();
            rotateD();
            rotateLA();
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->up7Button->styleSheet() == ui->front5Button->styleSheet() && ui->front1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left3Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateFA();
            rotateDA();
            rotateF();
            rotateDA();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->up7Button->styleSheet() == ui->up5Button->styleSheet() && ui->front1Button->styleSheet() == ui->back5Button->styleSheet() && ui->left3Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateL();
            rotateD();
            rotateLA();
            rotateD();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
        }
        else if(ui->up7Button->styleSheet() == ui->right5Button->styleSheet() && ui->front1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left3Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateFA();
            rotateDA();
            rotateF();
            rotateR();
            rotateDA();
            rotateRA();
        }
        else if(ui->up7Button->styleSheet() == ui->back5Button->styleSheet() && ui->front1Button->styleSheet() == ui->right5Button->styleSheet() && ui->left3Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateL();
            rotateD();
            rotateLA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->up7Button->styleSheet() == ui->up5Button->styleSheet() && ui->front1Button->styleSheet() == ui->left5Button->styleSheet() && ui->left3Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateL();
            rotateDA();
            rotateLA();
            rotateDA();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->up7Button->styleSheet() == ui->left5Button->styleSheet() && ui->front1Button->styleSheet() == ui->back5Button->styleSheet() && ui->left3Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateL();
            rotateD();
            rotateLA();
            rotateD();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->up7Button->styleSheet() == ui->back5Button->styleSheet() && ui->front1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left3Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateFA();
            rotateDA();
            rotateF();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
    }
}
void MainWindow::on_left3Button_clicked()
{
    if(ui->left3Button->styleSheet() == ui->up5Button->styleSheet())
        ui->left3Button->setStyleSheet("background-color: white");
    else if(ui->left3Button->styleSheet() == ui->front5Button->styleSheet())
        ui->left3Button->setStyleSheet("background-color: orange");
    else if(ui->left3Button->styleSheet() == ui->left5Button->styleSheet())
        ui->left3Button->setStyleSheet("background-color: red");
    else if(ui->left3Button->styleSheet() == ui->right5Button->styleSheet())
        ui->left3Button->setStyleSheet("background-color: green");
    else if(ui->left3Button->styleSheet() == ui->down5Button->styleSheet())
        ui->left3Button->setStyleSheet("background-color: yellow");
    else if(ui->left3Button->styleSheet() == ui->back5Button->styleSheet())
        ui->left3Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_front1Button_clicked()
{
    if(ui->front1Button->styleSheet() == ui->up5Button->styleSheet())
        ui->front1Button->setStyleSheet("background-color: white");
    else if(ui->front1Button->styleSheet() == ui->front5Button->styleSheet())
        ui->front1Button->setStyleSheet("background-color: orange");
    else if(ui->front1Button->styleSheet() == ui->left5Button->styleSheet())
        ui->front1Button->setStyleSheet("background-color: red");
    else if(ui->front1Button->styleSheet() == ui->right5Button->styleSheet())
        ui->front1Button->setStyleSheet("background-color: green");
    else if(ui->front1Button->styleSheet() == ui->down5Button->styleSheet())
        ui->front1Button->setStyleSheet("background-color: yellow");
    else if(ui->front1Button->styleSheet() == ui->back5Button->styleSheet())
        ui->front1Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_up7Button_clicked()
{
    if(ui->up7Button->styleSheet() == ui->up5Button->styleSheet())
        ui->up7Button->setStyleSheet("background-color: white");
    else if(ui->up7Button->styleSheet() == ui->front5Button->styleSheet())
        ui->up7Button->setStyleSheet("background-color: orange");
    else if(ui->up7Button->styleSheet() == ui->left5Button->styleSheet())
        ui->up7Button->setStyleSheet("background-color: red");
    else if(ui->up7Button->styleSheet() == ui->right5Button->styleSheet())
        ui->up7Button->setStyleSheet("background-color: green");
    else if(ui->up7Button->styleSheet() == ui->down5Button->styleSheet())
        ui->up7Button->setStyleSheet("background-color: yellow");
    else if(ui->up7Button->styleSheet() == ui->back5Button->styleSheet())
        ui->up7Button->setStyleSheet("background-color: blue");
}
void MainWindow::QuinaNordeste()
{
    if(ui->up3Button->styleSheet() != ui->up5Button->styleSheet() || ui->right3Button->styleSheet() != ui->right5Button->styleSheet() || ui->back9Button->styleSheet() != ui->back5Button->styleSheet())
    {
        if(ui->up3Button->styleSheet() == ui->right5Button->styleSheet() && ui->right3Button->styleSheet() == ui->back5Button->styleSheet() && ui->back9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateBA();
            rotateDA();
            rotateB();
            rotateD();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
        }
        else if(ui->up3Button->styleSheet() == ui->back5Button->styleSheet() && ui->right3Button->styleSheet() == ui->up5Button->styleSheet() && ui->back9Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateR();
            rotateD();
            rotateRA();
            rotateDA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->up3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right3Button->styleSheet() == ui->front5Button->styleSheet() && ui->back9Button->styleSheet() == ui->right5Button->styleSheet())
        {
             rotateBA();
             rotateDA();
             rotateB();
             rotateDA();
             rotateRA();
             rotateD();
             rotateR();
        }
        else if(ui->up3Button->styleSheet() == ui->right5Button->styleSheet() && ui->right3Button->styleSheet() == ui->up5Button->styleSheet() && ui->back9Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateR();
            rotateD();
            rotateRA();
            rotateD();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->up3Button->styleSheet() == ui->front5Button->styleSheet() && ui->right3Button->styleSheet() == ui->right5Button->styleSheet() && ui->back9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateBA();
            rotateDA();
            rotateB();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->up3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right3Button->styleSheet() == ui->left5Button->styleSheet() && ui->back9Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateBA();
            rotateDA();
            rotateB();
            rotateDA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->up3Button->styleSheet() == ui->left5Button->styleSheet() && ui->right3Button->styleSheet() == ui->front5Button->styleSheet() && ui->back9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateBA();
            rotateDA();
            rotateB();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->up3Button->styleSheet() == ui->front5Button->styleSheet() && ui->right3Button->styleSheet() == ui->up5Button->styleSheet() && ui->back9Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateR();
            rotateD();
            rotateRA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->up3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right3Button->styleSheet() == ui->back5Button->styleSheet() && ui->back9Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateR();
            rotateD();
            rotateRA();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->up3Button->styleSheet() == ui->left5Button->styleSheet() && ui->right3Button->styleSheet() == ui->up5Button->styleSheet() && ui->back9Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateR();
            rotateLA();
            rotateD();
            rotateL();
            rotateRA();
        }
        else if(ui->up3Button->styleSheet() == ui->back5Button->styleSheet() && ui->right3Button->styleSheet() == ui->left5Button->styleSheet() && ui->back9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateBA();
            rotateDA();
            rotateB();
            rotateDA();
            rotateB();
            rotateDA();
            rotateBA();
        }
    }
}
void MainWindow::on_up3Button_clicked()
{
    if(ui->up3Button->styleSheet() == ui->up5Button->styleSheet())
        ui->up3Button->setStyleSheet("background-color: white");
    else if(ui->up3Button->styleSheet() == ui->front5Button->styleSheet())
        ui->up3Button->setStyleSheet("background-color: orange");
    else if(ui->up3Button->styleSheet() == ui->left5Button->styleSheet())
        ui->up3Button->setStyleSheet("background-color: red");
    else if(ui->up3Button->styleSheet() == ui->right5Button->styleSheet())
        ui->up3Button->setStyleSheet("background-color: green");
    else if(ui->up3Button->styleSheet() == ui->down5Button->styleSheet())
        ui->up3Button->setStyleSheet("background-color: yellow");
    else if(ui->up3Button->styleSheet() == ui->back5Button->styleSheet())
        ui->up3Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_right3Button_clicked()
{
    if(ui->right3Button->styleSheet() == ui->up5Button->styleSheet())
        ui->right3Button->setStyleSheet("background-color: white");
    else if(ui->right3Button->styleSheet() == ui->front5Button->styleSheet())
        ui->right3Button->setStyleSheet("background-color: orange");
    else if(ui->right3Button->styleSheet() == ui->left5Button->styleSheet())
        ui->right3Button->setStyleSheet("background-color: red");
    else if(ui->right3Button->styleSheet() == ui->right5Button->styleSheet())
        ui->right3Button->setStyleSheet("background-color: green");
    else if(ui->right3Button->styleSheet() == ui->down5Button->styleSheet())
        ui->right3Button->setStyleSheet("background-color: yellow");
    else if(ui->right3Button->styleSheet() == ui->back5Button->styleSheet())
        ui->right3Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_back9Button_clicked()
{
    if(ui->back9Button->styleSheet() == ui->up5Button->styleSheet())
        ui->back9Button->setStyleSheet("background-color: white");
    else if(ui->back9Button->styleSheet() == ui->front5Button->styleSheet())
        ui->back9Button->setStyleSheet("background-color: orange");
    else if(ui->back9Button->styleSheet() == ui->left5Button->styleSheet())
        ui->back9Button->setStyleSheet("background-color: red");
    else if(ui->back9Button->styleSheet() == ui->right5Button->styleSheet())
        ui->back9Button->setStyleSheet("background-color: green");
    else if(ui->back9Button->styleSheet() == ui->down5Button->styleSheet())
        ui->back9Button->setStyleSheet("background-color: yellow");
    else if(ui->back9Button->styleSheet() == ui->back5Button->styleSheet())
        ui->back9Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_up1Button_clicked()
{
    if(ui->up1Button->styleSheet() == ui->up5Button->styleSheet())
        ui->up1Button->setStyleSheet("background-color: white");
    else if(ui->up1Button->styleSheet() == ui->front5Button->styleSheet())
        ui->up1Button->setStyleSheet("background-color: orange");
    else if(ui->up1Button->styleSheet() == ui->left5Button->styleSheet())
        ui->up1Button->setStyleSheet("background-color: red");
    else if(ui->up1Button->styleSheet() == ui->right5Button->styleSheet())
        ui->up1Button->setStyleSheet("background-color: green");
    else if(ui->up1Button->styleSheet() == ui->down5Button->styleSheet())
        ui->up1Button->setStyleSheet("background-color: yellow");
    else if(ui->up1Button->styleSheet() == ui->back5Button->styleSheet())
        ui->up1Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_left1Button_clicked()
{
    if(ui->left1Button->styleSheet() == ui->up5Button->styleSheet())
        ui->left1Button->setStyleSheet("background-color: white");
    else if(ui->left1Button->styleSheet() == ui->front5Button->styleSheet())
        ui->left1Button->setStyleSheet("background-color: orange");
    else if(ui->left1Button->styleSheet() == ui->left5Button->styleSheet())
        ui->left1Button->setStyleSheet("background-color: red");
    else if(ui->left1Button->styleSheet() == ui->right5Button->styleSheet())
        ui->left1Button->setStyleSheet("background-color: green");
    else if(ui->left1Button->styleSheet() == ui->down5Button->styleSheet())
        ui->left1Button->setStyleSheet("background-color: yellow");
    else if(ui->left1Button->styleSheet() == ui->back5Button->styleSheet())
        ui->left1Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_back7Button_clicked()
{
    if(ui->back7Button->styleSheet() == ui->up5Button->styleSheet())
        ui->back7Button->setStyleSheet("background-color: white");
    else if(ui->back7Button->styleSheet() == ui->front5Button->styleSheet())
        ui->back7Button->setStyleSheet("background-color: orange");
    else if(ui->back7Button->styleSheet() == ui->left5Button->styleSheet())
        ui->back7Button->setStyleSheet("background-color: red");
    else if(ui->back7Button->styleSheet() == ui->right5Button->styleSheet())
        ui->back7Button->setStyleSheet("background-color: green");
    else if(ui->back7Button->styleSheet() == ui->down5Button->styleSheet())
        ui->back7Button->setStyleSheet("background-color: yellow");
    else if(ui->back7Button->styleSheet() == ui->back5Button->styleSheet())
        ui->back7Button->setStyleSheet("background-color: blue");
}
void MainWindow::QuinaNoroeste()
{
    if(ui->up1Button->styleSheet() != ui->up5Button->styleSheet() || ui->left1Button->styleSheet() != ui->left5Button->styleSheet() || ui->back7Button->styleSheet() != ui->back5Button->styleSheet())
    {
        if(ui->up1Button->styleSheet() == ui->left5Button->styleSheet() && ui->left1Button->styleSheet() == ui->back5Button->styleSheet() && ui->back7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateB();
            rotateD();
            rotateBA();
            rotateDA();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->up1Button->styleSheet() == ui->back5Button->styleSheet() && ui->left1Button->styleSheet() == ui->up5Button->styleSheet() && ui->back7Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateLA();
            rotateDA();
            rotateL();
            rotateD();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->up1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left1Button->styleSheet() == ui->right5Button->styleSheet() && ui->back7Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateB();
            rotateD();
            rotateBA();
            rotateD();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->up1Button->styleSheet() == ui->right5Button->styleSheet() && ui->left1Button->styleSheet() == ui->front5Button->styleSheet() && ui->back7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateB();
            rotateD();
            rotateBA();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->up1Button->styleSheet() == ui->front5Button->styleSheet() && ui->left1Button->styleSheet() == ui->up5Button->styleSheet() && ui->back7Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateLA();
            rotateDA();
            rotateL();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->up1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left1Button->styleSheet() == ui->front5Button->styleSheet() && ui->back7Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateB();
            rotateD();
            rotateBA();
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->up1Button->styleSheet() == ui->front5Button->styleSheet() && ui->left1Button->styleSheet() == ui->left5Button->styleSheet() && ui->back7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateB();
            rotateD();
            rotateBA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->up1Button->styleSheet() == ui->left5Button->styleSheet() && ui->left1Button->styleSheet() == ui->up5Button->styleSheet() && ui->back7Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateLA();
            rotateD();
            rotateD();
            rotateL();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->up1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left1Button->styleSheet() == ui->back5Button->styleSheet() && ui->back7Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateLA();
            rotateDA();
            rotateL();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->up1Button->styleSheet() == ui->back5Button->styleSheet() && ui->left1Button->styleSheet() == ui->right5Button->styleSheet() && ui->back7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateB();
            rotateD();
            rotateBA();
            rotateD();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->up1Button->styleSheet() == ui->right5Button->styleSheet() && ui->left1Button->styleSheet() == ui->up5Button->styleSheet() && ui->back7Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateLA();
            rotateDA();
            rotateL();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
        }
    }
}
void MainWindow::SudesteBaixo()
{
    if(ui->front9Button->styleSheet() == ui->up5Button->styleSheet() || ui->down3Button->styleSheet() == ui->up5Button->styleSheet() || ui->right7Button->styleSheet() == ui->up5Button->styleSheet())
    {
        if(ui->front9Button->styleSheet() == ui->front5Button->styleSheet() && ui->down3Button->styleSheet() == ui->right5Button->styleSheet() && ui->right7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->front9Button->styleSheet() == ui->up5Button->styleSheet() && ui->down3Button->styleSheet() == ui->front5Button->styleSheet() && ui->right7Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->front9Button->styleSheet() == ui->right5Button->styleSheet() && ui->down3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right7Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateF();
            rotateDA();
            rotateFA();
            rotateD();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->front9Button->styleSheet() == ui->front5Button->styleSheet() && ui->down3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right7Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateF();
            rotateDA();
            rotateFA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->front9Button->styleSheet() == ui->up5Button->styleSheet() && ui->down3Button->styleSheet() == ui->left5Button->styleSheet() && ui->right7Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateDA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->front9Button->styleSheet() == ui->left5Button->styleSheet() && ui->down3Button->styleSheet() == ui->front5Button->styleSheet() && ui->right7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->front9Button->styleSheet() == ui->left5Button->styleSheet() && ui->down3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right7Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateF();
            rotateDA();
            rotateFA();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->front9Button->styleSheet() == ui->up5Button->styleSheet() && ui->down3Button->styleSheet() == ui->back5Button->styleSheet() && ui->right7Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateD();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->front9Button->styleSheet() == ui->back5Button->styleSheet() && ui->down3Button->styleSheet() == ui->left5Button->styleSheet() && ui->right7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateDA();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->front9Button->styleSheet() == ui->back5Button->styleSheet() && ui->down3Button->styleSheet() == ui->up5Button->styleSheet() && ui->right7Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateRA();
            rotateD();
            rotateR();
            rotateR();
            rotateDA();
            rotateRA();
        }
        else if(ui->front9Button->styleSheet() == ui->up5Button->styleSheet() && ui->down3Button->styleSheet() == ui->right5Button->styleSheet() && ui->right7Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->front9Button->styleSheet() == ui->right5Button->styleSheet() && ui->down3Button->styleSheet() == ui->back5Button->styleSheet() && ui->right7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
        }
    }
}
void MainWindow::on_front9Button_clicked()
{
    if(ui->front9Button->styleSheet() == ui->up5Button->styleSheet())
        ui->front9Button->setStyleSheet("background-color: white");
    else if(ui->front9Button->styleSheet() == ui->front5Button->styleSheet())
        ui->front9Button->setStyleSheet("background-color: orange");
    else if(ui->front9Button->styleSheet() == ui->left5Button->styleSheet())
        ui->front9Button->setStyleSheet("background-color: red");
    else if(ui->front9Button->styleSheet() == ui->right5Button->styleSheet())
        ui->front9Button->setStyleSheet("background-color: green");
    else if(ui->front9Button->styleSheet() == ui->down5Button->styleSheet())
        ui->front9Button->setStyleSheet("background-color: yellow");
    else if(ui->front9Button->styleSheet() == ui->back5Button->styleSheet())
        ui->front9Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_right7Button_clicked()
{
    if(ui->right7Button->styleSheet() == ui->up5Button->styleSheet())
        ui->right7Button->setStyleSheet("background-color: white");
    else if(ui->right7Button->styleSheet() == ui->front5Button->styleSheet())
        ui->right7Button->setStyleSheet("background-color: orange");
    else if(ui->right7Button->styleSheet() == ui->left5Button->styleSheet())
        ui->right7Button->setStyleSheet("background-color: red");
    else if(ui->right7Button->styleSheet() == ui->right5Button->styleSheet())
        ui->right7Button->setStyleSheet("background-color: green");
    else if(ui->right7Button->styleSheet() == ui->down5Button->styleSheet())
        ui->right7Button->setStyleSheet("background-color: yellow");
    else if(ui->right7Button->styleSheet() == ui->back5Button->styleSheet())
        ui->right7Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_down3Button_clicked()
{
    if(ui->down3Button->styleSheet() == ui->up5Button->styleSheet())
        ui->down3Button->setStyleSheet("background-color: white");
    else if(ui->down3Button->styleSheet() == ui->front5Button->styleSheet())
        ui->down3Button->setStyleSheet("background-color: orange");
    else if(ui->down3Button->styleSheet() == ui->left5Button->styleSheet())
        ui->down3Button->setStyleSheet("background-color: red");
    else if(ui->down3Button->styleSheet() == ui->right5Button->styleSheet())
        ui->down3Button->setStyleSheet("background-color: green");
    else if(ui->down3Button->styleSheet() == ui->down5Button->styleSheet())
        ui->down3Button->setStyleSheet("background-color: yellow");
    else if(ui->down3Button->styleSheet() == ui->back5Button->styleSheet())
        ui->down3Button->setStyleSheet("background-color: blue");
}
void MainWindow::SudoesteBaixo()
{
    if(ui->front7Button->styleSheet() == ui->up5Button->styleSheet() || ui->down1Button->styleSheet() == ui->up5Button->styleSheet() || ui->left9Button->styleSheet() == ui->up5Button->styleSheet())
    {
        if(ui->front7Button->styleSheet() == ui->front5Button->styleSheet() && ui->down1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left9Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateFA();
            rotateD();
            rotateF();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->front7Button->styleSheet() == ui->up5Button->styleSheet() && ui->down1Button->styleSheet() == ui->right5Button->styleSheet() && ui->left9Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->front7Button->styleSheet() == ui->right5Button->styleSheet() && ui->down1Button->styleSheet() == ui->front5Button->styleSheet() && ui->left9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->front7Button->styleSheet() == ui->up5Button->styleSheet() && ui->down1Button->styleSheet() == ui->front5Button->styleSheet() && ui->left9Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->front7Button->styleSheet() == ui->front5Button->styleSheet() && ui->down1Button->styleSheet() == ui->left5Button->styleSheet() && ui->left9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->front7Button->styleSheet() == ui->left5Button->styleSheet() && ui->down1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left9Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateFA();
            rotateD();
            rotateF();
            rotateDA();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->front7Button->styleSheet() == ui->back5Button->styleSheet() && ui->down1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left9Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateL();
            rotateDA();
            rotateLA();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->front7Button->styleSheet() == ui->up5Button->styleSheet() && ui->down1Button->styleSheet() == ui->left5Button->styleSheet() && ui->left9Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->front7Button->styleSheet() == ui->left5Button->styleSheet() && ui->down1Button->styleSheet() == ui->back5Button->styleSheet() && ui->left9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateDA();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->front7Button->styleSheet() == ui->back5Button->styleSheet() && ui->down1Button->styleSheet() == ui->right5Button->styleSheet() && ui->left9Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->front7Button->styleSheet() == ui->right5Button->styleSheet() && ui->down1Button->styleSheet() == ui->up5Button->styleSheet() && ui->left9Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateFA();
            rotateD();
            rotateF();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
        }
        else if(ui->front7Button->styleSheet() == ui->up5Button->styleSheet() && ui->down1Button->styleSheet() == ui->back5Button->styleSheet() && ui->left9Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateDA();
            rotateR();
            rotateDA();
            rotateRA();
        }
    }
}
void MainWindow::on_front7Button_clicked()
{
    if(ui->front7Button->styleSheet() == ui->up5Button->styleSheet())
        ui->front7Button->setStyleSheet("background-color: white");
    else if(ui->front7Button->styleSheet() == ui->front5Button->styleSheet())
        ui->front7Button->setStyleSheet("background-color: orange");
    else if(ui->front7Button->styleSheet() == ui->left5Button->styleSheet())
        ui->front7Button->setStyleSheet("background-color: red");
    else if(ui->front7Button->styleSheet() == ui->right5Button->styleSheet())
        ui->front7Button->setStyleSheet("background-color: green");
    else if(ui->front7Button->styleSheet() == ui->down5Button->styleSheet())
        ui->front7Button->setStyleSheet("background-color: yellow");
    else if(ui->front7Button->styleSheet() == ui->back5Button->styleSheet())
        ui->front7Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_left9Button_clicked()
{
    if(ui->left9Button->styleSheet() == ui->up5Button->styleSheet())
        ui->left9Button->setStyleSheet("background-color: white");
    else if(ui->left9Button->styleSheet() == ui->front5Button->styleSheet())
        ui->left9Button->setStyleSheet("background-color: orange");
    else if(ui->left9Button->styleSheet() == ui->left5Button->styleSheet())
        ui->left9Button->setStyleSheet("background-color: red");
    else if(ui->left9Button->styleSheet() == ui->right5Button->styleSheet())
        ui->left9Button->setStyleSheet("background-color: green");
    else if(ui->left9Button->styleSheet() == ui->down5Button->styleSheet())
        ui->left9Button->setStyleSheet("background-color: yellow");
    else if(ui->left9Button->styleSheet() == ui->back5Button->styleSheet())
        ui->left9Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_down1Button_clicked()
{
    if(ui->down1Button->styleSheet() == ui->up5Button->styleSheet())
        ui->down1Button->setStyleSheet("background-color: white");
    else if(ui->down1Button->styleSheet() == ui->front5Button->styleSheet())
        ui->down1Button->setStyleSheet("background-color: orange");
    else if(ui->down1Button->styleSheet() == ui->left5Button->styleSheet())
        ui->down1Button->setStyleSheet("background-color: red");
    else if(ui->down1Button->styleSheet() == ui->right5Button->styleSheet())
        ui->down1Button->setStyleSheet("background-color: green");
    else if(ui->down1Button->styleSheet() == ui->down5Button->styleSheet())
        ui->down1Button->setStyleSheet("background-color: yellow");
    else if(ui->down1Button->styleSheet() == ui->back5Button->styleSheet())
        ui->down1Button->setStyleSheet("background-color: blue");
}
void MainWindow::NordesteBaixo()
{
    if(ui->back3Button->styleSheet() == ui->up5Button->styleSheet() || ui->down9Button->styleSheet() == ui->up5Button->styleSheet() || ui->right9Button->styleSheet() == ui->up5Button->styleSheet())
    {
       if(ui->back3Button->styleSheet() == ui->front5Button->styleSheet() && ui->down9Button->styleSheet() == ui->up5Button->styleSheet() && ui->right9Button->styleSheet() == ui->right5Button->styleSheet())
       {
            rotateR();
            rotateDA();
            rotateRA();
            rotateRA();
            rotateD();
            rotateR();
       }
       else if(ui->back3Button->styleSheet() == ui->up5Button->styleSheet() && ui->down9Button->styleSheet() == ui->right5Button->styleSheet() && ui->right9Button->styleSheet() == ui->front5Button->styleSheet())
       {
            rotateF();
            rotateDA();
            rotateFA();
       }
       else if(ui->back3Button->styleSheet() == ui->right5Button->styleSheet() && ui->down9Button->styleSheet() == ui->front5Button->styleSheet() && ui->right9Button->styleSheet() == ui->up5Button->styleSheet())
       {
            rotateDA();
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
       }
       else if(ui->back3Button->styleSheet() == ui->up5Button->styleSheet() && ui->down9Button->styleSheet() == ui->front5Button->styleSheet() && ui->right9Button->styleSheet() == ui->left5Button->styleSheet())
       {
            rotateDA();
            rotateL();
            rotateDA();
            rotateLA();
       }
       else if(ui->back3Button->styleSheet() == ui->front5Button->styleSheet() && ui->down9Button->styleSheet() == ui->left5Button->styleSheet() && ui->right9Button->styleSheet() == ui->up5Button->styleSheet())
       {
            rotateD();
            rotateFA();
            rotateD();
            rotateF();
       }
       else if(ui->back3Button->styleSheet() == ui->left5Button->styleSheet() && ui->down9Button->styleSheet() == ui->up5Button->styleSheet() && ui->right9Button->styleSheet() == ui->front5Button->styleSheet())
       {
            rotateR();
            rotateDA();
            rotateRA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
       }
       else if(ui->back3Button->styleSheet() == ui->back5Button->styleSheet() && ui->down9Button->styleSheet() == ui->up5Button->styleSheet() && ui->right9Button->styleSheet() == ui->left5Button->styleSheet())
       {
            rotateBA();
            rotateD();
            rotateB();
            rotateB();
            rotateDA();
            rotateBA();
       }
       else if(ui->back3Button->styleSheet() == ui->up5Button->styleSheet() && ui->down9Button->styleSheet() == ui->left5Button->styleSheet() && ui->right9Button->styleSheet() == ui->back5Button->styleSheet())
       {
            rotateD();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
       }
       else if(ui->back3Button->styleSheet() == ui->left5Button->styleSheet() && ui->down9Button->styleSheet() == ui->back5Button->styleSheet() && ui->right9Button->styleSheet() == ui->up5Button->styleSheet())
       {
            rotateLA();
            rotateD();
            rotateL();
       }
       else if(ui->back3Button->styleSheet() == ui->back5Button->styleSheet() && ui->down9Button->styleSheet() == ui->right5Button->styleSheet() && ui->right9Button->styleSheet() == ui->up5Button->styleSheet())
       {
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
       }
       else if(ui->back3Button->styleSheet() == ui->right5Button->styleSheet() && ui->down9Button->styleSheet() == ui->up5Button->styleSheet() && ui->right9Button->styleSheet() == ui->back5Button->styleSheet())
       {
            rotateR();
            rotateDA();
            rotateRA();
            rotateD();
            rotateBA();
            rotateD();
            rotateB();
       }
       else if(ui->back3Button->styleSheet() == ui->up5Button->styleSheet() && ui->down9Button->styleSheet() == ui->back5Button->styleSheet() && ui->right9Button->styleSheet() == ui->right5Button->styleSheet())
       {
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
       }
    }
}
void MainWindow::on_down9Button_clicked()
{
    if(ui->down9Button->styleSheet() == ui->up5Button->styleSheet())
        ui->down9Button->setStyleSheet("background-color: white");
    else if(ui->down9Button->styleSheet() == ui->front5Button->styleSheet())
        ui->down9Button->setStyleSheet("background-color: orange");
    else if(ui->down9Button->styleSheet() == ui->left5Button->styleSheet())
        ui->down9Button->setStyleSheet("background-color: red");
    else if(ui->down9Button->styleSheet() == ui->right5Button->styleSheet())
        ui->down9Button->setStyleSheet("background-color: green");
    else if(ui->down9Button->styleSheet() == ui->down5Button->styleSheet())
        ui->down9Button->setStyleSheet("background-color: yellow");
    else if(ui->down9Button->styleSheet() == ui->back5Button->styleSheet())
        ui->down9Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_right9Button_clicked()
{
    if(ui->right9Button->styleSheet() == ui->up5Button->styleSheet())
        ui->right9Button->setStyleSheet("background-color: white");
    else if(ui->right9Button->styleSheet() == ui->front5Button->styleSheet())
        ui->right9Button->setStyleSheet("background-color: orange");
    else if(ui->right9Button->styleSheet() == ui->left5Button->styleSheet())
        ui->right9Button->setStyleSheet("background-color: red");
    else if(ui->right9Button->styleSheet() == ui->right5Button->styleSheet())
        ui->right9Button->setStyleSheet("background-color: green");
    else if(ui->right9Button->styleSheet() == ui->down5Button->styleSheet())
        ui->right9Button->setStyleSheet("background-color: yellow");
    else if(ui->right9Button->styleSheet() == ui->back5Button->styleSheet())
        ui->right9Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_back3Button_clicked()
{
    if(ui->back3Button->styleSheet() == ui->up5Button->styleSheet())
        ui->back3Button->setStyleSheet("background-color: white");
    else if(ui->back3Button->styleSheet() == ui->front5Button->styleSheet())
        ui->back3Button->setStyleSheet("background-color: orange");
    else if(ui->back3Button->styleSheet() == ui->left5Button->styleSheet())
        ui->back3Button->setStyleSheet("background-color: red");
    else if(ui->back3Button->styleSheet() == ui->right5Button->styleSheet())
        ui->back3Button->setStyleSheet("background-color: green");
    else if(ui->back3Button->styleSheet() == ui->down5Button->styleSheet())
        ui->back3Button->setStyleSheet("background-color: yellow");
    else if(ui->back3Button->styleSheet() == ui->back5Button->styleSheet())
        ui->back3Button->setStyleSheet("background-color: blue");
}
void MainWindow::NoroesteBaixo()
{
    if(ui->back1Button->styleSheet() == ui->up5Button->styleSheet() || ui->down7Button->styleSheet() == ui->up5Button->styleSheet() || ui->left7Button->styleSheet() == ui->up5Button->styleSheet())
    {
        if(ui->back1Button->styleSheet() == ui->front5Button->styleSheet() && ui->down7Button->styleSheet() == ui->right5Button->styleSheet() && ui->left7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateDA();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->back1Button->styleSheet() == ui->up5Button->styleSheet() && ui->down7Button->styleSheet() == ui->front5Button->styleSheet() && ui->left7Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateD();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->back1Button->styleSheet() == ui->right5Button->styleSheet() && ui->down7Button->styleSheet() == ui->up5Button->styleSheet() && ui->left7Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateLA();
            rotateD();
            rotateL();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->back1Button->styleSheet() == ui->front5Button->styleSheet() && ui->down7Button->styleSheet() == ui->up5Button->styleSheet() && ui->left7Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateLA();
            rotateD();
            rotateL();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->back1Button->styleSheet() == ui->up5Button->styleSheet() && ui->down7Button->styleSheet() == ui->left5Button->styleSheet() && ui->left7Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->back1Button->styleSheet() == ui->left5Button->styleSheet() && ui->down7Button->styleSheet() == ui->front5Button->styleSheet() && ui->left7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->back1Button->styleSheet() == ui->left5Button->styleSheet() && ui->down7Button->styleSheet() == ui->up5Button->styleSheet() && ui->left7Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateB();
            rotateDA();
            rotateBA();
            rotateD();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->back1Button->styleSheet() == ui->up5Button->styleSheet() && ui->down7Button->styleSheet() == ui->back5Button->styleSheet() && ui->left7Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->back1Button->styleSheet() == ui->back5Button->styleSheet() && ui->down7Button->styleSheet() == ui->left5Button->styleSheet() && ui->left7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->back1Button->styleSheet() == ui->back5Button->styleSheet() && ui->down7Button->styleSheet() == ui->up5Button->styleSheet() && ui->left7Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateB();
            rotateDA();
            rotateBA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->back1Button->styleSheet() == ui->up5Button->styleSheet() && ui->down7Button->styleSheet() == ui->right5Button->styleSheet() && ui->left7Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateDA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->back1Button->styleSheet() == ui->right5Button->styleSheet() && ui->down7Button->styleSheet() == ui->back5Button->styleSheet() && ui->left7Button->styleSheet() == ui->up5Button->styleSheet())
        {
            rotateR();
            rotateDA();
            rotateRA();
        }
    }
}
void MainWindow::on_back1Button_clicked()
{
    if(ui->back1Button->styleSheet() == ui->up5Button->styleSheet())
        ui->back1Button->setStyleSheet("background-color: white");
    else if(ui->back1Button->styleSheet() == ui->front5Button->styleSheet())
        ui->back1Button->setStyleSheet("background-color: orange");
    else if(ui->back1Button->styleSheet() == ui->left5Button->styleSheet())
        ui->back1Button->setStyleSheet("background-color: red");
    else if(ui->back1Button->styleSheet() == ui->right5Button->styleSheet())
        ui->back1Button->setStyleSheet("background-color: green");
    else if(ui->back1Button->styleSheet() == ui->down5Button->styleSheet())
        ui->back1Button->setStyleSheet("background-color: yellow");
    else if(ui->back1Button->styleSheet() == ui->back5Button->styleSheet())
        ui->back1Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_down7Button_clicked()
{
    if(ui->down7Button->styleSheet() == ui->up5Button->styleSheet())
        ui->down7Button->setStyleSheet("background-color: white");
    else if(ui->down7Button->styleSheet() == ui->front5Button->styleSheet())
        ui->down7Button->setStyleSheet("background-color: orange");
    else if(ui->down7Button->styleSheet() == ui->left5Button->styleSheet())
        ui->down7Button->setStyleSheet("background-color: red");
    else if(ui->down7Button->styleSheet() == ui->right5Button->styleSheet())
        ui->down7Button->setStyleSheet("background-color: green");
    else if(ui->down7Button->styleSheet() == ui->down5Button->styleSheet())
        ui->down7Button->setStyleSheet("background-color: yellow");
    else if(ui->down7Button->styleSheet() == ui->back5Button->styleSheet())
        ui->down7Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_left7Button_clicked()
{
    if(ui->left7Button->styleSheet() == ui->up5Button->styleSheet())
        ui->left7Button->setStyleSheet("background-color: white");
    else if(ui->left7Button->styleSheet() == ui->front5Button->styleSheet())
        ui->left7Button->setStyleSheet("background-color: orange");
    else if(ui->left7Button->styleSheet() == ui->left5Button->styleSheet())
        ui->left7Button->setStyleSheet("background-color: red");
    else if(ui->left7Button->styleSheet() == ui->right5Button->styleSheet())
        ui->left7Button->setStyleSheet("background-color: green");
    else if(ui->left7Button->styleSheet() == ui->down5Button->styleSheet())
        ui->left7Button->setStyleSheet("background-color: yellow");
    else if(ui->left7Button->styleSheet() == ui->back5Button->styleSheet())
        ui->left7Button->setStyleSheet("background-color: blue");
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
void MainWindow::frontSecond()
{
    if(ui->front4Button->styleSheet() != ui->front5Button->styleSheet() || ui->front6Button->styleSheet() != ui->front5Button->styleSheet() ||
            ui->left6Button->styleSheet() != ui->left5Button->styleSheet() || ui->right4Button->styleSheet() != ui->right5Button->styleSheet())
    {
        if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down2Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down2Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->left8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->left8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateD();
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->right8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateDA();
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->right8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateDA();
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->back2Button->styleSheet() == ui->front5Button->styleSheet() && ui->down8Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->back2Button->styleSheet() == ui->front5Button->styleSheet() && ui->down8Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
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
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            ult = "1LL";
            
        }
        else if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down4Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
            ult = "2LL";
            
        }
        else if(ui->back2Button->styleSheet() == ui->left5Button->styleSheet() && ui->down8Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            ult = "3LL";
            
        }
        else if(ui->back2Button->styleSheet() == ui->left5Button->styleSheet() && ui->down8Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateD();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
            ult = "4LL";
            
        }
        else if(ui->front8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down2Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateDA();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            ult = "5LL";
            
        }
        else if(ui->front8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down2Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateDA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
            ult = "6LL";
            
        }
        else if(ui->right8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            ult = "7LL";
            
        }
        else if(ui->right8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down6Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
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
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->right8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
        }
        else if(ui->front8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down2Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->front8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down2Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateD();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
        }
        else if(ui->back2Button->styleSheet() == ui->right5Button->styleSheet() && ui->down8Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateDA();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->back2Button->styleSheet() == ui->right5Button->styleSheet() && ui->down8Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateDA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
        }
        else if(ui->left8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down4Button->styleSheet() == ui->front5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->left8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
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
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->back2Button->styleSheet() == ui->back5Button->styleSheet() && ui->down8Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->right8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down6Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->right8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down6Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateD();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->left8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down4Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateDA();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->left8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down4Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateDA();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->front8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down2Button->styleSheet() == ui->right5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->front8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down2Button->styleSheet() == ui->left5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
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
void MainWindow::debugaFront()
{
    if(ui->front4Button->styleSheet() != ui->front5Button->styleSheet() || ui->front6Button->styleSheet() != ui->front5Button->styleSheet() ||
                ui->left6Button->styleSheet() != ui->left5Button->styleSheet() || ui->right4Button->styleSheet() != ui->right5Button->styleSheet())
        {
            if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateL();
                rotateDA();
                rotateLA();
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
            }
            else if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
            }
            else if(ui->left8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateD();
                rotateL();
                rotateDA();
                rotateLA();
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
            }
            else if(ui->left8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
            }
            else if(ui->right8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateDA();
                rotateD();
                rotateL();
                rotateDA();
                rotateLA();
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
            }
            else if(ui->right8Button->styleSheet() == ui->front5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateDA();
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
            }
            else if(ui->back2Button->styleSheet() == ui->front5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateD();
                rotateD();
                rotateL();
                rotateDA();
                rotateLA();
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
            }
            else if(ui->back2Button->styleSheet() == ui->front5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateD();
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
            }
            else if(ui->front4Button->styleSheet() == ui->left5Button->styleSheet() && ui->left6Button->styleSheet() == ui->front5Button->styleSheet())
            {
                rotateD();
                rotateL();
                rotateDA();
                rotateLA();
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
            }
            else if(ui->front6Button->styleSheet() == ui->right5Button->styleSheet() && ui->right4Button->styleSheet() == ui->front5Button->styleSheet())
            {
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
            }
            else
            {
                rotateD();
                rotateL();
                rotateDA();
                rotateLA();
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
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
                rotateD();
                rotateB();
                rotateDA();
                rotateBA();
                rotateDA();
                rotateLA();
                rotateD();
                rotateL();
            }
            else if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
                rotateD();
                rotateL();
                rotateDA();
                rotateLA();
            }
            else if(ui->back2Button->styleSheet() == ui->left5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateD();
                rotateB();
                rotateDA();
                rotateBA();
                rotateDA();
                rotateLA();
                rotateD();
                rotateL();
            }
            else if(ui->back2Button->styleSheet() == ui->left5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
                rotateD();
                rotateL();
                rotateDA();
                rotateLA();
            }
            else if(ui->front8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateDA();
                rotateD();
                rotateB();
                rotateDA();
                rotateBA();
                rotateDA();
                rotateLA();
                rotateD();
                rotateL();
            }
            else if(ui->front8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateDA();
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
                rotateD();
                rotateL();
                rotateDA();
                rotateLA();
            }
            else if(ui->right8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateD();
                rotateD();
                rotateB();
                rotateDA();
                rotateBA();
                rotateDA();
                rotateLA();
                rotateD();
                rotateL();
            }
            else if(ui->right8Button->styleSheet() == ui->left5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateD();
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
                rotateD();
                rotateL();
                rotateDA();
                rotateLA();
            }
            else if(ui->left4Button->styleSheet() == ui->down5Button->styleSheet() && ui->down4Button->styleSheet() == ui->left5Button->styleSheet())
            {
                rotateD();
                rotateB();
                rotateDA();
                rotateBA();
                rotateDA();
                rotateLA();
                rotateD();
                rotateL();
            }
            else if(ui->left6Button->styleSheet() == ui->front5Button->styleSheet() && ui->front4Button->styleSheet() == ui->left5Button->styleSheet())
            {
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
                rotateD();
                rotateL();
                rotateDA();
                rotateLA();
            }
            else
            {
                rotateD();
                rotateB();
                rotateDA();
                rotateBA();
                rotateDA();
                rotateLA();
                rotateD();
                rotateL();
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
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
            }
            else if(ui->right8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateDA();
                rotateBA();
                rotateD();
                rotateB();
                rotateD();
                rotateR();
                rotateDA();
                rotateRA();
            }
            else if(ui->front8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
            }
            else if(ui->front8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateDA();
                rotateBA();
                rotateD();
                rotateB();
                rotateD();
                rotateR();
                rotateDA();
                rotateRA();
            }
            else if(ui->back2Button->styleSheet() == ui->right5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateDA();
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
            }
            else if(ui->back2Button->styleSheet() == ui->right5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateDA();
                rotateDA();
                rotateBA();
                rotateD();
                rotateB();
                rotateD();
                rotateR();
                rotateDA();
                rotateRA();
            }
            else if(ui->left8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateD();
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
            }
            else if(ui->left8Button->styleSheet() == ui->right5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
            {
                rotateD();
                rotateD();
                rotateDA();
                rotateBA();
                rotateD();
                rotateB();
                rotateD();
                rotateR();
                rotateDA();
                rotateRA();
            }
            else if(ui->right4Button->styleSheet() == ui->front5Button->styleSheet() && ui->front6Button->styleSheet() == ui->right5Button->styleSheet())
            {
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
            }
            else if(ui->right6Button->styleSheet() == ui->down5Button->styleSheet() && ui->down6Button->styleSheet() == ui->right5Button->styleSheet())
            {
                rotateDA();
                rotateBA();
                rotateD();
                rotateB();
                rotateD();
                rotateR();
                rotateDA();
                rotateRA();
            }
            else
            {
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
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
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->back2Button->styleSheet() == ui->back5Button->styleSheet() && ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
        {
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->right8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->right8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
        {
            rotateD();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->left8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
        {
            rotateDA();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->left8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
        {
            rotateDA();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->front8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->front8Button->styleSheet() == ui->back5Button->styleSheet() && ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
        {
            rotateD();
            rotateD();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->back6Button->styleSheet() == ui->right5Button->styleSheet() && ui->right6Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->back4Button->styleSheet() == ui->left5Button->styleSheet() && ui->left4Button->styleSheet() == ui->back5Button->styleSheet())
        {
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else
        {
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
    }
}
void MainWindow::on_downCrossButton_clicked()
{
    if(ui->down2Button->styleSheet() != ui->down5Button->styleSheet() && ui->down4Button->styleSheet() != ui->down5Button->styleSheet() &&
            ui->down6Button->styleSheet() != ui->down5Button->styleSheet() && ui->down8Button->styleSheet() != ui->down5Button->styleSheet())
    {
        rotateF();
        rotateL();
        rotateD();
        rotateLA();
        rotateDA();
        rotateFA();
    }
    while((ui->down2Button->styleSheet() != ui->down5Button->styleSheet() || ui->down4Button->styleSheet() != ui->down5Button->styleSheet()) &&
          (ui->down2Button->styleSheet() != ui->down5Button->styleSheet() || ui->down8Button->styleSheet() != ui->down5Button->styleSheet()) &&
           (ui->down4Button->styleSheet() != ui->down5Button->styleSheet() || ui->down6Button->styleSheet() != ui->down5Button->styleSheet()))
        rotateD();
    if(ui->down6Button->styleSheet() != ui->down5Button->styleSheet() && ui->down8Button->styleSheet() != ui->down5Button->styleSheet())
    {
        rotateF();
        rotateL();
        rotateD();
        rotateLA();
        rotateDA();
        rotateFA();
    }
    
    if((ui->down2Button->styleSheet() == ui->down5Button->styleSheet() &&
        ui->down8Button->styleSheet() == ui->down5Button->styleSheet()) &&
            ui->down4Button->styleSheet() != ui->down5Button->styleSheet() &&
            ui->down6Button->styleSheet() != ui->down5Button->styleSheet())
        rotateD();
    if(ui->down2Button->styleSheet() != ui->down5Button->styleSheet() || ui->down4Button->styleSheet() != ui->down5Button->styleSheet() ||
            ui->down6Button->styleSheet() != ui->down5Button->styleSheet() || ui->down8Button->styleSheet() != ui->down5Button->styleSheet())
    {
        rotateF();
        rotateL();
        rotateD();
        rotateLA();
        rotateDA();
        rotateFA();
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
            rotateDA();
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
                rotateD();
                rotateD();
            }
            if(ui->front8Button->styleSheet() != ui->front5Button->styleSheet() && ui->left8Button->styleSheet() != ui->left5Button->styleSheet())
            {
                rotateL();
                rotateD();
                rotateD();
                rotateLA();
                rotateDA();
                rotateL();
                rotateDA();
                rotateLA();
                rotateDA();
            }
            else if(ui->back2Button->styleSheet() != ui->back2Button->styleSheet() && ui->left8Button->styleSheet() != ui->left5Button->styleSheet())
            {
                rotateB();
                rotateD();
                rotateD();
                rotateBA();
                rotateDA();
                rotateB();
                rotateDA();
                rotateBA();
                rotateDA();
            }
            else if(ui->back2Button->styleSheet() != ui->back5Button->styleSheet() && ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
            {
                rotateR();
                rotateD();
                rotateD();
                rotateRA();
                rotateDA();
                rotateR();
                rotateDA();
                rotateRA();
                rotateDA();
            }
            else if(ui->front8Button->styleSheet() != ui->front2Button->styleSheet() && ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
            {
                rotateF();
                rotateD();
                rotateD();
                rotateFA();
                rotateDA();
                rotateF();
                rotateDA();
                rotateFA();
                rotateDA();
            }
            else
                rotateDA();
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
                rotateD();
            }
            else if (i == 1)
            {
                rotateL();
                rotateD();
                rotateD();
                rotateLA();
                rotateDA();
                rotateL();
                rotateDA();
                rotateLA();
                rotateDA();
            }
            if(i == 2)
            {
                if(ui->front8Button->styleSheet() != ui->front5Button->styleSheet() && ui->left8Button->styleSheet() != ui->left5Button->styleSheet())
                {
                    rotateL();
                    rotateD();
                    rotateD();
                    rotateLA();
                    rotateDA();
                    rotateL();
                    rotateDA();
                    rotateLA();
                    rotateDA();
                }
                else if(ui->back2Button->styleSheet() != ui->back2Button->styleSheet() && ui->left8Button->styleSheet() != ui->left5Button->styleSheet())
                {
                    rotateF();
                    rotateD();
                    rotateD();
                    rotateFA();
                    rotateDA();
                    rotateF();
                    rotateDA();
                    rotateFA();
                    rotateDA();
                }
                else if(ui->back2Button->styleSheet() != ui->back5Button->styleSheet() && ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
                {
                    rotateL();
                    rotateD();
                    rotateD();
                    rotateLA();
                    rotateDA();
                    rotateL();
                    rotateDA();
                    rotateLA();
                    rotateDA();
                }
                else if(ui->front8Button->styleSheet() != ui->front2Button->styleSheet() && ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
                {
                    rotateF();
                    rotateD();
                    rotateD();
                    rotateFA();
                    rotateDA();
                    rotateF();
                    rotateDA();
                    rotateFA();
                    rotateDA();
                }
                else
                {
                    rotateL();
                    rotateD();
                    rotateD();
                    rotateLA();
                    rotateDA();
                    rotateL();
                    rotateDA();
                    rotateLA();
                    rotateDA();
                }
            }
            else
            {
                rotateD();
                rotateD();
            }
            if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet() && ui->right8Button->styleSheet() == ui->right5Button->styleSheet())
            {
                rotateL();
                rotateD();
                rotateD();
                rotateLA();
                rotateDA();
                rotateL();
                rotateDA();
                rotateLA();
            }
        }
        if(ui->front8Button->styleSheet() != ui->front5Button->styleSheet() && ui->left8Button->styleSheet() != ui->left5Button->styleSheet() &&
                ui->back2Button->styleSheet() != ui->back5Button->styleSheet() && ui->right8Button->styleSheet() != ui->right5Button->styleSheet())
        {
            rotateDA();
            rotateL();
            rotateD();
            rotateD();
            rotateLA();
            rotateDA();
            rotateL();
            rotateDA();
            rotateLA();
            rotateD();
        }
    }*/
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
void MainWindow::on_placeDownEdgesButton_clicked()
{
    while(!sudestecorreto() || !sudoestecorreto() || !nordestecorreto() || !noroestecorreto())
    {
        if(sudoestecorreto())
        {
            rotateRA();
            rotateD();
            rotateL();
            rotateDA();
            rotateR();
            rotateD();
            rotateLA();
            rotateDA();
            ult = "1SS";
            
        }
        else if(sudestecorreto())
        {
            rotateBA();
            rotateD();
            rotateF();
            rotateDA();
            rotateB();
            rotateD();
            rotateFA();
            rotateDA();
            ult = "2SS";
            
        }
        else if(noroestecorreto())
        {
            rotateFA();
            rotateD();
            rotateB();
            rotateDA();
            rotateF();
            rotateD();
            rotateBA();
            rotateDA();
            ult = "3SS";
            
        }
        else if(nordestecorreto())
        {
            rotateRA();
            rotateD();
            rotateL();
            rotateDA();
            rotateR();
            rotateD();
            rotateLA();
            rotateDA();
            ult = "4SS";
            
        }
        else
        {
            rotateRA();
            rotateD();
            rotateL();
            rotateDA();
            rotateR();
            rotateD();
            rotateLA();
            rotateDA();
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
            rotateL();
            rotateD();
            rotateD();
            rotateLA();
            rotateDA();
            rotateL();
            rotateDA();
            rotateLA();
            rotateRA();
            rotateD();
            rotateD();
            rotateR();
            rotateD();
            rotateRA();
            rotateD();
            rotateR();
            ult = "1FF";
            
        }
        else if((ui->back1Button->styleSheet() != ui->back5Button->styleSheet() || ui->left7Button->styleSheet() != ui->left5Button->styleSheet() || ui->down7Button->styleSheet() != ui->down7Button->styleSheet()) &&
                (ui->back3Button->styleSheet() != ui->back5Button->styleSheet() || ui->right9Button->styleSheet() != ui->right5Button->styleSheet() || ui->down9Button->styleSheet() != ui->down7Button->styleSheet()))
        {
            rotateB();
            rotateD();
            rotateD();
            rotateBA();
            rotateDA();
            rotateB();
            rotateDA();
            rotateBA();
            rotateFA();
            rotateD();
            rotateD();
            rotateF();
            rotateD();
            rotateFA();
            rotateD();
            rotateF();
            ult = "2FF";
            
        }
        else if((ui->back3Button->styleSheet() != ui->back5Button->styleSheet() || ui->right9Button->styleSheet() != ui->right5Button->styleSheet() || ui->down9Button->styleSheet() != ui->down7Button->styleSheet()) &&
                (ui->front9Button->styleSheet() != ui->front5Button->styleSheet() || ui->right7Button->styleSheet() != ui->right5Button->styleSheet() || ui->down3Button->styleSheet() != ui->down7Button->styleSheet()))
        {
            rotateR();
            rotateD();
            rotateD();
            rotateRA();
            rotateDA();
            rotateR();
            rotateDA();
            rotateRA();
            rotateLA();
            rotateD();
            rotateD();
            rotateL();
            rotateD();
            rotateLA();
            rotateD();
            rotateL();
            ult = "3FF";
            
        }
        else if((ui->front9Button->styleSheet() != ui->front5Button->styleSheet() || ui->right7Button->styleSheet() != ui->right5Button->styleSheet() || ui->down3Button->styleSheet() != ui->down7Button->styleSheet()) &&
                (ui->front7Button->styleSheet() != ui->front5Button->styleSheet() || ui->left9Button->styleSheet() != ui->left5Button->styleSheet() || ui->down1Button->styleSheet() != ui->down5Button->styleSheet()))
        {
            rotateF();
            rotateD();
            rotateD();
            rotateFA();
            rotateDA();
            rotateF();
            rotateDA();
            rotateFA();
            rotateBA();
            rotateD();
            rotateD();
            rotateB();
            rotateD();
            rotateBA();
            rotateD();
            rotateB();
            ult = "4FF";
            
        }
        else if(ui->front7Button->styleSheet() != ui->front5Button->styleSheet() || ui->left9Button->styleSheet() != ui->left5Button->styleSheet() || ui->down1Button->styleSheet() != ui->down5Button->styleSheet())
        {
            rotateL();
            rotateD();
            rotateD();
            rotateLA();
            rotateDA();
            rotateL();
            rotateDA();
            rotateLA();
            rotateRA();
            rotateD();
            rotateD();
            rotateR();
            rotateD();
            rotateRA();
            rotateD();
            rotateR();
            ult = "5FF";
            
        }
        else if(ui->back1Button->styleSheet() != ui->back5Button->styleSheet() || ui->left7Button->styleSheet() != ui->left5Button->styleSheet() || ui->down7Button->styleSheet() != ui->down7Button->styleSheet())
        {
            rotateB();
            rotateD();
            rotateD();
            rotateBA();
            rotateDA();
            rotateB();
            rotateDA();
            rotateBA();
            rotateFA();
            rotateD();
            rotateD();
            rotateF();
            rotateD();
            rotateFA();
            rotateD();
            rotateF();
            ult = "6FF";
            
        }
        else if(ui->back3Button->styleSheet() != ui->back5Button->styleSheet() || ui->right9Button->styleSheet() != ui->right5Button->styleSheet() || ui->down9Button->styleSheet() != ui->down7Button->styleSheet())
        {
            rotateR();
            rotateD();
            rotateD();
            rotateRA();
            rotateDA();
            rotateR();
            rotateDA();
            rotateRA();
            rotateLA();
            rotateD();
            rotateD();
            rotateL();
            rotateD();
            rotateLA();
            rotateD();
            rotateL();
            ult = "7FF";
            
        }
        else if(ui->front9Button->styleSheet() != ui->front5Button->styleSheet() || ui->right7Button->styleSheet() != ui->right5Button->styleSheet() || ui->down3Button->styleSheet() != ui->down7Button->styleSheet())
        {
            rotateF();
            rotateD();
            rotateD();
            rotateFA();
            rotateDA();
            rotateF();
            rotateDA();
            rotateFA();
            rotateBA();
            rotateD();
            rotateD();
            rotateB();
            rotateD();
            rotateBA();
            rotateD();
            rotateB();
            ult = "8FF";
            
        }
    }
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
        rotateF();
        rotateF();
    }
    else if( s == "F")
        rotateF();
    else if ( s == "F'" )
        rotateFA();
    else if ( s == "2T" )
    {
        rotateR();
        rotateR();
    }
    else if ( s == "T" )
        rotateR();
    else if ( s == "T'" )
        rotateRA();
    else if ( s == "2D" )
    {
        rotateL();
        rotateL();
    }
    else if ( s == "D" )
        rotateL();
    else if ( s == "D'" )
        rotateLA();
    else if ( s == "2L" )
    {
        rotateU();
        rotateU();
    }
    else if ( s == "L" )
        rotateU();
    else if ( s == "L'" )
        rotateUA();
    else if ( s == "2R" )
    {
        rotateD();
        rotateD();
    }
    else if ( s == "R" )
        rotateD();
    else if ( s == "R'" )
        rotateDA();
    else if ( s == "2B" )
    {
        rotateB();
        rotateB();
    }
    else if ( s == "B" )
        rotateB();
    else if ( s == "B'" )
        rotateBA();
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
void MainWindow::on_back8Button_clicked()
{
    if(ui->back8Button->styleSheet() == ui->up5Button->styleSheet())
        ui->back8Button->setStyleSheet("background-color: white");
    else if(ui->back8Button->styleSheet() == ui->front5Button->styleSheet())
        ui->back8Button->setStyleSheet("background-color: orange");
    else if(ui->back8Button->styleSheet() == ui->left5Button->styleSheet())
        ui->back8Button->setStyleSheet("background-color: red");
    else if(ui->back8Button->styleSheet() == ui->right5Button->styleSheet())
        ui->back8Button->setStyleSheet("background-color: green");
    else if(ui->back8Button->styleSheet() == ui->down5Button->styleSheet())
        ui->back8Button->setStyleSheet("background-color: yellow");
    else if(ui->back8Button->styleSheet() == ui->back5Button->styleSheet())
        ui->back8Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_back2Button_clicked()
{
    if(ui->back2Button->styleSheet() == ui->up5Button->styleSheet())
        ui->back2Button->setStyleSheet("background-color: white");
    else if(ui->back2Button->styleSheet() == ui->front5Button->styleSheet())
        ui->back2Button->setStyleSheet("background-color: orange");
    else if(ui->back2Button->styleSheet() == ui->left5Button->styleSheet())
        ui->back2Button->setStyleSheet("background-color: red");
    else if(ui->back2Button->styleSheet() == ui->right5Button->styleSheet())
        ui->back2Button->setStyleSheet("background-color: green");
    else if(ui->back2Button->styleSheet() == ui->down5Button->styleSheet())
        ui->back2Button->setStyleSheet("background-color: yellow");
    else if(ui->back2Button->styleSheet() == ui->back5Button->styleSheet())
        ui->back2Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_back6Button_clicked()
{
    if(ui->back6Button->styleSheet() == ui->up5Button->styleSheet())
        ui->back6Button->setStyleSheet("background-color: white");
    else if(ui->back6Button->styleSheet() == ui->front5Button->styleSheet())
        ui->back6Button->setStyleSheet("background-color: orange");
    else if(ui->back6Button->styleSheet() == ui->left5Button->styleSheet())
        ui->back6Button->setStyleSheet("background-color: red");
    else if(ui->back6Button->styleSheet() == ui->right5Button->styleSheet())
        ui->back6Button->setStyleSheet("background-color: green");
    else if(ui->back6Button->styleSheet() == ui->down5Button->styleSheet())
        ui->back6Button->setStyleSheet("background-color: yellow");
    else if(ui->back6Button->styleSheet() == ui->back5Button->styleSheet())
        ui->back6Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_back4Button_clicked()
{
    if(ui->back4Button->styleSheet() == ui->up5Button->styleSheet())
        ui->back4Button->setStyleSheet("background-color: white");
    else if(ui->back4Button->styleSheet() == ui->front5Button->styleSheet())
        ui->back4Button->setStyleSheet("background-color: orange");
    else if(ui->back4Button->styleSheet() == ui->left5Button->styleSheet())
        ui->back4Button->setStyleSheet("background-color: red");
    else if(ui->back4Button->styleSheet() == ui->right5Button->styleSheet())
        ui->back4Button->setStyleSheet("background-color: green");
    else if(ui->back4Button->styleSheet() == ui->down5Button->styleSheet())
        ui->back4Button->setStyleSheet("background-color: yellow");
    else if(ui->back4Button->styleSheet() == ui->back5Button->styleSheet())
        ui->back4Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_down8Button_clicked()
{
    if(ui->down8Button->styleSheet() == ui->up5Button->styleSheet())
        ui->down8Button->setStyleSheet("background-color: white");
    else if(ui->down8Button->styleSheet() == ui->front5Button->styleSheet())
        ui->down8Button->setStyleSheet("background-color: orange");
    else if(ui->down8Button->styleSheet() == ui->left5Button->styleSheet())
        ui->down8Button->setStyleSheet("background-color: red");
    else if(ui->down8Button->styleSheet() == ui->right5Button->styleSheet())
        ui->down8Button->setStyleSheet("background-color: green");
    else if(ui->down8Button->styleSheet() == ui->down5Button->styleSheet())
        ui->down8Button->setStyleSheet("background-color: yellow");
    else if(ui->down8Button->styleSheet() == ui->back5Button->styleSheet())
        ui->down8Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_down6Button_clicked()
{
    if(ui->down6Button->styleSheet() == ui->up5Button->styleSheet())
        ui->down6Button->setStyleSheet("background-color: white");
    else if(ui->down6Button->styleSheet() == ui->front5Button->styleSheet())
        ui->down6Button->setStyleSheet("background-color: orange");
    else if(ui->down6Button->styleSheet() == ui->left5Button->styleSheet())
        ui->down6Button->setStyleSheet("background-color: red");
    else if(ui->down6Button->styleSheet() == ui->right5Button->styleSheet())
        ui->down6Button->setStyleSheet("background-color: green");
    else if(ui->down6Button->styleSheet() == ui->down5Button->styleSheet())
        ui->down6Button->setStyleSheet("background-color: yellow");
    else if(ui->down6Button->styleSheet() == ui->back5Button->styleSheet())
        ui->down6Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_down4Button_clicked()
{
    if(ui->down4Button->styleSheet() == ui->up5Button->styleSheet())
        ui->down4Button->setStyleSheet("background-color: white");
    else if(ui->down4Button->styleSheet() == ui->front5Button->styleSheet())
        ui->down4Button->setStyleSheet("background-color: orange");
    else if(ui->down4Button->styleSheet() == ui->left5Button->styleSheet())
        ui->down4Button->setStyleSheet("background-color: red");
    else if(ui->down4Button->styleSheet() == ui->right5Button->styleSheet())
        ui->down4Button->setStyleSheet("background-color: green");
    else if(ui->down4Button->styleSheet() == ui->down5Button->styleSheet())
        ui->down4Button->setStyleSheet("background-color: yellow");
    else if(ui->down4Button->styleSheet() == ui->back5Button->styleSheet())
        ui->down4Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_down2Button_clicked()
{
    if(ui->down2Button->styleSheet() == ui->up5Button->styleSheet())
        ui->down2Button->setStyleSheet("background-color: white");
    else if(ui->down2Button->styleSheet() == ui->front5Button->styleSheet())
        ui->down2Button->setStyleSheet("background-color: orange");
    else if(ui->down2Button->styleSheet() == ui->left5Button->styleSheet())
        ui->down2Button->setStyleSheet("background-color: red");
    else if(ui->down2Button->styleSheet() == ui->right5Button->styleSheet())
        ui->down2Button->setStyleSheet("background-color: green");
    else if(ui->down2Button->styleSheet() == ui->down5Button->styleSheet())
        ui->down2Button->setStyleSheet("background-color: yellow");
    else if(ui->down2Button->styleSheet() == ui->back5Button->styleSheet())
        ui->down2Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_front8Button_clicked()
{
    if(ui->front8Button->styleSheet() == ui->up5Button->styleSheet())
        ui->front8Button->setStyleSheet("background-color: white");
    else if(ui->front8Button->styleSheet() == ui->front5Button->styleSheet())
        ui->front8Button->setStyleSheet("background-color: orange");
    else if(ui->front8Button->styleSheet() == ui->left5Button->styleSheet())
        ui->front8Button->setStyleSheet("background-color: red");
    else if(ui->front8Button->styleSheet() == ui->right5Button->styleSheet())
        ui->front8Button->setStyleSheet("background-color: green");
    else if(ui->front8Button->styleSheet() == ui->down5Button->styleSheet())
        ui->front8Button->setStyleSheet("background-color: yellow");
    else if(ui->front8Button->styleSheet() == ui->back5Button->styleSheet())
        ui->front8Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_front4Button_clicked()
{
    if(ui->front4Button->styleSheet() == ui->up5Button->styleSheet())
        ui->front4Button->setStyleSheet("background-color: white");
    else if(ui->front4Button->styleSheet() == ui->front5Button->styleSheet())
        ui->front4Button->setStyleSheet("background-color: orange");
    else if(ui->front4Button->styleSheet() == ui->left5Button->styleSheet())
        ui->front4Button->setStyleSheet("background-color: red");
    else if(ui->front4Button->styleSheet() == ui->right5Button->styleSheet())
        ui->front4Button->setStyleSheet("background-color: green");
    else if(ui->front4Button->styleSheet() == ui->down5Button->styleSheet())
        ui->front4Button->setStyleSheet("background-color: yellow");
    else if(ui->front4Button->styleSheet() == ui->back5Button->styleSheet())
        ui->front4Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_front6Button_clicked()
{
    if(ui->front6Button->styleSheet() == ui->up5Button->styleSheet())
        ui->front6Button->setStyleSheet("background-color: white");
    else if(ui->front6Button->styleSheet() == ui->front5Button->styleSheet())
        ui->front6Button->setStyleSheet("background-color: orange");
    else if(ui->front6Button->styleSheet() == ui->left5Button->styleSheet())
        ui->front6Button->setStyleSheet("background-color: red");
    else if(ui->front6Button->styleSheet() == ui->right5Button->styleSheet())
        ui->front6Button->setStyleSheet("background-color: green");
    else if(ui->front6Button->styleSheet() == ui->down5Button->styleSheet())
        ui->front6Button->setStyleSheet("background-color: yellow");
    else if(ui->front6Button->styleSheet() == ui->back5Button->styleSheet())
        ui->front6Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_front2Button_clicked()
{
    if(ui->front2Button->styleSheet() == ui->up5Button->styleSheet())
        ui->front2Button->setStyleSheet("background-color: white");
    else if(ui->front2Button->styleSheet() == ui->front5Button->styleSheet())
        ui->front2Button->setStyleSheet("background-color: orange");
    else if(ui->front2Button->styleSheet() == ui->left5Button->styleSheet())
        ui->front2Button->setStyleSheet("background-color: red");
    else if(ui->front2Button->styleSheet() == ui->right5Button->styleSheet())
        ui->front2Button->setStyleSheet("background-color: green");
    else if(ui->front2Button->styleSheet() == ui->down5Button->styleSheet())
        ui->front2Button->setStyleSheet("background-color: yellow");
    else if(ui->front2Button->styleSheet() == ui->back5Button->styleSheet())
        ui->front2Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_left8Button_clicked()
{
    if(ui->left8Button->styleSheet() == ui->up5Button->styleSheet())
        ui->left8Button->setStyleSheet("background-color: white");
    else if(ui->left8Button->styleSheet() == ui->front5Button->styleSheet())
        ui->left8Button->setStyleSheet("background-color: orange");
    else if(ui->left8Button->styleSheet() == ui->left5Button->styleSheet())
        ui->left8Button->setStyleSheet("background-color: red");
    else if(ui->left8Button->styleSheet() == ui->right5Button->styleSheet())
        ui->left8Button->setStyleSheet("background-color: green");
    else if(ui->left8Button->styleSheet() == ui->down5Button->styleSheet())
        ui->left8Button->setStyleSheet("background-color: yellow");
    else if(ui->left8Button->styleSheet() == ui->back5Button->styleSheet())
        ui->left8Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_left4Button_clicked()
{
    if(ui->left4Button->styleSheet() == ui->up5Button->styleSheet())
        ui->left4Button->setStyleSheet("background-color: white");
    else if(ui->left4Button->styleSheet() == ui->front5Button->styleSheet())
        ui->left4Button->setStyleSheet("background-color: orange");
    else if(ui->left4Button->styleSheet() == ui->left5Button->styleSheet())
        ui->left4Button->setStyleSheet("background-color: red");
    else if(ui->left4Button->styleSheet() == ui->right5Button->styleSheet())
        ui->left4Button->setStyleSheet("background-color: green");
    else if(ui->left4Button->styleSheet() == ui->down5Button->styleSheet())
        ui->left4Button->setStyleSheet("background-color: yellow");
    else if(ui->left4Button->styleSheet() == ui->back5Button->styleSheet())
        ui->left4Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_left2Button_clicked()
{
    if(ui->left2Button->styleSheet() == ui->up5Button->styleSheet())
        ui->left2Button->setStyleSheet("background-color: white");
    else if(ui->left2Button->styleSheet() == ui->front5Button->styleSheet())
        ui->left2Button->setStyleSheet("background-color: orange");
    else if(ui->left2Button->styleSheet() == ui->left5Button->styleSheet())
        ui->left2Button->setStyleSheet("background-color: red");
    else if(ui->left2Button->styleSheet() == ui->right5Button->styleSheet())
        ui->left2Button->setStyleSheet("background-color: green");
    else if(ui->left2Button->styleSheet() == ui->down5Button->styleSheet())
        ui->left2Button->setStyleSheet("background-color: yellow");
    else if(ui->left2Button->styleSheet() == ui->back5Button->styleSheet())
        ui->left2Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_left6Button_clicked()
{
    if(ui->left6Button->styleSheet() == ui->up5Button->styleSheet())
        ui->left6Button->setStyleSheet("background-color: white");
    else if(ui->left6Button->styleSheet() == ui->front5Button->styleSheet())
        ui->left6Button->setStyleSheet("background-color: orange");
    else if(ui->left6Button->styleSheet() == ui->left5Button->styleSheet())
        ui->left6Button->setStyleSheet("background-color: red");
    else if(ui->left6Button->styleSheet() == ui->right5Button->styleSheet())
        ui->left6Button->setStyleSheet("background-color: green");
    else if(ui->left6Button->styleSheet() == ui->down5Button->styleSheet())
        ui->left6Button->setStyleSheet("background-color: yellow");
    else if(ui->left6Button->styleSheet() == ui->back5Button->styleSheet())
        ui->left6Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_right4Button_clicked()
{
    if(ui->right4Button->styleSheet() == ui->up5Button->styleSheet())
        ui->right4Button->setStyleSheet("background-color: white");
    else if(ui->right4Button->styleSheet() == ui->front5Button->styleSheet())
        ui->right4Button->setStyleSheet("background-color: orange");
    else if(ui->right4Button->styleSheet() == ui->left5Button->styleSheet())
        ui->right4Button->setStyleSheet("background-color: red");
    else if(ui->right4Button->styleSheet() == ui->right5Button->styleSheet())
        ui->right4Button->setStyleSheet("background-color: green");
    else if(ui->right4Button->styleSheet() == ui->down5Button->styleSheet())
        ui->right4Button->setStyleSheet("background-color: yellow");
    else if(ui->right4Button->styleSheet() == ui->back5Button->styleSheet())
        ui->right4Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_right8Button_clicked()
{
    if(ui->right8Button->styleSheet() == ui->up5Button->styleSheet())
        ui->right8Button->setStyleSheet("background-color: white");
    else if(ui->right8Button->styleSheet() == ui->front5Button->styleSheet())
        ui->right8Button->setStyleSheet("background-color: orange");
    else if(ui->right8Button->styleSheet() == ui->left5Button->styleSheet())
        ui->right8Button->setStyleSheet("background-color: red");
    else if(ui->right8Button->styleSheet() == ui->right5Button->styleSheet())
        ui->right8Button->setStyleSheet("background-color: green");
    else if(ui->right8Button->styleSheet() == ui->down5Button->styleSheet())
        ui->right8Button->setStyleSheet("background-color: yellow");
    else if(ui->right8Button->styleSheet() == ui->back5Button->styleSheet())
        ui->right8Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_right6Button_clicked()
{
    if(ui->right6Button->styleSheet() == ui->up5Button->styleSheet())
        ui->right6Button->setStyleSheet("background-color: white");
    else if(ui->right6Button->styleSheet() == ui->front5Button->styleSheet())
        ui->right6Button->setStyleSheet("background-color: orange");
    else if(ui->right6Button->styleSheet() == ui->left5Button->styleSheet())
        ui->right6Button->setStyleSheet("background-color: red");
    else if(ui->right6Button->styleSheet() == ui->right5Button->styleSheet())
        ui->right6Button->setStyleSheet("background-color: green");
    else if(ui->right6Button->styleSheet() == ui->down5Button->styleSheet())
        ui->right6Button->setStyleSheet("background-color: yellow");
    else if(ui->right6Button->styleSheet() == ui->back5Button->styleSheet())
        ui->right6Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_right2Button_clicked()
{
    if(ui->right2Button->styleSheet() == ui->up5Button->styleSheet())
        ui->right2Button->setStyleSheet("background-color: white");
    else if(ui->right2Button->styleSheet() == ui->front5Button->styleSheet())
        ui->right2Button->setStyleSheet("background-color: orange");
    else if(ui->right2Button->styleSheet() == ui->left5Button->styleSheet())
        ui->right2Button->setStyleSheet("background-color: red");
    else if(ui->right2Button->styleSheet() == ui->right5Button->styleSheet())
        ui->right2Button->setStyleSheet("background-color: green");
    else if(ui->right2Button->styleSheet() == ui->down5Button->styleSheet())
        ui->right2Button->setStyleSheet("background-color: yellow");
    else if(ui->right2Button->styleSheet() == ui->back5Button->styleSheet())
        ui->right2Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_up8Button_clicked()
{
    if(ui->up8Button->styleSheet() == ui->up5Button->styleSheet())
        ui->up8Button->setStyleSheet("background-color: white");
    else if(ui->up8Button->styleSheet() == ui->front5Button->styleSheet())
        ui->up8Button->setStyleSheet("background-color: orange");
    else if(ui->up8Button->styleSheet() == ui->left5Button->styleSheet())
        ui->up8Button->setStyleSheet("background-color: red");
    else if(ui->up8Button->styleSheet() == ui->right5Button->styleSheet())
        ui->up8Button->setStyleSheet("background-color: green");
    else if(ui->up8Button->styleSheet() == ui->down5Button->styleSheet())
        ui->up8Button->setStyleSheet("background-color: yellow");
    else if(ui->up8Button->styleSheet() == ui->back5Button->styleSheet())
        ui->up8Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_up4Button_clicked()
{
    if(ui->up4Button->styleSheet() == ui->up5Button->styleSheet())
        ui->up4Button->setStyleSheet("background-color: white");
    else if(ui->up4Button->styleSheet() == ui->front5Button->styleSheet())
        ui->up4Button->setStyleSheet("background-color: orange");
    else if(ui->up4Button->styleSheet() == ui->left5Button->styleSheet())
        ui->up4Button->setStyleSheet("background-color: red");
    else if(ui->up4Button->styleSheet() == ui->right5Button->styleSheet())
        ui->up4Button->setStyleSheet("background-color: green");
    else if(ui->up4Button->styleSheet() == ui->down5Button->styleSheet())
        ui->up4Button->setStyleSheet("background-color: yellow");
    else if(ui->up4Button->styleSheet() == ui->back5Button->styleSheet())
        ui->up4Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_up6Button_clicked()
{
    if(ui->up6Button->styleSheet() == ui->up5Button->styleSheet())
        ui->up6Button->setStyleSheet("background-color: white");
    else if(ui->up6Button->styleSheet() == ui->front5Button->styleSheet())
        ui->up6Button->setStyleSheet("background-color: orange");
    else if(ui->up6Button->styleSheet() == ui->left5Button->styleSheet())
        ui->up6Button->setStyleSheet("background-color: red");
    else if(ui->up6Button->styleSheet() == ui->right5Button->styleSheet())
        ui->up6Button->setStyleSheet("background-color: green");
    else if(ui->up6Button->styleSheet() == ui->down5Button->styleSheet())
        ui->up6Button->setStyleSheet("background-color: yellow");
    else if(ui->up6Button->styleSheet() == ui->back5Button->styleSheet())
        ui->up6Button->setStyleSheet("background-color: blue");
}
void MainWindow::on_up2Button_clicked()
{
    if(ui->up2Button->styleSheet() == ui->up5Button->styleSheet())
        ui->up2Button->setStyleSheet("background-color: white");
    else if(ui->up2Button->styleSheet() == ui->front5Button->styleSheet())
        ui->up2Button->setStyleSheet("background-color: orange");
    else if(ui->up2Button->styleSheet() == ui->left5Button->styleSheet())
        ui->up2Button->setStyleSheet("background-color: red");
    else if(ui->up2Button->styleSheet() == ui->right5Button->styleSheet())
        ui->up2Button->setStyleSheet("background-color: green");
    else if(ui->up2Button->styleSheet() == ui->down5Button->styleSheet())
        ui->up2Button->setStyleSheet("background-color: yellow");
    else if(ui->up2Button->styleSheet() == ui->back5Button->styleSheet())
        ui->up2Button->setStyleSheet("background-color: blue");
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
