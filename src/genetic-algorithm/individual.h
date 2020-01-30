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

        vector<Route>& getRoutes() {return routes;};
        float fitness();
        void mutate();
        void print();
        Individual crossover(Individual ind);
};

#endif