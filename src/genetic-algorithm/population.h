#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "individual.h"

class Population {
    private:
        std::vector<Individual> individuals;
    
    public:
        Individual getFittestIndividual();
        insertIndividuals(std::vector<Individual> inds);
};

#endif