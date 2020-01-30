#include "individual.h"
#include <iostream>
#include <cstdlib>

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

void Individual::mutation(){
	bool done = false;
	do {
		randomRouteA = this->routes[rand() % this->routes.size()]; //+1
		randomRouteB = this->routes[rand() % this->routes.size()];

		if (randomRouteA.size() == 0 and randomRouteB.size() != 0){
			random_customer = rand() % randomRouteB.size();
			if (randomRouteA.canAddCustomer(random_customer)){
				randomRouteA.add(random_customer);
				//function to remove random_custom de randomRouteB
				done = true;
			}	
		}
		else if (randomRouteA.size() != 0 and randomRouteB.size() == 0){
			random_customer = rand() % randomRouteA.size();
			if (randomRouteB.canAddCustomer(random_customer)){
				randomRouteB.add(random_customer);
				//function to remove random_custom de randomRouteB
				done = true;
			}
		}
		else{
			customerA = rand() % randomRouteA.size();
			customerB = rand() % randomRouteB.size();
		}
	}
	while (!done)
}