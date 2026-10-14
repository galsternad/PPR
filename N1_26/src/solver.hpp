#include "sequence_generator.hpp"
#include "sequence.hpp"
#include "calculator.hpp"

class Solver {
    private:
        SequenceGenerator generator;
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
        Sequence sequence;

    public:
        Solver(
            const unsigned int L = 100,
            const unsigned int seed = time(nullptr),
            const unsigned int nfesLmt = 1000,
            const unsigned int threads = 1
        ) : generator(seed), L(L), seed(seed), nfesLmt(nfesLmt), 
            threads(threads), nfes(0), runtime(0), cpuTime(0), 
            energyConsumption(0), speed(0), sequence() {
            
        }

        inline unsigned int getRunTime() const { return this->runtime; }
        inline const Sequence& getSequence() const { return this->sequence; }

        const std::string toString() const;

        void randomSearch();

        unsigned int valueFlipEval(const Sequence& sequence);
        void evaluateNeighbours();
        void steepestDescentLocalSearch();
        
        void tabuSearch();
};