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
std::ofstream cross;
std::ofstream quinas;
std::ofstream segundacamada;
std::ofstream crossdown;
std::ofstream cantosdown;
std::ofstream place;
std::ofstream turn;
std::ofstream otimotempo;
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
    ui->label_2->setVisible(false);
    ui->ultimo->setVisible(false);
    ui->camadasReal->setVisible(false);
    ui->otimoReal->setVisible(false);
    ui->QuinaNordeste->setVisible(false);
    ui->QuinaNoroeste->setVisible(false);
    ui->QuinaSudeste->setVisible(false);
    ui->QuinaSudoeste->setVisible(false);
    ui->NoroesteBaixo->setVisible(false);
    ui->NordesteBaixo->setVisible(false);
    ui->SudoesteBaixo->setVisible(false);
    ui->SudesteBaixo->setVisible(false);
    ui->CrossNorte->setVisible(false);
    ui->CrossSul->setVisible(false);
    ui->CruzLeste->setVisible(false);
    ui->CruzOeste->setVisible(false);
    ui->consertaSegunda->setVisible(false);
    ui->leftSecond->setVisible(false);
    ui->rightSecond->setVisible(false);
    ui->frontSecond->setVisible(false);
    ui->backSecond->setVisible(false);
    completeCube();
    conf.readCalibrateColors(&conf);
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
    cross.open("1cross_r2.txt");
    quinas.open("2quinas_r2.txt");
    segundacamada.open("3segundacamada_r2.txt");
    crossdown.open("4crossdown_r2.txt");
    cantosdown.open("5cantosdown_r2.txt");
    place.open("6place_r2.txt");
    turn.open("7turn_r2.txt");
    otimotempo.open("otimotempo.txt");
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
void MainWindow::on_Randomize_clicked()
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
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateU()
{
    if(!simulacao)
    {
                delay(35000*del);
    }
    QString auxF1 = ui->front1->styleSheet();
    QString auxF2 = ui->front2->styleSheet();
    QString auxF3 = ui->front3->styleSheet();
    QString auxL1 = ui->left1->styleSheet();
    QString auxL2 = ui->left2->styleSheet();
    QString auxL3 = ui->left3->styleSheet();
    QString auxR1 = ui->right1->styleSheet();
    QString auxR2 = ui->right2->styleSheet();
    QString auxR3 = ui->right3->styleSheet();
    QString auxB7 = ui->back7->styleSheet();
    QString auxB8 = ui->back8->styleSheet();
    QString auxB9 = ui->back9->styleSheet();
    QString auxU1 = ui->up1->styleSheet();
    QString auxU2 = ui->up2->styleSheet();
    QString auxU3 = ui->up3->styleSheet();
    QString auxU4 = ui->up4->styleSheet();
    QString auxU6 = ui->up6->styleSheet();
    QString auxU7 = ui->up7->styleSheet();
    QString auxU8 = ui->up8->styleSheet();
    QString auxU9 = ui->up9->styleSheet();
    ui->front1->setStyleSheet(auxR1);
    ui->front2->setStyleSheet(auxR2);
    ui->front3->setStyleSheet(auxR3);
    ui->left1->setStyleSheet(auxF1);
    ui->left2->setStyleSheet(auxF2);
    ui->left3->setStyleSheet(auxF3);
    ui->right1->setStyleSheet(auxB9);
    ui->right2->setStyleSheet(auxB8);
    ui->right3->setStyleSheet(auxB7);
    ui->back9->setStyleSheet(auxL1);
    ui->back8->setStyleSheet(auxL2);
    ui->back7->setStyleSheet(auxL3);
    ui->up1->setStyleSheet(auxU7);
    ui->up2->setStyleSheet(auxU4);
    ui->up3->setStyleSheet(auxU1);
    ui->up4->setStyleSheet(auxU8);
    ui->up6->setStyleSheet(auxU2);
    ui->up7->setStyleSheet(auxU9);
    ui->up8->setStyleSheet(auxU6);
    ui->up9->setStyleSheet(auxU3);
    ui->up5->repaint();
    movimentos++;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateUA()
{
    if(!simulacao)
    {
                delay(20000*del);
    }
    QString auxF1 = ui->front1->styleSheet();
    QString auxF2 = ui->front2->styleSheet();
    QString auxF3 = ui->front3->styleSheet();
    QString auxL1 = ui->left1->styleSheet();
    QString auxL2 = ui->left2->styleSheet();
    QString auxL3 = ui->left3->styleSheet();
    QString auxR1 = ui->right1->styleSheet();
    QString auxR2 = ui->right2->styleSheet();
    QString auxR3 = ui->right3->styleSheet();
    QString auxB7 = ui->back7->styleSheet();
    QString auxB8 = ui->back8->styleSheet();
    QString auxB9 = ui->back9->styleSheet();
    QString auxU1 = ui->up1->styleSheet();
    QString auxU2 = ui->up2->styleSheet();
    QString auxU3 = ui->up3->styleSheet();
    QString auxU4 = ui->up4->styleSheet();
    QString auxU6 = ui->up6->styleSheet();
    QString auxU7 = ui->up7->styleSheet();
    QString auxU8 = ui->up8->styleSheet();
    QString auxU9 = ui->up9->styleSheet();
    ui->front1->setStyleSheet(auxL1);
    ui->front2->setStyleSheet(auxL2);
    ui->front3->setStyleSheet(auxL3);
    ui->left1->setStyleSheet(auxB9);
    ui->left2->setStyleSheet(auxB8);
    ui->left3->setStyleSheet(auxB7);
    ui->right1->setStyleSheet(auxF1);
    ui->right2->setStyleSheet(auxF2);
    ui->right3->setStyleSheet(auxF3);
    ui->back9->setStyleSheet(auxR1);
    ui->back8->setStyleSheet(auxR2);
    ui->back7->setStyleSheet(auxR3);
    ui->up1->setStyleSheet(auxU3);
    ui->up2->setStyleSheet(auxU6);
    ui->up3->setStyleSheet(auxU9);
    ui->up4->setStyleSheet(auxU2);
    ui->up6->setStyleSheet(auxU8);
    ui->up7->setStyleSheet(auxU1);
    ui->up8->setStyleSheet(auxU4);
    ui->up9->setStyleSheet(auxU7);
    ui->up5->repaint();
    movimentos++;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateD()
{
    if(!simulacao)
    {
                delay(31000*del);
    }
    QString auxF7 = ui->front7->styleSheet();
    QString auxF8 = ui->front8->styleSheet();
    QString auxF9 = ui->front9->styleSheet();
    QString auxL7 = ui->left7->styleSheet();
    QString auxL8 = ui->left8->styleSheet();
    QString auxL9 = ui->left9->styleSheet();
    QString auxR7 = ui->right7->styleSheet();
    QString auxR8 = ui->right8->styleSheet();
    QString auxR9 = ui->right9->styleSheet();
    QString auxB1 = ui->back1->styleSheet();
    QString auxB2 = ui->back2->styleSheet();
    QString auxB3 = ui->back3->styleSheet();
    QString auxD1 = ui->down1->styleSheet();
    QString auxD2 = ui->down2->styleSheet();
    QString auxD3 = ui->down3->styleSheet();
    QString auxD4 = ui->down4->styleSheet();
    QString auxD6 = ui->down6->styleSheet();
    QString auxD7 = ui->down7->styleSheet();
    QString auxD8 = ui->down8->styleSheet();
    QString auxD9 = ui->down9->styleSheet();
    ui->front7->setStyleSheet(auxL7);
    ui->front8->setStyleSheet(auxL8);
    ui->front9->setStyleSheet(auxL9);
    ui->left7->setStyleSheet(auxB3);
    ui->left8->setStyleSheet(auxB2);
    ui->left9->setStyleSheet(auxB1);
    ui->right7->setStyleSheet(auxF7);
    ui->right8->setStyleSheet(auxF8);
    ui->right9->setStyleSheet(auxF9);
    ui->back3->setStyleSheet(auxR7);
    ui->back2->setStyleSheet(auxR8);
    ui->back1->setStyleSheet(auxR9);
    ui->down1->setStyleSheet(auxD7);
    ui->down2->setStyleSheet(auxD4);
    ui->down3->setStyleSheet(auxD1);
    ui->down4->setStyleSheet(auxD8);
    ui->down6->setStyleSheet(auxD2);
    ui->down7->setStyleSheet(auxD9);
    ui->down8->setStyleSheet(auxD6);
    ui->down9->setStyleSheet(auxD3);
    ui->down5->repaint();
    movimentos++;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateDA()
{
    if(!simulacao)
    {
                delay(20000*del);
    }
    QString auxF7 = ui->front7->styleSheet();
    QString auxF8 = ui->front8->styleSheet();
    QString auxF9 = ui->front9->styleSheet();
    QString auxL7 = ui->left7->styleSheet();
    QString auxL8 = ui->left8->styleSheet();
    QString auxL9 = ui->left9->styleSheet();
    QString auxR7 = ui->right7->styleSheet();
    QString auxR8 = ui->right8->styleSheet();
    QString auxR9 = ui->right9->styleSheet();
    QString auxB1 = ui->back1->styleSheet();
    QString auxB2 = ui->back2->styleSheet();
    QString auxB3 = ui->back3->styleSheet();
    QString auxD1 = ui->down1->styleSheet();
    QString auxD2 = ui->down2->styleSheet();
    QString auxD3 = ui->down3->styleSheet();
    QString auxD4 = ui->down4->styleSheet();
    QString auxD6 = ui->down6->styleSheet();
    QString auxD7 = ui->down7->styleSheet();
    QString auxD8 = ui->down8->styleSheet();
    QString auxD9 = ui->down9->styleSheet();
    ui->front7->setStyleSheet(auxR7);
    ui->front8->setStyleSheet(auxR8);
    ui->front9->setStyleSheet(auxR9);
    ui->left7->setStyleSheet(auxF7);
    ui->left8->setStyleSheet(auxF8);
    ui->left9->setStyleSheet(auxF9);
    ui->right7->setStyleSheet(auxB3);
    ui->right8->setStyleSheet(auxB2);
    ui->right9->setStyleSheet(auxB1);
    ui->back3->setStyleSheet(auxL7);
    ui->back2->setStyleSheet(auxL8);
    ui->back1->setStyleSheet(auxL9);
    ui->down1->setStyleSheet(auxD3);
    ui->down2->setStyleSheet(auxD6);
    ui->down3->setStyleSheet(auxD9);
    ui->down4->setStyleSheet(auxD2);
    ui->down6->setStyleSheet(auxD8);
    ui->down7->setStyleSheet(auxD1);
    ui->down8->setStyleSheet(auxD4);
    ui->down9->setStyleSheet(auxD7);
    ui->down5->repaint();
    movimentos++;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateR()
{
    if(!simulacao)
    {
                delay(39000*del);
    }
    QString auxF3 = ui->front3->styleSheet();
    QString auxF6 = ui->front6->styleSheet();
    QString auxF9 = ui->front9->styleSheet();
    QString auxU3 = ui->up3->styleSheet();
    QString auxU6 = ui->up6->styleSheet();
    QString auxU9 = ui->up9->styleSheet();
    QString auxB3 = ui->back3->styleSheet();
    QString auxB6 = ui->back6->styleSheet();
    QString auxB9 = ui->back9->styleSheet();
    QString auxD3 = ui->down3->styleSheet();
    QString auxD6 = ui->down6->styleSheet();
    QString auxD9 = ui->down9->styleSheet();
    QString auxR1 = ui->right1->styleSheet();
    QString auxR2 = ui->right2->styleSheet();
    QString auxR3 = ui->right3->styleSheet();
    QString auxR4 = ui->right4->styleSheet();
    QString auxR6 = ui->right6->styleSheet();
    QString auxR7 = ui->right7->styleSheet();
    QString auxR8 = ui->right8->styleSheet();
    QString auxR9 = ui->right9->styleSheet();
    ui->front3->setStyleSheet(auxD3);
    ui->front6->setStyleSheet(auxD6);
    ui->front9->setStyleSheet(auxD9);
    ui->up3->setStyleSheet(auxF3);
    ui->up6->setStyleSheet(auxF6);
    ui->up9->setStyleSheet(auxF9);
    ui->back3->setStyleSheet(auxU3);
    ui->back6->setStyleSheet(auxU6);
    ui->back9->setStyleSheet(auxU9);
    ui->down3->setStyleSheet(auxB3);
    ui->down6->setStyleSheet(auxB6);
    ui->down9->setStyleSheet(auxB9);
    ui->right1->setStyleSheet(auxR7);
    ui->right2->setStyleSheet(auxR4);
    ui->right3->setStyleSheet(auxR1);
    ui->right4->setStyleSheet(auxR8);
    ui->right6->setStyleSheet(auxR2);
    ui->right7->setStyleSheet(auxR9);
    ui->right8->setStyleSheet(auxR6);
    ui->right9->setStyleSheet(auxR3);
    ui->right5->repaint();
    movimentos++;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateRA()
{
    if(!simulacao)
    {
                delay(29000*del);
    }
    QString auxF3 = ui->front3->styleSheet();
    QString auxF6 = ui->front6->styleSheet();
    QString auxF9 = ui->front9->styleSheet();
    QString auxU3 = ui->up3->styleSheet();
    QString auxU6 = ui->up6->styleSheet();
    QString auxU9 = ui->up9->styleSheet();
    QString auxB3 = ui->back3->styleSheet();
    QString auxB6 = ui->back6->styleSheet();
    QString auxB9 = ui->back9->styleSheet();
    QString auxD3 = ui->down3->styleSheet();
    QString auxD6 = ui->down6->styleSheet();
    QString auxD9 = ui->down9->styleSheet();
    QString auxR1 = ui->right1->styleSheet();
    QString auxR2 = ui->right2->styleSheet();
    QString auxR3 = ui->right3->styleSheet();
    QString auxR4 = ui->right4->styleSheet();
    QString auxR6 = ui->right6->styleSheet();
    QString auxR7 = ui->right7->styleSheet();
    QString auxR8 = ui->right8->styleSheet();
    QString auxR9 = ui->right9->styleSheet();
    ui->front3->setStyleSheet(auxU3);
    ui->front6->setStyleSheet(auxU6);
    ui->front9->setStyleSheet(auxU9);
    ui->up3->setStyleSheet(auxB3);
    ui->up6->setStyleSheet(auxB6);
    ui->up9->setStyleSheet(auxB9);
    ui->back3->setStyleSheet(auxD3);
    ui->back6->setStyleSheet(auxD6);
    ui->back9->setStyleSheet(auxD9);
    ui->down3->setStyleSheet(auxF3);
    ui->down6->setStyleSheet(auxF6);
    ui->down9->setStyleSheet(auxF9);
    ui->right1->setStyleSheet(auxR3);
    ui->right2->setStyleSheet(auxR6);
    ui->right3->setStyleSheet(auxR9);
    ui->right4->setStyleSheet(auxR2);
    ui->right6->setStyleSheet(auxR8);
    ui->right7->setStyleSheet(auxR1);
    ui->right8->setStyleSheet(auxR4);
    ui->right9->setStyleSheet(auxR7);
    ui->right5->repaint();
    movimentos++;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateL()
{
    if(!simulacao)
    {
                delay(39000*del);
    }
    QString auxF1 = ui->front1->styleSheet();
    QString auxF4 = ui->front4->styleSheet();
    QString auxF7 = ui->front7->styleSheet();
    QString auxU1 = ui->up1->styleSheet();
    QString auxU4 = ui->up4->styleSheet();
    QString auxU7 = ui->up7->styleSheet();
    QString auxB1 = ui->back1->styleSheet();
    QString auxB4 = ui->back4->styleSheet();
    QString auxB7 = ui->back7->styleSheet();
    QString auxD1 = ui->down1->styleSheet();
    QString auxD4 = ui->down4->styleSheet();
    QString auxD7 = ui->down7->styleSheet();
    QString auxL1 = ui->left1->styleSheet();
    QString auxL2 = ui->left2->styleSheet();
    QString auxL3 = ui->left3->styleSheet();
    QString auxL4 = ui->left4->styleSheet();
    QString auxL6 = ui->left6->styleSheet();
    QString auxL7 = ui->left7->styleSheet();
    QString auxL8 = ui->left8->styleSheet();
    QString auxL9 = ui->left9->styleSheet();
    ui->front1->setStyleSheet(auxU1);
    ui->front4->setStyleSheet(auxU4);
    ui->front7->setStyleSheet(auxU7);
    ui->up1->setStyleSheet(auxB1);
    ui->up4->setStyleSheet(auxB4);
    ui->up7->setStyleSheet(auxB7);
    ui->back1->setStyleSheet(auxD1);
    ui->back4->setStyleSheet(auxD4);
    ui->back7->setStyleSheet(auxD7);
    ui->down1->setStyleSheet(auxF1);
    ui->down4->setStyleSheet(auxF4);
    ui->down7->setStyleSheet(auxF7);
    ui->left1->setStyleSheet(auxL7);
    ui->left2->setStyleSheet(auxL4);
    ui->left3->setStyleSheet(auxL1);
    ui->left4->setStyleSheet(auxL8);
    ui->left6->setStyleSheet(auxL2);
    ui->left7->setStyleSheet(auxL9);
    ui->left8->setStyleSheet(auxL6);
    ui->left9->setStyleSheet(auxL3);
    ui->left5->repaint();
    movimentos++;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateLA()
{
    if(!simulacao)
    {
                delay(21000*del);
    }
    QString auxF1 = ui->front1->styleSheet();
    QString auxF4 = ui->front4->styleSheet();
    QString auxF7 = ui->front7->styleSheet();
    QString auxU1 = ui->up1->styleSheet();
    QString auxU4 = ui->up4->styleSheet();
    QString auxU7 = ui->up7->styleSheet();
    QString auxB1 = ui->back1->styleSheet();
    QString auxB4 = ui->back4->styleSheet();
    QString auxB7 = ui->back7->styleSheet();
    QString auxD1 = ui->down1->styleSheet();
    QString auxD4 = ui->down4->styleSheet();
    QString auxD7 = ui->down7->styleSheet();
    QString auxL1 = ui->left1->styleSheet();
    QString auxL2 = ui->left2->styleSheet();
    QString auxL3 = ui->left3->styleSheet();
    QString auxL4 = ui->left4->styleSheet();
    QString auxL6 = ui->left6->styleSheet();
    QString auxL7 = ui->left7->styleSheet();
    QString auxL8 = ui->left8->styleSheet();
    QString auxL9 = ui->left9->styleSheet();
    ui->front1->setStyleSheet(auxD1);
    ui->front4->setStyleSheet(auxD4);
    ui->front7->setStyleSheet(auxD7);
    ui->up1->setStyleSheet(auxF1);
    ui->up4->setStyleSheet(auxF4);
    ui->up7->setStyleSheet(auxF7);
    ui->back1->setStyleSheet(auxU1);
    ui->back4->setStyleSheet(auxU4);
    ui->back7->setStyleSheet(auxU7);
    ui->down1->setStyleSheet(auxB1);
    ui->down4->setStyleSheet(auxB4);
    ui->down7->setStyleSheet(auxB7);
    ui->left1->setStyleSheet(auxL3);
    ui->left2->setStyleSheet(auxL6);
    ui->left3->setStyleSheet(auxL9);
    ui->left4->setStyleSheet(auxL2);
    ui->left6->setStyleSheet(auxL8);
    ui->left7->setStyleSheet(auxL1);
    ui->left8->setStyleSheet(auxL4);
    ui->left9->setStyleSheet(auxL7);
    ui->left5->repaint();
    movimentos++;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateF()
{
    if(!simulacao)
    {
                delay(35000*del);
    }
    QString auxU7 = ui->up7->styleSheet();
    QString auxU8 = ui->up8->styleSheet();
    QString auxU9 = ui->up9->styleSheet();
    QString auxR1 = ui->right1->styleSheet();
    QString auxR4 = ui->right4->styleSheet();
    QString auxR7 = ui->right7->styleSheet();
    QString auxD1 = ui->down1->styleSheet();
    QString auxD2 = ui->down2->styleSheet();
    QString auxD3 = ui->down3->styleSheet();
    QString auxL3 = ui->left3->styleSheet();
    QString auxL6 = ui->left6->styleSheet();
    QString auxL9 = ui->left9->styleSheet();
    QString auxF1 = ui->front1->styleSheet();
    QString auxF2 = ui->front2->styleSheet();
    QString auxF3 = ui->front3->styleSheet();
    QString auxF4 = ui->front4->styleSheet();
    QString auxF6 = ui->front6->styleSheet();
    QString auxF7 = ui->front7->styleSheet();
    QString auxF8 = ui->front8->styleSheet();
    QString auxF9 = ui->front9->styleSheet();
    ui->up7->setStyleSheet(auxL9);
    ui->up8->setStyleSheet(auxL6);
    ui->up9->setStyleSheet(auxL3);
    ui->right1->setStyleSheet(auxU7);
    ui->right4->setStyleSheet(auxU8);
    ui->right7->setStyleSheet(auxU9);
    ui->down1->setStyleSheet(auxR7);
    ui->down2->setStyleSheet(auxR4);
    ui->down3->setStyleSheet(auxR1);
    ui->left3->setStyleSheet(auxD1);
    ui->left6->setStyleSheet(auxD2);
    ui->left9->setStyleSheet(auxD3);
    ui->front1->setStyleSheet(auxF7);
    ui->front2->setStyleSheet(auxF4);
    ui->front3->setStyleSheet(auxF1);
    ui->front4->setStyleSheet(auxF8);
    ui->front6->setStyleSheet(auxF2);
    ui->front7->setStyleSheet(auxF9);
    ui->front8->setStyleSheet(auxF6);
    ui->front9->setStyleSheet(auxF3);
    ui->front5->repaint();
    movimentos++;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateFA()
{
    if(!simulacao)
    {
                delay(20000*del);
    }
    QString auxU7 = ui->up7->styleSheet();
    QString auxU8 = ui->up8->styleSheet();
    QString auxU9 = ui->up9->styleSheet();
    QString auxR1 = ui->right1->styleSheet();
    QString auxR4 = ui->right4->styleSheet();
    QString auxR7 = ui->right7->styleSheet();
    QString auxD1 = ui->down1->styleSheet();
    QString auxD2 = ui->down2->styleSheet();
    QString auxD3 = ui->down3->styleSheet();
    QString auxL3 = ui->left3->styleSheet();
    QString auxL6 = ui->left6->styleSheet();
    QString auxL9 = ui->left9->styleSheet();
    QString auxF1 = ui->front1->styleSheet();
    QString auxF2 = ui->front2->styleSheet();
    QString auxF3 = ui->front3->styleSheet();
    QString auxF4 = ui->front4->styleSheet();
    QString auxF6 = ui->front6->styleSheet();
    QString auxF7 = ui->front7->styleSheet();
    QString auxF8 = ui->front8->styleSheet();
    QString auxF9 = ui->front9->styleSheet();
    ui->left9->setStyleSheet(auxU7);
    ui->left6->setStyleSheet(auxU8);
    ui->left3->setStyleSheet(auxU9);
    ui->up7->setStyleSheet(auxR1);
    ui->up8->setStyleSheet(auxR4);
    ui->up9->setStyleSheet(auxR7);
    ui->right7->setStyleSheet(auxD1);
    ui->right4->setStyleSheet(auxD2);
    ui->right1->setStyleSheet(auxD3);
    ui->down1->setStyleSheet(auxL3);
    ui->down2->setStyleSheet(auxL6);
    ui->down3->setStyleSheet(auxL9);
    ui->front1->setStyleSheet(auxF3);
    ui->front2->setStyleSheet(auxF6);
    ui->front3->setStyleSheet(auxF9);
    ui->front4->setStyleSheet(auxF2);
    ui->front6->setStyleSheet(auxF8);
    ui->front7->setStyleSheet(auxF1);
    ui->front8->setStyleSheet(auxF4);
    ui->front9->setStyleSheet(auxF7);
    ui->front5->repaint();
    movimentos++;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateB()
{
    if(!simulacao)
    {
                delay(27000*del);
    }
    QString auxU1 = ui->up1->styleSheet();
    QString auxU2 = ui->up2->styleSheet();
    QString auxU3 = ui->up3->styleSheet();
    QString auxR3 = ui->right3->styleSheet();
    QString auxR6 = ui->right6->styleSheet();
    QString auxR9 = ui->right9->styleSheet();
    QString auxD7 = ui->down7->styleSheet();
    QString auxD8 = ui->down8->styleSheet();
    QString auxD9 = ui->down9->styleSheet();
    QString auxL1 = ui->left1->styleSheet();
    QString auxL4 = ui->left4->styleSheet();
    QString auxL7 = ui->left7->styleSheet();
    QString auxB1 = ui->back1->styleSheet();
    QString auxB2 = ui->back2->styleSheet();
    QString auxB3 = ui->back3->styleSheet();
    QString auxB4 = ui->back4->styleSheet();
    QString auxB6 = ui->back6->styleSheet();
    QString auxB7 = ui->back7->styleSheet();
    QString auxB8 = ui->back8->styleSheet();
    QString auxB9 = ui->back9->styleSheet();
    ui->up1->setStyleSheet(auxR3);
    ui->up2->setStyleSheet(auxR6);
    ui->up3->setStyleSheet(auxR9);
    ui->right3->setStyleSheet(auxD9);
    ui->right6->setStyleSheet(auxD8);
    ui->right9->setStyleSheet(auxD7);
    ui->down7->setStyleSheet(auxL1);
    ui->down8->setStyleSheet(auxL4);
    ui->down9->setStyleSheet(auxL7);
    ui->left1->setStyleSheet(auxU3);
    ui->left4->setStyleSheet(auxU2);
    ui->left7->setStyleSheet(auxU1);
    ui->back1->setStyleSheet(auxB7);
    ui->back2->setStyleSheet(auxB4);
    ui->back3->setStyleSheet(auxB1);
    ui->back4->setStyleSheet(auxB8);
    ui->back6->setStyleSheet(auxB2);
    ui->back7->setStyleSheet(auxB9);
    ui->back8->setStyleSheet(auxB6);
    ui->back9->setStyleSheet(auxB3);
    ui->back5->repaint();
    movimentos++;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::rotateBA()
{
    if(!simulacao)
    {
                delay(19000*del);
    }
    QString auxU1 = ui->up1->styleSheet();
    QString auxU2 = ui->up2->styleSheet();
    QString auxU3 = ui->up3->styleSheet();
    QString auxR3 = ui->right3->styleSheet();
    QString auxR6 = ui->right6->styleSheet();
    QString auxR9 = ui->right9->styleSheet();
    QString auxD7 = ui->down7->styleSheet();
    QString auxD8 = ui->down8->styleSheet();
    QString auxD9 = ui->down9->styleSheet();
    QString auxL1 = ui->left1->styleSheet();
    QString auxL4 = ui->left4->styleSheet();
    QString auxL7 = ui->left7->styleSheet();
    QString auxB1 = ui->back1->styleSheet();
    QString auxB2 = ui->back2->styleSheet();
    QString auxB3 = ui->back3->styleSheet();
    QString auxB4 = ui->back4->styleSheet();
    QString auxB6 = ui->back6->styleSheet();
    QString auxB7 = ui->back7->styleSheet();
    QString auxB8 = ui->back8->styleSheet();
    QString auxB9 = ui->back9->styleSheet();
    ui->up1->setStyleSheet(auxL7);
    ui->up2->setStyleSheet(auxL4);
    ui->up3->setStyleSheet(auxL1);
    ui->right3->setStyleSheet(auxU1);
    ui->right6->setStyleSheet(auxU2);
    ui->right9->setStyleSheet(auxU3);
    ui->down7->setStyleSheet(auxR9);
    ui->down8->setStyleSheet(auxR6);
    ui->down9->setStyleSheet(auxR3);
    ui->left1->setStyleSheet(auxD7);
    ui->left4->setStyleSheet(auxD8);
    ui->left7->setStyleSheet(auxD9);
    ui->back1->setStyleSheet(auxB3);
    ui->back2->setStyleSheet(auxB6);
    ui->back3->setStyleSheet(auxB9);
    ui->back4->setStyleSheet(auxB2);
    ui->back6->setStyleSheet(auxB8);
    ui->back7->setStyleSheet(auxB1);
    ui->back8->setStyleSheet(auxB4);
    ui->back9->setStyleSheet(auxB7);
    ui->back5->repaint();
    movimentos++;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
}
void MainWindow::completeCube()
{
    
    ui->up1->setStyleSheet("background-color: blue");
    ui->up2->setStyleSheet("background-color: blue");
    ui->up3->setStyleSheet("background-color: blue");
    ui->up4->setStyleSheet("background-color: blue");
    ui->up5->setStyleSheet("background-color: blue");
    ui->up6->setStyleSheet("background-color: blue");
    ui->up7->setStyleSheet("background-color: blue");
    ui->up8->setStyleSheet("background-color: blue");
    ui->up9->setStyleSheet("background-color: blue");
    ui->front1->setStyleSheet("background-color: white");
    ui->front2->setStyleSheet("background-color: white");
    ui->front3->setStyleSheet("background-color: white");
    ui->front4->setStyleSheet("background-color: white");
    ui->front5->setStyleSheet("background-color: white");
    ui->front6->setStyleSheet("background-color: white");
    ui->front7->setStyleSheet("background-color: white");
    ui->front8->setStyleSheet("background-color: white");
    ui->front9->setStyleSheet("background-color: white");
    ui->left1->setStyleSheet("background-color: orange");
    ui->left2->setStyleSheet("background-color: orange");
    ui->left3->setStyleSheet("background-color: orange");
    ui->left4->setStyleSheet("background-color: orange");
    ui->left5->setStyleSheet("background-color: orange");
    ui->left6->setStyleSheet("background-color: orange");
    ui->left7->setStyleSheet("background-color: orange");
    ui->left8->setStyleSheet("background-color: orange");
    ui->left9->setStyleSheet("background-color: orange");
    ui->right1->setStyleSheet("background-color: red");
    ui->right2->setStyleSheet("background-color: red");
    ui->right3->setStyleSheet("background-color: red");
    ui->right4->setStyleSheet("background-color: red");
    ui->right5->setStyleSheet("background-color: red");
    ui->right6->setStyleSheet("background-color: red");
    ui->right7->setStyleSheet("background-color: red");
    ui->right8->setStyleSheet("background-color: red");
    ui->right9->setStyleSheet("background-color: red");
    ui->down1->setStyleSheet("background-color: green");
    ui->down2->setStyleSheet("background-color: green");
    ui->down3->setStyleSheet("background-color: green");
    ui->down4->setStyleSheet("background-color: green");
    ui->down5->setStyleSheet("background-color: green");
    ui->down6->setStyleSheet("background-color: green");
    ui->down7->setStyleSheet("background-color: green");
    ui->down8->setStyleSheet("background-color: green");
    ui->down9->setStyleSheet("background-color: green");
    ui->back1->setStyleSheet("background-color: yellow");
    ui->back2->setStyleSheet("background-color: yellow");
    ui->back3->setStyleSheet("background-color: yellow");
    ui->back4->setStyleSheet("background-color: yellow");
    ui->back5->setStyleSheet("background-color: yellow");
    ui->back6->setStyleSheet("background-color: yellow");
    ui->back7->setStyleSheet("background-color: yellow");
    ui->back8->setStyleSheet("background-color: yellow");
    ui->back9->setStyleSheet("background-color: yellow");
    
    /*
    ui->up5->setStyleSheet("background-color: blue");
    ui->down5->setStyleSheet("background-color: green");
    ui->right5->setStyleSheet("background-color: red");
    ui->front5->setStyleSheet("background-color: white");
    ui->left5->setStyleSheet("background-color: orange");
    ui->back5->setStyleSheet("background-color: yellow");
    ui->back1->setStyleSheet("background-color: blue");
    ui->down7->setStyleSheet("background-color: blue");
    ui->left7->setStyleSheet("background-color: blue");
    */
}
void MainWindow::on_U_clicked()
{
   rotateU();
}
void MainWindow::on_UA_clicked()
{
    rotateUA();
}
void MainWindow::on_D_clicked()
{
    rotateD();
}
void MainWindow::on_DA_clicked()
{
   rotateDA();
}
void MainWindow::on_R_clicked()
{
    rotateR();
}
void MainWindow::on_RA_clicked()
{
    rotateRA();
}
void MainWindow::on_L_clicked()
{
    rotateL();
}
void MainWindow::on_LA_clicked()
{
    rotateLA();
}
void MainWindow::on_F_clicked()
{
    rotateF();
}
void MainWindow::on_FA_clicked()
{
    rotateFA();
}
void MainWindow::on_B_clicked()
{
    rotateB();
}
void MainWindow::on_BA_clicked()
{
    rotateBA();
}
void MainWindow::CruzSul()
{
    if(ui->up8->styleSheet() != ui->up5->styleSheet() || ui->front2->styleSheet() != ui->front5->styleSheet())
    {
        if(ui->front2->styleSheet() == ui->up5->styleSheet() && ui->up8->styleSheet() == ui->front5->styleSheet())
        {
            rotateF();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->front6->styleSheet() == ui->up5->styleSheet() && ui->right4->styleSheet() == ui->front5->styleSheet())
        {
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->front6->styleSheet() == ui->front5->styleSheet() && ui->right4->styleSheet() == ui->up5->styleSheet())
        {
            rotateFA();
        }
        else if(ui->front4->styleSheet() == ui->up5->styleSheet() && ui->left6->styleSheet() == ui->front5->styleSheet())
        {
            rotateU();
            rotateLA();
            rotateUA();
        }
        else if(ui->front4->styleSheet() == ui->front5->styleSheet() && ui->left6->styleSheet() == ui->up5->styleSheet())
        {
            rotateF();
        }
        else if(ui->front8->styleSheet() == ui->front5->styleSheet() && ui->down2->styleSheet() == ui->up5->styleSheet())
        {
            rotateF();
            rotateF();
        }
        else if(ui->front8->styleSheet() == ui->up5->styleSheet() && ui->down2->styleSheet() == ui->front5->styleSheet())
        {
            rotateFA();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->right2->styleSheet() == ui->front5->styleSheet() && ui->up6->styleSheet() == ui->up5->styleSheet())
        {
            rotateU();
        }
        else if(ui->right2->styleSheet() == ui->up5->styleSheet() && ui->up6->styleSheet() == ui->front5->styleSheet())
        {
            rotateU();
            rotateF();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->left2->styleSheet() == ui->front5->styleSheet() && ui->up4->styleSheet() == ui->up5->styleSheet())
        {
            rotateUA();
        }
        else if(ui->left2->styleSheet() == ui->up5->styleSheet() && ui->up4->styleSheet() == ui->front5->styleSheet())
        {
            rotateUA();
            rotateF();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->right8->styleSheet() == ui->front5->styleSheet() && ui->down6->styleSheet() == ui->up5->styleSheet())
        {
            rotateDA();
            rotateF();
            rotateF();
        }
        else if(ui->right8->styleSheet() == ui->up5->styleSheet() && ui->down6->styleSheet() == ui->front5->styleSheet())
        {
            rotateDA();
            rotateFA();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->left8->styleSheet() == ui->front5->styleSheet() && ui->down4->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateF();
            rotateF();
        }
        else if(ui->left8->styleSheet() == ui->up5->styleSheet() && ui->down4->styleSheet() == ui->front5->styleSheet())
        {
            rotateD();
            rotateFA();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->back8->styleSheet() == ui->front5->styleSheet() && ui->up2->styleSheet() == ui->up5->styleSheet())
        {
            rotateU();
            rotateU();
        }
        else if(ui->back8->styleSheet() == ui->up5->styleSheet() && ui->up2->styleSheet() == ui->front5->styleSheet())
        {
            rotateU();
            rotateU();
            rotateF();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->back6->styleSheet() == ui->front5->styleSheet() && ui->right6->styleSheet() == ui->up5->styleSheet())
        {
            rotateB();
            rotateU();
            rotateU();
        }
        else if(ui->back6->styleSheet() == ui->up5->styleSheet() && ui->right6->styleSheet() == ui->front5->styleSheet())
        {
            rotateB();
            rotateU();
            rotateU();
            rotateF();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->back4->styleSheet() == ui->front5->styleSheet() && ui->left4->styleSheet() == ui->up5->styleSheet())
        {
            rotateBA();
            rotateU();
            rotateU();
        }
        else if(ui->back4->styleSheet() == ui->up5->styleSheet() && ui->left4->styleSheet() == ui->front5->styleSheet())
        {
            rotateBA();
            rotateU();
            rotateU();
            rotateF();
            rotateUA();
            rotateR();
            rotateU();
        }
        else if(ui->back2->styleSheet() == ui->front5->styleSheet() && ui->down8->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateD();
            rotateF();
            rotateF();
        }
        else if(ui->back2->styleSheet() == ui->up5->styleSheet() && ui->down8->styleSheet() == ui->front5->styleSheet())
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
    if(ui->up6->styleSheet() != ui->up5->styleSheet() || ui->right2->styleSheet() != ui->right5->styleSheet())
    {
        if(ui->up6->styleSheet() == ui->right5->styleSheet() && ui->right2->styleSheet() == ui->up5->styleSheet())
        {
            rotateR();
            rotateUA();
            rotateB();
            rotateU();
            ult = "1L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right6->styleSheet() == ui->up5->styleSheet() && ui->back6->styleSheet() == ui->right5->styleSheet())
        {
            rotateUA();
            rotateB();
            rotateU();
            ult = "2L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right6->styleSheet() == ui->right5->styleSheet() && ui->back6->styleSheet() == ui->up5->styleSheet())
        {
            rotateRA();
            ult = "3L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right4->styleSheet() == ui->up5->styleSheet() && ui->front6->styleSheet() == ui->right5->styleSheet())
        {
            rotateU();
            rotateFA();
            rotateUA();
            ult = "4L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right4->styleSheet() == ui->right5->styleSheet() && ui->front6->styleSheet() == ui->up5->styleSheet())
        {
            rotateR();
            ult = "5L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right8->styleSheet() == ui->right5->styleSheet() && ui->down6->styleSheet() == ui->up5->styleSheet())
        {
            rotateR();
            rotateR();
            ult = "6L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right8->styleSheet() == ui->up5->styleSheet() && ui->down6->styleSheet() == ui->right5->styleSheet())
        {
            rotateRA();
            rotateUA();
            rotateB();
            rotateU();
            ult = "7L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back8->styleSheet() == ui->right5->styleSheet() && ui->up2->styleSheet() == ui->up5->styleSheet())
        {
            rotateBA();
            rotateUA();
            rotateB();
            rotateU();
            ult = "8L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back8->styleSheet() == ui->up5->styleSheet() && ui->up2->styleSheet() == ui->right5->styleSheet())
        {
            rotateU();
            rotateR();
            rotateUA();
            rotateB();
            rotateU();
            ult = "9L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front2->styleSheet() == ui->right5->styleSheet() && ui->up8->styleSheet() == ui->up5->styleSheet())
        {
            rotateFA();
            rotateU();
            rotateF();
            rotateUA();
            ult = "10L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front2->styleSheet() == ui->up5->styleSheet() && ui->up8->styleSheet() == ui->right5->styleSheet())
        {
            rotateF();
            rotateR();
            ult = "11L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back2->styleSheet() == ui->right5->styleSheet() && ui->down8->styleSheet() == ui->up5->styleSheet())
        {
            rotateDA();
            rotateR();
            rotateR();
            ult = "12L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back2->styleSheet() == ui->up5->styleSheet() && ui->down8->styleSheet() == ui->right5->styleSheet())
        {
            rotateDA();
            rotateRA();
            rotateUA();
            rotateB();
            rotateU();
            ult = "13L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front8->styleSheet() == ui->right5->styleSheet() && ui->down2->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateR();
            rotateR();
            ult = "14L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front8->styleSheet() == ui->up5->styleSheet() && ui->down2->styleSheet() == ui->right5->styleSheet())
        {
            rotateD();
            rotateRA();
            rotateUA();
            rotateB();
            rotateU();
            ult = "15L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left2->styleSheet() == ui->right5->styleSheet() && ui->up4->styleSheet() == ui->up5->styleSheet())
        {
            rotateL();
            rotateL();
            rotateD();
            rotateD();
            rotateR();
            rotateR();
            ult = "16L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left2->styleSheet() == ui->up5->styleSheet() && ui->up4->styleSheet() == ui->right5->styleSheet())
        {
            rotateU();
            rotateU();
            rotateR();
            rotateUA();
            rotateB();
            rotateU();
            ult = "17L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left4->styleSheet() == ui->right5->styleSheet() && ui->back4->styleSheet() == ui->up5->styleSheet())
        {
            rotateL();
            rotateU();
            rotateU();
            ult = "18L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left4->styleSheet() == ui->up5->styleSheet() && ui->back4->styleSheet() == ui->right5->styleSheet())
        {
            rotateL();
            rotateU();
            rotateU();
            rotateR();
            rotateUA();
            rotateB();
            rotateU();
            ult = "19L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left6->styleSheet() == ui->right5->styleSheet() && ui->front4->styleSheet() == ui->up5->styleSheet())
        {
            
            rotateU();
            rotateU();
            rotateLA();
            rotateUA();
            rotateUA();
            ult = "20L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left6->styleSheet() == ui->up5->styleSheet() && ui->front4->styleSheet() == ui->right5->styleSheet())
        {
            rotateLA();
            rotateU();
            rotateU();
            rotateR();
            rotateUA();
            rotateB();
            rotateU();
            ult = "21L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left8->styleSheet() == ui->right5->styleSheet() && ui->down4->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateD();
            rotateR();
            rotateR();
            ult = "22L";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left8->styleSheet() == ui->up5->styleSheet() && ui->down4->styleSheet() == ui->right5->styleSheet())
        {
            rotateD();
            rotateD();
            rotateRA();
            rotateUA();
            rotateB();
            rotateU();
            ult = "23L";
            ui->ultimo->setPlainText(ult);
        }
    }
}
void MainWindow::CruzOeste()
{
    if(ui->up4->styleSheet() != ui->up5->styleSheet() || ui->left2->styleSheet() != ui->right5->styleSheet())
    {
        if(ui->left2->styleSheet() == ui->up5->styleSheet() && ui->up4->styleSheet() == ui->left5->styleSheet())
        {
            rotateL();
            rotateUA();
            rotateF();
            rotateU();
            ult = "1O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left6->styleSheet() == ui->up5->styleSheet() && ui->front4->styleSheet() == ui->left5->styleSheet())
        {
            rotateUA();
            rotateF();
            rotateU();
            ult = "2O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left6->styleSheet() == ui->left5->styleSheet() && ui->front4->styleSheet() == ui->up5->styleSheet())
        {
            rotateLA();
            ult = "3O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left4->styleSheet() == ui->up5->styleSheet() && ui->back4->styleSheet() == ui->left5->styleSheet())
        {
            rotateU();
            rotateBA();
            rotateUA();
            ult = "4O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left4->styleSheet() == ui->left5->styleSheet() && ui->back4->styleSheet() == ui->up5->styleSheet())
        {
            rotateL();
            ult = "5O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left8->styleSheet() == ui->left5->styleSheet() && ui->down4->styleSheet() == ui->up5->styleSheet())
        {
            rotateL();
            rotateL();
            ult = "6O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left8->styleSheet() == ui->up5->styleSheet() && ui->down4->styleSheet() == ui->left5->styleSheet())
        {
            rotateLA();
            rotateUA();
            rotateF();
            rotateU();
            ult = "7O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front2->styleSheet() == ui->left5->styleSheet() && ui->up8->styleSheet() == ui->up5->styleSheet())
        {
            rotateU();
            ult = "8O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front2->styleSheet() == ui->up5->styleSheet() && ui->up8->styleSheet() == ui->left5->styleSheet())
        {
            rotateU();
            rotateL();
            rotateUA();
            rotateF();
            rotateU();
            ult = "9O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back8->styleSheet() == ui->left5->styleSheet() && ui->up2->styleSheet() == ui->up5->styleSheet())
        {
            rotateUA();
            ult = "10O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back8->styleSheet() == ui->up5->styleSheet() && ui->up2->styleSheet() == ui->left5->styleSheet())
        {
            rotateUA();
            rotateL();
            rotateUA();
            rotateF();
            rotateU();
            ult = "11O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front8->styleSheet() == ui->left5->styleSheet() && ui->down2->styleSheet() == ui->up5->styleSheet())
        {
            rotateDA();
            rotateL();
            rotateL();
            ult = "12O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front8->styleSheet() == ui->up5->styleSheet() && ui->down2->styleSheet() == ui->left5->styleSheet())
        {
            rotateDA();
            rotateLA();
            rotateUA();
            rotateF();
            rotateU();
            ult = "13O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back2->styleSheet() == ui->left5->styleSheet() && ui->down8->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateL();
            rotateL();
            ult = "14O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back2->styleSheet() == ui->up5->styleSheet() && ui->down8->styleSheet() == ui->left5->styleSheet())
        {
            rotateD();
            rotateLA();
            rotateUA();
            rotateF();
            rotateU();
            ult = "15O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right2->styleSheet() == ui->left5->styleSheet() && ui->up6->styleSheet() == ui->up5->styleSheet())
        {
            rotateU();
            rotateU();
            ult = "16O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right2->styleSheet() == ui->up5->styleSheet() && ui->up6->styleSheet() == ui->left5->styleSheet())
        {
            rotateU();
            rotateU();
            rotateL();
            rotateUA();
            rotateF();
            rotateU();
            ult = "17O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right4->styleSheet() == ui->left5->styleSheet() && ui->front6->styleSheet() == ui->up5->styleSheet())
        {
            rotateU();
            rotateU();
            rotateR();
            rotateUA();
            rotateUA();
            ult = "18O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right4->styleSheet() == ui->up5->styleSheet() && ui->front6->styleSheet() == ui->left5->styleSheet())
        {
            rotateR();
            rotateU();
            rotateU();
            rotateL();
            rotateUA();
            rotateF();
            rotateU();
            ult = "19O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right6->styleSheet() == ui->left5->styleSheet() && ui->back6->styleSheet() == ui->up5->styleSheet())
        {
            rotateR();
            rotateD();
            rotateD();
            rotateL();
            rotateL();
            ult = "20O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right6->styleSheet() == ui->up5->styleSheet() && ui->back6->styleSheet() == ui->left5->styleSheet())
        {
            rotateRA();
            rotateU();
            rotateU();
            rotateL();
            rotateUA();
            rotateF();
            rotateU();
            ult = "21O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right8->styleSheet() == ui->left5->styleSheet() && ui->down6->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateD();
            rotateL();
            rotateL();
            ult = "22O";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right8->styleSheet() == ui->up5->styleSheet() && ui->down6->styleSheet() == ui->left5->styleSheet())
        {
            rotateD();
            rotateD();
            rotateLA();
            rotateUA();
            rotateF();
            rotateU();
            ult = "23O";
            ui->ultimo->setPlainText(ult);
        }
    }
}
void MainWindow::CruzNorte()
{
    if(ui->up2->styleSheet() != ui->up5->styleSheet() || ui->back8->styleSheet() != ui->back5->styleSheet())
    {
        if(ui->up2->styleSheet() == ui->back5->styleSheet() && ui->back8->styleSheet() == ui->up5->styleSheet())
        {
            rotateB();
            rotateUA();
            rotateL();
            rotateU();
            ult = "1N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back4->styleSheet() == ui->up5->styleSheet() && ui->left4->styleSheet() == ui->back5->styleSheet())
        {
            rotateUA();
            rotateL();
            rotateU();
            ult = "2N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back4->styleSheet() == ui->back5->styleSheet() && ui->left4->styleSheet() == ui->up5->styleSheet())
        {
            rotateBA();
            ult = "3N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back6->styleSheet() == ui->up5->styleSheet() && ui->right6->styleSheet() == ui->back5->styleSheet())
        {
            rotateU();
            rotateRA();
            rotateUA();
            ult = "4N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back6->styleSheet() == ui->back5->styleSheet() && ui->right6->styleSheet() == ui->up5->styleSheet())
        {
            rotateB();
            ult = "5N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back2->styleSheet() == ui->back5->styleSheet() && ui->down8->styleSheet() == ui->up5->styleSheet())
        {
            rotateB();
            rotateB();
            ult = "6N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back2->styleSheet() == ui->up5->styleSheet() && ui->down8->styleSheet() == ui->back5->styleSheet())
        {
            rotateBA();
            rotateUA();
            rotateL();
            rotateU();
            ult = "7N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left2->styleSheet() == ui->back5->styleSheet() && ui->up4->styleSheet() == ui->up5->styleSheet())
        {
            rotateLA();
            rotateUA();
            rotateL();
            rotateU();
            ult = "8N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left2->styleSheet() == ui->up5->styleSheet() && ui->up4->styleSheet() == ui->back5->styleSheet())
        {
            rotateU();
            rotateB();
            rotateUA();
            rotateL();
            rotateU();
            ult = "9N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right2->styleSheet() == ui->back5->styleSheet() && ui->up6->styleSheet() == ui->up5->styleSheet())
        {
            rotateR();
            rotateU();
            rotateRA();
            rotateUA();
            ult = "10N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right2->styleSheet() == ui->up5->styleSheet() && ui->up6->styleSheet() == ui->back5->styleSheet())
        {
            rotateUA();
            rotateB();
            rotateUA();
            rotateL();
            rotateU();
            ult = "11N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left8->styleSheet() == ui->back5->styleSheet() && ui->down4->styleSheet() == ui->up5->styleSheet())
        {
            rotateDA();
            rotateB();
            rotateB();
            ult = "12N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left8->styleSheet() == ui->up5->styleSheet() && ui->down4->styleSheet() == ui->back5->styleSheet())
        {
            rotateDA();
            rotateBA();
            rotateUA();
            rotateL();
            rotateU();
            ult = "13N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right8->styleSheet() == ui->back5->styleSheet() && ui->down6->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateB();
            rotateB();
            ult = "14N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right8->styleSheet() == ui->up5->styleSheet() && ui->down6->styleSheet() == ui->back5->styleSheet())
        {
            rotateD();
            rotateBA();
            rotateUA();
            rotateL();
            rotateU();
            ult = "15N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front2->styleSheet() == ui->back5->styleSheet() && ui->up8->styleSheet() == ui->up5->styleSheet())
        {
            rotateU();
            rotateU();
            ult = "16N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front2->styleSheet() == ui->up5->styleSheet() && ui->up8->styleSheet() == ui->back5->styleSheet())
        {
            rotateU();
            rotateU();
            rotateB();
            rotateUA();
            rotateL();
            rotateU();
            ult = "17N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front4->styleSheet() == ui->back5->styleSheet() && ui->left6->styleSheet() == ui->up5->styleSheet())
        {
            rotateF();
            rotateU();
            rotateU();
            ult = "18N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front4->styleSheet() == ui->up5->styleSheet() && ui->left6->styleSheet() == ui->back5->styleSheet())
        {
            rotateF();
            rotateU();
            rotateU();
            rotateB();
            rotateUA();
            rotateL();
            rotateU();
            ult = "19N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front6->styleSheet() == ui->back5->styleSheet() && ui->right4->styleSheet() == ui->up5->styleSheet())
        {
            rotateFA();
            rotateU();
            rotateU();
            ult = "20N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front6->styleSheet() == ui->up5->styleSheet() && ui->right4->styleSheet() == ui->back5->styleSheet())
        {
            rotateFA();
            rotateU();
            rotateU();
            rotateB();
            rotateUA();
            rotateL();
            rotateU();
            ult = "21N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front8->styleSheet() == ui->back5->styleSheet() && ui->down2->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateD();
            rotateF();
            rotateF();
            rotateBA();
            rotateBA();
            ult = "22N";
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front8->styleSheet() == ui->up5->styleSheet() && ui->down2->styleSheet() == ui->back5->styleSheet())
        {
            rotateD();
            rotateD();
            rotateBA();
            rotateUA();
            rotateL();
            rotateU();
            ult = "23N";
            ui->ultimo->setPlainText(ult);
        }
    }
}
void MainWindow::on_Cross_clicked()
{
    while(ui->up2->styleSheet() != ui->up5->styleSheet() || ui->back8->styleSheet() != ui->back5->styleSheet() ||
          ui->up4->styleSheet() != ui->up5->styleSheet() || ui->left2->styleSheet() != ui->left5->styleSheet() ||
          ui->up6->styleSheet() != ui->up5->styleSheet() || ui->right2->styleSheet() != ui->right5->styleSheet() ||
          ui->up8->styleSheet() != ui->up5->styleSheet() || ui->front2->styleSheet() != ui->front5->styleSheet() )
    {
        CruzSul();
        CruzNorte();
        
        
        CruzLeste();
        CruzOeste();
     }
    
    
    
    
    
    
}
void MainWindow::on_zerar_clicked()
{
    completeCube();
    movimentos = 0;
    ui->qtdMovimentos->setPlainText(QString::number(movimentos));
    ui->ultimo->setPlainText(QString::number(movimentos));
}
void MainWindow::on_CrossSul_clicked()
{
    CruzSul();
}
void MainWindow::on_CrossNorte_clicked()
{
    CruzNorte();
}
void MainWindow::on_CruzLeste_clicked()
{
    CruzLeste();
}
void MainWindow::on_CruzOeste_clicked()
{
    CruzOeste();
}
void MainWindow::on_QuinaSudeste_clicked()
{
    if(ui->up9->styleSheet() != ui->up5->styleSheet() || ui->front3->styleSheet() != ui->front5->styleSheet() || ui->right1->styleSheet() != ui->right5->styleSheet())
    {
        if(ui->up9->styleSheet() == ui->up5->styleSheet())
        {
            rotateRA();
            rotateDA();
            rotateR();
            if(ui->front7->styleSheet() == ui->front5->styleSheet()) 
            {
                rotateDA();
                rotateFA();
                rotateD();
                rotateF();
            }
            else if(ui->front7->styleSheet() == ui->left5->styleSheet()) 
            {
                rotateDA();
                rotateDA();
                rotateLA();
                rotateD();
                rotateL();
            }
            else if(ui->front7->styleSheet() == ui->back5->styleSheet()) 
            {
                rotateD();
                rotateBA();
                rotateD();
                rotateB();
            }
        }
        else if(ui->front3->styleSheet() == ui->up5->styleSheet())
        {
            rotateF();
            rotateD();
            rotateFA();
            if(ui->back3->styleSheet() == ui->front5->styleSheet())
            {
                rotateD();
                rotateFA();
                rotateD();
                rotateF();
            }
            else if(ui->back3->styleSheet() == ui->left5->styleSheet())
            {
                rotateLA();
                rotateD();
                rotateL();
            }
            else if(ui->back3->styleSheet() == ui->right5->styleSheet())
            {
                rotateDA();
                rotateDA();
                rotateRA();
                rotateD();
                rotateR();
            }
            else if(ui->back3->styleSheet() == ui->back5->styleSheet())  
            {
                rotateDA();
                rotateBA();
                rotateD();
                rotateB();
            }
        }
        else if(ui->right1->styleSheet() == ui->up5->styleSheet())
        {
            rotateF();
            rotateDA();
            rotateFA();
            if(ui->front9->styleSheet() == ui->front5->styleSheet()) 
            {
                rotateD();
                rotateF();
                rotateDA();
                rotateFA();
            }
            else if(ui->front9->styleSheet() == ui->left5->styleSheet()) 
            {
                rotateL();
                rotateDA();
                rotateLA();
            }
            else if(ui->front9->styleSheet() == ui->right5->styleSheet())
            {
                rotateD();
                rotateD();
                rotateR();
                rotateDA();
                rotateRA();
            }
            else if(ui->front9->styleSheet() == ui->back5->styleSheet())
            {
                rotateDA();
                rotateB();
                rotateDA();
                rotateBA();
            }
        }
    }
}
void MainWindow::on_up9_clicked()
{
    if(ui->up9->styleSheet() == ui->up5->styleSheet())
        ui->up9->setStyleSheet("background-color: white");
    else if(ui->up9->styleSheet() == ui->front5->styleSheet())
        ui->up9->setStyleSheet("background-color: orange");
    else if(ui->up9->styleSheet() == ui->left5->styleSheet())
        ui->up9->setStyleSheet("background-color: red");
    else if(ui->up9->styleSheet() == ui->right5->styleSheet())
        ui->up9->setStyleSheet("background-color: green");
    else if(ui->up9->styleSheet() == ui->down5->styleSheet())
        ui->up9->setStyleSheet("background-color: yellow");
    else if(ui->up9->styleSheet() == ui->back5->styleSheet())
        ui->up9->setStyleSheet("background-color: blue");
}
void MainWindow::on_front3_clicked()
{
    if(ui->front3->styleSheet() == ui->up5->styleSheet())
        ui->front3->setStyleSheet("background-color: white");
    else if(ui->front3->styleSheet() == ui->front5->styleSheet())
        ui->front3->setStyleSheet("background-color: orange");
    else if(ui->front3->styleSheet() == ui->left5->styleSheet())
        ui->front3->setStyleSheet("background-color: red");
    else if(ui->front3->styleSheet() == ui->right5->styleSheet())
        ui->front3->setStyleSheet("background-color: green");
    else if(ui->front3->styleSheet() == ui->down5->styleSheet())
        ui->front3->setStyleSheet("background-color: yellow");
    else if(ui->front3->styleSheet() == ui->back5->styleSheet())
        ui->front3->setStyleSheet("background-color: blue");
}
void MainWindow::on_right1_clicked()
{
    if(ui->right1->styleSheet() == ui->up5->styleSheet())
        ui->right1->setStyleSheet("background-color: white");
    else if(ui->right1->styleSheet() == ui->front5->styleSheet())
        ui->right1->setStyleSheet("background-color: orange");
    else if(ui->right1->styleSheet() == ui->left5->styleSheet())
        ui->right1->setStyleSheet("background-color: red");
    else if(ui->right1->styleSheet() == ui->right5->styleSheet())
        ui->right1->setStyleSheet("background-color: green");
    else if(ui->right1->styleSheet() == ui->down5->styleSheet())
        ui->right1->setStyleSheet("background-color: yellow");
    else if(ui->right1->styleSheet() == ui->back5->styleSheet())
        ui->right1->setStyleSheet("background-color: blue");
}
void MainWindow::on_QuinaSudoeste_clicked()
{
    if(ui->up7->styleSheet() != ui->up5->styleSheet() || ui->front1->styleSheet() != ui->front5->styleSheet() || ui->left3->styleSheet() != ui->left5->styleSheet())
    {
        if(ui->up7->styleSheet() == ui->front5->styleSheet() && ui->front1->styleSheet() == ui->left5->styleSheet() && ui->left3->styleSheet() == ui->up5->styleSheet())
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
        else if(ui->up7->styleSheet() == ui->left5->styleSheet() && ui->front1->styleSheet() == ui->up5->styleSheet() && ui->left3->styleSheet() == ui->front5->styleSheet())
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
        else if(ui->up7->styleSheet() == ui->up5->styleSheet() && ui->front1->styleSheet() == ui->right5->styleSheet() && ui->left3->styleSheet() == ui->front5->styleSheet())
        {
            rotateL();
            rotateD();
            rotateLA();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->up7->styleSheet() == ui->right5->styleSheet() && ui->front1->styleSheet() == ui->front5->styleSheet() && ui->left3->styleSheet() == ui->up5->styleSheet())
        {
            rotateL();
            rotateD();
            rotateLA();
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->up7->styleSheet() == ui->front5->styleSheet() && ui->front1->styleSheet() == ui->up5->styleSheet() && ui->left3->styleSheet() == ui->right5->styleSheet())
        {
            rotateFA();
            rotateDA();
            rotateF();
            rotateDA();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->up7->styleSheet() == ui->up5->styleSheet() && ui->front1->styleSheet() == ui->back5->styleSheet() && ui->left3->styleSheet() == ui->right5->styleSheet())
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
        else if(ui->up7->styleSheet() == ui->right5->styleSheet() && ui->front1->styleSheet() == ui->up5->styleSheet() && ui->left3->styleSheet() == ui->back5->styleSheet())
        {
            rotateFA();
            rotateDA();
            rotateF();
            rotateR();
            rotateDA();
            rotateRA();
        }
        else if(ui->up7->styleSheet() == ui->back5->styleSheet() && ui->front1->styleSheet() == ui->right5->styleSheet() && ui->left3->styleSheet() == ui->up5->styleSheet())
        {
            rotateL();
            rotateD();
            rotateLA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->up7->styleSheet() == ui->up5->styleSheet() && ui->front1->styleSheet() == ui->left5->styleSheet() && ui->left3->styleSheet() == ui->back5->styleSheet())
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
        else if(ui->up7->styleSheet() == ui->left5->styleSheet() && ui->front1->styleSheet() == ui->back5->styleSheet() && ui->left3->styleSheet() == ui->up5->styleSheet())
        {
            rotateL();
            rotateD();
            rotateLA();
            rotateD();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->up7->styleSheet() == ui->back5->styleSheet() && ui->front1->styleSheet() == ui->up5->styleSheet() && ui->left3->styleSheet() == ui->left5->styleSheet())
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
void MainWindow::on_left3_clicked()
{
    if(ui->left3->styleSheet() == ui->up5->styleSheet())
        ui->left3->setStyleSheet("background-color: white");
    else if(ui->left3->styleSheet() == ui->front5->styleSheet())
        ui->left3->setStyleSheet("background-color: orange");
    else if(ui->left3->styleSheet() == ui->left5->styleSheet())
        ui->left3->setStyleSheet("background-color: red");
    else if(ui->left3->styleSheet() == ui->right5->styleSheet())
        ui->left3->setStyleSheet("background-color: green");
    else if(ui->left3->styleSheet() == ui->down5->styleSheet())
        ui->left3->setStyleSheet("background-color: yellow");
    else if(ui->left3->styleSheet() == ui->back5->styleSheet())
        ui->left3->setStyleSheet("background-color: blue");
}
void MainWindow::on_front1_clicked()
{
    if(ui->front1->styleSheet() == ui->up5->styleSheet())
        ui->front1->setStyleSheet("background-color: white");
    else if(ui->front1->styleSheet() == ui->front5->styleSheet())
        ui->front1->setStyleSheet("background-color: orange");
    else if(ui->front1->styleSheet() == ui->left5->styleSheet())
        ui->front1->setStyleSheet("background-color: red");
    else if(ui->front1->styleSheet() == ui->right5->styleSheet())
        ui->front1->setStyleSheet("background-color: green");
    else if(ui->front1->styleSheet() == ui->down5->styleSheet())
        ui->front1->setStyleSheet("background-color: yellow");
    else if(ui->front1->styleSheet() == ui->back5->styleSheet())
        ui->front1->setStyleSheet("background-color: blue");
}
void MainWindow::on_up7_clicked()
{
    if(ui->up7->styleSheet() == ui->up5->styleSheet())
        ui->up7->setStyleSheet("background-color: white");
    else if(ui->up7->styleSheet() == ui->front5->styleSheet())
        ui->up7->setStyleSheet("background-color: orange");
    else if(ui->up7->styleSheet() == ui->left5->styleSheet())
        ui->up7->setStyleSheet("background-color: red");
    else if(ui->up7->styleSheet() == ui->right5->styleSheet())
        ui->up7->setStyleSheet("background-color: green");
    else if(ui->up7->styleSheet() == ui->down5->styleSheet())
        ui->up7->setStyleSheet("background-color: yellow");
    else if(ui->up7->styleSheet() == ui->back5->styleSheet())
        ui->up7->setStyleSheet("background-color: blue");
}
void MainWindow::on_QuinaNordeste_clicked()
{
    if(ui->up3->styleSheet() != ui->up5->styleSheet() || ui->right3->styleSheet() != ui->right5->styleSheet() || ui->back9->styleSheet() != ui->back5->styleSheet())
    {
        if(ui->up3->styleSheet() == ui->right5->styleSheet() && ui->right3->styleSheet() == ui->back5->styleSheet() && ui->back9->styleSheet() == ui->up5->styleSheet())
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
        else if(ui->up3->styleSheet() == ui->back5->styleSheet() && ui->right3->styleSheet() == ui->up5->styleSheet() && ui->back9->styleSheet() == ui->right5->styleSheet())
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
        else if(ui->up3->styleSheet() == ui->up5->styleSheet() && ui->right3->styleSheet() == ui->front5->styleSheet() && ui->back9->styleSheet() == ui->right5->styleSheet())
        {
             rotateBA();
             rotateDA();
             rotateB();
             rotateDA();
             rotateRA();
             rotateD();
             rotateR();
        }
        else if(ui->up3->styleSheet() == ui->right5->styleSheet() && ui->right3->styleSheet() == ui->up5->styleSheet() && ui->back9->styleSheet() == ui->front5->styleSheet())
        {
            rotateR();
            rotateD();
            rotateRA();
            rotateD();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->up3->styleSheet() == ui->front5->styleSheet() && ui->right3->styleSheet() == ui->right5->styleSheet() && ui->back9->styleSheet() == ui->up5->styleSheet())
        {
            rotateBA();
            rotateDA();
            rotateB();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->up3->styleSheet() == ui->up5->styleSheet() && ui->right3->styleSheet() == ui->left5->styleSheet() && ui->back9->styleSheet() == ui->front5->styleSheet())
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
        else if(ui->up3->styleSheet() == ui->left5->styleSheet() && ui->right3->styleSheet() == ui->front5->styleSheet() && ui->back9->styleSheet() == ui->up5->styleSheet())
        {
            rotateBA();
            rotateDA();
            rotateB();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->up3->styleSheet() == ui->front5->styleSheet() && ui->right3->styleSheet() == ui->up5->styleSheet() && ui->back9->styleSheet() == ui->left5->styleSheet())
        {
            rotateR();
            rotateD();
            rotateRA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->up3->styleSheet() == ui->up5->styleSheet() && ui->right3->styleSheet() == ui->back5->styleSheet() && ui->back9->styleSheet() == ui->left5->styleSheet())
        {
            rotateR();
            rotateD();
            rotateRA();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->up3->styleSheet() == ui->left5->styleSheet() && ui->right3->styleSheet() == ui->up5->styleSheet() && ui->back9->styleSheet() == ui->back5->styleSheet())
        {
            rotateR();
            rotateLA();
            rotateD();
            rotateL();
            rotateRA();
        }
        else if(ui->up3->styleSheet() == ui->back5->styleSheet() && ui->right3->styleSheet() == ui->left5->styleSheet() && ui->back9->styleSheet() == ui->up5->styleSheet())
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
void MainWindow::on_up3_clicked()
{
    if(ui->up3->styleSheet() == ui->up5->styleSheet())
        ui->up3->setStyleSheet("background-color: white");
    else if(ui->up3->styleSheet() == ui->front5->styleSheet())
        ui->up3->setStyleSheet("background-color: orange");
    else if(ui->up3->styleSheet() == ui->left5->styleSheet())
        ui->up3->setStyleSheet("background-color: red");
    else if(ui->up3->styleSheet() == ui->right5->styleSheet())
        ui->up3->setStyleSheet("background-color: green");
    else if(ui->up3->styleSheet() == ui->down5->styleSheet())
        ui->up3->setStyleSheet("background-color: yellow");
    else if(ui->up3->styleSheet() == ui->back5->styleSheet())
        ui->up3->setStyleSheet("background-color: blue");
}
void MainWindow::on_right3_clicked()
{
    if(ui->right3->styleSheet() == ui->up5->styleSheet())
        ui->right3->setStyleSheet("background-color: white");
    else if(ui->right3->styleSheet() == ui->front5->styleSheet())
        ui->right3->setStyleSheet("background-color: orange");
    else if(ui->right3->styleSheet() == ui->left5->styleSheet())
        ui->right3->setStyleSheet("background-color: red");
    else if(ui->right3->styleSheet() == ui->right5->styleSheet())
        ui->right3->setStyleSheet("background-color: green");
    else if(ui->right3->styleSheet() == ui->down5->styleSheet())
        ui->right3->setStyleSheet("background-color: yellow");
    else if(ui->right3->styleSheet() == ui->back5->styleSheet())
        ui->right3->setStyleSheet("background-color: blue");
}
void MainWindow::on_back9_clicked()
{
    if(ui->back9->styleSheet() == ui->up5->styleSheet())
        ui->back9->setStyleSheet("background-color: white");
    else if(ui->back9->styleSheet() == ui->front5->styleSheet())
        ui->back9->setStyleSheet("background-color: orange");
    else if(ui->back9->styleSheet() == ui->left5->styleSheet())
        ui->back9->setStyleSheet("background-color: red");
    else if(ui->back9->styleSheet() == ui->right5->styleSheet())
        ui->back9->setStyleSheet("background-color: green");
    else if(ui->back9->styleSheet() == ui->down5->styleSheet())
        ui->back9->setStyleSheet("background-color: yellow");
    else if(ui->back9->styleSheet() == ui->back5->styleSheet())
        ui->back9->setStyleSheet("background-color: blue");
}
void MainWindow::on_up1_clicked()
{
    if(ui->up1->styleSheet() == ui->up5->styleSheet())
        ui->up1->setStyleSheet("background-color: white");
    else if(ui->up1->styleSheet() == ui->front5->styleSheet())
        ui->up1->setStyleSheet("background-color: orange");
    else if(ui->up1->styleSheet() == ui->left5->styleSheet())
        ui->up1->setStyleSheet("background-color: red");
    else if(ui->up1->styleSheet() == ui->right5->styleSheet())
        ui->up1->setStyleSheet("background-color: green");
    else if(ui->up1->styleSheet() == ui->down5->styleSheet())
        ui->up1->setStyleSheet("background-color: yellow");
    else if(ui->up1->styleSheet() == ui->back5->styleSheet())
        ui->up1->setStyleSheet("background-color: blue");
}
void MainWindow::on_left1_clicked()
{
    if(ui->left1->styleSheet() == ui->up5->styleSheet())
        ui->left1->setStyleSheet("background-color: white");
    else if(ui->left1->styleSheet() == ui->front5->styleSheet())
        ui->left1->setStyleSheet("background-color: orange");
    else if(ui->left1->styleSheet() == ui->left5->styleSheet())
        ui->left1->setStyleSheet("background-color: red");
    else if(ui->left1->styleSheet() == ui->right5->styleSheet())
        ui->left1->setStyleSheet("background-color: green");
    else if(ui->left1->styleSheet() == ui->down5->styleSheet())
        ui->left1->setStyleSheet("background-color: yellow");
    else if(ui->left1->styleSheet() == ui->back5->styleSheet())
        ui->left1->setStyleSheet("background-color: blue");
}
void MainWindow::on_back7_clicked()
{
    if(ui->back7->styleSheet() == ui->up5->styleSheet())
        ui->back7->setStyleSheet("background-color: white");
    else if(ui->back7->styleSheet() == ui->front5->styleSheet())
        ui->back7->setStyleSheet("background-color: orange");
    else if(ui->back7->styleSheet() == ui->left5->styleSheet())
        ui->back7->setStyleSheet("background-color: red");
    else if(ui->back7->styleSheet() == ui->right5->styleSheet())
        ui->back7->setStyleSheet("background-color: green");
    else if(ui->back7->styleSheet() == ui->down5->styleSheet())
        ui->back7->setStyleSheet("background-color: yellow");
    else if(ui->back7->styleSheet() == ui->back5->styleSheet())
        ui->back7->setStyleSheet("background-color: blue");
}
void MainWindow::on_QuinaNoroeste_clicked()
{
    if(ui->up1->styleSheet() != ui->up5->styleSheet() || ui->left1->styleSheet() != ui->left5->styleSheet() || ui->back7->styleSheet() != ui->back5->styleSheet())
    {
        if(ui->up1->styleSheet() == ui->left5->styleSheet() && ui->left1->styleSheet() == ui->back5->styleSheet() && ui->back7->styleSheet() == ui->up5->styleSheet())
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
        else if(ui->up1->styleSheet() == ui->back5->styleSheet() && ui->left1->styleSheet() == ui->up5->styleSheet() && ui->back7->styleSheet() == ui->left5->styleSheet())
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
        else if(ui->up1->styleSheet() == ui->up5->styleSheet() && ui->left1->styleSheet() == ui->right5->styleSheet() && ui->back7->styleSheet() == ui->front5->styleSheet())
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
        else if(ui->up1->styleSheet() == ui->right5->styleSheet() && ui->left1->styleSheet() == ui->front5->styleSheet() && ui->back7->styleSheet() == ui->up5->styleSheet())
        {
            rotateB();
            rotateD();
            rotateBA();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->up1->styleSheet() == ui->front5->styleSheet() && ui->left1->styleSheet() == ui->up5->styleSheet() && ui->back7->styleSheet() == ui->right5->styleSheet())
        {
            rotateLA();
            rotateDA();
            rotateL();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->up1->styleSheet() == ui->up5->styleSheet() && ui->left1->styleSheet() == ui->front5->styleSheet() && ui->back7->styleSheet() == ui->left5->styleSheet())
        {
            rotateB();
            rotateD();
            rotateBA();
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->up1->styleSheet() == ui->front5->styleSheet() && ui->left1->styleSheet() == ui->left5->styleSheet() && ui->back7->styleSheet() == ui->up5->styleSheet())
        {
            rotateB();
            rotateD();
            rotateBA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->up1->styleSheet() == ui->left5->styleSheet() && ui->left1->styleSheet() == ui->up5->styleSheet() && ui->back7->styleSheet() == ui->front5->styleSheet())
        {
            rotateLA();
            rotateD();
            rotateD();
            rotateL();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->up1->styleSheet() == ui->up5->styleSheet() && ui->left1->styleSheet() == ui->back5->styleSheet() && ui->back7->styleSheet() == ui->right5->styleSheet())
        {
            rotateLA();
            rotateDA();
            rotateL();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->up1->styleSheet() == ui->back5->styleSheet() && ui->left1->styleSheet() == ui->right5->styleSheet() && ui->back7->styleSheet() == ui->up5->styleSheet())
        {
            rotateB();
            rotateD();
            rotateBA();
            rotateD();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->up1->styleSheet() == ui->right5->styleSheet() && ui->left1->styleSheet() == ui->up5->styleSheet() && ui->back7->styleSheet() == ui->back5->styleSheet())
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
void MainWindow::on_SudesteBaixo_clicked()
{
    if(ui->front9->styleSheet() == ui->up5->styleSheet() || ui->down3->styleSheet() == ui->up5->styleSheet() || ui->right7->styleSheet() == ui->up5->styleSheet())
    {
        if(ui->front9->styleSheet() == ui->front5->styleSheet() && ui->down3->styleSheet() == ui->right5->styleSheet() && ui->right7->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->front9->styleSheet() == ui->up5->styleSheet() && ui->down3->styleSheet() == ui->front5->styleSheet() && ui->right7->styleSheet() == ui->right5->styleSheet())
        {
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->front9->styleSheet() == ui->right5->styleSheet() && ui->down3->styleSheet() == ui->up5->styleSheet() && ui->right7->styleSheet() == ui->front5->styleSheet())
        {
            rotateF();
            rotateDA();
            rotateFA();
            rotateD();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->front9->styleSheet() == ui->front5->styleSheet() && ui->down3->styleSheet() == ui->up5->styleSheet() && ui->right7->styleSheet() == ui->left5->styleSheet())
        {
            rotateF();
            rotateDA();
            rotateFA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->front9->styleSheet() == ui->up5->styleSheet() && ui->down3->styleSheet() == ui->left5->styleSheet() && ui->right7->styleSheet() == ui->front5->styleSheet())
        {
            rotateDA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->front9->styleSheet() == ui->left5->styleSheet() && ui->down3->styleSheet() == ui->front5->styleSheet() && ui->right7->styleSheet() == ui->up5->styleSheet())
        {
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->front9->styleSheet() == ui->left5->styleSheet() && ui->down3->styleSheet() == ui->up5->styleSheet() && ui->right7->styleSheet() == ui->back5->styleSheet())
        {
            rotateF();
            rotateDA();
            rotateFA();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->front9->styleSheet() == ui->up5->styleSheet() && ui->down3->styleSheet() == ui->back5->styleSheet() && ui->right7->styleSheet() == ui->left5->styleSheet())
        {
            rotateD();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->front9->styleSheet() == ui->back5->styleSheet() && ui->down3->styleSheet() == ui->left5->styleSheet() && ui->right7->styleSheet() == ui->up5->styleSheet())
        {
            rotateDA();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->front9->styleSheet() == ui->back5->styleSheet() && ui->down3->styleSheet() == ui->up5->styleSheet() && ui->right7->styleSheet() == ui->right5->styleSheet())
        {
            rotateRA();
            rotateD();
            rotateR();
            rotateR();
            rotateDA();
            rotateRA();
        }
        else if(ui->front9->styleSheet() == ui->up5->styleSheet() && ui->down3->styleSheet() == ui->right5->styleSheet() && ui->right7->styleSheet() == ui->back5->styleSheet())
        {
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->front9->styleSheet() == ui->right5->styleSheet() && ui->down3->styleSheet() == ui->back5->styleSheet() && ui->right7->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
        }
    }
}
void MainWindow::on_front9_clicked()
{
    if(ui->front9->styleSheet() == ui->up5->styleSheet())
        ui->front9->setStyleSheet("background-color: white");
    else if(ui->front9->styleSheet() == ui->front5->styleSheet())
        ui->front9->setStyleSheet("background-color: orange");
    else if(ui->front9->styleSheet() == ui->left5->styleSheet())
        ui->front9->setStyleSheet("background-color: red");
    else if(ui->front9->styleSheet() == ui->right5->styleSheet())
        ui->front9->setStyleSheet("background-color: green");
    else if(ui->front9->styleSheet() == ui->down5->styleSheet())
        ui->front9->setStyleSheet("background-color: yellow");
    else if(ui->front9->styleSheet() == ui->back5->styleSheet())
        ui->front9->setStyleSheet("background-color: blue");
}
void MainWindow::on_right7_clicked()
{
    if(ui->right7->styleSheet() == ui->up5->styleSheet())
        ui->right7->setStyleSheet("background-color: white");
    else if(ui->right7->styleSheet() == ui->front5->styleSheet())
        ui->right7->setStyleSheet("background-color: orange");
    else if(ui->right7->styleSheet() == ui->left5->styleSheet())
        ui->right7->setStyleSheet("background-color: red");
    else if(ui->right7->styleSheet() == ui->right5->styleSheet())
        ui->right7->setStyleSheet("background-color: green");
    else if(ui->right7->styleSheet() == ui->down5->styleSheet())
        ui->right7->setStyleSheet("background-color: yellow");
    else if(ui->right7->styleSheet() == ui->back5->styleSheet())
        ui->right7->setStyleSheet("background-color: blue");
}
void MainWindow::on_down3_clicked()
{
    if(ui->down3->styleSheet() == ui->up5->styleSheet())
        ui->down3->setStyleSheet("background-color: white");
    else if(ui->down3->styleSheet() == ui->front5->styleSheet())
        ui->down3->setStyleSheet("background-color: orange");
    else if(ui->down3->styleSheet() == ui->left5->styleSheet())
        ui->down3->setStyleSheet("background-color: red");
    else if(ui->down3->styleSheet() == ui->right5->styleSheet())
        ui->down3->setStyleSheet("background-color: green");
    else if(ui->down3->styleSheet() == ui->down5->styleSheet())
        ui->down3->setStyleSheet("background-color: yellow");
    else if(ui->down3->styleSheet() == ui->back5->styleSheet())
        ui->down3->setStyleSheet("background-color: blue");
}
void MainWindow::on_SudoesteBaixo_clicked()
{
    if(ui->front7->styleSheet() == ui->up5->styleSheet() || ui->down1->styleSheet() == ui->up5->styleSheet() || ui->left9->styleSheet() == ui->up5->styleSheet())
    {
        if(ui->front7->styleSheet() == ui->front5->styleSheet() && ui->down1->styleSheet() == ui->up5->styleSheet() && ui->left9->styleSheet() == ui->right5->styleSheet())
        {
            rotateFA();
            rotateD();
            rotateF();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->front7->styleSheet() == ui->up5->styleSheet() && ui->down1->styleSheet() == ui->right5->styleSheet() && ui->left9->styleSheet() == ui->front5->styleSheet())
        {
            rotateD();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->front7->styleSheet() == ui->right5->styleSheet() && ui->down1->styleSheet() == ui->front5->styleSheet() && ui->left9->styleSheet() == ui->up5->styleSheet())
        {
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->front7->styleSheet() == ui->up5->styleSheet() && ui->down1->styleSheet() == ui->front5->styleSheet() && ui->left9->styleSheet() == ui->left5->styleSheet())
        {
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->front7->styleSheet() == ui->front5->styleSheet() && ui->down1->styleSheet() == ui->left5->styleSheet() && ui->left9->styleSheet() == ui->up5->styleSheet())
        {
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->front7->styleSheet() == ui->left5->styleSheet() && ui->down1->styleSheet() == ui->up5->styleSheet() && ui->left9->styleSheet() == ui->front5->styleSheet())
        {
            rotateFA();
            rotateD();
            rotateF();
            rotateDA();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->front7->styleSheet() == ui->back5->styleSheet() && ui->down1->styleSheet() == ui->up5->styleSheet() && ui->left9->styleSheet() == ui->left5->styleSheet())
        {
            rotateL();
            rotateDA();
            rotateLA();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->front7->styleSheet() == ui->up5->styleSheet() && ui->down1->styleSheet() == ui->left5->styleSheet() && ui->left9->styleSheet() == ui->back5->styleSheet())
        {
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->front7->styleSheet() == ui->left5->styleSheet() && ui->down1->styleSheet() == ui->back5->styleSheet() && ui->left9->styleSheet() == ui->up5->styleSheet())
        {
            rotateDA();
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->front7->styleSheet() == ui->back5->styleSheet() && ui->down1->styleSheet() == ui->right5->styleSheet() && ui->left9->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->front7->styleSheet() == ui->right5->styleSheet() && ui->down1->styleSheet() == ui->up5->styleSheet() && ui->left9->styleSheet() == ui->back5->styleSheet())
        {
            rotateFA();
            rotateD();
            rotateF();
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
        }
        else if(ui->front7->styleSheet() == ui->up5->styleSheet() && ui->down1->styleSheet() == ui->back5->styleSheet() && ui->left9->styleSheet() == ui->right5->styleSheet())
        {
            rotateDA();
            rotateR();
            rotateDA();
            rotateRA();
        }
    }
}
void MainWindow::on_front7_clicked()
{
    if(ui->front7->styleSheet() == ui->up5->styleSheet())
        ui->front7->setStyleSheet("background-color: white");
    else if(ui->front7->styleSheet() == ui->front5->styleSheet())
        ui->front7->setStyleSheet("background-color: orange");
    else if(ui->front7->styleSheet() == ui->left5->styleSheet())
        ui->front7->setStyleSheet("background-color: red");
    else if(ui->front7->styleSheet() == ui->right5->styleSheet())
        ui->front7->setStyleSheet("background-color: green");
    else if(ui->front7->styleSheet() == ui->down5->styleSheet())
        ui->front7->setStyleSheet("background-color: yellow");
    else if(ui->front7->styleSheet() == ui->back5->styleSheet())
        ui->front7->setStyleSheet("background-color: blue");
}
void MainWindow::on_left9_clicked()
{
    if(ui->left9->styleSheet() == ui->up5->styleSheet())
        ui->left9->setStyleSheet("background-color: white");
    else if(ui->left9->styleSheet() == ui->front5->styleSheet())
        ui->left9->setStyleSheet("background-color: orange");
    else if(ui->left9->styleSheet() == ui->left5->styleSheet())
        ui->left9->setStyleSheet("background-color: red");
    else if(ui->left9->styleSheet() == ui->right5->styleSheet())
        ui->left9->setStyleSheet("background-color: green");
    else if(ui->left9->styleSheet() == ui->down5->styleSheet())
        ui->left9->setStyleSheet("background-color: yellow");
    else if(ui->left9->styleSheet() == ui->back5->styleSheet())
        ui->left9->setStyleSheet("background-color: blue");
}
void MainWindow::on_down1_clicked()
{
    if(ui->down1->styleSheet() == ui->up5->styleSheet())
        ui->down1->setStyleSheet("background-color: white");
    else if(ui->down1->styleSheet() == ui->front5->styleSheet())
        ui->down1->setStyleSheet("background-color: orange");
    else if(ui->down1->styleSheet() == ui->left5->styleSheet())
        ui->down1->setStyleSheet("background-color: red");
    else if(ui->down1->styleSheet() == ui->right5->styleSheet())
        ui->down1->setStyleSheet("background-color: green");
    else if(ui->down1->styleSheet() == ui->down5->styleSheet())
        ui->down1->setStyleSheet("background-color: yellow");
    else if(ui->down1->styleSheet() == ui->back5->styleSheet())
        ui->down1->setStyleSheet("background-color: blue");
}
void MainWindow::on_NordesteBaixo_clicked()
{
    if(ui->back3->styleSheet() == ui->up5->styleSheet() || ui->down9->styleSheet() == ui->up5->styleSheet() || ui->right9->styleSheet() == ui->up5->styleSheet())
    {
       if(ui->back3->styleSheet() == ui->front5->styleSheet() && ui->down9->styleSheet() == ui->up5->styleSheet() && ui->right9->styleSheet() == ui->right5->styleSheet())
       {
            rotateR();
            rotateDA();
            rotateRA();
            rotateRA();
            rotateD();
            rotateR();
       }
       else if(ui->back3->styleSheet() == ui->up5->styleSheet() && ui->down9->styleSheet() == ui->right5->styleSheet() && ui->right9->styleSheet() == ui->front5->styleSheet())
       {
            rotateF();
            rotateDA();
            rotateFA();
       }
       else if(ui->back3->styleSheet() == ui->right5->styleSheet() && ui->down9->styleSheet() == ui->front5->styleSheet() && ui->right9->styleSheet() == ui->up5->styleSheet())
       {
            rotateDA();
            rotateDA();
            rotateRA();
            rotateD();
            rotateR();
       }
       else if(ui->back3->styleSheet() == ui->up5->styleSheet() && ui->down9->styleSheet() == ui->front5->styleSheet() && ui->right9->styleSheet() == ui->left5->styleSheet())
       {
            rotateDA();
            rotateL();
            rotateDA();
            rotateLA();
       }
       else if(ui->back3->styleSheet() == ui->front5->styleSheet() && ui->down9->styleSheet() == ui->left5->styleSheet() && ui->right9->styleSheet() == ui->up5->styleSheet())
       {
            rotateD();
            rotateFA();
            rotateD();
            rotateF();
       }
       else if(ui->back3->styleSheet() == ui->left5->styleSheet() && ui->down9->styleSheet() == ui->up5->styleSheet() && ui->right9->styleSheet() == ui->front5->styleSheet())
       {
            rotateR();
            rotateDA();
            rotateRA();
            rotateDA();
            rotateFA();
            rotateD();
            rotateF();
       }
       else if(ui->back3->styleSheet() == ui->back5->styleSheet() && ui->down9->styleSheet() == ui->up5->styleSheet() && ui->right9->styleSheet() == ui->left5->styleSheet())
       {
            rotateBA();
            rotateD();
            rotateB();
            rotateB();
            rotateDA();
            rotateBA();
       }
       else if(ui->back3->styleSheet() == ui->up5->styleSheet() && ui->down9->styleSheet() == ui->left5->styleSheet() && ui->right9->styleSheet() == ui->back5->styleSheet())
       {
            rotateD();
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
       }
       else if(ui->back3->styleSheet() == ui->left5->styleSheet() && ui->down9->styleSheet() == ui->back5->styleSheet() && ui->right9->styleSheet() == ui->up5->styleSheet())
       {
            rotateLA();
            rotateD();
            rotateL();
       }
       else if(ui->back3->styleSheet() == ui->back5->styleSheet() && ui->down9->styleSheet() == ui->right5->styleSheet() && ui->right9->styleSheet() == ui->up5->styleSheet())
       {
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
       }
       else if(ui->back3->styleSheet() == ui->right5->styleSheet() && ui->down9->styleSheet() == ui->up5->styleSheet() && ui->right9->styleSheet() == ui->back5->styleSheet())
       {
            rotateR();
            rotateDA();
            rotateRA();
            rotateD();
            rotateBA();
            rotateD();
            rotateB();
       }
       else if(ui->back3->styleSheet() == ui->up5->styleSheet() && ui->down9->styleSheet() == ui->back5->styleSheet() && ui->right9->styleSheet() == ui->right5->styleSheet())
       {
            rotateD();
            rotateR();
            rotateDA();
            rotateRA();
       }
    }
}
void MainWindow::on_down9_clicked()
{
    if(ui->down9->styleSheet() == ui->up5->styleSheet())
        ui->down9->setStyleSheet("background-color: white");
    else if(ui->down9->styleSheet() == ui->front5->styleSheet())
        ui->down9->setStyleSheet("background-color: orange");
    else if(ui->down9->styleSheet() == ui->left5->styleSheet())
        ui->down9->setStyleSheet("background-color: red");
    else if(ui->down9->styleSheet() == ui->right5->styleSheet())
        ui->down9->setStyleSheet("background-color: green");
    else if(ui->down9->styleSheet() == ui->down5->styleSheet())
        ui->down9->setStyleSheet("background-color: yellow");
    else if(ui->down9->styleSheet() == ui->back5->styleSheet())
        ui->down9->setStyleSheet("background-color: blue");
}
void MainWindow::on_right9_clicked()
{
    if(ui->right9->styleSheet() == ui->up5->styleSheet())
        ui->right9->setStyleSheet("background-color: white");
    else if(ui->right9->styleSheet() == ui->front5->styleSheet())
        ui->right9->setStyleSheet("background-color: orange");
    else if(ui->right9->styleSheet() == ui->left5->styleSheet())
        ui->right9->setStyleSheet("background-color: red");
    else if(ui->right9->styleSheet() == ui->right5->styleSheet())
        ui->right9->setStyleSheet("background-color: green");
    else if(ui->right9->styleSheet() == ui->down5->styleSheet())
        ui->right9->setStyleSheet("background-color: yellow");
    else if(ui->right9->styleSheet() == ui->back5->styleSheet())
        ui->right9->setStyleSheet("background-color: blue");
}
void MainWindow::on_back3_clicked()
{
    if(ui->back3->styleSheet() == ui->up5->styleSheet())
        ui->back3->setStyleSheet("background-color: white");
    else if(ui->back3->styleSheet() == ui->front5->styleSheet())
        ui->back3->setStyleSheet("background-color: orange");
    else if(ui->back3->styleSheet() == ui->left5->styleSheet())
        ui->back3->setStyleSheet("background-color: red");
    else if(ui->back3->styleSheet() == ui->right5->styleSheet())
        ui->back3->setStyleSheet("background-color: green");
    else if(ui->back3->styleSheet() == ui->down5->styleSheet())
        ui->back3->setStyleSheet("background-color: yellow");
    else if(ui->back3->styleSheet() == ui->back5->styleSheet())
        ui->back3->setStyleSheet("background-color: blue");
}
void MainWindow::on_NoroesteBaixo_clicked()
{
    if(ui->back1->styleSheet() == ui->up5->styleSheet() || ui->down7->styleSheet() == ui->up5->styleSheet() || ui->left7->styleSheet() == ui->up5->styleSheet())
    {
        if(ui->back1->styleSheet() == ui->front5->styleSheet() && ui->down7->styleSheet() == ui->right5->styleSheet() && ui->left7->styleSheet() == ui->up5->styleSheet())
        {
            rotateDA();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->back1->styleSheet() == ui->up5->styleSheet() && ui->down7->styleSheet() == ui->front5->styleSheet() && ui->left7->styleSheet() == ui->right5->styleSheet())
        {
            rotateD();
            rotateRA();
            rotateD();
            rotateR();
        }
        else if(ui->back1->styleSheet() == ui->right5->styleSheet() && ui->down7->styleSheet() == ui->up5->styleSheet() && ui->left7->styleSheet() == ui->front5->styleSheet())
        {
            rotateLA();
            rotateD();
            rotateL();
            rotateD();
            rotateF();
            rotateDA();
            rotateFA();
        }
        else if(ui->back1->styleSheet() == ui->front5->styleSheet() && ui->down7->styleSheet() == ui->up5->styleSheet() && ui->left7->styleSheet() == ui->left5->styleSheet())
        {
            rotateLA();
            rotateD();
            rotateL();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->back1->styleSheet() == ui->up5->styleSheet() && ui->down7->styleSheet() == ui->left5->styleSheet() && ui->left7->styleSheet() == ui->front5->styleSheet())
        {
            rotateFA();
            rotateD();
            rotateF();
        }
        else if(ui->back1->styleSheet() == ui->left5->styleSheet() && ui->down7->styleSheet() == ui->front5->styleSheet() && ui->left7->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateD();
            rotateL();
            rotateDA();
            rotateLA();
        }
        else if(ui->back1->styleSheet() == ui->left5->styleSheet() && ui->down7->styleSheet() == ui->up5->styleSheet() && ui->left7->styleSheet() == ui->back5->styleSheet())
        {
            rotateB();
            rotateDA();
            rotateBA();
            rotateD();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->back1->styleSheet() == ui->up5->styleSheet() && ui->down7->styleSheet() == ui->back5->styleSheet() && ui->left7->styleSheet() == ui->left5->styleSheet())
        {
            rotateDA();
            rotateLA();
            rotateD();
            rotateL();
        }
        else if(ui->back1->styleSheet() == ui->back5->styleSheet() && ui->down7->styleSheet() == ui->left5->styleSheet() && ui->left7->styleSheet() == ui->up5->styleSheet())
        {
            rotateD();
            rotateB();
            rotateDA();
            rotateBA();
        }
        else if(ui->back1->styleSheet() == ui->back5->styleSheet() && ui->down7->styleSheet() == ui->up5->styleSheet() && ui->left7->styleSheet() == ui->right5->styleSheet())
        {
            rotateB();
            rotateDA();
            rotateBA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->back1->styleSheet() == ui->up5->styleSheet() && ui->down7->styleSheet() == ui->right5->styleSheet() && ui->left7->styleSheet() == ui->back5->styleSheet())
        {
            rotateDA();
            rotateDA();
            rotateBA();
            rotateD();
            rotateB();
        }
        else if(ui->back1->styleSheet() == ui->right5->styleSheet() && ui->down7->styleSheet() == ui->back5->styleSheet() && ui->left7->styleSheet() == ui->up5->styleSheet())
        {
            rotateR();
            rotateDA();
            rotateRA();
        }
    }
}
void MainWindow::on_back1_clicked()
{
    if(ui->back1->styleSheet() == ui->up5->styleSheet())
        ui->back1->setStyleSheet("background-color: white");
    else if(ui->back1->styleSheet() == ui->front5->styleSheet())
        ui->back1->setStyleSheet("background-color: orange");
    else if(ui->back1->styleSheet() == ui->left5->styleSheet())
        ui->back1->setStyleSheet("background-color: red");
    else if(ui->back1->styleSheet() == ui->right5->styleSheet())
        ui->back1->setStyleSheet("background-color: green");
    else if(ui->back1->styleSheet() == ui->down5->styleSheet())
        ui->back1->setStyleSheet("background-color: yellow");
    else if(ui->back1->styleSheet() == ui->back5->styleSheet())
        ui->back1->setStyleSheet("background-color: blue");
}
void MainWindow::on_down7_clicked()
{
    if(ui->down7->styleSheet() == ui->up5->styleSheet())
        ui->down7->setStyleSheet("background-color: white");
    else if(ui->down7->styleSheet() == ui->front5->styleSheet())
        ui->down7->setStyleSheet("background-color: orange");
    else if(ui->down7->styleSheet() == ui->left5->styleSheet())
        ui->down7->setStyleSheet("background-color: red");
    else if(ui->down7->styleSheet() == ui->right5->styleSheet())
        ui->down7->setStyleSheet("background-color: green");
    else if(ui->down7->styleSheet() == ui->down5->styleSheet())
        ui->down7->setStyleSheet("background-color: yellow");
    else if(ui->down7->styleSheet() == ui->back5->styleSheet())
        ui->down7->setStyleSheet("background-color: blue");
}
void MainWindow::on_left7_clicked()
{
    if(ui->left7->styleSheet() == ui->up5->styleSheet())
        ui->left7->setStyleSheet("background-color: white");
    else if(ui->left7->styleSheet() == ui->front5->styleSheet())
        ui->left7->setStyleSheet("background-color: orange");
    else if(ui->left7->styleSheet() == ui->left5->styleSheet())
        ui->left7->setStyleSheet("background-color: red");
    else if(ui->left7->styleSheet() == ui->right5->styleSheet())
        ui->left7->setStyleSheet("background-color: green");
    else if(ui->left7->styleSheet() == ui->down5->styleSheet())
        ui->left7->setStyleSheet("background-color: yellow");
    else if(ui->left7->styleSheet() == ui->back5->styleSheet())
        ui->left7->setStyleSheet("background-color: blue");
}
void MainWindow::on_Quinas_clicked()
{
    while(ui->up1->styleSheet() != ui->up5->styleSheet() || ui->back7->styleSheet() != ui->back5->styleSheet() || ui->left1->styleSheet() != ui->left5->styleSheet()
          || ui->up3->styleSheet() != ui->up5->styleSheet() || ui->back9->styleSheet() != ui->back5->styleSheet() || ui->right3->styleSheet() != ui->right5->styleSheet()
          || ui->up7->styleSheet() != ui->up5->styleSheet() || ui->front1->styleSheet() != ui->front5->styleSheet() || ui->left3->styleSheet() != ui->left5->styleSheet()
          || ui->up9->styleSheet() != ui->up5->styleSheet() || ui->front3->styleSheet() != ui->front5->styleSheet() || ui->right1->styleSheet() != ui->right5->styleSheet())
    {
        on_QuinaNordeste_clicked();
        on_QuinaNoroeste_clicked();
        on_QuinaSudeste_clicked();
        on_QuinaSudoeste_clicked();
        on_SudesteBaixo_clicked();
        on_SudoesteBaixo_clicked();
        on_NordesteBaixo_clicked();
        on_NoroesteBaixo_clicked();
    }
}
void MainWindow::on_frontSecond_clicked()
{
    if(ui->front4->styleSheet() != ui->front5->styleSheet() || ui->front6->styleSheet() != ui->front5->styleSheet() ||
            ui->left6->styleSheet() != ui->left5->styleSheet() || ui->right4->styleSheet() != ui->right5->styleSheet())
    {
        if(ui->front8->styleSheet() == ui->front5->styleSheet() && ui->down2->styleSheet() == ui->left5->styleSheet())
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
        else if(ui->front8->styleSheet() == ui->front5->styleSheet() && ui->down2->styleSheet() == ui->right5->styleSheet())
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
        else if(ui->left8->styleSheet() == ui->front5->styleSheet() && ui->down4->styleSheet() == ui->left5->styleSheet())
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
        else if(ui->left8->styleSheet() == ui->front5->styleSheet() && ui->down4->styleSheet() == ui->right5->styleSheet())
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
        else if(ui->right8->styleSheet() == ui->front5->styleSheet() && ui->down6->styleSheet() == ui->left5->styleSheet())
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
        else if(ui->right8->styleSheet() == ui->front5->styleSheet() && ui->down6->styleSheet() == ui->right5->styleSheet())
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
        else if(ui->back2->styleSheet() == ui->front5->styleSheet() && ui->down8->styleSheet() == ui->left5->styleSheet())
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
        else if(ui->back2->styleSheet() == ui->front5->styleSheet() && ui->down8->styleSheet() == ui->right5->styleSheet())
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
void MainWindow::on_leftSecond_clicked()
{
    if(ui->left4->styleSheet() != ui->left5->styleSheet() || ui->left6->styleSheet() != ui->left5->styleSheet() ||
            ui->front4->styleSheet() != ui->front5->styleSheet() || ui->back4->styleSheet() != ui->back5->styleSheet())
    {
        if(ui->left8->styleSheet() == ui->left5->styleSheet() && ui->down4->styleSheet() == ui->back5->styleSheet())
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
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->left8->styleSheet() == ui->left5->styleSheet() && ui->down4->styleSheet() == ui->front5->styleSheet())
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
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back2->styleSheet() == ui->left5->styleSheet() && ui->down8->styleSheet() == ui->back5->styleSheet())
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
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back2->styleSheet() == ui->left5->styleSheet() && ui->down8->styleSheet() == ui->front5->styleSheet())
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
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front8->styleSheet() == ui->left5->styleSheet() && ui->down2->styleSheet() == ui->back5->styleSheet())
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
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front8->styleSheet() == ui->left5->styleSheet() && ui->down2->styleSheet() == ui->front5->styleSheet())
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
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right8->styleSheet() == ui->left5->styleSheet() && ui->down6->styleSheet() == ui->back5->styleSheet())
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
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->right8->styleSheet() == ui->left5->styleSheet() && ui->down6->styleSheet() == ui->front5->styleSheet())
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
            ui->ultimo->setPlainText(ult);
        }
    }
}
void MainWindow::on_rightSecond_clicked()
{
    if(ui->right4->styleSheet() != ui->right5->styleSheet() || ui->right6->styleSheet() != ui->right5->styleSheet() ||
            ui->back6->styleSheet() != ui->back5->styleSheet() || ui->front6->styleSheet() != ui->front5->styleSheet())
    {
        if(ui->right8->styleSheet() == ui->right5->styleSheet() && ui->down6->styleSheet() == ui->front5->styleSheet())
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
        else if(ui->right8->styleSheet() == ui->right5->styleSheet() && ui->down6->styleSheet() == ui->back5->styleSheet())
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
        else if(ui->front8->styleSheet() == ui->right5->styleSheet() && ui->down2->styleSheet() == ui->front5->styleSheet())
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
        else if(ui->front8->styleSheet() == ui->right5->styleSheet() && ui->down2->styleSheet() == ui->back5->styleSheet())
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
        else if(ui->back2->styleSheet() == ui->right5->styleSheet() && ui->down8->styleSheet() == ui->front5->styleSheet())
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
        else if(ui->back2->styleSheet() == ui->right5->styleSheet() && ui->down8->styleSheet() == ui->back5->styleSheet())
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
        else if(ui->left8->styleSheet() == ui->right5->styleSheet() && ui->down4->styleSheet() == ui->front5->styleSheet())
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
        else if(ui->left8->styleSheet() == ui->right5->styleSheet() && ui->down4->styleSheet() == ui->back5->styleSheet())
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
void MainWindow::on_backSecond_clicked()
{
    if(ui->back4->styleSheet() != ui->back5->styleSheet() || ui->back6->styleSheet() != ui->back5->styleSheet() ||
            ui->left4->styleSheet() != ui->left5->styleSheet() || ui->right6->styleSheet() != ui->right5->styleSheet())
    {
        if(ui->back2->styleSheet() == ui->back5->styleSheet() && ui->down8->styleSheet() == ui->right5->styleSheet())
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
        else if(ui->back2->styleSheet() == ui->back5->styleSheet() && ui->down8->styleSheet() == ui->left5->styleSheet())
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
        else if(ui->right8->styleSheet() == ui->back5->styleSheet() && ui->down6->styleSheet() == ui->right5->styleSheet())
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
        else if(ui->right8->styleSheet() == ui->back5->styleSheet() && ui->down6->styleSheet() == ui->left5->styleSheet())
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
        else if(ui->left8->styleSheet() == ui->back5->styleSheet() && ui->down4->styleSheet() == ui->right5->styleSheet())
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
        else if(ui->left8->styleSheet() == ui->back5->styleSheet() && ui->down4->styleSheet() == ui->left5->styleSheet())
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
        else if(ui->front8->styleSheet() == ui->back5->styleSheet() && ui->down2->styleSheet() == ui->right5->styleSheet())
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
        else if(ui->front8->styleSheet() == ui->back5->styleSheet() && ui->down2->styleSheet() == ui->left5->styleSheet())
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
void MainWindow::on_segundaCamada_clicked()
{
    int j=0;
    int k=0;
    while(ui->front4->styleSheet() != ui->front5->styleSheet() || ui->front6->styleSheet() != ui->front5->styleSheet() ||
          ui->left4->styleSheet() != ui->left5->styleSheet() || ui->left6->styleSheet() != ui->left5->styleSheet() ||
          ui->right4->styleSheet() != ui->right5->styleSheet() || ui->right6->styleSheet() != ui->right5->styleSheet() ||
          ui->back4->styleSheet() != ui->back5->styleSheet() || ui->back6->styleSheet() != ui->back5->styleSheet())
    {
        
        
            on_rightSecond_clicked();
        
            on_frontSecond_clicked();
        
            on_backSecond_clicked();
        
            on_leftSecond_clicked();
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
        /*on_rightSecond_clicked();
        on_rightSecond_clicked();
        on_rightSecond_clicked();
        on_rightSecond_clicked();
        debugaRight();
        on_frontSecond_clicked();
        on_frontSecond_clicked();
        on_frontSecond_clicked();
        on_frontSecond_clicked();
        on_frontSecond_clicked();
        debugaFront();
        on_backSecond_clicked();
        on_backSecond_clicked();
        on_backSecond_clicked();
        on_backSecond_clicked();
        on_backSecond_clicked();
        debugaBack();
        on_leftSecond_clicked();
        on_leftSecond_clicked();
        on_leftSecond_clicked();
        on_leftSecond_clicked();
        on_leftSecond_clicked();
        debugaLeft();
        i++;
        on_frontSecond_clicked();
        on_leftSecond_clicked();
        on_rightSecond_clicked();
        on_backSecond_clicked();
        on_frontSecond_clicked();
        on_leftSecond_clicked();
        on_rightSecond_clicked();
        on_backSecond_clicked();
        on_frontSecond_clicked();
        on_leftSecond_clicked();
        on_rightSecond_clicked();
        on_backSecond_clicked();
        on_frontSecond_clicked();
        on_leftSecond_clicked();
        on_rightSecond_clicked();
        on_backSecond_clicked();*/
        
       
       
       
       
       
       
    }
}
void MainWindow::debugaFront()
{
    if(ui->front4->styleSheet() != ui->front5->styleSheet() || ui->front6->styleSheet() != ui->front5->styleSheet() ||
                ui->left6->styleSheet() != ui->left5->styleSheet() || ui->right4->styleSheet() != ui->right5->styleSheet())
        {
            if(ui->front8->styleSheet() == ui->front5->styleSheet() && ui->down2->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->front8->styleSheet() == ui->front5->styleSheet() && ui->down2->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->left8->styleSheet() == ui->front5->styleSheet() && ui->down4->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->left8->styleSheet() == ui->front5->styleSheet() && ui->down4->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->right8->styleSheet() == ui->front5->styleSheet() && ui->down6->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->right8->styleSheet() == ui->front5->styleSheet() && ui->down6->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->back2->styleSheet() == ui->front5->styleSheet() && ui->down8->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->back2->styleSheet() == ui->front5->styleSheet() && ui->down8->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->front4->styleSheet() == ui->left5->styleSheet() && ui->left6->styleSheet() == ui->front5->styleSheet())
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
            else if(ui->front6->styleSheet() == ui->right5->styleSheet() && ui->right4->styleSheet() == ui->front5->styleSheet())
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
    if(ui->left4->styleSheet() != ui->left5->styleSheet() || ui->left6->styleSheet() != ui->left5->styleSheet() ||
                ui->front4->styleSheet() != ui->front5->styleSheet() || ui->back4->styleSheet() != ui->back5->styleSheet())
        {
            if(ui->left8->styleSheet() == ui->left5->styleSheet() && ui->down4->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->left8->styleSheet() == ui->left5->styleSheet() && ui->down4->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->back2->styleSheet() == ui->left5->styleSheet() && ui->down8->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->back2->styleSheet() == ui->left5->styleSheet() && ui->down8->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->front8->styleSheet() == ui->left5->styleSheet() && ui->down2->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->front8->styleSheet() == ui->left5->styleSheet() && ui->down2->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->right8->styleSheet() == ui->left5->styleSheet() && ui->down6->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->right8->styleSheet() == ui->left5->styleSheet() && ui->down6->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->left4->styleSheet() == ui->down5->styleSheet() && ui->down4->styleSheet() == ui->left5->styleSheet())
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
            else if(ui->left6->styleSheet() == ui->front5->styleSheet() && ui->front4->styleSheet() == ui->left5->styleSheet())
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
    if(ui->right4->styleSheet() != ui->right5->styleSheet() || ui->right6->styleSheet() != ui->right5->styleSheet() ||
                ui->back6->styleSheet() != ui->back5->styleSheet() || ui->front6->styleSheet() != ui->front5->styleSheet())
        {
            if(ui->right8->styleSheet() == ui->right5->styleSheet() && ui->down6->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->right8->styleSheet() == ui->right5->styleSheet() && ui->down6->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->front8->styleSheet() == ui->right5->styleSheet() && ui->down2->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->front8->styleSheet() == ui->right5->styleSheet() && ui->down2->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->back2->styleSheet() == ui->right5->styleSheet() && ui->down8->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->back2->styleSheet() == ui->right5->styleSheet() && ui->down8->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->left8->styleSheet() == ui->right5->styleSheet() && ui->down4->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->left8->styleSheet() == ui->right5->styleSheet() && ui->down4->styleSheet() == ui->down5->styleSheet())
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
            else if(ui->right4->styleSheet() == ui->front5->styleSheet() && ui->front6->styleSheet() == ui->right5->styleSheet())
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
            else if(ui->right6->styleSheet() == ui->down5->styleSheet() && ui->down6->styleSheet() == ui->right5->styleSheet())
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
    if(ui->back4->styleSheet() != ui->back5->styleSheet() || ui->back6->styleSheet() != ui->back5->styleSheet() ||
                ui->left4->styleSheet() != ui->left5->styleSheet() || ui->right6->styleSheet() != ui->right5->styleSheet())
    {
        if(ui->back2->styleSheet() == ui->back5->styleSheet() && ui->down8->styleSheet() == ui->down5->styleSheet())
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
        else if(ui->back2->styleSheet() == ui->back5->styleSheet() && ui->down8->styleSheet() == ui->down5->styleSheet())
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
        else if(ui->right8->styleSheet() == ui->back5->styleSheet() && ui->down6->styleSheet() == ui->down5->styleSheet())
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
        else if(ui->right8->styleSheet() == ui->back5->styleSheet() && ui->down6->styleSheet() == ui->down5->styleSheet())
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
        else if(ui->left8->styleSheet() == ui->back5->styleSheet() && ui->down4->styleSheet() == ui->down5->styleSheet())
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
        else if(ui->left8->styleSheet() == ui->back5->styleSheet() && ui->down4->styleSheet() == ui->down5->styleSheet())
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
        else if(ui->front8->styleSheet() == ui->back5->styleSheet() && ui->down2->styleSheet() == ui->down5->styleSheet())
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
        else if(ui->front8->styleSheet() == ui->back5->styleSheet() && ui->down2->styleSheet() == ui->down5->styleSheet())
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
        else if(ui->back6->styleSheet() == ui->right5->styleSheet() && ui->right6->styleSheet() == ui->back5->styleSheet())
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
        else if(ui->back4->styleSheet() == ui->left5->styleSheet() && ui->left4->styleSheet() == ui->back5->styleSheet())
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
void MainWindow::on_consertaSegunda_clicked()
{
    debugaFront();
    debugaLeft();
    debugaRight();
    debugaBack();
}
void MainWindow::on_Tudo_clicked()
{
    QElapsedTimer tempo;
    int movimentos_old;
    int dif;
    tempo.start();
        movimentos_old=0;
        on_Randomize_clicked();
        on_Cross_clicked();
        cross << std::to_string(movimentos)+"\n";
        cross << QString::number(tempo.elapsed()).toStdString() + "\n";
        movimentos_old = movimentos;
        on_Quinas_clicked();
        dif = movimentos-movimentos_old;
        movimentos_old = movimentos;
        quinas << std::to_string(movimentos);
        quinas << " ";
        quinas << std::to_string(dif)+"\n";
        quinas << QString::number(tempo.elapsed()).toStdString() + "\n";
        on_segundaCamada_clicked();
        dif = movimentos-movimentos_old;
        movimentos_old = movimentos;
        segundacamada << std::to_string(movimentos);
        segundacamada << " ";
        segundacamada << std::to_string(dif)+"\n";
        segundacamada << QString::number(tempo.elapsed()).toStdString() + "\n";
        on_CrossDown_clicked();
        dif = movimentos-movimentos_old;
        movimentos_old = movimentos;
        crossdown << std::to_string(movimentos);
        crossdown << " ";
        crossdown << std::to_string(dif)+"\n";
        crossdown << QString::number(tempo.elapsed()).toStdString() + "\n";
        on_CantosDown_clicked();
        dif = movimentos-movimentos_old;
        movimentos_old = movimentos;
        cantosdown << std::to_string(movimentos);
        cantosdown << " ";
        cantosdown << std::to_string(dif)+"\n";
        cantosdown << QString::number(tempo.elapsed()).toStdString() + "\n";
        on_Place_clicked();
        dif = movimentos-movimentos_old;
        movimentos_old = movimentos;
        place << std::to_string(movimentos);
        place << " ";
        place << std::to_string(dif)+"\n";
        place << QString::number(tempo.elapsed()).toStdString() + "\n";
        on_Turn_clicked();
        dif = movimentos-movimentos_old;
        movimentos_old = movimentos;
        turn << std::to_string(movimentos);
        turn << " ";
        turn << std::to_string(dif)+"\n";
        turn << QString::number(tempo.elapsed()).toStdString() + "\n";
        std::cout << QString::number(tempo.elapsed()).toStdString() << std::endl;
    cross.close();
    quinas.close();
    segundacamada.close();
    crossdown.close();
    cantosdown.close();
    place.close();
    turn.close();
}
void MainWindow::on_CrossDown_clicked()
{
    if(ui->down2->styleSheet() != ui->down5->styleSheet() && ui->down4->styleSheet() != ui->down5->styleSheet() &&
            ui->down6->styleSheet() != ui->down5->styleSheet() && ui->down8->styleSheet() != ui->down5->styleSheet())
    {
        rotateF();
        rotateL();
        rotateD();
        rotateLA();
        rotateDA();
        rotateFA();
    }
    while((ui->down2->styleSheet() != ui->down5->styleSheet() || ui->down4->styleSheet() != ui->down5->styleSheet()) &&
          (ui->down2->styleSheet() != ui->down5->styleSheet() || ui->down8->styleSheet() != ui->down5->styleSheet()) &&
           (ui->down4->styleSheet() != ui->down5->styleSheet() || ui->down6->styleSheet() != ui->down5->styleSheet()))
        rotateD();
    if(ui->down6->styleSheet() != ui->down5->styleSheet() && ui->down8->styleSheet() != ui->down5->styleSheet())
    {
        rotateF();
        rotateL();
        rotateD();
        rotateLA();
        rotateDA();
        rotateFA();
    }
    
    if((ui->down2->styleSheet() == ui->down5->styleSheet() &&
        ui->down8->styleSheet() == ui->down5->styleSheet()) &&
            ui->down4->styleSheet() != ui->down5->styleSheet() &&
            ui->down6->styleSheet() != ui->down5->styleSheet())
        rotateD();
    if(ui->down2->styleSheet() != ui->down5->styleSheet() || ui->down4->styleSheet() != ui->down5->styleSheet() ||
            ui->down6->styleSheet() != ui->down5->styleSheet() || ui->down8->styleSheet() != ui->down5->styleSheet())
    {
        rotateF();
        rotateL();
        rotateD();
        rotateLA();
        rotateDA();
        rotateFA();
    }
}
void MainWindow::on_CantosDown_clicked()
{
    int i;
    int j=0;
    while(ui->front8->styleSheet() != ui->front5->styleSheet() || ui->left8->styleSheet() != ui->left5->styleSheet() ||
          ui->back2->styleSheet() != ui->back5->styleSheet() || ui->right8->styleSheet() != ui->right5->styleSheet())
    {
        j++;
        if (j%30==0)
            rotateDA();
        if(ui->front8->styleSheet() != ui->front5->styleSheet() || ui->left8->styleSheet() != ui->left5->styleSheet() ||
                ui->back2->styleSheet() != ui->back5->styleSheet() || ui->right8->styleSheet() != ui->right5->styleSheet())
        {
            i = 0;
            if(ui->front8->styleSheet() == ui->front5->styleSheet())
                i++;
            if(ui->left8->styleSheet() == ui->left5->styleSheet())
                i++;
            if(ui->right8->styleSheet() == ui->right5->styleSheet())
                i++;
            if(ui->back2->styleSheet() == ui->back5->styleSheet())
                i++;
            if(i == 0)
            {
                rotateD();
                rotateD();
            }
            if(ui->front8->styleSheet() != ui->front5->styleSheet() && ui->left8->styleSheet() != ui->left5->styleSheet())
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
            else if(ui->back2->styleSheet() != ui->back2->styleSheet() && ui->left8->styleSheet() != ui->left5->styleSheet())
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
            else if(ui->back2->styleSheet() != ui->back5->styleSheet() && ui->right8->styleSheet() != ui->right5->styleSheet())
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
            else if(ui->front8->styleSheet() != ui->front2->styleSheet() && ui->right8->styleSheet() != ui->right5->styleSheet())
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
    while(ui->front8->styleSheet() != ui->front5->styleSheet() || ui->left8->styleSheet() != ui->left5->styleSheet() ||
          ui->back2->styleSheet() != ui->back5->styleSheet() || ui->right8->styleSheet() != ui->right5->styleSheet())
    {
        if(ui->front8->styleSheet() != ui->front5->styleSheet() || ui->left8->styleSheet() != ui->left5->styleSheet() ||
                ui->back2->styleSheet() != ui->back5->styleSheet() || ui->right8->styleSheet() != ui->right5->styleSheet())
        {
            i = 0;
            if(ui->front8->styleSheet() == ui->front5->styleSheet())
                i++;
            if(ui->left8->styleSheet() == ui->left5->styleSheet())
                i++;
            if(ui->right8->styleSheet() == ui->right5->styleSheet())
                i++;
            if(ui->back2->styleSheet() == ui->back5->styleSheet())
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
                if(ui->front8->styleSheet() != ui->front5->styleSheet() && ui->left8->styleSheet() != ui->left5->styleSheet())
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
                else if(ui->back2->styleSheet() != ui->back2->styleSheet() && ui->left8->styleSheet() != ui->left5->styleSheet())
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
                else if(ui->back2->styleSheet() != ui->back5->styleSheet() && ui->right8->styleSheet() != ui->right5->styleSheet())
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
                else if(ui->front8->styleSheet() != ui->front2->styleSheet() && ui->right8->styleSheet() != ui->right5->styleSheet())
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
            if(ui->left8->styleSheet() == ui->left5->styleSheet() && ui->right8->styleSheet() == ui->right5->styleSheet())
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
        if(ui->front8->styleSheet() != ui->front5->styleSheet() && ui->left8->styleSheet() != ui->left5->styleSheet() &&
                ui->back2->styleSheet() != ui->back5->styleSheet() && ui->right8->styleSheet() != ui->right5->styleSheet())
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
    if((ui->front9->styleSheet() == ui->front5->styleSheet() || ui->front9->styleSheet() == ui->down5->styleSheet() || ui->front9->styleSheet() == ui->right5->styleSheet()) &&
            (ui->down3->styleSheet() == ui->front5->styleSheet() || ui->down3->styleSheet() == ui->down5->styleSheet() || ui->down3->styleSheet() == ui->right5->styleSheet()) &&
            (ui->right7->styleSheet() == ui->front5->styleSheet() || ui->right7->styleSheet() == ui->down5->styleSheet() || ui->right7->styleSheet() == ui->right5->styleSheet()))
        return true;
    else
        return false;
}
bool MainWindow::sudoestecorreto()
{
    if((ui->front7->styleSheet() == ui->front5->styleSheet() || ui->front7->styleSheet() == ui->down5->styleSheet() || ui->front7->styleSheet() == ui->left5->styleSheet()) &&
            (ui->down1->styleSheet() == ui->front5->styleSheet() || ui->down1->styleSheet() == ui->down5->styleSheet() || ui->down1->styleSheet() == ui->left5->styleSheet()) &&
            (ui->left9->styleSheet() == ui->front5->styleSheet() || ui->left9->styleSheet() == ui->down5->styleSheet() || ui->left9->styleSheet() == ui->left5->styleSheet()))
        return true;
    else
        return false;
}
bool MainWindow::nordestecorreto()
{
    if((ui->back3->styleSheet() == ui->back5->styleSheet() || ui->back3->styleSheet() == ui->down5->styleSheet() || ui->back3->styleSheet() == ui->right5->styleSheet()) &&
                    (ui->down9->styleSheet() == ui->back5->styleSheet() || ui->down9->styleSheet() == ui->down5->styleSheet() || ui->down9->styleSheet() == ui->right5->styleSheet()) &&
                    (ui->right9->styleSheet() == ui->back5->styleSheet() || ui->right9->styleSheet() == ui->down5->styleSheet() || ui->right9->styleSheet() == ui->right5->styleSheet()))
        return true;
    else
        return false;
}
bool MainWindow::noroestecorreto()
{
    if((ui->back1->styleSheet() == ui->back5->styleSheet() || ui->back1->styleSheet() == ui->down5->styleSheet() || ui->back1->styleSheet() == ui->left5->styleSheet()) &&
            (ui->down7->styleSheet() == ui->back5->styleSheet() || ui->down7->styleSheet() == ui->down5->styleSheet() || ui->down7->styleSheet() == ui->left5->styleSheet()) &&
            (ui->left7->styleSheet() == ui->back5->styleSheet() || ui->left7->styleSheet() == ui->down5->styleSheet() || ui->left7->styleSheet() == ui->left5->styleSheet()))
        return true;
    else
        return false;
}
void MainWindow::on_Place_clicked()
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
            ui->ultimo->setPlainText(ult);
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
            ui->ultimo->setPlainText(ult);
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
            ui->ultimo->setPlainText(ult);
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
            ui->ultimo->setPlainText(ult);
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
            ui->ultimo->setPlainText(ult);
        }
    }
}
void MainWindow::on_Turn_clicked()
{
    while(ui->front1->styleSheet() != ui->front5->styleSheet() || ui->front2->styleSheet() != ui->front5->styleSheet() || ui->front3->styleSheet() != ui->front5->styleSheet() ||
          ui->front4->styleSheet() != ui->front5->styleSheet() || ui->front6->styleSheet() != ui->front5->styleSheet() || ui->front7->styleSheet() != ui->front5->styleSheet() ||
          ui->front8->styleSheet() != ui->front5->styleSheet() || ui->front9->styleSheet() != ui->front5->styleSheet() ||
          ui->left1->styleSheet() != ui->left5->styleSheet() || ui->left2->styleSheet() != ui->left5->styleSheet() || ui->left3->styleSheet() != ui->left5->styleSheet() ||
          ui->left4->styleSheet() != ui->left5->styleSheet() || ui->left6->styleSheet() != ui->left5->styleSheet() || ui->left7->styleSheet() != ui->left5->styleSheet() ||
          ui->left8->styleSheet() != ui->left5->styleSheet() || ui->left9->styleSheet() != ui->left5->styleSheet() ||
          ui->right1->styleSheet() != ui->right5->styleSheet() || ui->right2->styleSheet() != ui->right5->styleSheet() || ui->right3->styleSheet() != ui->right5->styleSheet() ||
          ui->right4->styleSheet() != ui->right5->styleSheet() || ui->right6->styleSheet() != ui->right5->styleSheet() || ui->right7->styleSheet() != ui->right5->styleSheet() ||
          ui->right8->styleSheet() != ui->right5->styleSheet() || ui->right9->styleSheet() != ui->right5->styleSheet() ||
          ui->back1->styleSheet() != ui->back5->styleSheet() || ui->back2->styleSheet() != ui->back5->styleSheet() || ui->back3->styleSheet() != ui->back5->styleSheet() ||
          ui->back4->styleSheet() != ui->back5->styleSheet() || ui->back6->styleSheet() != ui->back5->styleSheet() || ui->back7->styleSheet() != ui->back5->styleSheet() ||
          ui->back8->styleSheet() != ui->back5->styleSheet() || ui->back9->styleSheet() != ui->back5->styleSheet() ||
          ui->up1->styleSheet() != ui->up5->styleSheet() || ui->up2->styleSheet() != ui->up5->styleSheet() || ui->up3->styleSheet() != ui->up5->styleSheet() ||
          ui->up4->styleSheet() != ui->up5->styleSheet() || ui->up6->styleSheet() != ui->up5->styleSheet() || ui->up7->styleSheet() != ui->up5->styleSheet() ||
          ui->up8->styleSheet() != ui->up5->styleSheet() || ui->up9->styleSheet() != ui->up5->styleSheet() ||
          ui->down1->styleSheet() != ui->down5->styleSheet() || ui->down2->styleSheet() != ui->down5->styleSheet() || ui->down3->styleSheet() != ui->down5->styleSheet() ||
          ui->down4->styleSheet() != ui->down5->styleSheet() || ui->down6->styleSheet() != ui->down5->styleSheet() || ui->down7->styleSheet() != ui->down5->styleSheet() ||
          ui->down8->styleSheet() != ui->down5->styleSheet() || ui->down9->styleSheet() != ui->down5->styleSheet())
    {
        if((ui->front7->styleSheet() != ui->front5->styleSheet() || ui->left9->styleSheet() != ui->left5->styleSheet() || ui->down1->styleSheet() != ui->down5->styleSheet()) &&
                (ui->back1->styleSheet() != ui->back5->styleSheet() || ui->left7->styleSheet() != ui->left5->styleSheet() || ui->down7->styleSheet() != ui->down7->styleSheet()))
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
            ui->ultimo->setPlainText(ult);
        }
        else if((ui->back1->styleSheet() != ui->back5->styleSheet() || ui->left7->styleSheet() != ui->left5->styleSheet() || ui->down7->styleSheet() != ui->down7->styleSheet()) &&
                (ui->back3->styleSheet() != ui->back5->styleSheet() || ui->right9->styleSheet() != ui->right5->styleSheet() || ui->down9->styleSheet() != ui->down7->styleSheet()))
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
            ui->ultimo->setPlainText(ult);
        }
        else if((ui->back3->styleSheet() != ui->back5->styleSheet() || ui->right9->styleSheet() != ui->right5->styleSheet() || ui->down9->styleSheet() != ui->down7->styleSheet()) &&
                (ui->front9->styleSheet() != ui->front5->styleSheet() || ui->right7->styleSheet() != ui->right5->styleSheet() || ui->down3->styleSheet() != ui->down7->styleSheet()))
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
            ui->ultimo->setPlainText(ult);
        }
        else if((ui->front9->styleSheet() != ui->front5->styleSheet() || ui->right7->styleSheet() != ui->right5->styleSheet() || ui->down3->styleSheet() != ui->down7->styleSheet()) &&
                (ui->front7->styleSheet() != ui->front5->styleSheet() || ui->left9->styleSheet() != ui->left5->styleSheet() || ui->down1->styleSheet() != ui->down5->styleSheet()))
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
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front7->styleSheet() != ui->front5->styleSheet() || ui->left9->styleSheet() != ui->left5->styleSheet() || ui->down1->styleSheet() != ui->down5->styleSheet())
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
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back1->styleSheet() != ui->back5->styleSheet() || ui->left7->styleSheet() != ui->left5->styleSheet() || ui->down7->styleSheet() != ui->down7->styleSheet())
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
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->back3->styleSheet() != ui->back5->styleSheet() || ui->right9->styleSheet() != ui->right5->styleSheet() || ui->down9->styleSheet() != ui->down7->styleSheet())
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
            ui->ultimo->setPlainText(ult);
        }
        else if(ui->front9->styleSheet() != ui->front5->styleSheet() || ui->right7->styleSheet() != ui->right5->styleSheet() || ui->down3->styleSheet() != ui->down7->styleSheet())
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
            ui->ultimo->setPlainText(ult);
        }
    }
}
void MainWindow::on_otimoButton_clicked()
{
    QString input = "";
    input += color(ui->right3->styleSheet());
    input += color(ui->right6->styleSheet());
    input += color(ui->right9->styleSheet());
    input += color(ui->right2->styleSheet());
    input += color(ui->right8->styleSheet());
    input += color(ui->right1->styleSheet());
    input += color(ui->right4->styleSheet());
    input += color(ui->right7->styleSheet());
    input += color(ui->up3->styleSheet());
    input += color(ui->up6->styleSheet());
    input += color(ui->up9->styleSheet());
    input += color(ui->front3->styleSheet());
    input += color(ui->front6->styleSheet());
    input += color(ui->front9->styleSheet());
    input += color(ui->down3->styleSheet());
    input += color(ui->down6->styleSheet());
    input += color(ui->down9->styleSheet());
    input += color(ui->back3->styleSheet());
    input += color(ui->back6->styleSheet());
    input += color(ui->back9->styleSheet());
    input += color(ui->up2->styleSheet());
    input += color(ui->up8->styleSheet());
    input += color(ui->front2->styleSheet());
    input += color(ui->front8->styleSheet());
    input += color(ui->down2->styleSheet());
    input += color(ui->down8->styleSheet());
    input += color(ui->back2->styleSheet());
    input += color(ui->back8->styleSheet());
    input += color(ui->up1->styleSheet());
    input += color(ui->up4->styleSheet());
    input += color(ui->up7->styleSheet());
    input += color(ui->front1->styleSheet());
    input += color(ui->front4->styleSheet());
    input += color(ui->front7->styleSheet());
    input += color(ui->down1->styleSheet());
    input += color(ui->down4->styleSheet());
    input += color(ui->down7->styleSheet());
    input += color(ui->back1->styleSheet());
    input += color(ui->back4->styleSheet());
    input += color(ui->back7->styleSheet());
    input += color(ui->left3->styleSheet());
    input += color(ui->left6->styleSheet());
    input += color(ui->left9->styleSheet());
    input += color(ui->left2->styleSheet());
    input += color(ui->left8->styleSheet());
    input += color(ui->left1->styleSheet());
    input += color(ui->left4->styleSheet());
    input += color(ui->left7->styleSheet());
    std::cout << input.toStdString() << std::endl;
    QElapsedTimer timer;
    timer.start();
    QString program = "../Rubiks-Cube-Solver/format.sh";
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
    program = "../Rubiks-Cube-Solver/run.sh";
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
         otimotempo << QString::number(timer.elapsed()).toStdString() + "\n";
        while ( std::getline (myfile2,line ))
        {
            if(line.size() == 1 || line.size() == 2)
            {
                solve(line);
            }
          std::cout << line << '\n';
          std::cout << "size: " << line.size() << std::endl;
        }
        otimotempo << QString::number(timer.elapsed()).toStdString() + "\n";
        std::cout << "tempo todo: " + QString::number(timer.restart()).toStdString() << std::endl;
        myfile2.close();
        otimotempo.close();
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
        ui->up1->setStyleSheet(QString::fromStdString(styles[0]));
        ui->up2->setStyleSheet(QString::fromStdString(styles[1]));
        ui->up3->setStyleSheet(QString::fromStdString(styles[2]));
        ui->up4->setStyleSheet(QString::fromStdString(styles[3]));
        ui->up5->setStyleSheet(QString::fromStdString(styles[4]));
        ui->up6->setStyleSheet(QString::fromStdString(styles[5]));
        ui->up7->setStyleSheet(QString::fromStdString(styles[6]));
        ui->up8->setStyleSheet(QString::fromStdString(styles[7]));
        ui->up9->setStyleSheet(QString::fromStdString(styles[8]));
    }
    else if(c == 1)
    {
        ui->front1->setStyleSheet(QString::fromStdString(styles[0]));
        ui->front2->setStyleSheet(QString::fromStdString(styles[1]));
        ui->front3->setStyleSheet(QString::fromStdString(styles[2]));
        ui->front4->setStyleSheet(QString::fromStdString(styles[3]));
        ui->front5->setStyleSheet(QString::fromStdString(styles[4]));
        ui->front6->setStyleSheet(QString::fromStdString(styles[5]));
        ui->front7->setStyleSheet(QString::fromStdString(styles[6]));
        ui->front8->setStyleSheet(QString::fromStdString(styles[7]));
        ui->front9->setStyleSheet(QString::fromStdString(styles[8]));
    }
    else if(c == 2)
    {
        ui->down1->setStyleSheet(QString::fromStdString(styles[0]));
        ui->down2->setStyleSheet(QString::fromStdString(styles[1]));
        ui->down3->setStyleSheet(QString::fromStdString(styles[2]));
        ui->down4->setStyleSheet(QString::fromStdString(styles[3]));
        ui->down5->setStyleSheet(QString::fromStdString(styles[4]));
        ui->down6->setStyleSheet(QString::fromStdString(styles[5]));
        ui->down7->setStyleSheet(QString::fromStdString(styles[6]));
        ui->down8->setStyleSheet(QString::fromStdString(styles[7]));
        ui->down9->setStyleSheet(QString::fromStdString(styles[8]));
    }
    else if(c == 3)
    {
        ui->back1->setStyleSheet(QString::fromStdString(styles[0]));
        ui->back2->setStyleSheet(QString::fromStdString(styles[1]));
        ui->back3->setStyleSheet(QString::fromStdString(styles[2]));
        ui->back4->setStyleSheet(QString::fromStdString(styles[3]));
        ui->back5->setStyleSheet(QString::fromStdString(styles[4]));
        ui->back6->setStyleSheet(QString::fromStdString(styles[5]));
        ui->back7->setStyleSheet(QString::fromStdString(styles[6]));
        ui->back8->setStyleSheet(QString::fromStdString(styles[7]));
        ui->back9->setStyleSheet(QString::fromStdString(styles[8]));
    }
    else if(c == 5)
    {
        ui->left1->setStyleSheet(QString::fromStdString(styles[0]));
        ui->left2->setStyleSheet(QString::fromStdString(styles[1]));
        ui->left3->setStyleSheet(QString::fromStdString(styles[2]));
        ui->left4->setStyleSheet(QString::fromStdString(styles[3]));
        ui->left5->setStyleSheet(QString::fromStdString(styles[4]));
        ui->left6->setStyleSheet(QString::fromStdString(styles[5]));
        ui->left7->setStyleSheet(QString::fromStdString(styles[6]));
        ui->left8->setStyleSheet(QString::fromStdString(styles[7]));
        ui->left9->setStyleSheet(QString::fromStdString(styles[8]));
    }
    else if(c == 4)
    {
        ui->right1->setStyleSheet(QString::fromStdString(styles[0]));
        ui->right2->setStyleSheet(QString::fromStdString(styles[1]));
        ui->right3->setStyleSheet(QString::fromStdString(styles[2]));
        ui->right4->setStyleSheet(QString::fromStdString(styles[3]));
        ui->right5->setStyleSheet(QString::fromStdString(styles[4]));
        ui->right6->setStyleSheet(QString::fromStdString(styles[5]));
        ui->right7->setStyleSheet(QString::fromStdString(styles[6]));
        ui->right8->setStyleSheet(QString::fromStdString(styles[7]));
        ui->right9->setStyleSheet(QString::fromStdString(styles[8]));
    }
}
bool MainWindow::verificaCoerencia()
{
    std::vector<QString> numberOfColors;
    numberOfColors.push_back(ui->up1->styleSheet());
    numberOfColors.push_back(ui->up2->styleSheet());
    numberOfColors.push_back(ui->up3->styleSheet());
    numberOfColors.push_back(ui->up4->styleSheet());
    numberOfColors.push_back(ui->up5->styleSheet());
    numberOfColors.push_back(ui->up6->styleSheet());
    numberOfColors.push_back(ui->up7->styleSheet());
    numberOfColors.push_back(ui->up8->styleSheet());
    numberOfColors.push_back(ui->up9->styleSheet());
    numberOfColors.push_back(ui->front1->styleSheet());
    numberOfColors.push_back(ui->front2->styleSheet());
    numberOfColors.push_back(ui->front3->styleSheet());
    numberOfColors.push_back(ui->front4->styleSheet());
    numberOfColors.push_back(ui->front5->styleSheet());
    numberOfColors.push_back(ui->front6->styleSheet());
    numberOfColors.push_back(ui->front7->styleSheet());
    numberOfColors.push_back(ui->front8->styleSheet());
    numberOfColors.push_back(ui->front9->styleSheet());
    numberOfColors.push_back(ui->down1->styleSheet());
    numberOfColors.push_back(ui->down2->styleSheet());
    numberOfColors.push_back(ui->down3->styleSheet());
    numberOfColors.push_back(ui->down4->styleSheet());
    numberOfColors.push_back(ui->down5->styleSheet());
    numberOfColors.push_back(ui->down6->styleSheet());
    numberOfColors.push_back(ui->down7->styleSheet());
    numberOfColors.push_back(ui->down8->styleSheet());
    numberOfColors.push_back(ui->down9->styleSheet());
    numberOfColors.push_back(ui->back1->styleSheet());
    numberOfColors.push_back(ui->back2->styleSheet());
    numberOfColors.push_back(ui->back3->styleSheet());
    numberOfColors.push_back(ui->back4->styleSheet());
    numberOfColors.push_back(ui->back5->styleSheet());
    numberOfColors.push_back(ui->back6->styleSheet());
    numberOfColors.push_back(ui->back7->styleSheet());
    numberOfColors.push_back(ui->back8->styleSheet());
    numberOfColors.push_back(ui->back9->styleSheet());
    numberOfColors.push_back(ui->left1->styleSheet());
    numberOfColors.push_back(ui->left2->styleSheet());
    numberOfColors.push_back(ui->left3->styleSheet());
    numberOfColors.push_back(ui->left4->styleSheet());
    numberOfColors.push_back(ui->left5->styleSheet());
    numberOfColors.push_back(ui->left6->styleSheet());
    numberOfColors.push_back(ui->left7->styleSheet());
    numberOfColors.push_back(ui->left8->styleSheet());
    numberOfColors.push_back(ui->left9->styleSheet());
    numberOfColors.push_back(ui->right1->styleSheet());
    numberOfColors.push_back(ui->right2->styleSheet());
    numberOfColors.push_back(ui->right3->styleSheet());
    numberOfColors.push_back(ui->right4->styleSheet());
    numberOfColors.push_back(ui->right5->styleSheet());
    numberOfColors.push_back(ui->right6->styleSheet());
    numberOfColors.push_back(ui->right7->styleSheet());
    numberOfColors.push_back(ui->right8->styleSheet());
    numberOfColors.push_back(ui->right9->styleSheet());
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
void MainWindow::on_pushButton_clicked()
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
void MainWindow::on_pushButton_2_clicked()
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
                    on_pushButton_2_clicked();
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
void MainWindow::calibrar()
{
    CalibrationOn = true;
    for(int i = 0; i < 6; i++)
    {
        std::vector<char> c;
        if(i == 0)
        {
            c.push_back(StyleSheet2Char((ui->up1->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->up2->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->up3->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->up4->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->up5->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->up6->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->up7->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->up8->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->up9->styleSheet()).toStdString()));
        }
        else if(i == 1)
        {
            c.push_back(StyleSheet2Char((ui->front1->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->front2->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->front3->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->front4->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->front5->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->front6->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->front7->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->front8->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->front9->styleSheet()).toStdString()));
        }
        else if(i == 4)
        {
            c.push_back(StyleSheet2Char((ui->right1->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->right2->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->right3->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->right4->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->right5->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->right6->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->right7->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->right8->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->right9->styleSheet()).toStdString()));
        }
        else if(i == 5)
        {
            c.push_back(StyleSheet2Char((ui->left1->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->left2->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->left3->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->left4->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->left5->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->left6->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->left7->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->left8->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->left9->styleSheet()).toStdString()));
        }
        else if(i == 2)
        {
            c.push_back(StyleSheet2Char((ui->down1->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->down2->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->down3->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->down4->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->down5->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->down6->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->down7->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->down8->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->down9->styleSheet()).toStdString()));
        }
        else if(i == 3)
        {
            c.push_back(StyleSheet2Char((ui->back1->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->back2->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->back3->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->back4->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->back5->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->back6->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->back7->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->back8->styleSheet()).toStdString()));
            c.push_back(StyleSheet2Char((ui->back9->styleSheet()).toStdString()));
        }
        takePictureAndProcess(c);
        
        lado++;
        std::string comando_aux = "0" + std::to_string(i+1);
        char* comando = new char[comando_aux.length()];
        strcpy(comando, comando_aux.c_str());
        if(arduino->isWritable())
        {
            arduino->write(comando);
            if(i+1 == 1 || i+1 == 2 || i+1 == 3)
                delay(5000);
            else if(i+1 == 4)
                delay(9000);
            else if(i+1 == 5)
                delay(7000);
            std::cout << comando << std::endl;
        }
        else
        {
             std::cout << "Couldn't write to serial!" << std::endl;
        }
    }
    verificaCoerencia();
    
    for(int i = 0; i < calibrateVector.size(); i++)
        calibrateVector[i] /= 900;
    config aux;
    aux.Gr = calibrateVector[0];
    aux.Gg = calibrateVector[1];
    aux.Gb = calibrateVector[2];
    aux.Rr = calibrateVector[3];
    aux.Rg = calibrateVector[4];
    aux.Rb = calibrateVector[5];
    aux.Br = calibrateVector[6];
    aux.Bg = calibrateVector[7];
    aux.Bb = calibrateVector[8];
    aux.Or = calibrateVector[9];
    aux.Og = calibrateVector[10];
    aux.Ob = calibrateVector[11];
    aux.Yr = calibrateVector[12];
    aux.Yg = calibrateVector[13];
    aux.Yb = calibrateVector[14];
    aux.Wr = calibrateVector[15];
    aux.Wg = calibrateVector[16];
    aux.Wb = calibrateVector[17];
    conf.writeCalibrateColors(&conf, &aux);
    calibrateVector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    CalibrationOn = false;
}
void MainWindow::on_back8_clicked()
{
    if(ui->back8->styleSheet() == ui->up5->styleSheet())
        ui->back8->setStyleSheet("background-color: white");
    else if(ui->back8->styleSheet() == ui->front5->styleSheet())
        ui->back8->setStyleSheet("background-color: orange");
    else if(ui->back8->styleSheet() == ui->left5->styleSheet())
        ui->back8->setStyleSheet("background-color: red");
    else if(ui->back8->styleSheet() == ui->right5->styleSheet())
        ui->back8->setStyleSheet("background-color: green");
    else if(ui->back8->styleSheet() == ui->down5->styleSheet())
        ui->back8->setStyleSheet("background-color: yellow");
    else if(ui->back8->styleSheet() == ui->back5->styleSheet())
        ui->back8->setStyleSheet("background-color: blue");
}
void MainWindow::on_back2_clicked()
{
    if(ui->back2->styleSheet() == ui->up5->styleSheet())
        ui->back2->setStyleSheet("background-color: white");
    else if(ui->back2->styleSheet() == ui->front5->styleSheet())
        ui->back2->setStyleSheet("background-color: orange");
    else if(ui->back2->styleSheet() == ui->left5->styleSheet())
        ui->back2->setStyleSheet("background-color: red");
    else if(ui->back2->styleSheet() == ui->right5->styleSheet())
        ui->back2->setStyleSheet("background-color: green");
    else if(ui->back2->styleSheet() == ui->down5->styleSheet())
        ui->back2->setStyleSheet("background-color: yellow");
    else if(ui->back2->styleSheet() == ui->back5->styleSheet())
        ui->back2->setStyleSheet("background-color: blue");
}
void MainWindow::on_back6_clicked()
{
    if(ui->back6->styleSheet() == ui->up5->styleSheet())
        ui->back6->setStyleSheet("background-color: white");
    else if(ui->back6->styleSheet() == ui->front5->styleSheet())
        ui->back6->setStyleSheet("background-color: orange");
    else if(ui->back6->styleSheet() == ui->left5->styleSheet())
        ui->back6->setStyleSheet("background-color: red");
    else if(ui->back6->styleSheet() == ui->right5->styleSheet())
        ui->back6->setStyleSheet("background-color: green");
    else if(ui->back6->styleSheet() == ui->down5->styleSheet())
        ui->back6->setStyleSheet("background-color: yellow");
    else if(ui->back6->styleSheet() == ui->back5->styleSheet())
        ui->back6->setStyleSheet("background-color: blue");
}
void MainWindow::on_back4_clicked()
{
    if(ui->back4->styleSheet() == ui->up5->styleSheet())
        ui->back4->setStyleSheet("background-color: white");
    else if(ui->back4->styleSheet() == ui->front5->styleSheet())
        ui->back4->setStyleSheet("background-color: orange");
    else if(ui->back4->styleSheet() == ui->left5->styleSheet())
        ui->back4->setStyleSheet("background-color: red");
    else if(ui->back4->styleSheet() == ui->right5->styleSheet())
        ui->back4->setStyleSheet("background-color: green");
    else if(ui->back4->styleSheet() == ui->down5->styleSheet())
        ui->back4->setStyleSheet("background-color: yellow");
    else if(ui->back4->styleSheet() == ui->back5->styleSheet())
        ui->back4->setStyleSheet("background-color: blue");
}
void MainWindow::on_down8_clicked()
{
    if(ui->down8->styleSheet() == ui->up5->styleSheet())
        ui->down8->setStyleSheet("background-color: white");
    else if(ui->down8->styleSheet() == ui->front5->styleSheet())
        ui->down8->setStyleSheet("background-color: orange");
    else if(ui->down8->styleSheet() == ui->left5->styleSheet())
        ui->down8->setStyleSheet("background-color: red");
    else if(ui->down8->styleSheet() == ui->right5->styleSheet())
        ui->down8->setStyleSheet("background-color: green");
    else if(ui->down8->styleSheet() == ui->down5->styleSheet())
        ui->down8->setStyleSheet("background-color: yellow");
    else if(ui->down8->styleSheet() == ui->back5->styleSheet())
        ui->down8->setStyleSheet("background-color: blue");
}
void MainWindow::on_down6_clicked()
{
    if(ui->down6->styleSheet() == ui->up5->styleSheet())
        ui->down6->setStyleSheet("background-color: white");
    else if(ui->down6->styleSheet() == ui->front5->styleSheet())
        ui->down6->setStyleSheet("background-color: orange");
    else if(ui->down6->styleSheet() == ui->left5->styleSheet())
        ui->down6->setStyleSheet("background-color: red");
    else if(ui->down6->styleSheet() == ui->right5->styleSheet())
        ui->down6->setStyleSheet("background-color: green");
    else if(ui->down6->styleSheet() == ui->down5->styleSheet())
        ui->down6->setStyleSheet("background-color: yellow");
    else if(ui->down6->styleSheet() == ui->back5->styleSheet())
        ui->down6->setStyleSheet("background-color: blue");
}
void MainWindow::on_down4_clicked()
{
    if(ui->down4->styleSheet() == ui->up5->styleSheet())
        ui->down4->setStyleSheet("background-color: white");
    else if(ui->down4->styleSheet() == ui->front5->styleSheet())
        ui->down4->setStyleSheet("background-color: orange");
    else if(ui->down4->styleSheet() == ui->left5->styleSheet())
        ui->down4->setStyleSheet("background-color: red");
    else if(ui->down4->styleSheet() == ui->right5->styleSheet())
        ui->down4->setStyleSheet("background-color: green");
    else if(ui->down4->styleSheet() == ui->down5->styleSheet())
        ui->down4->setStyleSheet("background-color: yellow");
    else if(ui->down4->styleSheet() == ui->back5->styleSheet())
        ui->down4->setStyleSheet("background-color: blue");
}
void MainWindow::on_down2_clicked()
{
    if(ui->down2->styleSheet() == ui->up5->styleSheet())
        ui->down2->setStyleSheet("background-color: white");
    else if(ui->down2->styleSheet() == ui->front5->styleSheet())
        ui->down2->setStyleSheet("background-color: orange");
    else if(ui->down2->styleSheet() == ui->left5->styleSheet())
        ui->down2->setStyleSheet("background-color: red");
    else if(ui->down2->styleSheet() == ui->right5->styleSheet())
        ui->down2->setStyleSheet("background-color: green");
    else if(ui->down2->styleSheet() == ui->down5->styleSheet())
        ui->down2->setStyleSheet("background-color: yellow");
    else if(ui->down2->styleSheet() == ui->back5->styleSheet())
        ui->down2->setStyleSheet("background-color: blue");
}
void MainWindow::on_front8_clicked()
{
    if(ui->front8->styleSheet() == ui->up5->styleSheet())
        ui->front8->setStyleSheet("background-color: white");
    else if(ui->front8->styleSheet() == ui->front5->styleSheet())
        ui->front8->setStyleSheet("background-color: orange");
    else if(ui->front8->styleSheet() == ui->left5->styleSheet())
        ui->front8->setStyleSheet("background-color: red");
    else if(ui->front8->styleSheet() == ui->right5->styleSheet())
        ui->front8->setStyleSheet("background-color: green");
    else if(ui->front8->styleSheet() == ui->down5->styleSheet())
        ui->front8->setStyleSheet("background-color: yellow");
    else if(ui->front8->styleSheet() == ui->back5->styleSheet())
        ui->front8->setStyleSheet("background-color: blue");
}
void MainWindow::on_front4_clicked()
{
    if(ui->front4->styleSheet() == ui->up5->styleSheet())
        ui->front4->setStyleSheet("background-color: white");
    else if(ui->front4->styleSheet() == ui->front5->styleSheet())
        ui->front4->setStyleSheet("background-color: orange");
    else if(ui->front4->styleSheet() == ui->left5->styleSheet())
        ui->front4->setStyleSheet("background-color: red");
    else if(ui->front4->styleSheet() == ui->right5->styleSheet())
        ui->front4->setStyleSheet("background-color: green");
    else if(ui->front4->styleSheet() == ui->down5->styleSheet())
        ui->front4->setStyleSheet("background-color: yellow");
    else if(ui->front4->styleSheet() == ui->back5->styleSheet())
        ui->front4->setStyleSheet("background-color: blue");
}
void MainWindow::on_front6_clicked()
{
    if(ui->front6->styleSheet() == ui->up5->styleSheet())
        ui->front6->setStyleSheet("background-color: white");
    else if(ui->front6->styleSheet() == ui->front5->styleSheet())
        ui->front6->setStyleSheet("background-color: orange");
    else if(ui->front6->styleSheet() == ui->left5->styleSheet())
        ui->front6->setStyleSheet("background-color: red");
    else if(ui->front6->styleSheet() == ui->right5->styleSheet())
        ui->front6->setStyleSheet("background-color: green");
    else if(ui->front6->styleSheet() == ui->down5->styleSheet())
        ui->front6->setStyleSheet("background-color: yellow");
    else if(ui->front6->styleSheet() == ui->back5->styleSheet())
        ui->front6->setStyleSheet("background-color: blue");
}
void MainWindow::on_front2_clicked()
{
    if(ui->front2->styleSheet() == ui->up5->styleSheet())
        ui->front2->setStyleSheet("background-color: white");
    else if(ui->front2->styleSheet() == ui->front5->styleSheet())
        ui->front2->setStyleSheet("background-color: orange");
    else if(ui->front2->styleSheet() == ui->left5->styleSheet())
        ui->front2->setStyleSheet("background-color: red");
    else if(ui->front2->styleSheet() == ui->right5->styleSheet())
        ui->front2->setStyleSheet("background-color: green");
    else if(ui->front2->styleSheet() == ui->down5->styleSheet())
        ui->front2->setStyleSheet("background-color: yellow");
    else if(ui->front2->styleSheet() == ui->back5->styleSheet())
        ui->front2->setStyleSheet("background-color: blue");
}
void MainWindow::on_left8_clicked()
{
    if(ui->left8->styleSheet() == ui->up5->styleSheet())
        ui->left8->setStyleSheet("background-color: white");
    else if(ui->left8->styleSheet() == ui->front5->styleSheet())
        ui->left8->setStyleSheet("background-color: orange");
    else if(ui->left8->styleSheet() == ui->left5->styleSheet())
        ui->left8->setStyleSheet("background-color: red");
    else if(ui->left8->styleSheet() == ui->right5->styleSheet())
        ui->left8->setStyleSheet("background-color: green");
    else if(ui->left8->styleSheet() == ui->down5->styleSheet())
        ui->left8->setStyleSheet("background-color: yellow");
    else if(ui->left8->styleSheet() == ui->back5->styleSheet())
        ui->left8->setStyleSheet("background-color: blue");
}
void MainWindow::on_left4_clicked()
{
    if(ui->left4->styleSheet() == ui->up5->styleSheet())
        ui->left4->setStyleSheet("background-color: white");
    else if(ui->left4->styleSheet() == ui->front5->styleSheet())
        ui->left4->setStyleSheet("background-color: orange");
    else if(ui->left4->styleSheet() == ui->left5->styleSheet())
        ui->left4->setStyleSheet("background-color: red");
    else if(ui->left4->styleSheet() == ui->right5->styleSheet())
        ui->left4->setStyleSheet("background-color: green");
    else if(ui->left4->styleSheet() == ui->down5->styleSheet())
        ui->left4->setStyleSheet("background-color: yellow");
    else if(ui->left4->styleSheet() == ui->back5->styleSheet())
        ui->left4->setStyleSheet("background-color: blue");
}
void MainWindow::on_left2_clicked()
{
    if(ui->left2->styleSheet() == ui->up5->styleSheet())
        ui->left2->setStyleSheet("background-color: white");
    else if(ui->left2->styleSheet() == ui->front5->styleSheet())
        ui->left2->setStyleSheet("background-color: orange");
    else if(ui->left2->styleSheet() == ui->left5->styleSheet())
        ui->left2->setStyleSheet("background-color: red");
    else if(ui->left2->styleSheet() == ui->right5->styleSheet())
        ui->left2->setStyleSheet("background-color: green");
    else if(ui->left2->styleSheet() == ui->down5->styleSheet())
        ui->left2->setStyleSheet("background-color: yellow");
    else if(ui->left2->styleSheet() == ui->back5->styleSheet())
        ui->left2->setStyleSheet("background-color: blue");
}
void MainWindow::on_left6_clicked()
{
    if(ui->left6->styleSheet() == ui->up5->styleSheet())
        ui->left6->setStyleSheet("background-color: white");
    else if(ui->left6->styleSheet() == ui->front5->styleSheet())
        ui->left6->setStyleSheet("background-color: orange");
    else if(ui->left6->styleSheet() == ui->left5->styleSheet())
        ui->left6->setStyleSheet("background-color: red");
    else if(ui->left6->styleSheet() == ui->right5->styleSheet())
        ui->left6->setStyleSheet("background-color: green");
    else if(ui->left6->styleSheet() == ui->down5->styleSheet())
        ui->left6->setStyleSheet("background-color: yellow");
    else if(ui->left6->styleSheet() == ui->back5->styleSheet())
        ui->left6->setStyleSheet("background-color: blue");
}
void MainWindow::on_right4_clicked()
{
    if(ui->right4->styleSheet() == ui->up5->styleSheet())
        ui->right4->setStyleSheet("background-color: white");
    else if(ui->right4->styleSheet() == ui->front5->styleSheet())
        ui->right4->setStyleSheet("background-color: orange");
    else if(ui->right4->styleSheet() == ui->left5->styleSheet())
        ui->right4->setStyleSheet("background-color: red");
    else if(ui->right4->styleSheet() == ui->right5->styleSheet())
        ui->right4->setStyleSheet("background-color: green");
    else if(ui->right4->styleSheet() == ui->down5->styleSheet())
        ui->right4->setStyleSheet("background-color: yellow");
    else if(ui->right4->styleSheet() == ui->back5->styleSheet())
        ui->right4->setStyleSheet("background-color: blue");
}
void MainWindow::on_right8_clicked()
{
    if(ui->right8->styleSheet() == ui->up5->styleSheet())
        ui->right8->setStyleSheet("background-color: white");
    else if(ui->right8->styleSheet() == ui->front5->styleSheet())
        ui->right8->setStyleSheet("background-color: orange");
    else if(ui->right8->styleSheet() == ui->left5->styleSheet())
        ui->right8->setStyleSheet("background-color: red");
    else if(ui->right8->styleSheet() == ui->right5->styleSheet())
        ui->right8->setStyleSheet("background-color: green");
    else if(ui->right8->styleSheet() == ui->down5->styleSheet())
        ui->right8->setStyleSheet("background-color: yellow");
    else if(ui->right8->styleSheet() == ui->back5->styleSheet())
        ui->right8->setStyleSheet("background-color: blue");
}
void MainWindow::on_right6_clicked()
{
    if(ui->right6->styleSheet() == ui->up5->styleSheet())
        ui->right6->setStyleSheet("background-color: white");
    else if(ui->right6->styleSheet() == ui->front5->styleSheet())
        ui->right6->setStyleSheet("background-color: orange");
    else if(ui->right6->styleSheet() == ui->left5->styleSheet())
        ui->right6->setStyleSheet("background-color: red");
    else if(ui->right6->styleSheet() == ui->right5->styleSheet())
        ui->right6->setStyleSheet("background-color: green");
    else if(ui->right6->styleSheet() == ui->down5->styleSheet())
        ui->right6->setStyleSheet("background-color: yellow");
    else if(ui->right6->styleSheet() == ui->back5->styleSheet())
        ui->right6->setStyleSheet("background-color: blue");
}
void MainWindow::on_right2_clicked()
{
    if(ui->right2->styleSheet() == ui->up5->styleSheet())
        ui->right2->setStyleSheet("background-color: white");
    else if(ui->right2->styleSheet() == ui->front5->styleSheet())
        ui->right2->setStyleSheet("background-color: orange");
    else if(ui->right2->styleSheet() == ui->left5->styleSheet())
        ui->right2->setStyleSheet("background-color: red");
    else if(ui->right2->styleSheet() == ui->right5->styleSheet())
        ui->right2->setStyleSheet("background-color: green");
    else if(ui->right2->styleSheet() == ui->down5->styleSheet())
        ui->right2->setStyleSheet("background-color: yellow");
    else if(ui->right2->styleSheet() == ui->back5->styleSheet())
        ui->right2->setStyleSheet("background-color: blue");
}
void MainWindow::on_up8_clicked()
{
    if(ui->up8->styleSheet() == ui->up5->styleSheet())
        ui->up8->setStyleSheet("background-color: white");
    else if(ui->up8->styleSheet() == ui->front5->styleSheet())
        ui->up8->setStyleSheet("background-color: orange");
    else if(ui->up8->styleSheet() == ui->left5->styleSheet())
        ui->up8->setStyleSheet("background-color: red");
    else if(ui->up8->styleSheet() == ui->right5->styleSheet())
        ui->up8->setStyleSheet("background-color: green");
    else if(ui->up8->styleSheet() == ui->down5->styleSheet())
        ui->up8->setStyleSheet("background-color: yellow");
    else if(ui->up8->styleSheet() == ui->back5->styleSheet())
        ui->up8->setStyleSheet("background-color: blue");
}
void MainWindow::on_up4_clicked()
{
    if(ui->up4->styleSheet() == ui->up5->styleSheet())
        ui->up4->setStyleSheet("background-color: white");
    else if(ui->up4->styleSheet() == ui->front5->styleSheet())
        ui->up4->setStyleSheet("background-color: orange");
    else if(ui->up4->styleSheet() == ui->left5->styleSheet())
        ui->up4->setStyleSheet("background-color: red");
    else if(ui->up4->styleSheet() == ui->right5->styleSheet())
        ui->up4->setStyleSheet("background-color: green");
    else if(ui->up4->styleSheet() == ui->down5->styleSheet())
        ui->up4->setStyleSheet("background-color: yellow");
    else if(ui->up4->styleSheet() == ui->back5->styleSheet())
        ui->up4->setStyleSheet("background-color: blue");
}
void MainWindow::on_up6_clicked()
{
    if(ui->up6->styleSheet() == ui->up5->styleSheet())
        ui->up6->setStyleSheet("background-color: white");
    else if(ui->up6->styleSheet() == ui->front5->styleSheet())
        ui->up6->setStyleSheet("background-color: orange");
    else if(ui->up6->styleSheet() == ui->left5->styleSheet())
        ui->up6->setStyleSheet("background-color: red");
    else if(ui->up6->styleSheet() == ui->right5->styleSheet())
        ui->up6->setStyleSheet("background-color: green");
    else if(ui->up6->styleSheet() == ui->down5->styleSheet())
        ui->up6->setStyleSheet("background-color: yellow");
    else if(ui->up6->styleSheet() == ui->back5->styleSheet())
        ui->up6->setStyleSheet("background-color: blue");
}
void MainWindow::on_up2_clicked()
{
    if(ui->up2->styleSheet() == ui->up5->styleSheet())
        ui->up2->setStyleSheet("background-color: white");
    else if(ui->up2->styleSheet() == ui->front5->styleSheet())
        ui->up2->setStyleSheet("background-color: orange");
    else if(ui->up2->styleSheet() == ui->left5->styleSheet())
        ui->up2->setStyleSheet("background-color: red");
    else if(ui->up2->styleSheet() == ui->right5->styleSheet())
        ui->up2->setStyleSheet("background-color: green");
    else if(ui->up2->styleSheet() == ui->down5->styleSheet())
        ui->up2->setStyleSheet("background-color: yellow");
    else if(ui->up2->styleSheet() == ui->back5->styleSheet())
        ui->up2->setStyleSheet("background-color: blue");
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
void MainWindow::on_camadasReal_clicked()
{
    on_Cross_clicked();
    on_Quinas_clicked();
    on_segundaCamada_clicked();
    on_CrossDown_clicked();
    on_CantosDown_clicked();
    on_Place_clicked();
    on_Turn_clicked();
}
void MainWindow::on_otimoReal_clicked()
{
    QString input = "";
    input += color(ui->right3->styleSheet());
    input += color(ui->right6->styleSheet());
    input += color(ui->right9->styleSheet());
    input += color(ui->right2->styleSheet());
    input += color(ui->right8->styleSheet());
    input += color(ui->right1->styleSheet());
    input += color(ui->right4->styleSheet());
    input += color(ui->right7->styleSheet());
    input += color(ui->up3->styleSheet());
    input += color(ui->up6->styleSheet());
    input += color(ui->up9->styleSheet());
    input += color(ui->front3->styleSheet());
    input += color(ui->front6->styleSheet());
    input += color(ui->front9->styleSheet());
    input += color(ui->down3->styleSheet());
    input += color(ui->down6->styleSheet());
    input += color(ui->down9->styleSheet());
    input += color(ui->back3->styleSheet());
    input += color(ui->back6->styleSheet());
    input += color(ui->back9->styleSheet());
    input += color(ui->up2->styleSheet());
    input += color(ui->up8->styleSheet());
    input += color(ui->front2->styleSheet());
    input += color(ui->front8->styleSheet());
    input += color(ui->down2->styleSheet());
    input += color(ui->down8->styleSheet());
    input += color(ui->back2->styleSheet());
    input += color(ui->back8->styleSheet());
    input += color(ui->up1->styleSheet());
    input += color(ui->up4->styleSheet());
    input += color(ui->up7->styleSheet());
    input += color(ui->front1->styleSheet());
    input += color(ui->front4->styleSheet());
    input += color(ui->front7->styleSheet());
    input += color(ui->down1->styleSheet());
    input += color(ui->down4->styleSheet());
    input += color(ui->down7->styleSheet());
    input += color(ui->back1->styleSheet());
    input += color(ui->back4->styleSheet());
    input += color(ui->back7->styleSheet());
    input += color(ui->left3->styleSheet());
    input += color(ui->left6->styleSheet());
    input += color(ui->left9->styleSheet());
    input += color(ui->left2->styleSheet());
    input += color(ui->left8->styleSheet());
    input += color(ui->left1->styleSheet());
    input += color(ui->left4->styleSheet());
    input += color(ui->left7->styleSheet());
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
void MainWindow::on_toggleMode_clicked()
{
}
void MainWindow::on_pushButton_3_clicked()
{
    if(simulacao == 1)
    {
        ui->simulation->setPlainText(QString("Arduino"));
        simulacao = 0;
    }
    else if(simulacao == 0)
    {
        ui->simulation->setPlainText(QString("Simulação"));
        simulacao = 1;
    }
}
