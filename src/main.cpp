#include <iostream>
#include <thread>

#include "random.h"
#include "mdvrp.h"
#include "mdvrp-genetic-algorithm.h"

using namespace std;

const string DATA_DIR = "../../testing-data/"; 
const string DEFAULT_INPUT_PROBLEM = "p01";
const int NUM_THREADS = 5;
const int GENERATIONS_PER_STEP = 1000;
const string SEPARATOR = "-------------------------------------------------------";

void waitOnThreads(vector<thread>& threads) {
    for(int i = 0; i < threads.size(); i++)
        threads[i].join();
}

void buildPopulationsInParallel(vector<MDVRPGeneticAlgorithm>& geneticAlgorithms) { 
    auto runFunction = [](MDVRPGeneticAlgorithm* ga) {ga->buildInitialPopulation();};
    vector<thread> threads; 
    for(int i = 0; i < geneticAlgorithms.size(); i++)
        threads.push_back(thread(runFunction, &geneticAlgorithms[i]));
    waitOnThreads(threads);
}

void runGenerationsInParallel(vector<MDVRPGeneticAlgorithm>& geneticAlgorithms, int numGenerations) {
    auto runFunction = [numGenerations](MDVRPGeneticAlgorithm* ga) {ga->runGenerations(numGenerations);};
    vector<thread> threads; 
    for(int i = 0; i < geneticAlgorithms.size(); i++)
        threads.push_back(thread(runFunction, &geneticAlgorithms[i]));
    waitOnThreads(threads); 
}

int main(int argc, char *argv[]) {
    // Get the input problem name from command line arguments (defaults to p01)
    string inputProblem = DEFAULT_INPUT_PROBLEM;
    for(int i = 1; i < argc; i++) {
        if(string(argv[i-1]) == "-p")
            inputProblem = argv[i];
    }
    
    // Read input data
    MDVRP problem((DATA_DIR + inputProblem).c_str());
    
    // Initialize GAs
    vector<MDVRPGeneticAlgorithm> geneticAlgorithms;
    for(int i = 0; i < NUM_THREADS; i++)
        geneticAlgorithms.push_back(MDVRPGeneticAlgorithm(problem, i==0));

    // Build initial populations in parallel
    cout << "\nGenerating initial populations...\n";
    buildPopulationsInParallel(geneticAlgorithms);    
    std::cout << "\rFinished generating initial populations.";
    std::cout << "\nIllegal distance tolerance factor: " << problem.getDistanceToleranceFactor();

    // Run GAs in parallel
    int allTerminated = false;
    int step = 0;
    while(!allTerminated) {
        allTerminated = true;

        cout << "\n\n" << SEPARATOR;
        for(int t = 0; t < geneticAlgorithms.size(); t++) {
            bool terminated = geneticAlgorithms[t].isTerminated();
            cout << "\n\n####### GA " << t+1 <<  " - ";
            if(terminated) cout << "(terminated)";
            else cout << "GENERATION " << geneticAlgorithms[t].getGenerationsRan();
            cout << " #######";
            geneticAlgorithms[t].printState();
            if(!terminated)
                allTerminated = false;
        }

        runGenerationsInParallel(geneticAlgorithms, GENERATIONS_PER_STEP);
        step++;
    }

    // Find best GA
    int bestIndex = 0;
    float bestFitness = geneticAlgorithms[0].getPopulation().getFittestLegalIndividual()->getFitness();
    for(int ga = 1; ga < geneticAlgorithms.size(); ga++) {
        float fitness = geneticAlgorithms[ga].getPopulation().getFittestLegalIndividual()->getFitness();
        if(fitness > bestFitness) {
            bestFitness = fitness;
            bestIndex = ga;
        }
    }
    cout << "\n\n" << SEPARATOR << "\nAll GAs were terminated.\nBest distance obtained: ";
    cout << geneticAlgorithms[bestIndex].getPopulation().getFittestLegalIndividual()->getTotalDistance();
    cout << " (after " << geneticAlgorithms[bestIndex].getGenerationsRan() << " generations).";

    return 1;
}
