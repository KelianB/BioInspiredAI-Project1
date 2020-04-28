#ifndef MDVRP_GENETIC_ALGORITHM_H
#define MDVRP_GENETIC_ALGORITHM_H

#include "population.h"
#include "mdvrp.h"

class MDVRPGeneticAlgorithm {
    private:
        /** ##### Hyperparameters ##### */ 
        const int POPULATION_SIZE = 50;
        const int NUMBER_OF_ELITES = 5;
        const double MUTATION_RATE = 0.4;
        const double CROSSOVER_RATE = 0.1;
        const int TERMINATION_FITNESS_STEPS = 5;

        bool printProgress;
        MDVRP& problem;
        Population population;

        bool terminated = false;
        
        float previousBestFitness;
        int numStepsWithoutFitnessChange = 0;
        int generationsRan = 0;

        Individual createIndividual(vector<int> customerNumbers);

        // Some private methods just to split our code in well defined parts
        vector<Individual> makeOffspring(int numOffsprings, float crossoverRate);
        void insertOffspring();
        void mutate(vector<Individual>& individuals, float mutationRate);
        bool shouldTerminate();
    public:
        MDVRPGeneticAlgorithm(MDVRP& pb, bool printProgress);

        void buildInitialPopulation();
        void runGenerations(int num);
        void printState();
        void outputFile();

        void terminate();

        bool isTerminated() {return terminated;}
        int getGenerationsRan() {return generationsRan;}
        Population& getPopulation() {return population;}
};

#endif