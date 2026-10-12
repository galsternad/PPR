#include "solver.hpp"
#include <chrono>

void Solver::solveNaive() {
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    for(unsigned int i = 0; i < this->nfesLmt; i++) {
        auto S = generator.generateSequence(this->L);
        auto C = Calculator::naiveCalculateC(S);
        auto newMeritFactor = Calculator::calculateMeritFactor(S, C);
        
        if(newMeritFactor > sequence.getMeritFactor()) {
            auto newPeakSidelobeLevel = Calculator::calculatePeakSidelobeLevel(C);
            auto newSequence = Sequence(S, C, newMeritFactor, newPeakSidelobeLevel);
            
            this->sequence = newSequence;
        }

        this->nfes++;
    }

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

    this->runTime = std::chrono::system_clock::to_time_t(end) - std::chrono::system_clock::to_time_t(start); 
}