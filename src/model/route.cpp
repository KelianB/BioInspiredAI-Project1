#include "route.h"
#include "depot.h"
#include "locatable.h"
#include "mdvrp.h"
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

Route::Route(MDVRP& pb, Depot dep): depot(dep), problem(pb) {
    this->totalDemand = 0;
    this->totalDistance = 0;
}

bool Route::canInsertCustomer(int customerNumber, int position) {
    if(position < 0 || position > this->customers.size()) {
        cout << "\n invalid position " << position;
        return false;
    }

    Customer& c = problem.getCustomerByNumber(customerNumber);

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

void Route::addCustomer(int customerNumber) {
    this->customers.push_back(customerNumber);
    this->totalDistanceRequireUpdate = true;
    this->totalDemand += problem.getCustomerByNumber(customerNumber).getDemand();
}

void Route::insertCustomer(int c, vector<int>::iterator pos){
    this->customers.insert(pos, c);
    this->totalDistanceRequireUpdate = true;
    this->totalDemand += problem.getCustomerByNumber(c).getDemand();
}

bool Route::hasCustomer(int customerNumber) {
    return std::find(getCustomers().begin(), getCustomers().end(), customerNumber) != getCustomers().end();
}

bool Route::removeCustomer(int customerNumber) {
    if(this->hasCustomer(customerNumber)) {
        customers.erase(std::find(getCustomers().begin(), getCustomers().end(), customerNumber));
        this->totalDistanceRequireUpdate = true;
        this->totalDemand -= problem.getCustomerByNumber(customerNumber).getDemand();
        return true;
    }
    return false;
}

void Route::setCustomers(vector<int> customerNumbers) {
    this->customers.clear();
    for(int i = 0; i < customerNumbers.size(); i++)
        this->addCustomer(customerNumbers[i]);
    this->totalDemandRequireUpdate = true;
}

float Route::getTotalDistance() {
    // Recalculate distance only if needed
    if(this->totalDistanceRequireUpdate) {
        if(this->customers.size() == 0)
            this->totalDistance = 0;
        else {
            float distance = problem.getDistance(this->getDepot(), problem.getCustomerByNumber(this->customers[0]));
            for(int i = 0; i < this->customers.size() - 1; i++) {
                int customerNumber = this->customers[i];
                int nextCustomerNumber = this->customers[i+1];
                distance += problem.getDistance(this->problem.getCustomerByNumber(customerNumber), 
                    this->problem.getCustomerByNumber(nextCustomerNumber));
            }
            distance += problem.getDistance(problem.getCustomerByNumber(this->customers[this->customers.size() - 1]), this->getDepot());
            this->totalDistance = distance;
        }
        this->totalDistanceRequireUpdate = false;
    }

    return this->totalDistance;
}

int Route::getTotalDemand() {
    if(this->totalDemandRequireUpdate) {
        int demand = 0; 
        for(int i = 0; i < this->customers.size(); i++)
            demand += this->problem.getCustomerByNumber(this->customers[i]).getDemand();
        this->totalDemand = demand;
        this->totalDemandRequireUpdate = false; 
    }
    
    return this->totalDemand;
}