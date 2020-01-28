#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "locatable.h"
#include "customer.h"
#include "depot.h"

using namespace std;

class Route { 
    private:
        Depot *depot;
        vector<Customer> customers;

        bool totalDistanceRequireUpdate;
        float totalDistance;

        void updateTotalDistance();
    public: 
        Route(Depot *depot);

        Depot* getDepot() {return depot;}

        bool canAddCustomer(Customer customer);
        void addCustomer(Customer customer);
        bool isValid();
        float getTotalDistance();
}; 

#endif