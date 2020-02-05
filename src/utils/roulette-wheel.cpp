#include "random.h"

#include <roulette-wheel.h>
#include <vector>
#include <cstdlib>

using namespace std;

/** Uses binary search to achieve O(log2(n)) time complexity **/
int roulettewheel::spin(vector<float> cumulativeWeights, float sumOfWeights) {
    int length = cumulativeWeights.size() - 1;
    double r = sumOfWeights * rd::gen();    
    // Look for the index of the entry just above r:
    int a = 0, b = length - 1;
    while(b-a > 1) {
        int mid = (a + b) / 2;
        if(cumulativeWeights[mid] > r) b = mid;
        else a = mid;
    }
    return a;
}