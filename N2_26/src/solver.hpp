#include "random_generator.hpp"
#include "sequence.hpp"
#include "metrics.hpp"
#include "sync_queue.hpp"
#include "reactor.hpp"
#include <functional>
#include <memory>
#include <stdexcept>
#include <thread>

#pragma once

enum Algorithm {
    RANDOM_SEARCH = 0,
    SDLS          = 1,
    TABU_SEARCH   = 2
};

enum ParallelMode {
    THREADS             = 0,
    PRODUCER_CONSUMER   = 1
};

struct NeighbourResult {
    uint32_t index;
    uint32_t energy;
    uint32_t numberOfEvaluations;
    bool foundMove;
    Sequence sequence;
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
        SyncQueue<std::function<void()>> tasksQueue;
        SyncQueue<NeighbourResult> resultsQueue;
        std::vector<std::unique_ptr<Reactor<std::function<void()>>>> reactors;
        Algorithm algorithm;
        ParallelMode parallelMode;
        
        uint32_t calculateSpeed() const;
        
        std::vector<int> getFlippedVector(const std::vector<int>& S, uint32_t index) const;
        
        NeighbourResult evaluateNeighboursRandom(uint32_t t);
        NeighbourResult evaluateNeighboursRandom(
            uint32_t start,
            uint32_t end,
            uint32_t t
        );
        NeighbourResult evaluateNeighboursSDLS(int i);
        NeighbourResult evaluateNeighboursSDLS(uint32_t start, uint32_t end);
        NeighbourResult evaluateNeighboursTabu(
            const int k,
            const int i,
            const Sequence& runBestSequence,
            const std::vector<int>& M
        );
        NeighbourResult evaluateNeighboursTabu(
            uint32_t start,
            uint32_t end,
            const int k,
            const Sequence& runBestSequence,
            const std::vector<int>& M
        );

        bool updateBestNeighbour(uint32_t index, uint32_t energy);

        void evaluateRandomQueue();
        void evaluateSDLSQueue(
            uint32_t& bestEnergy,
            uint32_t& bestIndex
        );
        void evaluateSDLSThreads(
            uint32_t& bestEnergy,
            uint32_t& bestIndex
        );
        void evaluateTabuQueue(
            uint32_t& bestEnergy,
            uint32_t& bestIndex,
            bool& foundMove,
            int k,
            const Sequence& runBestSequence,
            const std::vector<int>& M 
        );
        void evaluateTabuThreads(
            uint32_t& bestEnergy,
            uint32_t& bestIndex,
            bool& foundMove,
            int k,
            const Sequence& runBestSequence,
            const std::vector<int>& M 
        );

        void createThreadsRandom(
            std::vector<std::thread>& workers,
            std::vector<NeighbourResult>& results
        );

        template <typename Evaluator>
        void createThreads(
            std::vector<std::thread>& workers,
            std::vector<NeighbourResult>& results,
            Evaluator evaluator
        );

        void startReactors();
        
    public:
        Solver(
            const uint32_t L = 250,
            const uint32_t seed = time(nullptr),
            const uint32_t nfesLmt = 1000000,
            const uint32_t threads = 1
        ) : L(L), seed(seed), nfesLmt(nfesLmt), threads(threads),
            nfes(0), metrics(Runtime(), CpuTime(), EnergyConsumption()), 
            currentSequence(), bestSequence(), generator(seed), algorithm(TABU_SEARCH),
            tasksQueue(), parallelMode() { 
            if((int32_t)threads < 1) {
                throw std::logic_error("Number of threads must be greater than 0.");
            }

            if((int32_t)L <= 0) {
                throw std::logic_error("Length of a sequence must be greater than 0.");
            }

            if((int32_t)nfesLmt < 0) {
                throw std::logic_error("Number of maximal evaluations must be greater than 0.");
            }

            if(parallelMode == ParallelMode::PRODUCER_CONSUMER) {
                for(uint32_t i = 0; i < this->threads; i++) {
                    reactors.push_back(
                        std::unique_ptr<Reactor<std::function<void()>>>(
                            new Reactor<std::function<void()>>(tasksQueue)
                        )
                    );
                }           
            }
        }

                Solver(
            const uint32_t L = 250,
            const uint32_t seed = time(nullptr),
            const uint32_t nfesLmt = 1000000,
            const uint32_t threads = 1,
            ParallelMode parallelMode = ParallelMode::THREADS
        ) : L(L), seed(seed), nfesLmt(nfesLmt), threads(threads),
            nfes(0), metrics(Runtime(), CpuTime(), EnergyConsumption()), 
            currentSequence(), bestSequence(), generator(seed), algorithm(TABU_SEARCH),
            tasksQueue(), parallelMode() { 
            if((int32_t)threads < 1) {
                throw std::logic_error("Number of threads must be greater than 0.");
            }

            if((int32_t)L <= 0) {
                throw std::logic_error("Length of a sequence must be greater than 0.");
            }

            if((int32_t)nfesLmt < 0) {
                throw std::logic_error("Number of maximal evaluations must be greater than 0.");
            }

            if(parallelMode == ParallelMode::PRODUCER_CONSUMER) {
                for(uint32_t i = 0; i < this->threads; i++) {
                    reactors.push_back(
                        std::unique_ptr<Reactor<std::function<void()>>>(
                            new Reactor<std::function<void()>>(tasksQueue)
                        )
                    );
                }           
            }
        }

        ~Solver() {
            tasksQueue.stop();
        }
        
        inline const uint32_t getSeed() const { return this->seed; }
        inline const uint32_t getThreads() const { return this->threads; }
        inline const uint32_t getNfes() const { return this->nfes; }
        inline const Metrics& getMetrics() const { return this->metrics; }
        inline const Sequence& getCurrentSequence() const { return this->currentSequence; }
        inline const Sequence& getBestSequence() const { return this->bestSequence; }
        inline const Algorithm getAlgorithm() const { return this->algorithm; }

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
        void solve(Algorithm arg);
};

template <typename Evaluator>
void Solver::createThreads(
    std::vector<std::thread>& workers,
    std::vector<NeighbourResult>& results,
    Evaluator evaluator
) {
    for(int t = 0; t < this->threads; t++) {
        const uint32_t chunkSize = this->L / this->threads;
        const uint32_t start = t * chunkSize;
        const uint32_t end = (t == this->threads - 1) 
            ? this->L
            : start + chunkSize;

        workers.emplace_back([&, t, start, end, evaluator]() {
            results[t] = evaluator(start, end);
        });
    }

    for(auto& t : workers) {
        t.join();
    }
}