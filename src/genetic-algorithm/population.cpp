#include <population.h>
#include <roulette-wheel.h>
#include <algorithm>

using namespace std;

Population::Population() {
 
}

void Population::addIndividual(Individual ind) {
    this->individuals.push_back(ind);
}

bool fitnessComparator(Individual a, Individual b) {
    return b.getFitness() - a.getFitness();
}


void Population::insertIndividuals(vector<Individual> inds, int numberOfElites) {
    bool debug = false;
    int popSize = this->getIndividuals().size();

    /**
     *  Fully fitness-based: add all then remove worse until we're back to the original population size
     */

    // Add all
    /*for(int i = 0; i < inds.size(); i++)
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
     *  Elitism strategy: keep best parents and replace others with best of new individuals
     */

    // Sort by ascending fitness
    /*sort(individuals.begin(), individuals.end());
    // Erase individuals except top 10
    individuals.erase(individuals.begin(), individuals.begin() + popSize - numberOfElites); 
    // Sort new individuals by ascending fitness
    sort(inds.begin(), inds.end());
    // Add best new individuals
    for(int i = numberOfElites; i < inds.size(); i++)
        this->addIndividual(inds[i]);*/

    vector<Individual> everyone;

    for(int i = 0; i < individuals.size(); i++)
        everyone.push_back(individuals[i]);
    for(int i = 0; i < inds.size(); i++)
        everyone.push_back(inds[i]);

    double total = 0;
    vector<float> cumulative;
    cumulative.push_back(0);
    for(int j = 0; j < everyone.size(); j++) {
        total += everyone[j].getFitness();        
        cumulative.push_back(total);
    }

    sort(individuals.begin(), individuals.end());
    individuals.erase(individuals.begin(), individuals.end() - numberOfElites); 

    for(int i = 0; i < popSize - numberOfElites; i++)
        this->addIndividual(everyone[roulettewheel::spin(cumulative, total)]);
}

Individual Population::getFittestIndividual() {
    int bestIndex = 0;
    double bestFitness = 0;
    for(int i = 0; i < getIndividuals().size(); i++) {
        double fitness = getIndividuals()[i].getFitness();
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