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
                routes.push_back(Route(problem.getDepots()[d]));
        }

        vector<Customer> customersRemaining = problem.getCustomers();

        for(int j = 0; j < routes.size(); j++) {
            cout << "\nroute " << j << ": " << routes[j].getDepot().getX();
        }

        while(customersRemaining.size() > 0) {
            // Get a random depot
            Depot& randomDepot = problem.getDepots()[rand() % problem.getDepots().size()];
            // Get a random vehicle from the depot
            int vehicleNumber = randomDepot.getNumber() * problem.getVehiclesPerDepot() + rand() % problem.getVehiclesPerDepot();
            // Get the corresponding route
            Route& r = routes[vehicleNumber];
            // Get the closest customer to this depot
            Customer closestCustomer = problem.getClosestCustomer(randomDepot, customersRemaining);
            //Customer closestCustomer = customersRemaining[rand() % customersRemaining.size()];
            
            /*for(int j = 0; j < routes.size(); j++) {
               cout << "\n1 route " << j << ": " << routes[j].getDepot().getNumber();
            }*/
            
            // Try to add it to the route   
            if(routes[vehicleNumber].canAddCustomer(&closestCustomer)) {
                routes[vehicleNumber].addCustomer(&closestCustomer);
                /*for(int j = 0; j < routes.size(); j++) {
                   cout << "\n3 route " << j << ": " << routes[j].getDepot().getNumber();
                }*/  
                cout << "\nAdding customer " << closestCustomer.getNumber() << " to route of vehicle " << vehicleNumber << " (depot #" << r.getDepot().getNumber() << ")"; 

                // Remove the customer from our local list
                vector<Customer>::iterator position = std::find(customersRemaining.begin(), customersRemaining.end(), closestCustomer);
                customersRemaining.erase(position);
            }
            else {

            }
        }
        
        /*vector<int> test;
        for(int counter = 1; counter < 10; counter++) {

            // WRONG after a few iterations
            for(int j = 0; j < routes.size(); j++)
                cout << "\nroute " << j << ": " << routes[j].getDepot().getNumber();

            // OK
            for(int j = 0; j < problem.getDepots().size(); j++)
                cout << "\ndepot " << j << ": " << problem.getDepots()[j].getNumber();
        
            test.push_back(300000); // depending on the value we push here more or less routes have a broken depot number
        }*/


        Individual ind(routes);
        
        float fitness = ind.fitness();
        cout << "\nFitness=" << fitness;
        pop.addIndividual(ind);
    }
    
    this->population = pop;

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