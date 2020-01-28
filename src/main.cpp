#include <iostream>
#include <cstdlib>
#include <ctime>

#include "mdvrp.h"
#include "mdvrp-genetic-algorithm.h"


using namespace std;


int main() {
    srand(time(NULL));

    MDVRP problem("../../testing-data/p01");
    MDVRPGeneticAlgorithm ga(&problem); 
    ga.buildInitialPopulation();
    cout << "\nDone";
    return 1;
}