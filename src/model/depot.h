#ifndef DEPOT_H
#define DEPOT_H

#include "locatable.h"
#include <iostream>

class Depot : public Locatable { 
    private:
        int maxRouteDuration;
        int maxVehicleLoad;
        int number;
    public: 
        Depot(int nb, int mrd, int mvl): Locatable(0, 0) {
            maxRouteDuration = mrd;
            maxVehicleLoad = mvl;
            number = nb;
        }

        int getNumber() {return number;}

        bool operator==(const Depot &c) const {
            return number == c.number;
        }
}; 

#endif