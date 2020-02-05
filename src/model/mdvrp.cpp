#include "mdvrp.h"
#include "customer.h"

#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

vector<int> lineToNumbers(string line) {
    stringstream ss(line);
    string rawIn;
    vector<int> numbers;
    while(getline(ss, rawIn, ' ')) {
        // Discard empty strings, which occur because numbers are padded with spaces (e.g. 3 is ' 3')
        if(rawIn.length() > 0)
            numbers.push_back(std::atoi(rawIn.c_str()));
    } 

    return numbers;
}

MDVRP::MDVRP(const char filePath[], string problemName) {
    this->problemName = problemName;

    ifstream fileStream;
    fileStream.open(filePath);
    
    if(fileStream.is_open()) {
        std::cout << "Parsing file " << filePath << "...\n";

        int numCustomers, numDepots;
        int lineIndex = 0;

        // Iterate through the lines of the input file
        string line;
        while(getline(fileStream, line)) {
            vector<int> numbers = lineToNumbers(line);
 
            // Read meta information in the first line
            if(lineIndex == 0) {
                this->vehiclesPerDepot = numbers[0];
                numCustomers = numbers[1];
                numDepots = numbers[2];
            }
            // Read depot definition
            else if(lineIndex <= numDepots)
                depots.push_back(Depot(depots.size() + 1, numbers[0], numbers[1]));  
            // Read customer definition
            else if(lineIndex <= numDepots + numCustomers)
                customers.push_back(Customer(numbers[0], numbers[1], numbers[2], numbers[3], numbers[4]));   
            // Read depot positions
            else if(lineIndex <= numDepots + numCustomers + numDepots)
                depots[lineIndex - numDepots - numCustomers - 1].setPosition(numbers[1], numbers[2]);
            else {
                cout << "Too many lines in input problem data.";
            }
            lineIndex++;
        }
        
        fileStream.close();

        cout << "\nSuccessfully parsed input data.\n";
        cout << "\nNumber of customers: " << this->customers.size();
        cout << "\nNumber of depots: " << this->depots.size();

        this->distanceMatrix = this->buildDistanceMatrix();
    }
    else {
        cout << "\n[CRITICAL ERROR] Couldn't read file " << filePath << ".";
    }
}

Customer& MDVRP::getCustomerByNumber(int number) {
    return getCustomers()[number - 1];
}

Depot& MDVRP::getDepotByNumber(int number) {
    return getDepots()[number - 1];
}

Customer& MDVRP::getClosestCustomer(Customer c, vector<int> customerNumbers) {
    float smallestDistance = 999999999;
    int minNumber;

    for(int i = 0; i < customerNumbers.size(); i++) {
        int customerNumber = customerNumbers[i];
        float distance = getDistance(c, getCustomerByNumber(customerNumber)); 
        if(distance < smallestDistance) {
            smallestDistance = distance;
            minNumber = customerNumber;
        }
    }
    return getCustomerByNumber(minNumber);
}

Depot& MDVRP::getClosestDepot(Customer c) {
    float smallestDistance = 999999999;
    int minNumber;

    for(int i = 0; i < depots.size(); i++) {
        float distance = getDistance(c, depots[i]); 
        if(distance < smallestDistance) {
            smallestDistance = distance;
            minNumber = depots[i].getNumber();
        }
    }
    return getDepotByNumber(minNumber);
}

void MDVRP::increaseDistanceToleranceFactor() {
    distanceToleranceFactor += 0.05;
}

float MDVRP::getDistance(Customer c, Depot d) {
    return distanceMatrix[c.getNumber() - 1][customers.size() + d.getNumber() - 1];
}
float MDVRP::getDistance(Depot d, Customer c) {
    return this->getDistance(c, d);
}
float MDVRP::getDistance(Customer a, Customer b) {
    return distanceMatrix[a.getNumber() - 1][b.getNumber() - 1];
}

float calculateDistance(Locatable a, Locatable b) {
    return sqrt((pow((a.getX() - b.getX()), 2) + pow((a.getY() - b.getY()), 2)));   
}

vector<vector<float>> MDVRP::buildDistanceMatrix() {
    vector<vector<float>> matrix;
    int numCustomers = this->getCustomers().size();
    int numDepots = this->getDepots().size();

    // Initialize a matrix of zeros
    for(int i = 0; i < numCustomers; i++) {
        vector<float> row;
        for(int j = 0; j < numCustomers + numDepots; j++)
            row.push_back(0);
        matrix.push_back(row);
    }

    // Fill the matrix using its symmetry property to save computation 
    for(int i = 0; i < numCustomers; i++) {
        Customer& c = this->getCustomers()[i];
        for(int j = 0; j < i; j++) {
            float distance = calculateDistance(c, this->getCustomers()[j]);
            matrix[i][j] = distance;
            matrix[j][i] = distance;
        }
        
        for(int j = 0; j < numDepots; j++)
            matrix[i][numCustomers + j] = calculateDistance(c, this->getDepots()[j]);
    }
    return matrix;
}