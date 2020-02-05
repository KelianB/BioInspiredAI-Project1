#ifndef MDVRP_H
#define MDVRP_H

#include "customer.h"
#include "depot.h"

#include <vector>
#include <string>

using namespace std;

/**
 * Class responsible for reading and storing problem data.
 * Provides some utility functions.
 */ 
class MDVRP { 
    private:
        int vehiclesPerDepot;
        string problemName;

        vector<Customer> customers;
        vector<Depot> depots;    
        vector<vector<float>> distanceMatrix;

        vector<vector<float>> buildDistanceMatrix();

        // By how much we allow the distance to go over the maximum distance of a depot. 1 means all solutions will be legal. 
        float distanceToleranceFactor = 1.0;
    public: 
        MDVRP(const char filePath[], string problemName);

        Customer& getCustomerByNumber(int number);
        Customer& getClosestCustomer(Customer c, vector<int> customers);

        Depot& getDepotByNumber(int number);
        Depot& getClosestDepot(Customer c);

        float getDistance(Customer c, Depot d);
        float getDistance(Depot d, Customer c);
        float getDistance(Customer a, Customer b);

        float getDistanceToleranceFactor() {return distanceToleranceFactor;}
        void increaseDistanceToleranceFactor();

        vector<Customer>& getCustomers() {return customers;}
        vector<Depot>& getDepots() {return depots;}
        int getVehiclesPerDepot() {return vehiclesPerDepot;}
        string getProblemName() {return problemName;}
}; 

#endif