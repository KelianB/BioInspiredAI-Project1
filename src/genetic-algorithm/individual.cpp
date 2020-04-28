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
    this->shouldUpdateFitness = true;
}

float Individual::getTotalDistance() {
    float totalDist = 0;
    for(int i = 0; i < routes.size(); i++)
        totalDist += routes[i].getTotalDistance();
    return totalDist;
}

float Individual::getFitness() {
    // Update only when required (saves about 88% of fitness computation)
    if(this->shouldUpdateFitness) {
        float dist = 0;
        for(int i = 0; i < routes.size(); i++) {
            dist += routes[i].getTotalDistance();

            // Add a penalty when the distance is above the maximum allowed
            float distanceOverflow = routes[i].getTotalDistance() - routes[i].getDepot().getMaxRouteDuration();
            if(distanceOverflow > 0)
                dist += 100 * distanceOverflow;
        }
        this->shouldUpdateFitness = false;
        this->fitness = 1 / pow(dist, 3);
    }

    return this->fitness;
}

bool tryCustomerSwap(Route* routeA, Route* routeB, int customerA, int customerB) {
    vector<int>::iterator posOfA = std::find(routeA->getCustomers().begin(), routeA->getCustomers().end(), customerA);
    vector<int>::iterator posOfB = std::find(routeB->getCustomers().begin(), routeB->getCustomers().end(), customerB);

    if(routeA->canReplaceCustomer(customerB, posOfA - routeA->getCustomers().begin()) && 
        routeB->canReplaceCustomer(customerA, posOfB - routeB->getCustomers().begin())) {
        routeA->removeCustomer(customerA);
        routeA->insertCustomer(customerB, posOfA);
        routeB->removeCustomer(customerB);
        routeB->insertCustomer(customerA, posOfB);
        return true;
    }
    return false;
}

bool tryCustomerMove(Route* from, Route* to, int customerNumber, vector<int>::iterator positionInDestination) {
    if(to->canInsertCustomer(customerNumber, positionInDestination - to->getCustomers().begin())) {
        from->removeCustomer(customerNumber);
        to->insertCustomer(customerNumber, positionInDestination);
        return true;
    }
    return false;
}

bool tryCustomerMove(Route* from, Route* to, int customerNumber) {
    return tryCustomerMove(from, to, customerNumber, to->getCustomers().end());
}

void Individual::mutationSwap() {
    bool debug = false;

	bool done = false;
	do {
        if(debug) cout << "\n[Mutation] Attempting swap mutation.";
        // Choose two random routes
        int routeIndexA = rd::gen(this->routes.size());
        int routeIndexB = rd::gen(this->routes.size());
        Route* randomRouteA = &this->routes[routeIndexA];
		Route* randomRouteB = &this->routes[routeIndexB];
        int routeASize = randomRouteA->getCustomers().size(), routeBSize = randomRouteB->getCustomers().size();
        if(debug) cout << "\n[Mutation] Routes chosen (A: " << routeIndexA << ", B: " << routeIndexB << ")";
		// If both routes are not empty
		if(routeASize != 0 && routeBSize != 0) {
            if(routeIndexA == routeIndexB && routeASize <= 2) // Avoid useless mutations
                continue;
            int customerA = randomRouteA->getCustomers()[rd::gen(routeASize)];
            int customerB = randomRouteB->getCustomers()[rd::gen(routeBSize)];
            
            if(debug) cout << "\n[Mutation] Trying to swap customer #" << customerA << " from route A with customer #" << customerB << " from route B .";
			done = tryCustomerSwap(randomRouteA, randomRouteB, customerA, customerB);
		}
	}
	while (!done);

    this->shouldUpdateFitness = true;

    if(debug) cout << "\n[Mutation] FINISHED";
}

void Individual::mutationMove() {
    bool debug = false;
	bool done = false;
	do {
        if(debug) cout << "\n[Mutation] Attempting move mutation.";
        int routeIndexA = rd::gen(this->routes.size());
        int routeIndexB = rd::gen(this->routes.size());
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
            int randomPos = rd::gen(routeBSize);
            
            if(debug) cout << "\n[Mutation] Trying to move customer #" << customerA << " from route A to route B";
			done = tryCustomerMove(randomRouteA, randomRouteB, customerA, randomRouteB->getCustomers().begin() + randomPos);
		}
	}
	while (!done);

    this->shouldUpdateFitness = true;

    if(debug) cout << "\n[Mutation] FINISHED";
}

