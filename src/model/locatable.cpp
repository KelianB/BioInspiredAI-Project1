#include "locatable.h"
#include <cmath>

Locatable::Locatable(float x, float y) {
    this->x = x;
    this->y = y;
}

float Locatable::distanceTo(Locatable l) {
    return sqrt(pow(getX() - l.getX(), 2) + pow(getY() - l.getY(), 2));
}   