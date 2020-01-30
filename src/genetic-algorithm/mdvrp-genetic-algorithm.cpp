#include "mdvrp-genetic-algorithm.h"
#include "mdvrp.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <list>

using namespace std;

MDVRPGeneticAlgorithm::MDVRPGeneticAlgorithm(MDVRP& pb): problem(pb) {
    
}

void MDVRPGeneticAlgorithm::buildInitialPopulation() {
    Population pop;

    int POP_SIZE = 1;
 
    cout << "POP_SIZE=" << POP_SIZE << "\n";
    
    for(int i = 0; i < POP_SIZE; i++) {
        // Initialize empty routes
        vector<Route> routes = vector<Route>();
        for(int d = 0; d < problem.getDepots().size(); d++) {
            for(int v = 0; v < problem.getVehiclesPerDepot(); v++)
                routes.push_back(Route(problem, problem.getDepots()[d]));
        }

        vector<int> customersRemaining;
        for(int n = 0; n < problem.getCustomers().size(); n++)
            customersRemaining.push_back(problem.getCustomers()[n].getNumber());

        while(customersRemaining.size() > 0) {
            // Get a random depot
            int randomDepotIndex = rand() % problem.getDepots().size();
            // Get a random vehicle from the depot
            int vehicleNumber = randomDepotIndex * problem.getVehiclesPerDepot() + rand() % problem.getVehiclesPerDepot();
            // Get the closest customer to this depot
            Customer closestCustomer = problem.getClosestCustomer(problem.getDepots()[randomDepotIndex], customersRemaining);
            //Customer closestCustomer = customersRemaining[rand() % customersRemaining.size()];
            
            // Try to add it to the route   
            if(routes[vehicleNumber].canAddCustomer(closestCustomer)) {
                routes[vehicleNumber].addCustomer(closestCustomer);
                cout << "\nAdding customer " << closestCustomer.getNumber() << " to route of vehicle " << vehicleNumber << " (depot #" << randomDepotIndex << ")"; 

                // Remove the customer from our local list
                customersRemaining.erase(std::find(customersRemaining.begin(), customersRemaining.end(), closestCustomer.getNumber()));
            }
            else {

            }
        }

        Individual ind(routes);
        
        float fitness = ind.fitness();
        cout << "\nFitness=" << fitness;
        pop.addIndividual(ind);
    }
    
    this->population = pop;
}