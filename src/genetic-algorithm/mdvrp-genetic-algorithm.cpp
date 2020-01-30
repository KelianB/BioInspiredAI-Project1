#include "mdvrp-genetic-algorithm.h"
#include "mdvrp.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <list>

using namespace std;

MDVRPGeneticAlgorithm::MDVRPGeneticAlgorithm(MDVRP& pb): problem(pb) {
    
}

void MDVRPGeneticAlgorithm::buildInitialPopulation(int populationSize) {
    Population pop;
    
    for(int i = 0; i < populationSize; i++) {
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
            //Customer closestCustomer = problem.getClosestCustomer(problem.getDepots()[randomDepotIndex], customersRemaining);
            Customer closestCustomer = problem.getCustomerByNumber(customersRemaining[rand() % customersRemaining.size()]);
            
            // Try to add it to the route   
            //cout << "\nTrying to add customer " << closestCustomer.getNumber() << " to route of vehicle " << vehicleNumber << " (depot #" << randomDepotIndex << ")"; 
            if(routes[vehicleNumber].canAddCustomer(closestCustomer.getNumber())) {
                routes[vehicleNumber].addCustomer(closestCustomer.getNumber());
                //cout << " success";
                // Remove the customer from our local list
                customersRemaining.erase(std::find(customersRemaining.begin(), customersRemaining.end(), closestCustomer.getNumber()));
            }
            else {

            }
        }

        Individual ind(routes);
        float fitness = ind.fitness();
        pop.addIndividual(ind);
    }
    
    this->population = pop;

    // Test crossover
    //this->population.getIndividuals()[0].crossover(this->population.getIndividuals()[1]);

    // Test mutation
    //this->population.getIndividuals()[0].mutation();
}

void MDVRPGeneticAlgorithm::solve() {
    this->buildInitialPopulation(50);
    cout << "\nFinished generating initial population.";

    for(int i = 0; i < 10; i++) {
        cout << "\nGeneration " << (i+1);
        // Mutate
        for(int j = 0; j < population.getIndividuals().size(); j++) {
            population.getIndividuals()[j].mutation();
            population.getIndividuals()[j].mutation();
            population.getIndividuals()[j].mutation();
        }

        //cout << "\nMutated";
        // Make offspring
        vector<Individual> offsprings;
        for(int j = 0; j < population.getIndividuals().size(); j++) {
            int parentA = rand() % population.getIndividuals().size();
            int parentB = rand() % population.getIndividuals().size();
            offsprings.push_back(population.getIndividuals()[parentA].crossover(population.getIndividuals()[parentB]));
            //offspring.push_back(population.getIndividuals()[parentB].crossover(population.getIndividuals()[parentA]));
        }
        //cout << "\nCreated offspring";
        population.insertIndividuals(offsprings);
        //cout << "\nInserted offspring";

        cout << "\nBest distance: " << population.getIndividuals()[0].getTotalDistance();
    }
}