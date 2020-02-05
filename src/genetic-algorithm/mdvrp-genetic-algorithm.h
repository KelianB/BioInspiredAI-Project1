#ifndef MDVRP_GENETIC_ALGORITHM_H
#define MDVRP_GENETIC_ALGORITHM_H

#include "population.h"
#include "mdvrp.h"

class MDVRPGeneticAlgorithm {
    private:
        /** ##### Hyperparameters ##### */ 
        const int GENERATIONS = 20000;
        const int POPULATION_SIZE = 50;
        const int NUMBER_OF_ELITES = 5;
        const double MUTATION_RATE = 0.8;
        const double CROSSOVER_RATE = 0;

        bool printProgress;
        MDVRP& problem;
        Population population;

        Individual createIndividual(vector<int> customerNumbers);

        // Some private methods just to split our code in well defined parts
        vector<Individual> makeOffspring(int numOffsprings, float crossoverRate);
        void insertOffspring();
        void mutate(vector<Individual>& individuals, float mutationRate);
    public:
        MDVRPGeneticAlgorithm(MDVRP& pb, bool printProgress);

        void buildInitialPopulation();
        void runGenerations(int num);
        void printState();
};

#endif