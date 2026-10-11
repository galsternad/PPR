#include "sequence_generator.hpp"

std::vector<int> SequenceGenerator::generateSequence(unsigned int L) {
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