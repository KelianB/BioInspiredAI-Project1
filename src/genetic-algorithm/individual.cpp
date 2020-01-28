#include "individual.h"
#include <iostream>

Individual::Individual(vector<Route> routes) {
    this->routes = routes;
}

float Individual::fitness() {
    float totalDist = 0;
    for(int i = 0; i < routes.size(); i++){
    	std::cout <<"distance de la route" << routes[i].getTotalDistance();
        totalDist += routes[i].getTotalDistance();
    }
    cout << "\nTotal distance of route: " << totalDist;
    return 1 / (totalDist + 0.00000001); // prevent divisions by zero
}