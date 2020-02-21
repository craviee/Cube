#pragma once

#include <QPushButton>
#include "color.h"

class Square
{
    public:
        Square(){};
        Square(QPushButton* squarePiece) : squarePiece{squarePiece}{}
        Color getColor();
        void setColor(Color color);
        void repaint();
    private:
        QPushButton* squarePiece;

};
