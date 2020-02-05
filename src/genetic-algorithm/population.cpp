#include <population.h>
#include <roulette-wheel.h>

#include <algorithm>
#include <cmath>
#include <chrono>

using namespace std;

Population::Population() {
 
}

void Population::addIndividual(Individual ind) {
    this->individuals.push_back(ind);
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

    // Append individuals to inds so we have all the individuals in a common vector
    inds.insert(inds.end(), individuals.begin(), individuals.end());

    // Build a vector of cumulative fitnesses (allows for O(log2(n)) roulette wheel selection)
    double total = 0;
    vector<float> cumulative;
    cumulative.push_back(0);
    for(int j = 0; j < inds.size(); j++) {
        total += inds[j].getFitness();        
        cumulative.push_back(total);
    }

    // Do a partial sort on the previous population so we can get the elites in linear time
    nth_element(individuals.begin(), individuals.begin() + numberOfElites, individuals.end(), [](Individual& a, Individual& b) {
        return b.getFitness() < a.getFitness(); 
    });

    // Remove all but the elite
    individuals.erase(individuals.begin() + numberOfElites, individuals.end());

    // Add offspring using roulette wheel selection, until we are back to the previous population size
    for(int i = 0; i < popSize - numberOfElites; i++)
        this->addIndividual(inds[roulettewheel::spin(cumulative, total)]);
}

Individual& Population::getFittestIndividual() {
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

Individual* Population::getFittestLegalIndividual() {
    int bestIndex = -1;
    double bestFitness = 0;
    for(int i = 0; i < getIndividuals().size(); i++) {
        double fitness = getIndividuals()[i].getFitness();
        if(fitness > bestFitness && getIndividuals()[i].isLegal()) {
            bestFitness = fitness;
            bestIndex = i;
        }
    }
    return bestIndex == -1 ? nullptr : &getIndividuals()[bestIndex];
}

float Population::calculateAverageDistance() {
    float sum = 0;
    for(int i = 0; i < getIndividuals().size(); i++)
        sum += getIndividuals()[i].getTotalDistance();
    return sum / getIndividuals().size();
}

float Population::calculateDistanceDeviation() {
    float average = this->calculateAverageDistance();
    float s = 0;
    int n = getIndividuals().size();
    for(int i = 0; i < n; i++)
        s += pow(getIndividuals()[i].getTotalDistance() - average, 2);
    return sqrt(s / n);
}

int Population::getNumberOfIllegalRoutes() {
    int n = 0;
    for(int i = 0; i < getIndividuals().size(); i++) {
        Individual& ind = getIndividuals()[i];
        for(int j = 0; j < ind.getRoutes().size(); j++) {
            if(!ind.getRoutes()[j].isLegal())
                n++;
        }
    }
    return n;
}