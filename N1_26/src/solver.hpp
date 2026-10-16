#include "random_generator.hpp"
#include "sequence.hpp"
#include "calculator.hpp"

struct NeighbourResult {
    unsigned int index;
    unsigned int energy;
    bool foundMove;
};

class Solver {
    private:
        const unsigned int L;
        const unsigned int seed;
        const unsigned int nfesLmt;
        const unsigned int threads;
        unsigned int nfes;
        // in milliseconds
        unsigned int runtime;
        unsigned int cpuTime;
        unsigned int energyConsumption;
        unsigned int speed;
        // neighbour evaluation cache
        std::vector<std::vector<int>> T;
        Sequence currentSequence;
        Sequence bestSequence;
        RandomGenerator generator;

        Sequence generateNewSequence();
        unsigned int valueFlip(
            const std::vector<int>& S,
            const std::vector<int>& C,
            const std::vector<std::vector<int>>& T,
            unsigned int index
        );

        unsigned int calculateSpeed() const;
        const unsigned int getEnergyFromFile(const std::string& path) const;

        std::vector<int> getFlippedVector(const std::vector<int>& S, unsigned int index) const;
        NeighbourResult evaluateNeighboursSDLS(unsigned int start, unsigned int end);
        NeighbourResult evaluateNeighboursTabu(
            unsigned int start,
            unsigned int end,
            unsigned int k,
            const Sequence& runBestSequence,
            const std::vector<int>& M
        );

        bool updateBestNeighbour(unsigned int index, unsigned int energy);

        void createThreadsSDLS(
            std::vector<std::thread>& threads,
            std::vector<NeighbourResult>& results
        );
        void createThreadsTabu(
            std::vector<std::thread>& threads,
            std::vector<NeighbourResult>& results,
            unsigned int k,
            const Sequence& runBestSequence,
            const std::vector<int>& M
        );
        
    public:
        Solver(
            const unsigned int L = 600,
            const unsigned int seed = time(nullptr),
            const unsigned int nfesLmt = 1000000,
            const unsigned int threads = 1
        ) : L(L), seed(seed), nfesLmt(nfesLmt), threads(threads),
        nfes(0), runtime(0), cpuTime(0), energyConsumption(0),
        speed(0), currentSequence(), generator(seed) { }
        
        inline const unsigned int getRunTime() const { return this->runtime; }
        inline const Sequence& getCurrentSequence() const { return this->currentSequence; }
        inline const Sequence& getBestSequence() const { return this->bestSequence; }
        
        const std::string toString() const;

        void randomSearch();
        void steepestDescentLocalSearch();        
        void tabuSearch();
        void solve();
};