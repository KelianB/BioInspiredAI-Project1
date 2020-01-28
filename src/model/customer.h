#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "position.h"

class Customer { 
    private:
        int number;
        int maxServiceDuration;
        int demand;
        Position pos;
    public: 
        Customer(int number, float x, float y, int maxServiceDuration, int demand) {
            this->number = number;
            this->maxServiceDuration = maxServiceDuration;
            this->demand = demand;
            pos.set(x, y);
        }

        int getNumber() {return number;}

        Position getPos() {return pos;}

        bool operator==(const Customer &c) const {
            return number == c.number;
        }
}; 

#endif