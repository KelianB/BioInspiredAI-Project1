#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "locatable.h"

class Customer: public Locatable { 
    private:
        unsigned int number;
        unsigned int maxServiceDuration;
        unsigned int demand;
    public: 
        Customer(int number, float x, float y, int maxServiceDuration, int demand)
        : Locatable(x, y) {
            this->number = number;
            this->maxServiceDuration = maxServiceDuration;
            this->demand = demand;
        }

}; 

#endif