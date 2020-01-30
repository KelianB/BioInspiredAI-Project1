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


        bool canAddCustomer(int customer_number);
        void addCustomer(int customer_number);
        bool hasCustomer(int customerNumber);
        bool removeCustomer(int customerNumber);
        vector<int> getCustomers() {return customers;};
        bool isValid();
        float getTotalDistance();
        void insertCustomer(int customer_number, int pos)

        Route &operator = (const Route &r) {
            depot = r.depot; return *this; 
        }
}; 

#endif