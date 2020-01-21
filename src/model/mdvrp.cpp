#include "mdvrp.h"
#include "customer.h"

#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

MDVRP::MDVRP(const char filePath[]) {
    std::ifstream fileStream;
    fileStream.open(filePath);
    
    if(fileStream.is_open()) {
        std::cout << "Parsing file " << filePath << "\n";

        unsigned int numCustomers;
        unsigned int numDepots;
        unsigned int lineIndex;

        // Extract the numbers
        std::string line;
        while(std::getline(fileStream, line)) {
            std::stringstream ss(line);

            std::string rawIn;
            std::vector<int> numbers;
            while(std::getline(ss, rawIn, ' ')) {
                numbers.push_back(std::atoi(rawIn.c_str()));
                std::cout << std::atoi(rawIn.c_str());
            }
 
            // Read meta information in the first line
            if(lineIndex == 0) {
                this->vehiclesPerDepot = numbers[0];
                numCustomers = numbers[1];
                numDepots = numbers[2];
            }
            // Read depot information
            else if(lineIndex <= numDepots) {

            }
            // Read customer information
            else if(lineIndex <= numDepots + numCustomers) {
                Customer *c = new Customer(numbers[0], numbers[1], numbers[2], numbers[3], numbers[4]);
                customers.push_back(c);   
            }
            else {
                std::cout << "Too many lines in input problem data.";
            }

            lineIndex++;
        }

    }
    fileStream.close();

    
}