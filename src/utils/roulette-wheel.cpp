#include <roulette-wheel.h>
#include <vector>
#include <cstdlib>
#include "random.h"

using namespace std;

// Uses binary search to achieve O(log2(n)) complexity
int roulettewheel::spin(vector<float> cumulativeWeights, float sumOfWeights) {
    int length = cumulativeWeights.size() - 1;
    double r = sumOfWeights * random::gen();    
    // Look for the index of the entry just above r:
    int a = 0, b = length - 1;
    while(b-a > 1) {
        int mid = (a + b) / 2;
        if(cumulativeWeights[mid] > r) b = mid;
        else a = mid;
    }
    return a;
}