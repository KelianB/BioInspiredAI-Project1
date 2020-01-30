#include "individual.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

Individual::Individual(vector<Route> routes) {
    this->routes = routes;
}

float Individual::fitness() {
    float totalDist = 0;
    for(int i = 0; i < routes.size(); i++){
    	//std::cout << "\nDistance de la route" << routes[i].getTotalDistance();
        totalDist += routes[i].getTotalDistance();
    }
    cout << "\nTotal distance of route: " << totalDist;
    return 1 / (totalDist + 0.00000001); // prevent divisions by zero
}

Individual Individual::crossover(Individual parentB) {
    cout << "\nCrossover:";
    cout << "\nParent A:";


    Individual offspring(routes);

    // "Choose an arbitrary part from the first parent"
    int randomRouteIndex = rand() % this->routes.size();
    // "Copy this part into the child"
    offspring.routes.at(randomRouteIndex) = this->routes[randomRouteIndex];
    // "Copy the numbers that are not in the first part to the child
    //  starting right from cut point of the copied part, using the order of the second parent, 
    //  and wrapping around at the end"
    for(int i = 0; i < routes.size()-1; i++) {
        int routeIndex = (randomRouteIndex + 1 + i) % routes.size(); // start after the cut-off point and wrap around the end
        // Add route at this index in parent B, after having removed customers that are in the route we copied to the child
        
        Route routeInB = parentB.routes[routeIndex];
        // Remove customers that were already in route[randomRouteIndex]
        for(int customer : routes[randomRouteIndex].getCustomers()) {
            if(routeInB.hasCustomer(customer))
                routeInB.removeCustomer(customer);
        }
        // Insert route in offspring
        offspring.routes[routeIndex] = routeInB;
    }

    return offspring;
}