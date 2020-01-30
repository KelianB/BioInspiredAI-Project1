#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "locatable.h"
#include "customer.h"
#include "depot.h"
#include "mdvrp.h"

using namespace std;

class Route { 
    private:
        MDVRP& problem;
        Depot depot;
        vector<int> customers;

        bool totalDistanceRequireUpdate;
        float totalDistance;

        void updateTotalDistance();
    public: 
        Route(MDVRP& problem, Depot dep);

        Depot getDepot() {return depot;}

        bool canAddCustomer(Customer customer);
        void addCustomer(Customer customer);
        bool isValid();
        float getTotalDistance();

        Route &operator = (const Route &r) {
            depot = r.depot; return *this; 
        }
}; 

#endif