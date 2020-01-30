#include <population.h>

using namespace std;

Population::Population() {
 
}

void Population::addIndividual(Individual ind) {
    this->individuals.push_back(ind);
}