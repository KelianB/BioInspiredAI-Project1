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
    const int NUM_THREADS = 5;

    string inputProblem = "p01";
    for(int i = 1; i < argc; i++) {
        if(string(argv[i-1]) == "-p")
            inputProblem = argv[i];
    }
    
    MDVRP problem(("../../testing-data/" + inputProblem).c_str());
    
    vector<thread> threads;
    for(int i = 0; i < NUM_THREADS; i++)
        threads.push_back(thread(runGA, &problem));  
    
    for(int i = 0; i < threads.size(); i++)
        threads[i].join();

    cout << "\nDone";
    return 1;
}
