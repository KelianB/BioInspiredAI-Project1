#include <iostream>
#include "model/locatable.h"
#include "model/mdvrp.h"

using namespace std;

int main() {
    /*
    Locatable* l1 = new Locatable(3,9);
    Locatable* l2 = new Locatable(6,9);
    cout << "Distance= " << l1->distanceTo(l2);
    */
    MDVRP* problem = new MDVRP("testing-data/p01");

    cin.get();    
    return 1;
}