#include "solver.hpp"

struct BestResult {
    Sequence sequence;
    Metrics metrics;
};

class Benchmark {
    private:
        uint32_t L;
        uint32_t nfesLmt;
        uint32_t maxNumberOfThreads;
        std::vector<uint32_t> seeds;
        Solver solver;
        Solver initialSolver;
        std::vector<Solver> solvers;
        static uint32_t iteration;

        void evaluateThreads(uint32_t seed, uint32_t algorithm);
        void evaluateAlgorithms(uint32_t seed);
        const BestResult getBestResult() const;

    public:
        Benchmark(
            const uint32_t L,
            const uint32_t initialSeed,
            const uint32_t nfesLmt,
            const uint32_t maxNumberOfThreads
        ) : L(L), nfesLmt(nfesLmt), maxNumberOfThreads(maxNumberOfThreads),
            seeds(), solver(), initialSolver(), solvers() {
            for(int i = 0; i < 3; i++) {
                seeds.push_back(initialSeed + i);
            }
        }

        inline const uint32_t getL() { return this->L; }
        const double calculateEnergyConsumptionImprovement(const Solver& solver) const;
        const double calculateSpeedImprovement(const Solver& solver) const;

        const std::string toString() const;
        void run();
        void evaluate();
};