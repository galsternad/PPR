#include "solver.hpp"

void Solver::solveNaive() {
    for(unsigned int i = 0; i < this->nfesLmt; i++) {
        auto S = generator.generateSequence(this->L);
        auto C = Calculator::naiveCalculateC(S);
        auto newPeakSidelobeLevel = Calculator::calculatePeakSidelobeLevel(C);
        auto newMeritFactor = Calculator::calculateMeritFactor(S, C);

        if(newMeritFactor > this->meritFactor) {
            this->sequence = S;
            this->meritFactor = newMeritFactor;
            this->peakSidelobeLevel = newPeakSidelobeLevel;
        }
        
        this->nfes++;
    }
}