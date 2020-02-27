#include "square.h"

Color Square::getColor() { return Utils::getColor(squarePiece); }
void Square::setColor(Color color) { Utils::setColor(color, squarePiece); }

void Square::repaint()
{
    squarePiece->repaint();
}