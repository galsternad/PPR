#include "random_generator.hpp"
#include "sequence.hpp"
#include "calculator.hpp"
#include "metrics.hpp"
#include <stdexcept>
#include <thread>

struct NeighbourResult {
    unsigned int index;
    unsigned int energy;
    bool foundMove;
};

class Solver {
    private:
        unsigned int L;
        unsigned int seed;
        unsigned int nfesLmt;
        unsigned int threads;
        unsigned int nfes;
        // neighbour evaluation cache
        std::vector<std::vector<int>> T;
        Metrics metrics;
        Sequence currentSequence;
        Sequence bestSequence;
        RandomGenerator generator;
        
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

        void createThreadsRandom(
            std::vector<std::thread>& workers,
            std::vector<Sequence>& results
        );
        void createThreadsSDLS(
            std::vector<std::thread>& workers,
            std::vector<NeighbourResult>& results
        );
        void createThreadsTabu(
            std::vector<std::thread>& workers,
            std::vector<NeighbourResult>& results,
            unsigned int k,
            const Sequence& runBestSequence,
            const std::vector<int>& M
        );
        
    public:
        Solver(
            const unsigned int L = 250,
            const unsigned int seed = time(nullptr),
            const unsigned int nfesLmt = 1000000,
            const unsigned int threads = 1
        ) : L(L), seed(seed), nfesLmt(nfesLmt), threads(threads),
            nfes(0), metrics(Runtime(), CpuTime(), EnergyConsumption()), 
            currentSequence(), bestSequence(), generator(seed) { 
            if(threads < 1) {
                throw std::logic_error("Number of threads can't be lesser than 1.");
            }
                
        }
        
        inline const Sequence& getCurrentSequence() const { return this->currentSequence; }
        inline const Sequence& getBestSequence() const { return this->bestSequence; }
        
        const std::string toString() const;
        
        Sequence generateNewSequence();
        unsigned int valueFlip(
            const std::vector<int>& S,
            const std::vector<int>& C,
            const std::vector<std::vector<int>>& T,
            unsigned int index
        );

        void randomSearch();
        void steepestDescentLocalSearch();        
        void tabuSearch();
        void solve(unsigned int arg);
};