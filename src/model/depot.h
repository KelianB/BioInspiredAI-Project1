#include "locatable.h"

#ifndef DEPOT_H
#define DEPOT_H

class Depot: public Locatable { 
    private:
        unsigned int maxRouteDuration;
        unsigned int maxVehicleLoad;
    public: 
        Depot(unsigned int maxRouteDuration, unsigned int maxVehicleLoad)
        : Locatable(0, 0) {
            this->maxRouteDuration = maxRouteDuration;
            this->maxVehicleLoad = maxVehicleLoad;
        }


}; 

#endif