#include "random_generator.hpp"
#include "sequence.hpp"
#include "metrics.hpp"
#include <stdexcept>
#include <thread>

#pragma once

#define RANDOM_SEARCH  0
#define SDLS           1
#define TABU_SEARCH    2

struct NeighbourResultNaive {
    Sequence sequence;
    uint32_t numberOfEvaluations;
};

struct NeighbourResult {
    uint32_t index;
    uint32_t energy;
    uint32_t numberOfEvaluations;
    bool foundMove;
};

class Solver {
    private:
        uint32_t L;
        uint32_t seed;
        uint32_t nfesLmt;
        uint32_t threads;
        uint32_t nfes;
        // neighbour evaluation cache
        std::vector<std::vector<int>> T;
        Metrics metrics;
        Sequence currentSequence;
        Sequence bestSequence;
        RandomGenerator generator;
        uint32_t algorithmUsed;
        
        uint32_t calculateSpeed() const;
        
        std::vector<int> getFlippedVector(const std::vector<int>& S, uint32_t index) const;
        
        NeighbourResultNaive evaluateNeighboursRandom(
            uint32_t start,
            uint32_t end,
            uint32_t t
        );
        NeighbourResult evaluateNeighboursSDLS(uint32_t start, uint32_t end);
        NeighbourResult evaluateNeighboursTabu(
            uint32_t start,
            uint32_t end,
            const int k,
            const Sequence& runBestSequence,
            const std::vector<int>& M
        );

        bool updateBestNeighbour(uint32_t index, uint32_t energy);

        void createThreadsRandom(
            std::vector<std::thread>& workers,
            std::vector<NeighbourResultNaive>& results
        );
        void createThreadsSDLS(
            std::vector<std::thread>& workers,
            std::vector<NeighbourResult>& results
        );
        void createThreadsTabu(
            std::vector<std::thread>& workers,
            std::vector<NeighbourResult>& results,
            const int k,
            const Sequence& runBestSequence,
            const std::vector<int>& M
        );
        
    public:
        Solver(
            const uint32_t L = 250,
            const uint32_t seed = time(nullptr),
            const uint32_t nfesLmt = 1000000,
            const uint32_t threads = 1
        ) : L(L), seed(seed), nfesLmt(nfesLmt), threads(threads),
            nfes(0), metrics(Runtime(), CpuTime(), EnergyConsumption()), 
            currentSequence(), bestSequence(), generator(seed), algorithmUsed(0) { 
            if((int32_t)threads < 1) {
                throw std::logic_error("Number of threads must be greater than 0.");
            }

            if((int32_t)L <= 0) {
                throw std::logic_error("Length of a sequence must be greater than 0.");
            }

            if((int32_t)nfesLmt < 0) {
                throw std::logic_error("Number of maximal evaluations must be greater than 0.");
            }
        }
        
        inline const uint32_t getSeed() const { return this->seed; }
        inline const uint32_t getThreads() const { return this->threads; }
        inline const uint32_t getNfes() const { return this->nfes; }
        inline const Metrics& getMetrics() const { return this->metrics; }
        inline const Sequence& getCurrentSequence() const { return this->currentSequence; }
        inline const Sequence& getBestSequence() const { return this->bestSequence; }
        inline const uint32_t getAlgorithmUsed() const { return this->algorithmUsed; }

        const std::string toString() const;
        
        Sequence generateNewSequence();
        uint32_t valueFlip(
            const std::vector<int>& S,
            const std::vector<int>& C,
            const std::vector<std::vector<int>>& T,
            uint32_t index
        );

        void randomSearch();
        void steepestDescentLocalSearch();        
        void tabuSearch();
        void solve(uint32_t arg);
};