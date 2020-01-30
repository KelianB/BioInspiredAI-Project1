#include "route.h"
#include "depot.h"
#include "locatable.h"
#include "mdvrp.h"
#include <iostream>
#include <algorithm>


Route::Route(MDVRP& pb, Depot dep): depot(dep), problem(pb) {
    this->totalDistanceRequireUpdate = true;
}

bool Route::canAddCustomer(Customer c) {
    // capacity limit: the total demand of the customers on any route does not exceed a vehicle’s capacity.

    // route limit: the total duration of a route does not exceed a preset value (for this project, it is only for those problems for which this value is mentioned in the test data, that is, the value in the test data is not 0).

    return true;
}

void Route::addCustomer(Customer c) {
    this->customers.push_back(c.getNumber());
    this->totalDistanceRequireUpdate = true;
}

bool Route::hasCustomer(int customerNumber) {
    return std::find(getCustomers().begin(), getCustomers().end(), customerNumber) != getCustomers().end();
}

bool Route::removeCustomer(int customerNumber) {
    if(this->hasCustomer(customerNumber)) {
        customers.erase(std::find(getCustomers().begin(), getCustomers().end(), customerNumber));
        return true;
    }
    return false;
}

void Route::setCustomers(vector<int> customerNumbers) {
    this->customers.clear();
    for(int i = 0; i < customerNumbers.size(); i++)
        this->customers.push_back(customerNumbers[i]);
}

float Route::getTotalDistance() {
    // Recalculate distance only if needed
    if(this->totalDistanceRequireUpdate) {
        if(this->customers.size() == 0)
            this->totalDistance = 0;
        else {
            float distance = this->getDepot().distanceTo(this->problem.getCustomers()[0]);
            for(int i = 0; i < this->customers.size() - 1; i++) {
                int customerNumber = this->customers[i];
                int nextCustomerNumber = this->customers[i+1];
                distance += this->problem.getCustomerByNumber(customerNumber).distanceTo(this->problem.getCustomerByNumber(i+1));
            }
            distance += this->problem.getCustomerByNumber(this->customers[this->customers.size() - 1]).distanceTo(this->getDepot());
            this->totalDistance = distance;
        }
        this->totalDistanceRequireUpdate = false;
    }

    return this->totalDistance;
}

int Route::getTotalDemand() {
    // TODO: recalculate only if needed
    int demand;
    if(this->customers.size() == 0)
        demand = 0;
    else {
        for(int i = 0; i < this->customers.size(); i++) {
            int customerNumber = this->customers[i];
            demand += this->problem.getCustomerByNumber(customerNumber).getDemand();
        }
    }

    return demand;
}