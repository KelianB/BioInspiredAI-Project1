#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "individual.h"

using namespace std;

class Population {
    private:
        vector<Individual> individuals;
    
    public:
        Population();
 
        Individual getFittestIndividual();
        vector<Individual> getIndividuals() {return individuals;};
        void addIndividual(Individual ind);
        void insertIndividuals(std::vector<Individual> inds);
};

#endif