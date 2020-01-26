#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include "route.h"

using namespace std;

class Individual {
    private:
        vector<Route> routes;
    
    public:
        Individual(vector<Route> routes) {};

        float fitness();
        void mutate();
        Individual makeOffspring(Individual ind);
};

#endif