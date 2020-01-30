#include "route.h"
#include "depot.h"
#include "locatable.h"
#include "mdvrp.h"
#include <iostream>

Route::Route(MDVRP& pb, Depot dep): depot(dep), problem(pb) {
    this->totalDistanceRequireUpdate = true;
}

bool Route::canAddCustomer(int c) {
    return true;
}

void Route::addCustomer(int c) {
    this->customers.push_back(c);
    this->totalDistanceRequireUpdate = true;
}

void Route::insertCustomer(int c, int pos){
    this.insert(pos, c);
    this->totalDistanceRequireUpdate = true;
}

float Route::getTotalDistance() {
    // Recalculate distance only if needed
    if(this->totalDistanceRequireUpdate) {
        if(this->customers.size() == 0)
            this->totalDistance = 0;
        else {
            float distance = this->getDepot().distanceTo(this->problem.getCustomers()[0]);
            for(int i = 0; i < this->customers.size() - 1; i++)
                distance += this->problem.getCustomers()[i].distanceTo(this->problem.getCustomers()[i+1]);
            distance += this->problem.getCustomers()[this->customers.size() - 1].distanceTo(this->getDepot());
            this->totalDistance = distance;
        }
    }

    return this->totalDistance;
}