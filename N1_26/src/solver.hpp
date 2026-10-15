#include "sequence_generator.hpp"
#include "sequence.hpp"
#include "calculator.hpp"

struct NeighbourResult {
    unsigned int index;
    unsigned int energy;
};

class Solver {
    private:
        const unsigned int L;
        const unsigned int seed;
        const unsigned int nfesLmt;
        const unsigned int threads;
        unsigned int nfes;
        unsigned int runtime;
        unsigned int cpuTime;
        unsigned int energyConsumption;
        unsigned int speed;
        // neighbour evaluation cache
        std::vector<std::vector<int>> T;
        Sequence currentSequence;
        Sequence bestSequence;
        SequenceGenerator generator;

        Sequence generateNewSequence();
        unsigned int valueFlip(
            const std::vector<int>& S,
            const std::vector<int>& C,
            const std::vector<std::vector<int>>& T,
            unsigned int index
        );
        std::vector<int> flipIndex(const std::vector<int>& S, unsigned int index) const;
        NeighbourResult evaluateNeighboursNaive();
        NeighbourResult evaluateNeighbours();
        bool updateBestNeighbourNaive(unsigned int index, unsigned int energy);
        bool updateBestNeighbour(unsigned int index, unsigned int energy);
        
    public:
        Solver(
            const unsigned int L = 100,
            const unsigned int seed = time(nullptr),
            const unsigned int nfesLmt = 1000,
            const unsigned int threads = 1
        ) : L(L), seed(seed), nfesLmt(nfesLmt), threads(threads),
        nfes(0), runtime(0), cpuTime(0), energyConsumption(0),
        speed(0), currentSequence(), generator(seed) { }
        
        inline unsigned int getRunTime() const { return this->runtime; }
        inline const Sequence& getCurrentSequence() const { return this->currentSequence; }
        inline const Sequence& getBestSequence() const { return this->bestSequence; }
        
        const std::string toString() const;

        void randomSearch();

        void steepestDescentLocalSearchNaive();
        void steepestDescentLocalSearch();
        void tabuSearch();
};