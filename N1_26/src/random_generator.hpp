#pragma once

#include <random>
#include <vector>

class RandomGenerator {
    private:
        uint32_t seed;
        std::mt19937 randomNumberGenerator;
        std::uniform_int_distribution<> distribution;

    public:
        RandomGenerator(uint32_t seed) : 
            seed(seed),
            randomNumberGenerator(std::mt19937(seed)),
            distribution(std::uniform_int_distribution<>(0, 1)) { }

        std::vector<int> generateSequence(uint32_t L);
        const int32_t generateRandomInt(int32_t min, int32_t max) ;
};