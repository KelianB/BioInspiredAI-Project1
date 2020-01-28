#include "route.h"
#include "depot.h"
#include "locatable.h"
#include <iostream>

Route::Route(Depot* depot) {
    this->depot = depot;
    this->totalDistanceRequireUpdate = true;
}


void Route::addCustomer(Customer c) {
    this->customers.push_back(c);
    this->totalDistanceRequireUpdate = true;
}

float Route::getTotalDistance() {
    // Recalculate distance only if needed
    if(this->totalDistanceRequireUpdate) {
        if(this->customers.size() == 0)
            this->totalDistance = 0;
        else {
            float distance = this->getDepot()->getPos().distanceTo(this->customers[0].getPos());
            std::cout << "\nDEPOT " << this->getDepot()->getNumber() << ":" << this->getDepot()->getPos().getX() << "," << this->getDepot()->getPos().getY();
    
            for(int i = 0; i < this->customers.size() - 1; i++) {
                distance += this->customers[i].getPos().distanceTo(this->customers[i+1].getPos());
            }
            std::cout << "\n";
            distance += this->customers[this->customers.size() - 1].getPos().distanceTo(this->getDepot()->getPos());
            this->totalDistance = distance;
        }
    }

    return this->totalDistance;
}