#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include "route.h"

using namespace std;

class Individual {
    private:
        vector<Route> routes;
    
    public:
        Individual(int numberOfRoutes);
        Individual(vector<Route> routes);

        float fitness();
        void mutate();
        Individual crossover(Individual ind);
};

#endif