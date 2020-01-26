#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "individual.h"

class Population {
    private:
        std::vector<Individual> individuals;
    
    public:
        Population();

        Individual getFittestIndividual();
        void addIndividual(Individual ind) {}
        void insertIndividuals(std::vector<Individual> inds);
};

#endif