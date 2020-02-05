#include <iostream>
#include <thread>

#include "random.h"
#include "mdvrp.h"
#include "mdvrp-genetic-algorithm.h"

using namespace std;

const string DATA_DIR = "../../testing-data/"; 
const string DEFAULT_INPUT_PROBLEM = "p01";

const int DEFAULT_NUM_THREADS = 5;
const int GENERATIONS_PER_STEP = 1000;

const string SEPARATOR = "-------------------------------------------------------";
vector<MDVRPGeneticAlgorithm> geneticAlgorithms;
bool finished = false;

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

void printAndSaveBest() {
    // Find best solution
    int bestIndex = 0;
    Individual* fittestLegal = nullptr;
    for(int ga = 0; ga < geneticAlgorithms.size(); ga++) {
        Individual* ind = geneticAlgorithms[ga].getPopulation().getFittestLegalIndividual(); 
        if(ind != nullptr && (fittestLegal == nullptr || ind->getFitness() > fittestLegal->getFitness())) {
            bestIndex = ga;
            fittestLegal = ind;
        }
    }

    if(fittestLegal != nullptr) {
        cout << "\nBest distance obtained: ";
        cout << fittestLegal->getTotalDistance();
        cout << " (after " << geneticAlgorithms[bestIndex].getGenerationsRan() << " generations).";
        
        geneticAlgorithms[bestIndex].outputFile();
    }
    else
        cout << "\nTerminated with no legal solutions.";
}

void onExit() {
    if(!finished)
        printAndSaveBest();
}

int main(int argc, char *argv[]) {
    // Make sure to save the best results on exit
    std::atexit(onExit);
    
    string inputProblem = DEFAULT_INPUT_PROBLEM;
    int numThreads = DEFAULT_NUM_THREADS;

    // Get command line arguments 
    for(int i = 1; i < argc; i++) {
        if(string(argv[i-1]) == "-p")
            inputProblem = argv[i];
        if(string(argv[i-1]) == "-t")
            numThreads = stoi(argv[i]);
    }
    
    // Read input data
    MDVRP problem((DATA_DIR + inputProblem).c_str(), inputProblem);
    
    // Initialize GAs
    for(int i = 0; i < numThreads; i++)
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

        // Print states
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

    cout << "\n\n" << SEPARATOR << "\n\nAll GAs were terminated.\n";
    printAndSaveBest();
    finished = true;

    return 0;
}
