#ifndef MDVRP_GENETIC_ALGORITHM_H
#define MDVRP_GENETIC_ALGORITHM_H

#include "population.h"
#include "mdvrp.h"

class MDVRPGeneticAlgorithm {
    private:
        MDVRP& problem;
        Population population;

        void mutate(vector<Individual>& individuals, float mutationRate);
        vector<Individual> makeOffspring(int numOffsprings, float crossoverRate);
        void insertOffspring();
    public:
        MDVRPGeneticAlgorithm(MDVRP& pb);

        void buildInitialPopulation(int populationSize);
        void solve();
};

#endif