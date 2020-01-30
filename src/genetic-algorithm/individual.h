#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include "route.h"

using namespace std;

class Individual {
    private:
        vector<Route> routes;
    
    public:
        Individual(vector<Route> routes);

        float getTotalDistance();
        vector<Route>& getRoutes() {return routes;};

        void print();

        Individual crossover(Individual ind);
	    void mutation();
        float fitness();

        bool operator > (Individual& ind) {
            return fitness() > ind.fitness();
        }
        bool operator < (Individual& ind) {
            return fitness() < ind.fitness();
        }
};

#endif
