#include "solver.hpp"
#include "calculator.hpp"
#include <stdexcept>
#include <string>
#include <sys/time.h>
#include <limits>
#include <sstream>
#include <thread>

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

    ss << "length:                  " << this->L        << "\n";
    ss << "seed:                    " << this->seed     << "\n";
    ss << "max evaluations:         " << this->nfesLmt  << "\n";
    ss << "threads:                 " << this->threads  << "\n"; 
    ss << "number of evaluations:   " << this->nfes     << "\n";
    ss << metrics.toString()                            << "\n";
    ss << bestSequence.toString()                       << "\n";

    return ss.str();
}

NeighbourResultNaive Solver::evaluateNeighboursRandom(
    uint32_t start, 
    uint32_t end,
    uint32_t t
) {
    auto localRandomGenerator = RandomGenerator(seed + t);
    auto localBest = Sequence();
    uint32_t evaluations = 0;

    for(int i = start; i < end; i++) {
        auto S = localRandomGenerator.generateSequence(this->L);
        auto C = Calculator::calculateC(S);
        auto energy = Calculator::calculateEnergy(C);

        if(energy < localBest.getEnergy()) {
            auto meritFactor = Calculator::calculateMeritFactor(S, C, energy);
            auto peakSidelobeLevel = Calculator::calculatePeakSidelobeLevel(C);
            
            localBest = Sequence(S, C, meritFactor, peakSidelobeLevel, energy);
        }

        evaluations++;
    }

    return { localBest, evaluations };
}

void Solver::createThreadsRandom(
    std::vector<std::thread>& workers,
    std::vector<NeighbourResultNaive>& results
) {
    for(int t = 0; t < this->threads; t++) {
        const uint32_t chunkSize = this->nfesLmt / this->threads;
        const uint32_t start = t * chunkSize;
        const uint32_t end = (t + 1) * chunkSize;
        
        workers.emplace_back([&, start, end, t]() {
            results[t] = evaluateNeighboursRandom(start, end, t);
        });
    }

    for(auto& t : workers) {
        t.join();
    }
}

void Solver::randomSearch() {
    this->bestSequence = Sequence();

    auto workers = std::vector<std::thread>();
    auto results = std::vector<NeighbourResultNaive>(this->threads);

    this->createThreadsRandom(workers, results);

    for(const auto& result : results) {
        if(result.sequence.getEnergy() < this->bestSequence.getEnergy()) {
            this->bestSequence = result.sequence;
        }
        
        this->nfes += result.numberOfEvaluations;
    }
}