void Individual::mutationInversion() {
    bool debug = false;
    bool done = false;

    while(!done) {
        Route& route = getRoutes()[rd::gen(getRoutes().size())];
        int startIndex = rd::gen(route.getCustomers().size());
        int endIndex = rd::gen(route.getCustomers().size());
        if(startIndex == endIndex)
            continue;
        
        if(endIndex < startIndex) {
            int temp = endIndex;
            endIndex = startIndex;
            startIndex = temp;
        }

        if(debug) {
            cout << "\n" << startIndex  << "<-->" << endIndex;
            cout << "\n";
            for(int i = 0; i < route.getCustomers().size(); i++)
                cout << route.getCustomers()[i] << ",";
        }

        // No legal check?
        route.reverseCustomers(startIndex, endIndex);
        
        if(debug) {
            cout << "\n";
            for(int i = 0; i < route.getCustomers().size(); i++)
                cout << route.getCustomers()[i] << ",";
        }
        done = true;
    }

    this->shouldUpdateFitness = true;
}

void Individual::mutate() {
    bool debug = false;
    float rand = rd::gen();
    if(rand < 0.33)
        mutationMove();
    else if(rand < 0.66)
        mutationSwap();
    else
        mutationInversion();

}

void Individual::print() {
    for(int i = 0; i < routes.size(); i++) {
        cout << "[";
        for(int j = 0; j < routes[i].getCustomers().size(); j++)
            cout << routes[i].getCustomers()[j] << (j == routes[i].getCustomers().size()-1 ? "" : ",");
        cout << "]";
    }
}

/*Individual Individual::crossover(Individual parentB) {
    bool debug = false;
    if(debug) {
        cout << " \n\n[CROSSOVER]";
        cout << "\nParent A:\n";
        print();
        cout << "\nParent B:\n";
        parentB.print();
    }

    Individual offspring(routes);

    // Store inserted customers
    vector<int> insertedCustomers;

    int nReinserted = 0;

    // "Choose an arbitrary part from the first parent"
    int randomRouteIndex = rd::gen(this->routes.size());
    int numberOfRoutesToCopy = 1; //+ rd::gen(this->routes.size() / 2); // copy between 1 and half of the routes 
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
        
        // Reinsertion
        if(i >= routes.size() - numberOfRoutesToCopy) {
            for(int j = 0; j < routeInB.getCustomers().size(); j++) {
                int customer = routeInB.getCustomers()[j];

                vector<int> neighbours = vector<int>(insertedCustomers.begin(), insertedCustomers.end());
                MDVRP &pb = routeInB.problem;
                sort(neighbours.begin(), neighbours.end(), [&pb, &customer](int a, int b) -> bool {
                    return pb.getDistance(pb.getCustomerByNumber(a), pb.getCustomerByNumber(customer)) < pb.getDistance(pb.getCustomerByNumber(b), pb.getCustomerByNumber(customer));
                });

                bool reinserted = false;
                if(neighbours.size() > 0) {
                    int neighbourIdx = 0;
                    do {
                        int neighbour = neighbours[neighbourIdx];
                        
                        cout << "2";
                        // Find route of this neighbour
                        int routeOfClosest = 0;
                        while(routeOfClosest < parentB.routes.size() && !parentB.routes[routeOfClosest].hasCustomer(neighbour))
                            routeOfClosest++;

                        // Find the position of the neighbour within its route
                        vector<int>::iterator insertionPos = find(parentB.routes[routeOfClosest].getCustomers().begin(), parentB.routes[routeOfClosest].getCustomers().end(), neighbour);
                        int insertionIndex = insertionPos - parentB.routes[routeOfClosest].getCustomers().begin();
                        cout << "4";
                        // Determine if we want to try inserting before or after that position, depending on the added cost
                        float costOfInsertingBefore = parentB.routes[routeOfClosest].getAddedDistanceOfInsert(customer, insertionIndex);
                        float costOfInsertingAfter = parentB.routes[routeOfClosest].getAddedDistanceOfInsert(customer, insertionIndex+1);
                        int insertionOffset [2] = {
                            costOfInsertingBefore < costOfInsertingAfter ? 0 : 1,
                            costOfInsertingBefore < costOfInsertingAfter ? 1 : 0
                        };
                        cout << "5";
                        // Try inserting
                        for(int offset : insertionOffset) {
                            if(parentB.routes[routeOfClosest].canInsertCustomer(customer, insertionIndex + offset)) {
                                parentB.routes[routeOfClosest].insertCustomer(customer, insertionPos + offset);
                                insertedCustomers.push_back(customer);
                                reinserted = true;
                                break;
                            }
                        }

                        neighbourIdx++;
                    } while(neighbourIdx < neighbours.size() && !reinserted);
                    cout << "8";
                }
                else {

                }

                if(!reinserted) {
                    cout << "\ncrossover aborted (reinserted: " << nReinserted << ")";
                    return crossover(parentB);
                }
                else {
                    nReinserted++;
                }

                // Because of our representation, we have to re-insert the customers in the overriden segments of B somewhere
                /*int targetOffspringRouteIndex = routeIndex;
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
                insertedCustomers.push_back(customer);*/

                //offspring.getRoutes()[targetOffspringRouteIndex].addCustomer(customer);
            /*} 
        }
        else {
            // Insert route in offspring
            offspring.getRoutes()[routeIndex].setCustomers(routeInB.getCustomers());
            insertedCustomers.insert(insertedCustomers.end(), routeInB.getCustomers().begin(), routeInB.getCustomers().end());
        }
    }

    if(debug) {
        cout << "\nOffspring:\n";
        offspring.print();
        cout << "\n";
        parentB.print();
    }

    offspring.shouldUpdateFitness = true;

    return offspring;
}*/

