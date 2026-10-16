#include "random_generator.hpp"
#include <random>

std::vector<int> RandomGenerator::generateSequence(unsigned int L) {
    std::vector<int> sequence(L);

    for(unsigned int i = 0; i < L; i++) {
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

const unsigned int RandomGenerator::generateRandomInt(unsigned int min, unsigned int max) {
    return std::uniform_int_distribution<unsigned int>{min, max}(this->randomNumberGenerator);
}