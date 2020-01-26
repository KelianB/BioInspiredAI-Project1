#include "mdvrp.h"
#include "customer.h"

#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

vector<int> lineToNumbers(string line) {
    stringstream ss(line);
    string rawIn;
    vector<int> numbers;
    while(getline(ss, rawIn, ' ')) {
        // Discard empty strings, which occur because numbers are padded with spaces (e.g. 3 is ' 3')
        if(rawIn.length() > 0) {
            numbers.push_back(std::atoi(rawIn.c_str()));
        }
    } 

    return numbers;
}

MDVRP::MDVRP(const char filePath[]) {
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
            else if(lineIndex <= numDepots) {
                depots.push_back(Depot(depots.size(), numbers[0], numbers[1]));  
            }
            // Read customer definition
            else if(lineIndex <= numDepots + numCustomers) {
                customers.push_back(Customer(numbers[0], numbers[1], numbers[2], numbers[3], numbers[4]));   
            }
            // Read depot positions
            else if(lineIndex < numDepots + numCustomers + numDepots) {
                depots.at(lineIndex - numDepots - numCustomers).setPosition(numbers[1], numbers[2]);
            }
            else {
                cout << "Too many lines in input problem data.";
            }

            lineIndex++;
        }
        
        fileStream.close();

        cout << "Successfully parsed input data.\n";
        cout << "Number of customers: " << this->customers.size() << "\n";
        cout << "Number of depots: " << this->depots.size() << "\n";
    }
    else {
        cout << "Couldn't read file " << filePath << ".";
    }
}

Customer MDVRP::getClosestCustomer(Locatable locatable, vector<Customer> customers) {
    Customer *closest;
    float smallestDistance = 10000000;

    for(int i = 0; i < customers.size(); i++) {
        Customer c = customers[i];
        float distance = locatable.distanceTo(c); 
        if(distance < smallestDistance) {
            smallestDistance = distance;
            closest = &c;
        }
    }
    return *closest;
}