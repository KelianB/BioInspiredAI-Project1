#ifndef DEPOT_H
#define DEPOT_H

#include "position.h"

class Depot { 
    private:
        int maxRouteDuration;
        int maxVehicleLoad;
        int number;
        Position pos;

    public: 
        Depot(int number, int maxRouteDuration, int maxVehicleLoad) {
            this->maxRouteDuration = maxRouteDuration;
            this->maxVehicleLoad = maxVehicleLoad;
            this->number = number;
        }

        int getNumber() {return number;}

        Position getPos() {return pos;}
}; 

#endif