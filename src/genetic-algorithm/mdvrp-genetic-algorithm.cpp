#include "mdvrp-genetic-algorithm.h"
#include "mdvrp.h"
#include "roulette-wheel.h"
#include "random.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <list>
#include <chrono>
#include <random>
#include <thread>
#include <fstream>
#include <string>

using namespace std;


MDVRPGeneticAlgorithm::MDVRPGeneticAlgorithm(MDVRP& pb, bool printProgress): problem(pb) {
    this->printProgress = printProgress;
}

Individual MDVRPGeneticAlgorithm::createIndividual(vector<int> customerNumbers) {
    // Initialize empty routes
    vector<Route> routes = vector<Route>();
    for(int d = 0; d < problem.getDepots().size(); d++) {
        for(int v = 0; v < problem.getVehiclesPerDepot(); v++)
            routes.push_back(Route(problem, problem.getDepots()[d]));
    }
    
    // Shuffle the customers - helps ensuring that our individual is not created in a deterministic way
    std::shuffle(customerNumbers.begin(), customerNumbers.end(), std::default_random_engine(rd::getThreadSafeSeed()));    

    // We iterate over all the customers and try to assign them to a route one by one
    for(int i = 0; i < customerNumbers.size(); i++) {
        int customerNumber = customerNumbers[i];
        Customer& customer = problem.getCustomerByNumber(customerNumber);
        Depot& closestDepot = problem.getClosestDepot(customer);
        
        // The first customer is simply assigned to the first route of its closest depot
        if(i == 0) {
            routes[(closestDepot.getNumber() - 1) * problem.getVehiclesPerDepot()].addCustomer(customerNumber);
            continue;            
        }

        // Build a vector of customers that were already assigned to a route, 
        // sorted by increasing distance to customer
        vector<int> neighbours = vector<int>(customerNumbers.begin(), customerNumbers.begin() + i);
        MDVRP &pb = problem;
        sort(neighbours.begin(), neighbours.end(), [&pb, &customer](int a, int b) -> bool {
            return pb.getDistance(pb.getCustomerByNumber(a), customer) < pb.getDistance(pb.getCustomerByNumber(b), customer);
        });
               
        // Iterate over the neighbours (by ascending distance to customer)
        int neighbourIdx = 0;
        int maxClosestNeighbour = neighbours.size() < 10 ? neighbours.size() : 10;
        bool done = false;
        do {
            int neighbourNum = neighbours[neighbourIdx];

            // If customer is closer to its closest depot than to this neighbour, 
            // have a random chance to try assigning it to an empty route of the depot
            if(problem.getDistance(customer, closestDepot) < problem.getDistance(customer, problem.getCustomerByNumber(neighbourNum))) {
                int firstRouteOfDepot = (closestDepot.getNumber()-1) * problem.getVehiclesPerDepot();
                int j = 0;
                while(j < problem.getVehiclesPerDepot() && routes[firstRouteOfDepot + j].getCustomers().size() > 0) {
                    j++;
                }
                // Found an empty route
                if(j != problem.getVehiclesPerDepot()) {
                    routes[firstRouteOfDepot + j].addCustomer(customerNumber);
                    done = true;
                } 
            }

            if(!done) {
                // Find neighbour's route
                int route = 0;
                while(route < routes.size() && !routes[route].hasCustomer(neighbourNum))
                    route++;

                // Random chance of going to another route
                //while(rd::gen() < 0.5)
                  //  route = (route + 1) % routes.size();

                if(route == routes.size()) {
                    // Critical error: the neighbour was never added to a route, which means that a problem occured before
                    cout << "\nCritical error generating individual (neighbour should have been added to a route)\n";
                    break;
                }
                // First, try to insert at a random position (this is just to add some randomness to our initialization)
                /*int randomPosition = rd::gen(routes[route].getCustomers().size());
                if(routes[route].canInsertCustomer(customerNumber, randomPosition)) {
                    routes[route].insertCustomer(customerNumber, routes[route].getCustomers().begin() + randomPosition);
                    done = true;
                    continue;
                }                    
                else if(routes[route].hasCustomer(neighbourNum)) {
                */
                    // Find the position of the neighbour within its route
                    int insertionIndex = find(routes[route].getCustomers().begin(), routes[route].getCustomers().end(), neighbourNum) - routes[route].getCustomers().begin();

                    // Determine if we want to try inserting before or after that position, depending on the added cost
                    float costOfInsertingBefore = routes[route].getAddedDistanceOfInsert(customerNumber, insertionIndex);
                    float costOfInsertingAfter = routes[route].getAddedDistanceOfInsert(customerNumber, insertionIndex+1);
                    insertionIndex = costOfInsertingBefore < costOfInsertingAfter ? insertionIndex : insertionIndex + 1;
                    
                    if(routes[route].canInsertCustomer(customerNumber, insertionIndex)) {
                        routes[route].insertCustomer(customerNumber, routes[route].getCustomers().begin() + insertionIndex);
                        done = true;
                        break;
                    }
                //}
            }
            neighbourIdx++;
        } while(neighbourIdx < maxClosestNeighbour && !done);

        if(!done) {
            // Unable to insert the customer to the route of one of its closest customers
            // => Revert to trying every route, starting from the routes of the closest depot
            int firstRouteOfDepot = (closestDepot.getNumber()-1) * problem.getVehiclesPerDepot();
            int j = 0;
            while(j < routes.size() && !done) {
                int idx = (j + firstRouteOfDepot) % routes.size();
                int position = 0;
                if(routes[idx].getCustomers().size() > 0) {
                    // Find the closest customer in that route and try inserting next to it
                    int closestInRoute = problem.getClosestCustomer(customer, routes[idx].getCustomers()).getNumber();
                    position = std::find(routes[idx].getCustomers().begin(), routes[idx].getCustomers().end(), closestInRoute) - routes[idx].getCustomers().begin();
                    // Determine if we want to try inserting before or after that position, depending on the added cost
                    float costOfInsertingBefore = routes[idx].getAddedDistanceOfInsert(customerNumber, position);
                    float costOfInsertingAfter = routes[idx].getAddedDistanceOfInsert(customerNumber, position+1);
                    position = costOfInsertingBefore < costOfInsertingAfter ? position : position + 1;
                }

                if(routes[idx].canInsertCustomer(customerNumber, position)) {
                    routes[idx].insertCustomer(customerNumber, routes[idx].getCustomers().begin() + position);
                    done = true;
                }
                j++;
            }
        }

        if(!done) {
            // Retry with a higher tolerance for distance overflow
            problem.increaseDistanceToleranceFactor();
            return createIndividual(customerNumbers);
        }

        /** Another method for creating individuals */
        // Start on a random route of the closest depot
        /*int closestDepotNumber = useClosestDepot ?
            problem.getClosestDepot(problem.getCustomerByNumber(customerNumber)).getNumber() :
            (1 + rd::gen(problem.getDepots().size()));
        int firstRouteOfDepot = (closestDepotNumber-1) * problem.getVehiclesPerDepot();
        int initialRoute = firstRouteOfDepot + rd::gen(problem.getVehiclesPerDepot());
        int route = initialRoute;
        // Iterate through all routes of this depot until we find a suitable one
        while(!routes[route].canAddCustomer(customerNumber)) {
            route = firstRouteOfDepot + (route - firstRouteOfDepot + 1) % problem.getVehiclesPerDepot();
            if(route == initialRoute)
                break;
        }
        // Revert back to random if not possible
        if(!routes[route].canAddCustomer(customerNumber)) {
            initialRoute = rd::gen(routes.size());
            route = initialRoute;
            // Iterate through all routes of this depot until we find a suitable one
            while(!routes[route].canAddCustomer(customerNumber)) {
                route = (route + 1) % routes.size();
                if(route == initialRoute)
                    break;
            }
        }*/
    }
    
    return Individual(routes);
}

