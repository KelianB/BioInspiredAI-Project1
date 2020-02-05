#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include "route.h"

using namespace std;

class Individual {
    private:
        vector<Route> routes;

        void mutationSwap();
        void mutationMove();
        void mutationInversion();

        float fitness;
        bool shouldUpdateFitness;
    public:
        Individual(vector<Route> routes);

        Individual crossover(Individual ind);
	    void mutate();
        void print();

        float getFitness();
        float getTotalDistance();
        bool isLegal();

        vector<Route>& getRoutes() {return routes;};

        // Comparators
        bool operator > (Individual& ind) {
            return getFitness() > ind.getFitness();
        }
        bool operator < (Individual& ind) {
            return getFitness() < ind.getFitness();
        }
};

#endif
