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

        bool canAddCustomer(int customerNumber);
        bool canInsertCustomer(int customerNumber, int position); 

        void addCustomer(Customer customer);
        bool hasCustomer(int customerNumber);
        bool removeCustomer(int customerNumber);
        void setCustomers(vector<int> customerNumbers);
        vector<int>& getCustomers() {return customers;};
        bool isValid();

        float getTotalDistance();
        int getTotalDemand();
        Depot getDepot() {return depot;}

        Route &operator = (const Route &r) {
            depot = r.depot; return *this; 
        }
}; 

#endif