const int PROGRESS_BAR_LENGTH = 30;
void printProgressBar(float progress) {
    int loaded = round(PROGRESS_BAR_LENGTH * progress);
    string str = "\r["; 
    for(int j = 0; j < PROGRESS_BAR_LENGTH; j++)
        str += (j == loaded ? ">" : j > loaded ? ":" : "-");
    str += "]";
    cout << str;
}

void MDVRPGeneticAlgorithm::buildInitialPopulation() {
    vector<int> customerNumbers;
    for(int n = 0; n < problem.getCustomers().size(); n++)
        customerNumbers.push_back(problem.getCustomers()[n].getNumber());

    for(int i = 0; i < POPULATION_SIZE; i++) {
        if(printProgress)
            printProgressBar(i / (float) POPULATION_SIZE);
        // Add a new individual
        population.addIndividual(createIndividual(customerNumbers));
    }
}

void MDVRPGeneticAlgorithm::runGenerations(int num) {
    if(population.getIndividuals().size() == 0) {
        cout << "\nUnable to run GA generations without building the initial population first.";
        return;
    }

    if(this->isTerminated())
        return;

    // int totalTime = 0, time1 = 0, time2 = 0, time3 = 0;
    
    if(printProgress)
        cout << "\n";

    for(int i = 0; i < num; i++) {  
        if(printProgress && i % (num/PROGRESS_BAR_LENGTH) == 0)
            printProgressBar(i / (float) num);
        // auto begin = chrono::high_resolution_clock::now();  

        // ### Make offspring
        vector<Individual> offsprings = makeOffspring(POPULATION_SIZE, CROSSOVER_RATE);

        // int dur1 = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - begin).count();  
        // begin = chrono::high_resolution_clock::now();

        // ### Mutate
        mutate(offsprings, MUTATION_RATE);

        // int dur2 = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - begin).count();  
        // begin = chrono::high_resolution_clock::now();

        // ### Insert offsprings
        population.insertIndividuals(offsprings, NUMBER_OF_ELITES);

        // int dur3 = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - begin).count();  
        
        // time1 += dur1; time2 += dur2; time3 += dur3; totalTime += dur1 + dur2 + dur3;

        generationsRan++;
    }

    // Update values used for termination
    float bestFitness = population.getFittestIndividual().getFitness();
    if(bestFitness != previousBestFitness) {
        previousBestFitness = bestFitness;
        numStepsWithoutFitnessChange = 0;
    }
    else
        numStepsWithoutFitnessChange++;

    if(shouldTerminate())
        terminate();

    if(printProgress)
        cout << "\r                                             ";
    
    /*std::cout << "\nTime spent generating offspring: " << time1 << " ms (" << 100 * time1 / totalTime << "%)";
    std::cout << "\nTime spent mutating: " << time2 << "ms (" << 100 * time2 / totalTime << "%)";
    std::cout << "\nTime spent inserting in population: " << time3 << " ms (" << 100 * time3 / totalTime << "%)";*/
}

