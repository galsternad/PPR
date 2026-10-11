#include <vector>

#include "sequence_generator.hpp"
#include "calculator.hpp"

class Solver {
    private:
        unsigned int L;
        unsigned int seed;
        unsigned int nfesLmt;
        unsigned int t;
        unsigned int nfes;
        unsigned int runTime;
        unsigned int cpuTime;
        unsigned int energyConsumption;
        unsigned int speed;
        std::vector<int> sequence;
        double meritFactor;
        unsigned int peakSidelobeLevel;
        SequenceGenerator generator;

    public:
        Solver(
            unsigned int L,
            unsigned int seed,
            unsigned int nfesLmt,
            unsigned int t
        ) : L(L), seed(seed), nfesLmt(nfesLmt), t(t),
            nfes(0), runTime(0), cpuTime(0), energyConsumption(0),
            speed(0), sequence(std::vector<int>(L)), meritFactor(0.0),
            peakSidelobeLevel(0), generator(seed) {
            
        }

        inline std::vector<int> getSequence() const { return this->sequence; }
        inline unsigned int getMeritFactor() const { return this->meritFactor; }
        inline unsigned int getPeakSidelobeLevel() const { return this->peakSidelobeLevel; }

        void solveNaive();
};