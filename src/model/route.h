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

<<<<<<< HEAD
        void addCustomer(Customer customer);
=======

        bool canAddCustomer(int customer_number);
        bool canInsertCustomer(int customer_number);
        void addCustomer(int customer_number);
>>>>>>> b4e6be3536907d230ccb6be1111186dd37d376eb
        bool hasCustomer(int customerNumber);
        bool removeCustomer(int customerNumber);
        void setCustomers(vector<int> customerNumbers);
        vector<int>& getCustomers() {return customers;};
        bool isValid();

        float getTotalDistance();
        void insertCustomer(int customer_number, vector<int>::iterator pos);
        int getTotalDemand();
        Depot getDepot() {return depot;}

        Route &operator = (const Route &r) {
            depot = r.depot; return *this; 
        }
}; 

#endif