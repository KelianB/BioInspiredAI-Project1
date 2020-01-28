#ifndef MDVRP_GENETIC_ALGORITHM_H
#define MDVRP_GENETIC_ALGORITHM_H

#include "population.h"
#include "mdvrp.h"

class MDVRPGeneticAlgorithm {
    private:
        MDVRP& problem;
        Population population;    
    public:
        MDVRPGeneticAlgorithm(MDVRP& pb);

        void buildInitialPopulation();
};

#endif