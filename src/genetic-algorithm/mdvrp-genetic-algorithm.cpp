#include "mdvrp-genetic-algorithm.h"
#include "mdvrp.h"
#include "roulette-wheel.h"
#include "random.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <list>
#include <chrono>

using namespace std;

MDVRPGeneticAlgorithm::MDVRPGeneticAlgorithm(MDVRP& pb): problem(pb) {

}

void MDVRPGeneticAlgorithm::buildInitialPopulation(int populationSize) {
    Population pop;
    
    vector<int> customerNumbers;
    for(int n = 0; n < problem.getCustomers().size(); n++)
        customerNumbers.push_back(problem.getCustomers()[n].getNumber());

    // Sort by decreasing demand to make initialization easier
    MDVRP &pb = problem;
    sort(customerNumbers.begin(), customerNumbers.end(), [&pb](int a, int b) -> bool {
        return pb.getCustomerByNumber(b).getDemand() < pb.getCustomerByNumber(a).getDemand();
    });
    
    for(int i = 0; i < populationSize; i++) {
        // Initialize empty routes
        vector<Route> routes = vector<Route>();
        for(int d = 0; d < problem.getDepots().size(); d++) {
            for(int v = 0; v < problem.getVehiclesPerDepot(); v++)
                routes.push_back(Route(problem, problem.getDepots()[d]));
        }

        for(int i = 0; i < customerNumbers.size(); i++) {
            int customerNumber = customerNumbers[i];
            
            // Random route
            /*int route = 0;
            do {
                route = rd::gen(routes.size());
            } while(!routes[route].canAddCustomer(customerNumber));*/
        
            // Random route of closest depot
            int closestDepotNumber = problem.getClosestDepot(problem.getCustomerByNumber(customerNumber)).getNumber();
            int route = (closestDepotNumber-1) * problem.getVehiclesPerDepot() + rd::gen(problem.getVehiclesPerDepot());
            // Revert back to random if not possible
            while(!routes[route].canAddCustomer(customerNumber))
                route = rd::gen(routes.size());
        
            // Add customer to the route
            //cout << "\Adding customer " << closestCustomer.getNumber() << " to route of vehicle " << vehicleNumber << " (depot #" << randomDepotIndex << ")"; 
            //cout << " success";
            routes[route].addCustomer(customerNumber);
        }

        pop.addIndividual(Individual(routes));
    }
    
    this->population = pop;

    // Test crossover
    //this->population.getIndividuals()[0].crossover(this->population.getIndividuals()[1]);

    // Test mutation
    //this->population.getIndividuals()[0].mutation();
}

void MDVRPGeneticAlgorithm::solve() {
    const int GENERATIONS = 10000;
    const int POPULATION_SIZE = 50;
    const int NUMBER_OF_ELITES = 5;
    const double MUTATION_RATE = 0.8;
    const double CROSSOVER_RATE = 0.5;

    this->buildInitialPopulation(POPULATION_SIZE);
    //std::cout << "\nFinished generating initial population." << " (best distance: " << population.getFittestIndividual().getTotalDistance() << ")";

    int totalTime = 0, time1 = 0, time2 = 0, time3 = 0;
    vector<float> cumulative(POPULATION_SIZE + 1);

    for(int i = 0; i < GENERATIONS; i++) {  
        auto begin = chrono::high_resolution_clock::now();  

        // ### Make offspring
        vector<Individual> offsprings = makeOffspring(POPULATION_SIZE, CROSSOVER_RATE);

        int dur1 = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - begin).count();  
        begin = chrono::high_resolution_clock::now();

        // ### Mutate
        mutate(offsprings, MUTATION_RATE);

        int dur2 = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - begin).count();  
        begin = chrono::high_resolution_clock::now();

        // ### Insert offsprings
        population.insertIndividuals(offsprings, NUMBER_OF_ELITES);

        int dur3 = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - begin).count();  
        begin = chrono::high_resolution_clock::now();
    
        if(i % 1000 == 0) {
            std::cout << "\n##### Generation " << i << " #####";
            //std::cout << "\nBest distance: " << population.getFittestIndividual().getTotalDistance();
            //std::cout << "\n";
            //population.getFittestIndividual().print();
            //std::cout << "\nAverage distance: " << population.getAverageDistance();
        }
    }
    
    std::cout << "\nTimes (ms): " << time1 << ", "<< time2 << ", "<< time3 << ", " << ", "<< totalTime << ", ";
    std::cout << "\nTime spent generating offspring: " << 100 * time1 / totalTime << " %";
    std::cout << "\nTime spent mutating: " << 100 * time2 / totalTime << " %";
    std::cout << "\nTime spent inserting in population: " << 100 * time3 / totalTime << " %";

    std::cout << "\n";
    std::cout << "\nBest distance: " << population.getFittestIndividual().getTotalDistance();
    std::cout << "\n";
}

vector<Individual> MDVRPGeneticAlgorithm::makeOffspring(int numOffsprings, float crossoverRate) {
    // Implementation of roulette wheel selection with O(log2(n)) selection
    // Start by creating an array of cumulative weights: O(n)
    float total = 0;
    vector<float> cumulative;
    cumulative[0] = 0;
    for(int j = 0; j < population.getIndividuals().size(); j++) {
        total += population.getIndividuals()[j].getFitness();        
        cumulative[j+1] = total;
    }
        
    vector<Individual> offsprings;
    for(int j = 0; j < numOffsprings; j++) {
        // int parentA = roulettewheel::spin(cumulative, total);
        if(rd::gen() < crossoverRate) {   
            int parentB = 0;
            do {
                parentB = roulettewheel::spin(cumulative, total);
            } while(j == parentB);

            if(rd::gen() < 0.5)
                offsprings.push_back(population.getIndividuals()[j].crossover(population.getIndividuals()[parentB]));
            else
                offsprings.push_back(population.getIndividuals()[parentB].crossover(population.getIndividuals()[j]));
        }
        else
            offsprings.push_back(population.getIndividuals()[j]);
    }
    return offsprings;
}

void MDVRPGeneticAlgorithm::mutate(vector<Individual>& individuals, float mutationRate) {
    for(int j = 0; j < individuals.size(); j++) {
        float mutRate = mutationRate;
        int numMutations = 0;
        while(mutRate > 0) {
            if(rd::gen() < mutRate)
                numMutations++;
            mutRate--;
        }                
        for(int m = 0; m < numMutations; m++)
            individuals[j].mutation();
    }

}