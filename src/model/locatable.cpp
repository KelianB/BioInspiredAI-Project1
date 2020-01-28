#include "locatable.h"
#include <cmath>
#include <iostream>

Locatable::Locatable(int x, int y) {
    this->x = x;
    this->y = y;
}

float Locatable::distanceTo(Locatable l) {
    return sqrt(pow(getX() - l.getX(), 2) + pow(getY() - l.getY(), 2));
}   