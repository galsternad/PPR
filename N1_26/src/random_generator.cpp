#include "random_generator.hpp"
#include <random>

std::vector<int> RandomGenerator::generateSequence(uint32_t L) {
    std::vector<int> sequence(L);

    for(int i = 0; i < L; i++) {
        bool randomBit = this->distribution(this->randomNumberGenerator);
        // 0 => 1
        if(!randomBit) {
            sequence[i] = 1;
        }
        // 1 => -1
        else {
            sequence[i] = -1;
        }
    }

    return sequence;
}

const int32_t RandomGenerator::generateRandomInt(int32_t min, int32_t max) {
    return std::uniform_int_distribution<int32_t>{min, max}(this->randomNumberGenerator);
}