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
        int routeIndexA = rand() % this->routes.size();
        int routeIndexB = rand() % this->routes.size();
		Route* randomRouteA = &this->routes[routeIndexA]; // choose a random route
		Route* randomRouteB = &this->routes[routeIndexB]; // choose a random route
        int routeASize = randomRouteA->getCustomers().size();
        int routeBSize = randomRouteB->getCustomers().size();
        if(debug) cout << "\n[Mutation] Routes chosen (A: " << routeIndexA << ", B: " << routeIndexB << ")";
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
            if(debug) cout << "\n[Mutation] Trying to insert customer #" << customerA << " from B in route A";
			vector<int>::iterator posA = std::find(randomRouteA->getCustomers().begin(), randomRouteA->getCustomers().end(), customerA);
			if (randomRouteA->canInsertCustomer(customerB, posA - randomRouteA->getCustomers().begin())) {
                if(debug) cout << " (success)";
                randomRouteA->insertCustomer(customerB, posA);
                randomRouteB->removeCustomer(customerB);
				done = true;
			}
            if(debug) cout << "\n[Mutation] Trying to insert customer #" << customerB << " from A in route B";
            vector<int>::iterator posB = std::find(randomRouteB->getCustomers().begin(), randomRouteB->getCustomers().end(), customerB);
			if (randomRouteB->canInsertCustomer(customerA, posB - randomRouteB->getCustomers().begin())) {
                if(debug) cout << " (success)";
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
        cout << "\n\n";
        cout << "\n[CROSSOVER]";
        cout << "\nParent A:\n";
        print();
        cout << "\nParent B:\n";
        parentB.print();
    }

    Individual offspring(routes);

    // "Choose an arbitrary part from the first parent"
    int randomRouteIndex = rand() % this->routes.size();
    int numberOfRoutesToCopy = 1 + rand() % (this->routes.size() / 2); // copy between 1 and half of the routes 
    // "Copy this part into the child"
    vector<int> copiedCustomers;
    for(int i = 0; i < numberOfRoutesToCopy; i++) {
        int routeIndex = (randomRouteIndex + i) % routes.size();
        for(int j = 0; j < this->routes[routeIndex].getCustomers().size(); j++)
            copiedCustomers.push_back(this->routes[routeIndex].getCustomers()[j]);
        offspring.routes.at(routeIndex) = this->routes[routeIndex];
    }
    if(debug) cout << "\nRange of copied routes: [" << randomRouteIndex << ", " << randomRouteIndex + numberOfRoutesToCopy - 1<< "]";
    // "Copy the numbers that are not in the first part to the child starting right from cut point
    // of the copied part, using the order of the second parent, and wrapping around at the end"
    for(int i = 0; i < routes.size(); i++) {
        int routeIndex = (randomRouteIndex + numberOfRoutesToCopy + i) % routes.size(); // start after the cut-off point and wrap around the end
        // Add route at this index in parent B, after having removed customers that are in the route we copied to the chil    
        Route routeInB = parentB.routes[routeIndex];
        // Remove customers that were already in one of the copied routes
        for(int j = 0; j < copiedCustomers.size(); j++) {
            int customer = copiedCustomers[j];
            if(routeInB.hasCustomer(customer))
                routeInB.removeCustomer(customer);
        }

        if(i >= routes.size() - numberOfRoutesToCopy) {
            for(int j = 0; j < routeInB.getCustomers().size(); j++) {
                int customer = routeInB.getCustomers()[j];
                // Because of our representation, we have to re-insert the customers  in the overriden segments of B somewhere
                int targetOffspringRouteIndex = rand() % offspring.getRoutes().size();
                while(!offspring.getRoutes()[targetOffspringRouteIndex].canAddCustomer(customer)) {
                    targetOffspringRouteIndex = (targetOffspringRouteIndex + 1) % routes.size();
                }   
                offspring.getRoutes()[targetOffspringRouteIndex].addCustomer(customer);
            }
        }
        else {
            // Insert route in offspring
            offspring.getRoutes()[routeIndex].setCustomers(routeInB.getCustomers());
        }
    }

    if(debug) {
        cout << "\nOffspring:\n";
        offspring.print();
        cout << "\n";
        parentB.print();
    }

    return offspring;
}