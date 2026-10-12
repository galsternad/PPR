#include <random>
#include <vector>

#define SIZE 3

class SequenceGenerator {
    private:
        unsigned int seed;
        std::mt19937 randomNumberGenerator;
        std::uniform_int_distribution<> distribution;

    public:
        SequenceGenerator(unsigned int seed) : 
            seed(seed),
            randomNumberGenerator(std::mt19937(seed)),
            distribution(std::uniform_int_distribution<>(0, 1)) { }

        std::vector<int> generateSequence(unsigned int L);
};