uint32_t Solver::valueFlip(
    const std::vector<int>& S,
    const std::vector<int>& C,
    const std::vector<std::vector<int>>& T,
    uint32_t index
) {
    uint32_t f = 0;
    
    for(int p = 1; p < S.size(); p++) {
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

std::vector<int> Solver::getFlippedVector(const std::vector<int>& S, uint32_t index) const {
    auto vec = S;
    vec[index] *= -1;
    
    return vec;
}

NeighbourResult Solver::evaluateNeighboursSDLS(uint32_t start, uint32_t end) {
    uint32_t bestIndex = 0;
    auto bestEnergy = std::numeric_limits<uint32_t>::max();
    uint32_t evaluations = 0;
    
    for(int i = start; i < end; i++) {
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

        evaluations++;
    }
    
    return NeighbourResult { bestIndex, bestEnergy, evaluations, false };
}

bool Solver::updateBestNeighbour(uint32_t index, uint32_t energy) {
    auto currentEnergy = this->currentSequence.getEnergy();
    
    if(energy >= currentEnergy) {
        return false;
    }
    
    // copy the sequence for flip
    auto newS = this->currentSequence.getS();
    // flip bit
    newS[index] *= -1;
    auto newC = Calculator::calculateC(newS);
    auto newMeritFactor = Calculator::calculateMeritFactor(newS, newC, energy);
    auto newPeakSidelobeLevel = Calculator::calculatePeakSidelobeLevel(newC);
    this->currentSequence = Sequence(newS, newC, newMeritFactor, newPeakSidelobeLevel, energy);
    this->T = Calculator::calculateT(newS);
    
    return true;
}

void Solver::createThreadsSDLS(
    std::vector<std::thread>& workers,
    std::vector<NeighbourResult>& results
) {
    for(int t = 0; t < this->threads; t++) {
        const uint32_t chunkSize = this->L / this->threads;
        const uint32_t start = t * chunkSize;
        const uint32_t end = (t == this->threads - 1) 
            ? this->L
            : start + chunkSize;

        workers.emplace_back([&, t, start, end]() {
            results[t] = evaluateNeighboursSDLS(start, end);
        });
    }

    for(auto& t : workers) {
        t.join();
    }
}

void Solver::steepestDescentLocalSearch() {
    this->currentSequence = this->generateNewSequence();
    this->bestSequence = this->currentSequence;
    this->T = Calculator::calculateT(this->currentSequence.getS());        
    
    while(this->nfes < this->nfesLmt) {
        bool canImprove = true;
        
        while(canImprove) {
            bool canEvaluateNeighbourhood = this->nfes + this->L <= this->nfesLmt;
            
            if(!canEvaluateNeighbourhood) {
                break;
            }
            
            uint32_t bestIndex = 0;
            auto bestEnergy = std::numeric_limits<uint32_t>::max();
            std::vector<std::thread> threads;
            std::vector<NeighbourResult> results(this->threads);
            this->createThreadsSDLS(threads, results);
            
            for(const auto& result : results) {
                if(result.energy < bestEnergy) {
                    bestEnergy = result.energy;
                    bestIndex = result.index;
                }

                this->nfes += result.numberOfEvaluations;
            }

            canImprove = this->updateBestNeighbour(bestIndex, bestEnergy);
        }
        
        if(this->currentSequence.getEnergy() < this->bestSequence.getEnergy()) {
            this->bestSequence = this->currentSequence;
        }
        
        bool canRestart = this->nfes + this->L + 1 <= this->nfesLmt;
        
        if(!canRestart) {
            break;
        }
        
        this->currentSequence = this->generateNewSequence();
        this->T = Calculator::calculateT(this->currentSequence.getS());
    }
}

NeighbourResult Solver::evaluateNeighboursTabu(
    const uint32_t start,
    const uint32_t end,
    const int k,
    const Sequence& runBestSequence,
    const std::vector<int>& M
) {
    uint32_t bestIndex = 0;
    auto bestEnergy = std::numeric_limits<uint32_t>::max();
    auto foundMove = false;
    uint32_t evaluations = 0;
    
    for(int i = start; i < end; i++) {
        auto energy = this->valueFlip(
            this->currentSequence.getS(),
            this->currentSequence.getC(),
            this->T,
            i
        );
        
        if((k >= M[i]) || (energy < runBestSequence.getEnergy())) {
            if(energy < bestEnergy) {
                bestEnergy = energy;
                bestIndex = i;
                foundMove = true;
            }
        }

        evaluations++;
    }
    
    return { bestIndex, bestEnergy, evaluations, foundMove };
}

void Solver::createThreadsTabu(
    std::vector<std::thread>& workers,
    std::vector<NeighbourResult>& results,
    const int k,
    const Sequence& runBestSequence,
    const std::vector<int>& M
) {
    for(int t = 0; t < this->threads; t++) {
        const uint32_t chunkSize = this->L / this->threads;
        const uint32_t start = t * chunkSize;
        const uint32_t end = (t == this->threads - 1)
        ? this->L
        : start + chunkSize;

        workers.emplace_back([&, t, start, end, k]() {
            results[t] = evaluateNeighboursTabu(
                start,
                end,
                k,
                runBestSequence,
                M  
            );
        });
    }
    
    for(auto& t : workers) {
        t.join();
    }

}

void Solver::tabuSearch() {
    this->currentSequence = this->generateNewSequence();
    this->bestSequence = this->currentSequence;
    this->T = Calculator::calculateT(this->currentSequence.getS());
    
    while(this->nfes < this->nfesLmt) {
        std::vector<int> M(this->L, 0);
        auto runBestSequence = this->currentSequence;
        uint32_t minValue = this->L / 2;
        uint32_t maxValue = 3 * this->L / 2;
        uint32_t maxIters = generator.generateRandomInt(minValue, maxValue);
        uint32_t minTabu = maxIters / 10;
        uint32_t extraTabu = maxIters / 50;
        
        if(extraTabu == 0) {
            ++extraTabu;
        }
        
        for(int k = 0; k < maxIters; k++) {
            bool canEvaluateNeighbourhood = this->nfes + this->L <= this->nfesLmt;
            bool foundMove = false;
            
            if(!canEvaluateNeighbourhood) {
                break;
            }
            
            auto bestEnergy = std::numeric_limits<uint32_t>::max();
            uint32_t bestIndex = 0;
            
            // threading
            std::vector<std::thread> workers;
            std::vector<NeighbourResult> results(this->threads);
            this->createThreadsTabu(
                workers,
                results,
                k,
                runBestSequence,
                M
            );
            
            for(const auto& result : results) {
                if(result.foundMove && result.energy < bestEnergy) {
                    bestEnergy = result.energy;
                    bestIndex = result.index;
                    foundMove = true;
                }

                this->nfes += result.numberOfEvaluations;
            }
            
            if(!foundMove) {
                break;
            }
            
            auto newS = this->getFlippedVector(this->currentSequence.getS(), bestIndex);
            auto newC = Calculator::calculateC(newS);
            auto newMeritFactor = Calculator::calculateMeritFactor(newS, newC, bestEnergy);
            auto newPeakSidelobeLevel = Calculator::calculatePeakSidelobeLevel(newC);
            auto newSequence = Sequence(newS, newC, newMeritFactor, newPeakSidelobeLevel, bestEnergy);
            this->currentSequence = newSequence;
            this->T = Calculator::calculateT(newS);
            M[bestIndex] = k + minTabu + generator.generateRandomInt(0, extraTabu - 1);
            
            if(this->currentSequence.getEnergy() < runBestSequence.getEnergy()) {
                runBestSequence = this->currentSequence;
            }
            
        }
        
        if(runBestSequence.getEnergy() < this->bestSequence.getEnergy()) {
            this->bestSequence = runBestSequence;
        }
        
        bool canRestart = this->nfes + this->L + 1 <= this->nfesLmt;
        
        if(!canRestart) {
            break;
        }
        
        this->currentSequence = this->generateNewSequence();
        this->T = Calculator::calculateT(this->currentSequence.getS());
    }
}

void Solver::solve(uint32_t arg) {
    this->metrics.start();
    
    switch(arg) {
        case RANDOM_SEARCH:
            this->randomSearch();
            this->algorithmUsed = RANDOM_SEARCH;
            break;
        case SDLS:
            this->steepestDescentLocalSearch();
            this->algorithmUsed = SDLS;
            break;
        case TABU_SEARCH:
            this->tabuSearch();
            this->algorithmUsed = TABU_SEARCH;
            break;
        default:
            throw std::logic_error("Invalid argument");
            break;
    }

    this->metrics.end();
    
    this->metrics.calculate(this->nfes);
}