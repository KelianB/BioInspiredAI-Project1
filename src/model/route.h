#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "locatable.h"
#include "customer.h"

using namespace std;

class Route { 
    private:
        vector<Locatable> locations;
        float totalDistance;

        void updateTotalDistance();
    public: 
        Route();
        Route(Route route, Customer customer);

        bool isValid();
        float getTotalDistance();
}; 

#endif