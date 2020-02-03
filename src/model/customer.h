#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>

#include "locatable.h"

class Customer : public Locatable { 
    private:
        int number;
        int maxServiceDuration;
        int demand;
    public: 
        Customer(int nb, float x, float y, int msd, int dmd): Locatable(x,y) {
            number = nb;
            maxServiceDuration = msd;
            demand = dmd;
        }

        int getNumber() {return number;}
        int getDemand() {return demand;}

        bool operator==(const Customer &c) const {
            return number == c.number;
        }
}; 

#endif