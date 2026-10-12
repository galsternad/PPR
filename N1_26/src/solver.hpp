#include "sequence_generator.hpp"
#include "sequence.hpp"
#include "calculator.hpp"

class Solver {
    private:
        SequenceGenerator generator;
        unsigned int L;
        unsigned int seed;
        unsigned int nfesLmt;
        unsigned int t;
        unsigned int nfes;
        unsigned int runTime;
        unsigned int cpuTime;
        unsigned int energyConsumption;
        unsigned int speed;
        Sequence sequence;

    public:
        Solver(
            unsigned int L,
            unsigned int seed,
            unsigned int nfesLmt,
            unsigned int t
        ) : generator(seed), L(L), seed(seed), nfesLmt(nfesLmt), t(t),
            nfes(0), runTime(0), cpuTime(0), energyConsumption(0),
            speed(0), sequence() {
            
        }

        inline unsigned int getRunTime() const { return this->runTime; }

        void solveNaive();
};