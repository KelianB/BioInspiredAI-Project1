#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include "route.h"

using namespace std;

class Individual {
    private:
        vector<Route> routes;

        bool shouldRecalculateFitness;
        float fitness;
    public:
        Individual(vector<Route> routes);

        float getTotalDistance();
        vector<Route>& getRoutes() {return routes;};

        void print();

        Individual crossover(Individual ind);
	    void mutation();
        float getFitness();

        bool operator > (Individual& ind) {
            return getFitness() > ind.getFitness();
        }
        bool operator < (Individual& ind) {
            return getFitness() < ind.getFitness();
        }
};

#endif
