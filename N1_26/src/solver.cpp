#include "solver.hpp"
#include <chrono>
#include <limits>
#include <sstream>

Sequence Solver::generateNewSequence() {
    auto S = generator.generateSequence(this->L);
    auto C = Calculator::calculateC(S);
    auto meritFactor = Calculator::calculateMeritFactor(S, C);
    auto peakSidelobeLevel = Calculator::calculatePeakSidelobeLevel(C);
    auto energy = Calculator::calculateEnergy(C);
    this->nfes++;

    return Sequence(S, C, meritFactor, peakSidelobeLevel, energy);
}

const std::string Solver::toString() const {
    std::stringstream ss;

    ss << "sequence length: " << this->L << "\n";
    ss << "seed: " << this->seed << "\n";
    ss << "max evaluations: " << this->nfesLmt << "\n";
    ss << "threads: " << this->threads << "\n";
    ss << "number of evaluations: " << this->nfes << "\n";
    ss << "runtime: " << this->runtime << "s\n";
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
        
        if(newMeritFactor > currentSequence.getMeritFactor()) {
            auto newPeakSidelobeLevel = Calculator::calculatePeakSidelobeLevel(C);
            auto newEnergy = Calculator::calculateEnergy(C);
            auto newSequence = Sequence(S, C, newMeritFactor, newPeakSidelobeLevel, newEnergy);
            
            this->currentSequence = newSequence;
            std::cout << "MF: " << newMeritFactor << ", PSL: " << newPeakSidelobeLevel << std::endl;
        }
        
        this->nfes++;
    }
}

NeighbourResult Solver::evaluateNeighboursNaive() {
    const auto& currentSequence = this->getCurrentSequence();
    unsigned int bestIndex = 0;
    auto bestEnergy = std::numeric_limits<unsigned int>::max();
    
    for(unsigned int i = 0; i < currentSequence.getS().size(); i++) {
        auto neighbourS = currentSequence.getS();
        
        neighbourS[i] *= -1;
        auto neighbourC = Calculator::calculateC(neighbourS);
        auto neighbourEnergy = Calculator::calculateEnergy(neighbourC);
        
        if(neighbourEnergy < bestEnergy) {
            bestEnergy = neighbourEnergy;
            bestIndex = i;
        }
        
        this->nfes++;
    }
    
    return NeighbourResult { bestIndex, bestEnergy };
}

bool Solver::updateBestNeighbourNaive(unsigned int index, unsigned int energy) {
    auto currentEnergy = this->currentSequence.getEnergy();
    
    auto newS = this->currentSequence.getS();
    // flip bit
    newS[index] *= -1;
    auto newC = Calculator::calculateC(newS);
    auto newEnergy = energy;
    
    if(newEnergy >= currentEnergy) {
        return false;
    }
    
    auto newMeritFactor = Calculator::calculateMeritFactor(newS, newC, newEnergy);
    auto newPeakSidelobeLevel = Calculator::calculatePeakSidelobeLevel(newC);
    this->currentSequence = Sequence(newS, newC, newMeritFactor, newPeakSidelobeLevel, newEnergy);
    
    std::cout << "MF: " << newMeritFactor << ", PSL: " << newPeakSidelobeLevel << std::endl;
    
    
    return true;
}

void Solver::steepestDescentLocalSearchNaive() {
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    
    this->currentSequence = this->generateNewSequence();
    this->bestSequence = currentSequence;
    
    while(this->nfes < this->nfesLmt) {
        bool canImprove = true;
        
        while(canImprove) {
            bool canEvaluateNeighbourhood = this->nfes + this->L <= this->nfesLmt;
            
            if(!canEvaluateNeighbourhood) {
                break;
            }

            auto result = evaluateNeighbours();
            canImprove = updateBestNeighbour(result.index, result.energy);
        }
        
        if(this->currentSequence.getEnergy() < this->bestSequence.getEnergy()) {
            bestSequence = currentSequence;
        }
        
        bool canRestart = this->nfes + this->L + 1 <= this->nfesLmt;

        if(!canRestart) {
            break;
        }

        this->currentSequence = this->generateNewSequence();
    }

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

    this->runtime = std::chrono::system_clock::to_time_t(end) - std::chrono::system_clock::to_time_t(start);
}

unsigned int Solver::valueFlip(
    const std::vector<int>& S,
    const std::vector<int>& C,
    const std::vector<std::vector<int>>& T,
    unsigned int index
) {
    unsigned int f = 0;

    for(unsigned int p = 1; p < S.size(); p++) {
        int value = C[p];

        if(p < S.size() - index) {
            value -= 2 * T[p][index];
        }

        if(p <= index) {
            value -= 2 * T[p][index - p];
        }

        f += value * value;
    }

    return f;
}

NeighbourResult Solver::evaluateNeighbours() {
    unsigned int bestIndex = 0;
    auto bestEnergy = std::numeric_limits<unsigned int>::max();
    
    for(unsigned int i = 0; i < currentSequence.getS().size(); i++) {
        auto neighbourEnergy = this->valueFlip(
            this->currentSequence.getS(), 
            this->currentSequence.getC(), 
            this->T, 
            i
        );
        
        if(neighbourEnergy < bestEnergy) {
            bestEnergy = neighbourEnergy;
            bestIndex = i;
        }
        
        this->nfes++;
    }
    
    return NeighbourResult { bestIndex, bestEnergy };
}

bool Solver::updateBestNeighbour(unsigned int index, unsigned int energy) {
    auto currentEnergy = this->currentSequence.getEnergy();
    
    // copy the sequence for flip
    auto newS = this->currentSequence.getS();
    // flip bit
    newS[index] *= -1;
    auto newC = Calculator::calculateC(newS);
    auto newEnergy = energy;

    if(newEnergy >= currentEnergy) {
       return false;
    }

    auto newMeritFactor = Calculator::calculateMeritFactor(newS, newC, newEnergy);
    auto newPeakSidelobeLevel = Calculator::calculatePeakSidelobeLevel(newC);
    this->currentSequence = Sequence(newS, newC, newMeritFactor, newPeakSidelobeLevel, newEnergy);
    this->T = Calculator::calculateT(newS);

    std::cout << "MF: " << newMeritFactor << ", PSL: " << newPeakSidelobeLevel << std::endl;

    return true;
}

void Solver::steepestDescentLocalSearch() {
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    
    this->currentSequence = this->generateNewSequence();
    this->bestSequence = currentSequence;
    this->T = Calculator::calculateT(this->currentSequence.getS());        
    
    while(this->nfes < this->nfesLmt) {
        bool canImprove = true;
        
        while(canImprove) {
            bool canEvaluateNeighbourhood = this->nfes + this->L <= this->nfesLmt;

            if(!canEvaluateNeighbourhood) {
                break;
            }

            auto result = evaluateNeighbours();
            canImprove = updateBestNeighbour(result.index, result.energy);
        }
        
        if(this->currentSequence.getEnergy() < this->bestSequence.getEnergy()) {
            bestSequence = currentSequence;
        }
        
        bool canRestart = this->nfes + this->L + 1 <= this->nfesLmt;

        if(!canRestart) {
            break;
        }

        this->currentSequence = this->generateNewSequence();
        this->T = Calculator::calculateT(this->currentSequence.getS());
    }

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

    this->runtime = std::chrono::system_clock::to_time_t(end) - std::chrono::system_clock::to_time_t(start);
}

void Solver::tabuSearch() {

}