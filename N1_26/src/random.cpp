#include "random.hpp"
#include <random>

const unsigned int Random::generateRandomInt(
    unsigned int seed,
    unsigned int min,
    unsigned int max
) {
    auto randomNumberGenerator = std::mt19937(seed);

    return std::uniform_int_distribution<unsigned int>{min, max}(randomNumberGenerator);
}