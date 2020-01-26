#ifndef MDVRP_H
#define MDVRP_H

#include "customer.h"
#include "depot.h"
#include <vector>
#include <string>

using namespace std;

class MDVRP { 
    private:
        unsigned int vehiclesPerDepot;
        
        vector<Customer> customers;
        vector<Depot> depots;    
    
    public: 
        MDVRP(const char filePath[]);

        Customer getClosestCustomer(Locatable locatable, vector<Customer> customers);
        
        vector<Customer> getCustomers() {return customers;}
        vector<Depot> getDepots() {return depots;}
        unsigned int getVehiclesPerDepot() {return vehiclesPerDepot;}
}; 

#endif