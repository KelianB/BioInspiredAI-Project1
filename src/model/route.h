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
        Depot& depot;
        vector<int> customers;

        bool totalDistanceRequireUpdate;
        float totalDistance;

        bool totalDemandRequireUpdate;
        float totalDemand;
    public: 
        Route(MDVRP& problem, Depot& dep);

        MDVRP& problem;

        bool canAddCustomer(int customerNumber);
        bool canInsertCustomer(int customerNumber, int position); 
        bool canReplaceCustomer(int customerNumber, int position);
        float getAddedDistanceOfInsert(int customerNumber, int position);

        bool isLegal();

        void addCustomer(int customerNumber);
        void insertCustomer(int customerNumber, vector<int>::iterator pos);
        bool removeCustomer(int customerNumber);
        void reverseCustomers(int beginIndex, int endIndex);
        void setCustomers(vector<int> customerNumbers);
        
        bool hasCustomer(int customerNumber);
        bool isValid();

        float getTotalDistance();
        int getTotalDemand();
        vector<int>& getCustomers() {return customers;};
        Depot& getDepot() {return depot;}

        Route &operator = (const Route &r) {
            depot = r.depot; return *this; 
        }
}; 

#endif