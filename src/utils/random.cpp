#include "random.h"
#include <random>
#include <ctime>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;

/** 
 * Thread-safe function that returns a random an integer in the range [0, max[.
 * Note: It is a bit slower than just calling rand(), but ensures thread-safety and more uniform distribution.
 */
int rd::gen(int max) {
    if(max == 0) return 0;
    static thread_local mt19937* generator = nullptr;
    if (!generator) generator = new mt19937(rd::getThreadSafeSeed());
    uniform_int_distribution<int> distribution(0, max-1);
    return distribution(*generator);
}

/** Returns a float in the range [0,1[ */
float rd::gen() {
    return rd::gen(INT8_MAX) / (float) INT8_MAX;
}

/** Gets a seed that will be different depending on the thread */
int rd::getThreadSafeSeed() {
    return clock() + hash<thread::id>()(this_thread::get_id());
}