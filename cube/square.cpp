#include "square.h"

Color Square::getColor()
{
    if(squarePiece->styleSheet() == "background-color: blue"){
        return Color::BLUE;
    }else if(squarePiece->styleSheet() == "background-color: white"){
        return Color::WHITE;
    }else if(squarePiece->styleSheet() == "background-color: orange"){
        return Color::ORANGE;
    }else if(squarePiece->styleSheet() == "background-color: red"){
        return Color::RED;
    }else if(squarePiece->styleSheet() == "background-color: green"){
        return Color::GREEN;
    }else if(squarePiece->styleSheet() == "background-color: yellow"){
        return Color::YELLOW;
    }else{
        return Color::ERROR;
    }
}

void Square::setColor(Color color)
{
    switch (color) {
    case Color::BLUE:
        squarePiece->setStyleSheet("background-color: blue");
        break;
    case Color::WHITE:
        squarePiece->setStyleSheet("background-color: white");
        break;
    case Color::ORANGE:
        squarePiece->setStyleSheet("background-color: orange");
        break;
    case Color::RED:
        squarePiece->setStyleSheet("background-color: red");
        break;
    case Color::GREEN:
        squarePiece->setStyleSheet("background-color: green");
        break;
    case Color::YELLOW:
        squarePiece->setStyleSheet("background-color: yellow");
        break;
    default:
        squarePiece->setStyleSheet("background-color: gray");
        break;
    }
}

void Square::repaint()
{
    squarePiece->repaint();
}