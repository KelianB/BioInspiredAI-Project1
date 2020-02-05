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
 
        Individual& getFittestIndividual();
        Individual* getFittestLegalIndividual();
        vector<Individual>& getIndividuals() {return individuals;};
        
        void addIndividual(Individual ind);
        void insertIndividuals(std::vector<Individual> inds, int numberOfElites);
        
        float calculateAverageDistance();
        float calculateDistanceDeviation();
        
        int getNumberOfIllegalRoutes();
};

#endif