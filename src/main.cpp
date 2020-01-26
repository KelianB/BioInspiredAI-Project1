#include <iostream>

#include "mdvrp.h"
#include "mdvrp-genetic-algorithm.h"

using namespace std;


int main() {
    MDVRP problem("../../testing-data/p01");
    MDVRPGeneticAlgorithm ga(&problem); 
    cout << "\nDone";
    return 1;
}