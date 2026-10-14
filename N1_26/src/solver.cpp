#include "solver.hpp"
#include <limits>

void Solver::randomSearch() {
    for(unsigned int i = 0; i < this->nfesLmt; i++) {
        auto S = generator.generateSequence(this->L);
        auto C = Calculator::calculateC(S);
        auto newMeritFactor = Calculator::calculateMeritFactor(S, C);
        
        if(newMeritFactor > sequence.getMeritFactor()) {
            auto newPeakSidelobeLevel = Calculator::calculatePeakSidelobeLevel(C);
            auto newSequence = Sequence(S, C, newMeritFactor, newPeakSidelobeLevel);
            
            this->sequence = newSequence;
        }

        this->nfes++;
    }
}

unsigned int Solver::valueFlipEval(const Sequence& S) {
    unsigned int index = 0;
    unsigned int bestEnergy = std::numeric_limits<unsigned int>::max();

    for(unsigned int i = 0; i < S.getS().size(); i++) {
        std::vector<int> neighbour = S.getS();

        neighbour[i] *= -1;
        std::vector<int> neighbourC = Calculator::calculateC(neighbour);
        unsigned int energy = Calculator::calculateEnergy(neighbourC);

        if(energy < bestEnergy) {
            bestEnergy = energy;
            index = i;
        }
    }

    return index;
}

void Solver::evaluateNeighbours() {

}