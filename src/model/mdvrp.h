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
        vector<vector<float>> distanceMatrix;

        vector<vector<float>> buildDistanceMatrix();
    
    public: 
        MDVRP(const char filePath[]);

        Customer& getCustomerByNumber(int number);
        Customer getClosestCustomer(Locatable locatable, vector<int> customers);

        Depot& getDepotByNumber(int number);
        Depot& getClosestDepot(Customer c);

        float getDistance(Customer c, Depot d);
        float getDistance(Depot d, Customer c);
        float getDistance(Customer a, Customer b);

        vector<Customer>& getCustomers() {return customers;}
        vector<Depot>& getDepots() {return depots;}
        int getVehiclesPerDepot() {return vehiclesPerDepot;}
}; 

#endif