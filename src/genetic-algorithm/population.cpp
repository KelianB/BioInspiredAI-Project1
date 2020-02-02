#include <population.h>
#include <bits/stdc++.h> 

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
     *  Fully fitness-based: add all then remove worse until we're back to the original population size
     */

    /*bool debug = false;
    
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
    }*/



    /**
     * Fully age-based: repace entire population
     */

    /*individuals.erase(individuals.begin(), individuals.end());
    for(int i = 0; i < inds.size(); i++)
        this->addIndividual(inds[i]);*/

    
    
    /**
     *  Elitism strategy: keep 10 best parents and replace others with best of new individuals
     */

    int popSize = this->getIndividuals().size();
    int parentsKept = 2;

    // Sort by ascending fitness
    sort(individuals.begin(), individuals.end());
    // Erase individuals except top 10
    individuals.erase(individuals.begin(), individuals.begin() + popSize - parentsKept); 
    // Sort new individuals by ascending fitness
    sort(inds.begin(), inds.end());
    // Add best new individuals
    for(int i = parentsKept; i < inds.size(); i++)
        this->addIndividual(inds[i]);
}

Individual Population::getFittestIndividual() {
    int bestIndex = 0;
    double bestFitness = 0;
    for(int i = 0; i < getIndividuals().size(); i++) {
        double fitness = getIndividuals()[i].fitness();
        if(fitness > bestFitness) {
            bestFitness = fitness;
            bestIndex = i;
        }
    }
    return getIndividuals()[bestIndex];
}

double Population::getAverageDistance() {
    double sum = 0;
    for(int i = 0; i < getIndividuals().size(); i++)
        sum += getIndividuals()[i].getTotalDistance();
    return sum / getIndividuals().size();
}