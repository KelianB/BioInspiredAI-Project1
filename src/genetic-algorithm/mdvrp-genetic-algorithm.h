#ifndef MDVRP_GENETIC_ALGORITHM_H
#define MDVRP_GENETIC_ALGORITHM_H

#include "population.h"
#include "mdvrp.h"

class MDVRPGeneticAlgorithm {
    private:
        Population population;
    
    public:
        MDVRPGeneticAlgorithm(MDVRP problem);
};

#endif