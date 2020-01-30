#include "route.h"
#include "depot.h"
#include "locatable.h"
#include "mdvrp.h"
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

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

void Route::insertCustomer(int c, vector<int>::iterator pos){
    this->getCustomers().insert(pos, c);
    this->totalDistanceRequireUpdate = true;
}

bool Route::hasCustomer(int customerNumber) {
    vector<int>::iterator pos = std::find(getCustomers().begin(), getCustomers().end(), customerNumber);
    return pos != getCustomers().end();
}

bool Route::removeCustomer(int customerNumber) {
    if(this->hasCustomer(customerNumber)) {
        customers.erase(std::find(getCustomers().begin(), getCustomers().end(), customerNumber));
        return true;
    }
    return false;
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