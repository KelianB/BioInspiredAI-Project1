#include "individual.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>

using namespace std;


Individual::Individual(vector<Route> routes) {
    this->routes = routes;
}

float Individual::getTotalDistance() {
    float totalDist = 0;
    for(int i = 0; i < routes.size(); i++)
        totalDist += routes[i].getTotalDistance();
    return totalDist;
}

float Individual::fitness() {
    return 1 / (getTotalDistance() + 0.00000001); // prevent divisions by zero
}

void Individual::mutation() {
    bool debug = false;

	bool done = false;
	do {
        if(debug) cout << "\n[Mutation] Attempting mutation.";
		Route* randomRouteA = &this->routes[rand() % this->routes.size()]; //choose a random route
		Route* randomRouteB = &this->routes[rand() % this->routes.size()]; //choose a random route
        int routeASize = randomRouteA->getCustomers().size();
        int routeBSize = randomRouteB->getCustomers().size();
        if(debug) cout << "\n[Mutation] Routes chosen (sizes: " << routeASize << ", " << routeBSize << ")";
 		// if exactly one of the route is empty
		if (routeASize == 0 && routeBSize != 0) { 
			if(debug) cout << "\n[Mutation] Route A is empty. Inserting customer B in route A";
            int random_customer = randomRouteB->getCustomers()[rand() % routeBSize];
			if (randomRouteA->canAddCustomer(random_customer)){
				randomRouteA->addCustomer(random_customer);
				randomRouteB->removeCustomer(random_customer);
                if(debug) cout << " (success)";
				done = true;
			}
		}
		else if (routeASize != 0 && routeBSize == 0) {
            if(debug) cout << "\n[Mutation] Route B is empty. Inserting customer A in route B";
			int random_customer = randomRouteA->getCustomers()[rand() % routeASize];
			if (randomRouteB->canAddCustomer(random_customer)){
				randomRouteB->addCustomer(random_customer);
				randomRouteA->removeCustomer(random_customer);
				done = true;
                if(debug) cout << " (success)";
			}
		}
		// If both routes are not empty
		else if(routeASize != 0 && routeBSize != 0) {
            int customerA = randomRouteA->getCustomers()[rand() % routeASize];
			int customerB = randomRouteB->getCustomers()[rand() % routeBSize];
            if(debug) cout << "\n[Mutation] Trying to insert customer B in route A";
			if (randomRouteA->canAddCustomer(customerB)) {
                if(debug) cout << " (success)";
				vector<int>::iterator posA = std::find(randomRouteA->getCustomers().begin(), randomRouteA->getCustomers().end(), customerA);
                randomRouteA->insertCustomer(customerB, posA);
                randomRouteB->removeCustomer(customerB);
				done = true;
			}
            if(debug) cout << "\n[Mutation] Trying to insert customer A in route B";
			if (randomRouteB->canAddCustomer(customerA)) {
                if(debug) cout << " (success)";
				vector<int>::iterator posB = std::find(randomRouteB->getCustomers().begin(), randomRouteB->getCustomers().end(), customerB);
				randomRouteB->insertCustomer(customerA, posB);
				randomRouteA->removeCustomer(customerA);
				done = true;
			}
		}
	}
	while (!done);
    if(debug) cout << "\n[Mutation] FINISHED";
}

void Individual::print() {
    for(int i = 0; i < routes.size(); i++) {
        cout << "[";
        for(int j = 0; j < routes[i].getCustomers().size(); j++)
            cout << routes[i].getCustomers()[j] << (j == routes[i].getCustomers().size()-1 ? "" : ",");
        cout << "]";
    }
}

Individual Individual::crossover(Individual parentB) {
    bool debug = false;
    if(debug) {
        cout << "[CROSSOVER]";
        cout << "\nParent A:\n";
        print();
        cout << "\nParent B:\n";
        parentB.print();
    }

    Individual offspring(routes);

    // "Choose an arbitrary part from the first parent"
    int randomRouteIndex = rand() % this->routes.size();
    // "Copy this part into the child"
    offspring.routes.at(randomRouteIndex) = this->routes[randomRouteIndex];
    if(debug) cout << "\nrandomRouteIndex=" << randomRouteIndex;
    // "Copy the numbers that are not in the first part to the child starting right from cut point
    // of the copied part, using the order of the second parent, and wrapping around at the end"
    for(int i = 0; i < routes.size()-1; i++) {
        int routeIndex = (randomRouteIndex + 1 + i) % routes.size(); // start after the cut-off point and wrap around the end
        // Add route at this index in parent B, after having removed customers that are in the route we copied to the chil    
        Route routeInB = parentB.routes[routeIndex];
        // Remove customers that were already in route[randomRouteIndex]
        for(int j = 0; j < this->routes[randomRouteIndex].getCustomers().size(); j++) {
            int customer = this->routes[randomRouteIndex].getCustomers()[j];
            
            if(routeInB.hasCustomer(customer))
                routeInB.removeCustomer(customer);
        }
        // Insert route in offspring
        offspring.getRoutes()[routeIndex].setCustomers(routeInB.getCustomers());
    }

    if(debug) {
        cout << "\nOffspring:\n";
        offspring.print();
    }

    return offspring;
}