vector<Individual> MDVRPGeneticAlgorithm::makeOffspring(int numOffsprings, float crossoverRate) {
    // Create an array of cumulative fitnesses (takes linear time, but allows us to do
    // roulette wheel selection wich O(log2(n)) selection).
    float total = 0;
    vector<float> cumulative(population.getIndividuals().size() + 1);
    cumulative[0] = 0;
    for(int j = 0; j < population.getIndividuals().size(); j++) {
        total += population.getIndividuals()[j].getFitness();        
        cumulative[j+1] = total;
    }
        
    vector<Individual> offsprings;

    for(int j = 0; j < numOffsprings; j++) {
        if(rd::gen() < crossoverRate) {   
            int parentB = 0;
            do {
                parentB = roulettewheel::spin(cumulative, total);
            } while(j == parentB);

            if(rd::gen() < 0.5)
                offsprings.push_back(population.getIndividuals()[j].crossover(population.getIndividuals()[parentB]));
            else
                offsprings.push_back(population.getIndividuals()[parentB].crossover(population.getIndividuals()[j]));
        }
        else
            offsprings.push_back(population.getIndividuals()[j]);
    }

    return offsprings;
}

void MDVRPGeneticAlgorithm::mutate(vector<Individual>& individuals, float mutationRate) {
    for(int j = 0; j < individuals.size(); j++) {
        // Allows for a mutation rate higher than 1 (ensures the average rate of mutations is correct)
        float mutRate = mutationRate;
        int numMutations = 0;
        while(mutRate > 0) {
            if(rd::gen() < mutRate)
                numMutations++;
            mutRate--;
        }                
        for(int m = 0; m < numMutations; m++)
            individuals[j].mutate();
    }
}

bool MDVRPGeneticAlgorithm::shouldTerminate() {
    return numStepsWithoutFitnessChange >= TERMINATION_FITNESS_STEPS
        // Do not terminate if there is still no legal individuals
        && population.getNumberOfIllegalIndividuals() < population.getIndividuals().size();
}

void MDVRPGeneticAlgorithm::terminate() {
    this->terminated = true;
}

void MDVRPGeneticAlgorithm::printState() {
    if(this->isTerminated()) {
        cout << "\nBest legal distance: " << population.getFittestLegalIndividual()->getTotalDistance();
    }
    else {
        cout << "\nBest distance: overall = " << population.getFittestIndividual().getTotalDistance() << ", legal = ";
        Individual* fittestLegal = population.getFittestLegalIndividual();
        if(fittestLegal == nullptr) cout << "none";
        else cout << fittestLegal->getTotalDistance();
        cout << "\nAverage distance: " << population.calculateAverageDistance() << ". ";
        cout << "Illegal routes: " << population.getNumberOfIllegalRoutes() << ". ";
        cout << "Illegal individuals: " << population.getNumberOfIllegalIndividuals() << ".";
    }

}

void MDVRPGeneticAlgorithm::outputFile() {
	const string OUTPUT_DIR = "../../solutions/";
    const string EXTENSION = ".res";

	Individual& bestInd = population.getFittestIndividual();
    ofstream f;

	f.open(OUTPUT_DIR + problem.getProblemName() + EXTENSION);

	f << round(bestInd.getTotalDistance() * 100) / 100 << "\n"; // first line of the output
	for(int i = 0; i < bestInd.getRoutes().size(); ++i){
        Route &r = bestInd.getRoutes()[i]; 
        if(r.getCustomers().size() == 0)
            continue;

		f << r.getDepot().getNumber() << "\t" << 1 + i % problem.getVehiclesPerDepot() << "\t" << round(r.getTotalDistance() * 100) / 100;
        f << "\t" << r.getTotalDemand() << "\t" << r.getDepot().getNumber() << "\t";
		for(int j = 0; j < r.getCustomers().size(); ++j)
			f << r.getCustomers()[j] << " ";

		f << "\n";
	}
	f.close();
}