#ifndef ROULETTE_WHEEL_H
#define ROULETTE_WHEEL_H

#include <vector>

namespace roulettewheel {
    int spin(std::vector<float> cumulativeWeights, float sumOfWeights);
}

#endif