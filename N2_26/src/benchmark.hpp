#pragma once

#include "metrics.hpp"
#include "sequence.hpp"
#include "solver.hpp"
#include <sstream>

struct Result {
    uint32_t L;
    uint32_t seed;
    uint32_t nfes;
    uint32_t threads;
    Algorithm algorithm;
    ParallelMode parallelMode;
    Sequence sequence;
    Metrics metrics;

    Result() { }

    Result(
        uint32_t L,
        uint32_t seed,
        uint32_t nfes,
        uint32_t threads,
        Algorithm algorithm,
        ParallelMode parallelMode,
        Sequence sequence,
        Metrics metrics
    ) : L(L), seed(seed), nfes(nfes), threads(threads), parallelMode(parallelMode),
        algorithm(algorithm), sequence(sequence), metrics(metrics) { }

    const std::string toString() const {
        std::stringstream ss;

        ss << "L:                       " << L          << "\n";
        ss << "seed:                    " << seed       << "\n";
        ss << "nfes:                    " << nfes       << "\n";
        ss << "threads:                 " << threads    << "\n";
        ss << "algorithm:               ";
        switch(algorithm) {
            case Algorithm::RANDOM_SEARCH:
                ss << "Random search\n";
                break;
            case Algorithm::SDLS:
                ss << "SDLS\n";
                break;
            case Algorithm::TABU_SEARCH:
                ss << "Tabu search\n";
                break;
            default:
                break;
        }       

        ss << "mode:                    ";
        switch(parallelMode) {
            case ParallelMode::THREADS:
                ss << "threads\n";
                break;
            case ParallelMode::PRODUCER_CONSUMER:
                ss << "producer/consumer\n";
                break;
            default:
                break;
        }

        ss << sequence.toString() << "\n";
        ss << metrics.toString() << "\n";

        return ss.str();
    }
};

class Benchmark {
    private:
        uint32_t L;
        uint32_t nfesLmt;
        uint32_t maxNumberOfThreads;
        std::vector<uint32_t> seeds;
        // first RandomSearch iteration
        Result initialResult;
        // vector, containing first iteration of each algorithm, where t = 1
        std::vector<Result> firstIterationResults;
        std::vector<Result> results;
        static uint32_t iteration;

        void evaluateThreads(uint32_t seed, Algorithm algorithm, ParallelMode parallelMode);
        void evaluateAlgorithms(uint32_t seed, ParallelMode parallelMode);
        void evaluateSeeds(ParallelMode parallelMode);
        const Result getBestResult() const;
        const Result createNewResult();

    public:
        Benchmark(
            const uint32_t L,
            const uint32_t initialSeed,
            const uint32_t nfesLmt,
            const uint32_t maxNumberOfThreads
        ) : L(L), nfesLmt(nfesLmt), maxNumberOfThreads(maxNumberOfThreads),
            seeds(), initialResult(), firstIterationResults(), results() {
            if((int32_t)maxNumberOfThreads < 1) {
                throw std::logic_error("Number of threads must be equal to or greater than 1.");
            }

            if((int32_t)L <= 0) {
                throw std::logic_error("Length of a sequence must be greater than 0.");
            }

            if((int32_t)nfesLmt <= 0) {
                throw std::logic_error("Number of maximal evaluations must be greater than 0.");
            }

            for(int i = 0; i < 3; i++) {
                seeds.push_back(initialSeed + i);
            }
        }

        inline const uint32_t getL() { return this->L; }
        const double calculateEnergyConsumptionImprovement(const Result& resultObj) const;
        const double calculateSpeedImprovement(const Result& resultObj) const;

        const std::string toString() const;
        void run();
        void evaluate();
};