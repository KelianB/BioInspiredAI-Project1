#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include "route.h"

class Individual {
    private:
        std::vector<Route> routes;
    
    public:
        float fitness();
        mutate();
        Individual makeOffspring(Individual ind);
};

#endif