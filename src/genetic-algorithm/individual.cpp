#include "individual.h"
#include "roulette-wheel.h"
#include "random.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;


Individual::Individual(vector<Route> routes) {
    this->routes = routes;
    this->shouldRecalculateFitness = true;
}

float Individual::getTotalDistance() {
    float totalDist = 0;
    for(int i = 0; i < routes.size(); i++)
        totalDist += routes[i].getTotalDistance();
    return totalDist;
}

float Individual::getFitness() {
    if(this->shouldRecalculateFitness) {
        this->fitness = pow(1 / getTotalDistance(), 3);
        this->shouldRecalculateFitness = false;
    }
    return this->fitness;
}

bool tryCustomerMove(Route* from, Route* to, int customerNumber, vector<int>::iterator positionInDestination) {
    if (to->canInsertCustomer(customerNumber, positionInDestination - to->getCustomers().begin())) {
        from->removeCustomer(customerNumber);
        to->insertCustomer(customerNumber, positionInDestination);
        return true;
    }
    return false;
}

bool tryCustomerMove(Route* from, Route* to, int customerNumber) {
    return tryCustomerMove(from, to, customerNumber, to->getCustomers().end());
}


void Individual::mutation() {
    bool debug = false;

    /*double total = 0;
    vector<float> cumulative;
    cumulative.push_back(0);
    for(int j = 0; j < this->routes.size(); j++) {
        total += routes[j].getCustomers().size() / 2.0;        
        cumulative.push_back(total);
    }*/

	bool done = false;
	do {
        if(debug) cout << "\n[Mutation] Attempting mutation.";
        int routeIndexA = rd::gen(this->routes.size());
        int routeIndexB = rd::gen(this->routes.size());
		//int routeIndexA = roulettewheel::spin(cumulative, total);
        //int routeIndexB = roulettewheel::spin(cumulative, total);
        Route* randomRouteA = &this->routes[routeIndexA]; // choose a random route
		Route* randomRouteB = &this->routes[routeIndexB]; // choose a random route
        int routeASize = randomRouteA->getCustomers().size();
        int routeBSize = randomRouteB->getCustomers().size();
        if(debug) cout << "\n[Mutation] Routes chosen (A: " << routeIndexA << ", B: " << routeIndexB << ")";
 		// If exactly one of the routes is empty
		if (routeASize == 0 && routeBSize != 0) { 
			if(debug) cout << "\n[Mutation] Route A is empty. Inserting customer B in route A";
            int random_customer = randomRouteB->getCustomers()[rd::gen(routeBSize)];
            done = tryCustomerMove(randomRouteB, randomRouteA, random_customer);
		}
		else if (routeASize != 0 && routeBSize == 0) {
            if(debug) cout << "\n[Mutation] Route B is empty. Inserting customer A in route B";
			int random_customer = randomRouteA->getCustomers()[rd::gen(routeASize)];
			done = tryCustomerMove(randomRouteA, randomRouteB, random_customer);
		}
		// If both routes are not empty
		else if(routeASize != 0 && routeBSize != 0) {
            if(routeIndexA == routeIndexB && routeASize <= 2)
                continue;
            int customerA = randomRouteA->getCustomers()[rd::gen(routeASize)];
            int customerB = randomRouteB->getCustomers()[rd::gen(routeBSize)];
            //cout << "\n" << customerA << "," << customerB;
            vector<int>::iterator posOfA = std::find(randomRouteA->getCustomers().begin(), randomRouteA->getCustomers().end(), customerA);
            vector<int>::iterator posOfB = std::find(randomRouteB->getCustomers().begin(), randomRouteB->getCustomers().end(), customerB);

            if(debug) cout << "\n[Mutation] Trying to move customer #" << customerB << " from route B to route A";
			done = tryCustomerMove(randomRouteB, randomRouteA, customerB, posOfA);
        
            if(!done || rd::gen() < 0.5) {
                if(debug) cout << "\n[Mutation] Trying to move customer #" << customerA << " from route A to route B";
                done = tryCustomerMove(randomRouteA, randomRouteB, customerA, posOfB) || done;
            }
		}
	}
	while (!done);
    this->shouldRecalculateFitness = true;
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
    int randomRouteIndex = rd::gen(this->routes.size());
    int numberOfRoutesToCopy = 1 + rd::gen(this->routes.size() / 2); // copy between 1 and half of the routes 
    // "Copy this part into the child"
    vector<int> copiedCustomers;
    for(int i = 0; i < numberOfRoutesToCopy; i++) {
        int routeIndex = (randomRouteIndex + i) % routes.size();
        for(int j = 0; j < this->routes[routeIndex].getCustomers().size(); j++)
            copiedCustomers.push_back(this->routes[routeIndex].getCustomers()[j]);
        // offspring.routes.at(routeIndex) = this->routes[routeIndex]; // already copied since offspring is a copy of this
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
            if(routeInB.hasCustomer(customer)) {
                routeInB.removeCustomer(customer);
                //copiedCustomers.erase(copiedCustomers.begin() + j);
                //j--;
            }
        }

        if(i >= routes.size() - numberOfRoutesToCopy) {
            for(int j = 0; j < routeInB.getCustomers().size(); j++) {
                int customer = routeInB.getCustomers()[j];
                // Because of our representation, we have to re-insert the customers in the overriden segments of B somewhere
                int targetOffspringRouteIndex = routeIndex;
                while(!offspring.getRoutes()[targetOffspringRouteIndex].canAddCustomer(customer)) {
                    targetOffspringRouteIndex = (targetOffspringRouteIndex + 1) % routes.size();

                    // Stop trying if we haven't found a way to add the customer back to any route
                    if(targetOffspringRouteIndex == routeIndex) {
                        cout << "\ncrossover aborted";
                        return crossover(parentB);
                    }
                }

                Route &r = offspring.getRoutes()[targetOffspringRouteIndex];
                r.insertCustomer(customer, r.getCustomers().begin() + rd::gen(r.getCustomers().size()));
                //offspring.getRoutes()[targetOffspringRouteIndex].addCustomer(customer);
            } 
        }
        else {
            // Insert route in offspring
            offspring.getRoutes()[routeIndex].setCustomers(routeInB.getCustomers());
        }
    }

    offspring.shouldRecalculateFitness = true;

    if(debug) {
        cout << "\nOffspring:\n";
        offspring.print();
        cout << "\n";
        parentB.print();
    }

    return offspring;
}