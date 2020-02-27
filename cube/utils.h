#pragma once

#include <QTime>
#include <QCoreApplication>
#include <QPushButton>
#include <QMessageBox>
#include <string>
#include "enums.h"

class Utils
{
    public:
        static std::string face2str(Face face);
        static std::string color2str(Color color);
        static void delay(int millisecondsToWait);
        static Color getColor(QPushButton *button);
        static void setColor(Color color, QPushButton *button);
        static Command int2command(int command);
        static void showDialog(std::string message);
    private:
        Utils(){}
};

