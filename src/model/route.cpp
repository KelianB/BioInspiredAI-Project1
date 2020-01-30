#include "route.h"
#include "depot.h"
#include "locatable.h"
#include "mdvrp.h"
#include <iostream>
#include <algorithm>


Route::Route(MDVRP& pb, Depot dep): depot(dep), problem(pb) {
    this->totalDistanceRequireUpdate = true;
}

bool Route::canInsertCustomer(int customerNumber, int position) {
    if(position < 0 || position > this->customers.size()) {
        cout << "\n invalid position " << position;
        return false;
    }

    Customer c = problem.getCustomerByNumber(customerNumber);

    // capacity limit: the total demand of the customers on any route does not exceed a vehicle’s capacity.
    bool capacityCriterion = this->getTotalDemand() + c.getDemand() <= depot.getMaxVehicleLoad();
    if(!capacityCriterion)
        return false;

    // route limit: the total duration of a route does not exceed a preset value (ignore criterion if value is 0)
    if(depot.getMaxRouteDuration() != 0) {
        Locatable previous = (position == 0 ? 
            static_cast<Locatable const &>(depot) : 
            problem.getCustomerByNumber(customers[position - 1]));
        Locatable next = (position == customers.size() ? 
            static_cast<Locatable const &>(depot) : 
            problem.getCustomerByNumber(customers[position]));
        float distanceDelta = previous.distanceTo(c) + c.distanceTo(next) - previous.distanceTo(next);

        bool routeDurationCriterion = this->getTotalDistance() + distanceDelta <= depot.getMaxRouteDuration();
        return routeDurationCriterion;
    }
    return true;
}

bool Route::canAddCustomer(int customerNumber) {
    return this->canInsertCustomer(customerNumber, this->customers.size());
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
    int demand = 0;
    if(this->customers.size() > 0) {
        for(int i = 0; i < this->customers.size(); i++) {
            int customerNumber = this->customers[i];
            demand += this->problem.getCustomerByNumber(customerNumber).getDemand();
        }
    }

    return demand;
}