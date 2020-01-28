#include "locatable.h"
#include "position.h"
#include <cmath>
#include <iostream>


Position::Position(int x, int y) {
    this->set(x, y);
}

float Position::distanceTo(Position l) {
    return sqrt((pow((getX() - l.getX()), 2) + pow((getY() - l.getY()), 2)));
}   