Individual crossoverAux(Individual& parentA, Individual& parentB, int sequenceSize) {
    Individual offspring(parentB.getRoutes());
    
    vector<int> routeIndicesDone;
    int numberOfReorders = 2 + rd::gen(5);

    for(int k = 0; k < numberOfReorders; k++) {
        // Get a sequence of customers of size sequenceSize from any route of parentA that is big enough
        int routeIndex = rd::gen(parentA.getRoutes().size());
        int i = 0;
        while(i < parentA.getRoutes().size() && (parentA.getRoutes()[routeIndex].getCustomers().size() < sequenceSize
            || find(routeIndicesDone.begin(), routeIndicesDone.end(), routeIndex) != routeIndicesDone.end())) {
            routeIndex = (routeIndex + i) % parentA.getRoutes().size();     
            i++;
        }

        if(i == parentA.getRoutes().size())
            return routeIndicesDone.size() > 0 ? offspring : crossoverAux(parentA, parentB, sequenceSize-1);

        Route& route = parentA.getRoutes()[routeIndex];
        int sequenceIndexOffset = rd::gen(route.getCustomers().size() -  sequenceSize);
        vector<int> sequence(route.getCustomers().begin() + sequenceIndexOffset, route.getCustomers().begin() + sequenceIndexOffset + sequenceSize);

        // Find a route in parentB that has the elements of this sequence
        i = 0;
        while(i < parentB.getRoutes().size()) {        
            bool foundOne = false, foundAll = true;
            for(int cust : sequence) {
                if(!parentB.getRoutes()[i].hasCustomer(cust))
                    foundAll = false;
                else
                    foundOne = true;
            }

            if(foundAll) // Success
                break;
            else if(foundOne) { // Failure
                i = parentB.getRoutes().size();
                break;
            }
            
            i++;
        }

        if(i == parentB.getRoutes().size())
            return routeIndicesDone.size() > 0 ? offspring : crossoverAux(parentA, parentB, sequenceSize - (rd::gen() < 0.5 ? 0 : 1));

        // Reorder the customers in the sequence in the route from parent B them so they match the order of parent A
        Route& r = offspring.getRoutes()[i];
        int posOfFirst = find(r.getCustomers().begin(), r.getCustomers().end(), sequence[0]) - r.getCustomers().begin();
        for(int i = 1; i < sequence.size(); i++) {
            int pos = find(r.getCustomers().begin(), r.getCustomers().end(), sequence[i]) - r.getCustomers().begin();
            r.swapCustomers(posOfFirst+i, pos);
            if(pos < posOfFirst && posOfFirst+i == r.getCustomers().size())
                posOfFirst--;
        }

        routeIndicesDone.push_back(routeIndex);
    }

    return offspring;
}

Individual Individual::crossover(Individual parentB) {
    return crossoverAux(*this, parentB, 5);
}

bool Individual::isLegal() {
    for(int i = 0; i < routes.size(); i++) {
        if(!routes[i].isLegal())
            return false;
    }
    return true;
}