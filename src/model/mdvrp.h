#ifndef MDVRP_H
#define MDVRP_H

#include "customer.h"
#include "depot.h"
#include "position.h"
#include <vector>
#include <string>

using namespace std;

class MDVRP { 
    private:
        int vehiclesPerDepot;
        
        vector<Customer> customers;
        vector<Depot> depots;    
    
    public: 
        MDVRP(const char filePath[]);

        Customer getClosestCustomer(Locatable locatable, vector<int> customers);
        
        vector<Customer> getCustomers() {return customers;}
        vector<Depot> getDepots() {return depots;}
        int getVehiclesPerDepot() {return vehiclesPerDepot;}
}; 

#endif