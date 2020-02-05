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

        // Build a vector of "neigbours" (customers that were already assigned to a route), 
        // which sorted by increasing distance to customer
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

            // If customer is closer to its closest depot than to this neighbour, try to assign it to an empty route of the depot
            if(problem.getDistance(customer, closestDepot) < problem.getDistance(customer, problem.getCustomerByNumber(neighbourNum))) {
                int firstRouteOfDepot = (closestDepot.getNumber()-1) * problem.getVehiclesPerDepot();
                int j = 0;
                while(routes[firstRouteOfDepot + j].getCustomers().size() > 0 && j < problem.getVehiclesPerDepot()) {
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
                //while(rd::gen() < 0)
                  //  route = (route + 1) % routes.size();


                if(route == routes.size()) {
                    // Critical error: the neighbour was never added to a route, which means there is was a problem before
                    cout << "\nCritical error generating individual (neighbour should have been added to a route)\n";
                }
                else {
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
                        vector<int>::iterator insertionPos = find(routes[route].getCustomers().begin(), routes[route].getCustomers().end(), neighbourNum);
                        int insertionIndex = insertionPos - routes[route].getCustomers().begin();

                        // Determine if we want to try inserting before or after that position, depending on the added cost
                        float costOfInsertingBefore = routes[route].getAddedDistanceOfInsert(customerNumber, insertionIndex);
                        float costOfInsertingAfter = routes[route].getAddedDistanceOfInsert(customerNumber, insertionIndex+1);
                        int insertionOffset [2] = {
                            costOfInsertingBefore < costOfInsertingAfter ? 0 : 1,
                            costOfInsertingBefore < costOfInsertingAfter ? 1 : 0
                        };
                        // Try inserting
                        for(int offset : insertionOffset) {
                            if(routes[route].canInsertCustomer(customerNumber, insertionIndex + offset)) {
                                routes[route].insertCustomer(customerNumber, insertionPos + offset);
                                done = true;
                                break;
                            }
                        }
                    //}
                }
            }
            neighbourIdx++;
        } while(neighbourIdx < maxClosestNeighbour && !done);

        if(!done) {
            // Unable to insert the customer to the route of one of its closest customers
            // => Revert to every route, starting from the routes of the closest depot
            int firstRouteOfDepot = (closestDepot.getNumber()-1) * problem.getVehiclesPerDepot();
            int j = 0, idx = 0, position = 0;
            while(j < routes.size()) {
                idx = (j + firstRouteOfDepot) % routes.size();
                // Try to insert at the position of the closest customer in the route
                //position = rd::gen(routes[idx].getCustomers().size());
                if(routes[idx].getCustomers().size() == 0)
                    position = 0;
                else {
                    int closestInRoute = problem.getClosestCustomer(customer, routes[idx].getCustomers()).getNumber();
                    position = std::find(routes[idx].getCustomers().begin(), routes[idx].getCustomers().end(), closestInRoute) - routes[idx].getCustomers().begin();
                    // 0.5 chance of inserting after the closest customer
                    //if(rd::gen() > 0.5) position++;
                    // Determine if we want to try inserting before or after that position, depending on the added cost
                    float costOfInsertingBefore = routes[idx].getAddedDistanceOfInsert(customerNumber, position);
                    float costOfInsertingAfter = routes[idx].getAddedDistanceOfInsert(customerNumber, position+1);
                    position = costOfInsertingBefore < costOfInsertingAfter ? position : position + 1;
                }
                if(routes[idx].canInsertCustomer(customerNumber, position)) {
                    routes[idx].insertCustomer(customerNumber, routes[idx].getCustomers().begin() + position);
                    done = true;
                    break;
                }
                j++;
            }
        }

        if(!done) {
            problem.increaseDistanceToleranceFactor();
            return createIndividual(customerNumbers);
            //cout << "\nCritical error 2 generating individual\n";
        }
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
    cout << "\r[";
    for(int j = 0; j < PROGRESS_BAR_LENGTH; j++)
        cout << (j == loaded ? ">" : j > loaded ? ":" : "-");
    cout << "]";
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