#include "route.h"
#include "depot.h"
#include "locatable.h"

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
            float distance = this->getDepot()->distanceTo(this->customers[0]);
            for(int i = 0; i < this->customers.size() - 1; i++)
                distance += this->customers[i].distanceTo(this->customers[i+1]);
            distance += this->customers[this->customers.size() - 1].distanceTo(*this->getDepot());
            this->totalDistance = distance;
        }
    }

    return this->totalDistance;
}