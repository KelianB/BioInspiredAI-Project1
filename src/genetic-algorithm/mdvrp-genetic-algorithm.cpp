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

// Uses binary search to achieve O(log(n)) complexity
int spinRouletteWheel(double cumulativeWeights[], double totalWeights, int popSize) {
    double r = totalWeights * (rand() / (double) RAND_MAX);    
    // Look for the index of the entry just above r:
    int a = 0, b = popSize - 1;
    while(b-a > 1) {
        int mid = (a + b) / 2;
        if(cumulativeWeights[mid] > r) b = mid;
        else a = mid;
    }
    return a;
}

void MDVRPGeneticAlgorithm::solve() {
    const int POPULATION_SIZE = 50;
    this->buildInitialPopulation(POPULATION_SIZE);
    cout << "\nFinished generating initial population.";

    for(int i = 0; i < 500; i++) {
        cout << "\n##### Generation " << (i+1) << " #####";
        
        // Implementation of roulette wheel selection with O(log2(n)) selection
         
        // Start by creating an array of cumulative weights: O(n)
        double total = 0;
        double cumulative [POPULATION_SIZE + 1];
        cumulative[0] = 0;
        for(int j = 0; j < population.getIndividuals().size(); j++) {
            total += population.getIndividuals()[j].fitness();
            cumulative[j+1] = total;
        }
        
        // Make offspring
        vector<Individual> offsprings;
        /*for(int j = 0; j < population.getIndividuals().size() / 2; j++) {
            int parentA = spinRouletteWheel(cumulative, total, POPULATION_SIZE);
            int parentB = 0;
            do {
                parentB = spinRouletteWheel(cumulative, total, POPULATION_SIZE);
            } while(parentB == parentA);
            offsprings.push_back(population.getIndividuals()[parentA].crossover(population.getIndividuals()[parentB]));
            offsprings.push_back(population.getIndividuals()[parentB].crossover(population.getIndividuals()[parentA]));
        }*/
        for(int j = 0; j < population.getIndividuals().size(); j++) {
            if(rand() % 100 < 25) {   
                int parentB = 0;
                do {
                    parentB = spinRouletteWheel(cumulative, total, POPULATION_SIZE);
                } while(parentB == j);
                offsprings.push_back(population.getIndividuals()[j].crossover(population.getIndividuals()[parentB]));
            }
            else
                offsprings.push_back(population.getIndividuals()[j]);
        }


        //cout << "\nCreated offspring";
        population.insertIndividuals(offsprings);
        //cout << "\nInserted offspring";

        // Mutate
        for(int j = 0; j < population.getIndividuals().size(); j++) {
            int numMutations = rand() % 3;
            for(int m = 0; m < numMutations; m++)
                population.getIndividuals()[j].mutation();
        }
        //cout << "\nMutated";
    
        cout << "\nBest distance: " << population.getFittestIndividual().getTotalDistance();
        cout << "\nAverage distance: " << population.getAverageDistance();
    }
    cout << "\n";
    population.getFittestIndividual().print();
    cout << "\n";
}