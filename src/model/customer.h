#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "locatable.h"

class Customer: public Locatable { 
    private:
        int number;
        int maxServiceDuration;
        int demand;
    public: 
        Customer(int number, float x, float y, int maxServiceDuration, int demand)
        : Locatable(x, y) {
            this->number = number;
            this->maxServiceDuration = maxServiceDuration;
            this->demand = demand;
        }

        int getNumber() {return number;}

        bool operator==(const Customer &c) const {
            return number == c.number;
        }
}; 

#endif