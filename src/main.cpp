#include <iostream>
#include <thread>

#include "random.h"
#include "mdvrp.h"
#include "mdvrp-genetic-algorithm.h"

using namespace std;

void runGA(MDVRP* problem) { 
    MDVRPGeneticAlgorithm ga(*problem); 
    ga.solve();
}

int main(int argc, char *argv[]) {
    const string DATA_DIR = "../../testing-data/"; 
    const string DEFAULT_INPUT_PROBLEM = "p01";
    const int NUM_THREADS = 1;

    // Get the input problem name from command line arguments (defaults to p01)
    string inputProblem = DEFAULT_INPUT_PROBLEM;
    for(int i = 1; i < argc; i++) {
        if(string(argv[i-1]) == "-p")
            inputProblem = argv[i];
    }
    
    // Read input data
    MDVRP problem((DATA_DIR + inputProblem).c_str());
    
    // Run GAs in parallel
    vector<thread> threads;
    for(int i = 0; i < NUM_THREADS; i++)
        threads.push_back(thread(runGA, &problem));  
    
    // Join the threads
    for(int i = 0; i < threads.size(); i++)
        threads[i].join();

    return 1;
}
