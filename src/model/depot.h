#ifndef DEPOT_H
#define DEPOT_H

#include "locatable.h"

class Depot: public Locatable { 
    private:
        int maxRouteDuration;
        int maxVehicleLoad;
        int number;
    public: 
        Depot(int number, int maxRouteDuration, int maxVehicleLoadx)
        : Locatable(0, 0) {
            this->maxRouteDuration = maxRouteDuration;
            this->maxVehicleLoad = maxVehicleLoad;
            this->number = number;
        }

        int getNumber() {return number;}
}; 

#endif