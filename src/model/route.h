#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "locatable.h"
#include "customer.h"
#include "depot.h"

using namespace std;

class Route { 
    private:
        vector<Locatable> locations;
        float totalDistance;

        void updateTotalDistance();
    public: 
        Route(Depot depot) {};

        bool canAddCustomer(Customer customer) {return true;};
        void addCustomer(Customer customer) {};
        bool isValid();
        float getTotalDistance();
}; 

#endif