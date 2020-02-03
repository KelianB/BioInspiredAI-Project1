#include "random.h"
#include <random>
#include <ctime>
#include <thread>

using namespace std;

/* Thread-safe function that returns a random number in [0, max[.
This function takes ~142% the time that calling  would take. For this extra
cost you get a better uniform distribution and thread-safety. */
int random::gen(int max) {
    if(max == 0) return 0;
    static thread_local mt19937* generator = nullptr;
    if (!generator) generator = new mt19937(clock() + hash<thread::id>()(this_thread::get_id()));
    uniform_int_distribution<int> distribution(0, max-1);
    return distribution(*generator);
}

float random::gen() {
    return random::gen(INT_MAX) / (float) INT_MAX;
}