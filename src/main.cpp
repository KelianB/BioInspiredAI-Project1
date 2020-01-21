#include <iostream>
#include "model/locatable.h"

using namespace std;

int main() {
    Locatable* l1 = new Locatable(3,9);
    Locatable* l2 = new Locatable(6,9);
    cout << "Distance= " << l1->distanceTo(l2);
    
    cin.get();    
    return 1;
}