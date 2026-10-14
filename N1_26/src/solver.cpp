#include "solver.hpp"
#include <limits>
#include <sstream>

const std::string Solver::toString() const {
    std::stringstream ss;

    ss << "L: " << this->L << "\n";
    ss << "seed: " << this->seed << "\n";
    ss << "max evaluations: " << this->nfesLmt << "\n";
    ss << "threads: " << this->threads << "\n";
    ss << "number of evaluations: " << this->nfes << "\n";
    ss << "runtime: " << this->runtime << "\n";
    ss << "cpu time: " << this->cpuTime << "\n";
    ss << "energy consumption: " << this->energyConsumption << "\n";
    ss << "speed: " << this->speed << "\n";

    return ss.str();
}

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
    unsigned int bestIndex = 0;
    unsigned int bestEnergy = std::numeric_limits<unsigned int>::max();

    for(unsigned int i = 0; i < S.getS().size(); i++) {
        std::vector<int> neighbour = S.getS();

        neighbour[i] *= -1;
        std::vector<int> neighbourC = Calculator::calculateC(neighbour);
        unsigned int energy = Calculator::calculateEnergy(neighbourC);

        if(energy < bestEnergy) {
            bestEnergy = energy;
            bestIndex = i;
        }
    }

    return bestIndex;
}

void Solver::evaluateNeighbours() {

}