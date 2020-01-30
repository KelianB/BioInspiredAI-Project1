#include <population.h>
#include<bits/stdc++.h> 

using namespace std;

Population::Population() {
 
}

void Population::addIndividual(Individual ind) {
    this->individuals.push_back(ind);
}

bool fitnessComparator(Individual a, Individual b) {
    return b.fitness() - a.fitness();
}

void Population::insertIndividuals(vector<Individual> inds) {
    /**
     *  First draft: add all then remove worse until we're back to the original population size
     */

    bool debug = false;
    
    int popSize = this->getIndividuals().size();

    // Add all
    for(int i = 0; i < inds.size(); i++)
        this->addIndividual(inds[i]);

    if(debug) {
        cout << "\nPopulation before selection:\n";
        for(int i = 0; i < individuals.size(); i++)
            cout << individuals[i].getTotalDistance() << " ";
    }

    // Sort by ascending fitness
    sort(individuals.begin(), individuals.end());

    // Erase worse individuals
    individuals.erase(individuals.begin(), individuals.begin() + popSize); 

    if(debug) { 
        cout << "\nPopulation after selection:\n";
        for(int i = 0; i < individuals.size(); i++)
            cout << individuals[i].getTotalDistance() << " ";
    }
}