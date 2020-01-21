#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "locatable.h"
#include "customer.h"


use namespace std;

class Route { 
    private:
        vector<Locatable> locations;
        float totalDistance;
        
        void updateTotalDistance();
    public: 
        boolean isValid();
        float getTotalDistance();

}; 

#endif