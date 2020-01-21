#ifndef MDVRP_H
#define MDVRP_H

#include "customer.h"
#include "depot.h"
#include <vector>
#include <string>

class MDVRP { 
    private:
        unsigned int vehiclesPerDepot;
        std::vector<Customer> customers;
        std::vector<Depot> depots;    
    
    public: 
        MDVRP(const char filePath[]);

}; 

#endif