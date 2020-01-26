#include "mdvrp-genetic-algorithm.h"
#include "mdvrp.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;

MDVRPGeneticAlgorithm::MDVRPGeneticAlgorithm(MDVRP* problem) {
    this->problem = problem;
    this->population = this->buildInitialPopulation();
}

Population MDVRPGeneticAlgorithm::buildInitialPopulation() {
    Population pop = Population();

    int POP_SIZE = 1;
 
    cout << "POP_SIZE=" << POP_SIZE << "\n";
    cout << "customers: " << problem->getCustomers().size() << "\n";

    for(int i = 0; i < POP_SIZE; i++) {
        //vector<Route> routes(problem->getDepots().size() * problem->getVehiclesPerDepot());
        // Initialize empty routes
        vector<Route> routes;
        for(int d = 0; d < problem->getDepots().size(); d++) {
            for(int v = 0; v < problem->getVehiclesPerDepot(); v++)
                routes.push_back(Route(problem->getDepots()[d]));
        }

        vector<Customer> customersRemaining;
        for(int j = 0; j < problem->getCustomers().size(); j++)
            customersRemaining.push_back(problem->getCustomers()[j]);
    
        while(customersRemaining.size() > 0) {
            /*cout << "\n";
            for(int j = 0; j < customersRemaining.size(); j++)
                cout << customersRemaining[j]->getNumber() << ",";
            cout << "\n";*/

            // Get a random depot
            Depot randomDepot = problem->getDepots()[rand() % problem->getDepots().size()];
            // Get a random vehicle from the depot
            int vehicleNumber = randomDepot.getNumber() * problem->getVehiclesPerDepot() + rand() % problem->getVehiclesPerDepot();
            // Get the corresponding route
            Route r = routes.at(vehicleNumber);
            // Get the closest customer to this depot
            Customer closestCustomer = problem->getClosestCustomer(randomDepot, customersRemaining);
            // Try to add it to the route   
            if(r.canAddCustomer(closestCustomer)) {
                r.addCustomer(closestCustomer);   
                cout << "\nAdding customer " << closestCustomer.getNumber() << " to route of vehicle " << vehicleNumber; 
                // Remove the customer from our local list
                vector<Customer>::iterator position = std::find(customersRemaining.begin(), customersRemaining.end(), closestCustomer);
                customersRemaining.erase(position);
            }
            else {

            }
        }

        Individual ind(routes);
        pop.addIndividual(ind);
    }
    
    return pop;

    /*
    routes = []
    vehicles = [1,2,3,4,5,6, 7,...]
    customersRemaining = [0001,0002,2343,3554,0230,3256]
    While customersRemaining.length > 0
        Vehicle = random(vehicles)
        route = new Route(vehicle.depot)
        pos = vehicle.depot.pos
        closestCustomer = null
        Do {
    closestCustomer=getClosestCustomer(pos, customersRemaining)
    newRoute = route.addCustomer(closestCustomer);
    newRoute = new Route(route, closestCustomer);
    if(newRoute.isValid())
    customersRemaining.remove(closestCustomer)
    route = newRoute
            else
                break
    }
        While(customersRemaining.length > 0)
            
    Add route to routes
    vehicles.remove(vehicle)

*/
}