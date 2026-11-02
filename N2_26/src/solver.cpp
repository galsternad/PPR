#include "solver.hpp"
#include "calculator.hpp"
#include "random_generator.hpp"
#include "sequence.hpp"
#include <iostream>
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

NeighbourResult Solver::evaluateNeighboursRandom(uint32_t t) {
    auto localRandomGenerator = RandomGenerator(seed + t);
    auto S = localRandomGenerator.generateSequence(this->L);
    auto C = Calculator::calculateC(S);
    auto energy = Calculator::calculateEnergy(C);
    auto meritFactor = Calculator::calculateMeritFactor(S, C, energy);
    auto peakSidelobeLevel = Calculator::calculatePeakSidelobeLevel(C);
    auto sequence = Sequence(
        S,
        C,
        meritFactor,
        peakSidelobeLevel,
        energy
    );

    return { 0, 0, 1, true, sequence };
}

NeighbourResult Solver::evaluateNeighboursRandom(
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

    return { 0, 0, evaluations, true, localBest };
}

void Solver::createThreadsRandom(
    std::vector<std::thread>& workers,
    std::vector<NeighbourResult>& results
) {
    for(int t = 0; t < this->threads; t++) {
        const uint32_t chunkSize = this->nfesLmt / this->threads;
        const uint32_t start = t * chunkSize;
        const uint32_t end = (t == this->threads - 1)
            ? this->nfesLmt
            : start + chunkSize;
        
        workers.emplace_back([&, start, end, t]() {
            results[t] = evaluateNeighboursRandom(start, end, t);
        });
    }

    for(auto& t : workers) {
        t.join();
    }
}

void Solver::evaluateRandomQueue() {
    for(int t = 0; t < this->nfesLmt; t++) {
        auto canExecute = tasksQueue.push([this, t]() {
            auto result = evaluateNeighboursRandom(t);
            resultsQueue.push(result);
        });

        if(!canExecute) {
            throw std::logic_error("Cannot execute tasks.");
        }
    }

    for(int t = 0; t < this->nfesLmt; t++) {
        NeighbourResult result;

        auto canExecute = resultsQueue.pop(result);

        if(!canExecute) {
            throw std::logic_error("Cannot retrieve result.");
        }

        if(result.sequence.getEnergy() < this->bestSequence.getEnergy()) {
            this->bestSequence = result.sequence;
        }

        this->nfes += result.numberOfEvaluations;
    }
}

void Solver::randomSearch() {
    this->bestSequence = Sequence();

    auto workers = std::vector<std::thread>();
    auto results = std::vector<NeighbourResult>(this->threads);

    if(parallelMode == ParallelMode::PRODUCER_CONSUMER) {
        evaluateRandomQueue();
    }
    else if(parallelMode == ParallelMode::THREADS) {
        createThreadsRandom(workers, results);
    }

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

NeighbourResult Solver::evaluateNeighboursSDLS(int i) {
    uint32_t bestIndex = 0;
    auto bestEnergy = std::numeric_limits<uint32_t>::max();
    uint32_t evaluations = 0;
    
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

    return { bestIndex, bestEnergy, evaluations, false };

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
    
    return { bestIndex, bestEnergy, evaluations, false };
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

void Solver::evaluateSDLSQueue(
    uint32_t& bestEnergy,
    uint32_t& bestIndex
) {
    for(int i = 0; i < this->L; i++) {
        auto canExecute = tasksQueue.push([this, i]() {
            auto result = evaluateNeighboursSDLS(i);
            resultsQueue.push(result);
        });
    
        if(!canExecute) {
            throw std::logic_error("Cannot execute tasks.");
        }
    }

    for(int i = 0; i < this->L; i++) {
        NeighbourResult result;

        auto canExecute = resultsQueue.pop(result);

        if(!canExecute) {
            throw std::logic_error("Cannot retrieve result.");
        }

        if(result.energy < bestEnergy) {
            bestEnergy = result.energy;
            bestIndex = result.index;
        }

        this->nfes += result.numberOfEvaluations;
    }

}

void Solver::evaluateSDLSThreads(
    uint32_t &bestEnergy,
    uint32_t &bestIndex
) {
    std::vector<std::thread> threads;
    std::vector<NeighbourResult> results(this->threads);

    createThreads(
        threads,
        results,
        [this](uint32_t start, uint32_t end) {
            return evaluateNeighboursSDLS(start, end);
        }
    );
    
    for(const auto& result : results) {
        if(result.energy < bestEnergy) {
            bestEnergy = result.energy;
            bestIndex = result.index;
        }

        this->nfes += result.numberOfEvaluations;
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

            if(parallelMode == ParallelMode::PRODUCER_CONSUMER) {
                evaluateSDLSQueue(bestEnergy, bestIndex);
            }
            else if(parallelMode == ParallelMode::THREADS) {
                evaluateSDLSThreads(bestEnergy, bestIndex);
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
    const int k,
    const int i,
    const Sequence& runBestSequence,
    const std::vector<int>& M
) {
    uint32_t bestIndex = 0;
    auto bestEnergy = std::numeric_limits<uint32_t>::max();
    uint32_t evaluations = 0;
    auto foundMove = false;
    
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

    return { bestIndex, bestEnergy, evaluations, foundMove };
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
    uint32_t evaluations = 0;
    auto foundMove = false;
    
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

void Solver::evaluateTabuQueue(
    uint32_t& bestEnergy,
    uint32_t& bestIndex,
    bool& foundMove,
    int k,
    const Sequence& runBestSequence,
    const std::vector<int>& M 
) {
    for(int i = 0; i < this->L; i++ ) {
        auto canExecute = tasksQueue.push([this, k, i, &runBestSequence, &M]() {
            auto result = evaluateNeighboursTabu(k, i, runBestSequence, M);
            resultsQueue.push(result);
        });

        if(!canExecute) {
            throw std::logic_error("Cannot execute tasks.");
        }
    }

    for(int i = 0; i < this->L; i++) {
        NeighbourResult result;

        auto canExecute = resultsQueue.pop(result);

        if(!canExecute) {
            throw std::logic_error("Cannot retrieve result.");
        }

        if(result.foundMove && result.energy < bestEnergy) {
            bestEnergy = result.energy;
            bestIndex = result.index;
            foundMove = true;
        }

        this->nfes += result.numberOfEvaluations;
    }
}

void Solver::evaluateTabuThreads(
    uint32_t& bestEnergy,
    uint32_t& bestIndex,
    bool& foundMove,
    int k,
    const Sequence& runBestSequence,
    const std::vector<int>& M 
) {
    std::vector<std::thread> workers;
    std::vector<NeighbourResult> results(this->threads);

    createThreads(
        workers,
        results,
        [this, k, &runBestSequence, &M](uint32_t start, uint32_t end) {
            return evaluateNeighboursTabu(
                start,
                end,
                k,
                runBestSequence,
                M
            );
        }
    );
    
    for(const auto& result : results) {
        if(result.foundMove && result.energy < bestEnergy) {
            bestEnergy = result.energy;
            bestIndex = result.index;
            foundMove = true;
        }

        this->nfes += result.numberOfEvaluations;
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
            if(this->parallelMode == ParallelMode::PRODUCER_CONSUMER) {
                evaluateTabuQueue(
                    bestEnergy,
                    bestIndex,
                    foundMove,
                    k,
                    runBestSequence,
                    M
                );
            }
            else if(this->parallelMode == ParallelMode::THREADS){
                evaluateTabuThreads(
                    bestEnergy,
                    bestIndex,
                    foundMove,
                    k,
                    runBestSequence,
                    M
                );
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

void Solver::solve(Algorithm arg) {
    this->metrics.start();
    
    switch(arg) {
        case Algorithm::RANDOM_SEARCH:
            this->randomSearch();
            this->algorithm = Algorithm::RANDOM_SEARCH;
            break;
        case Algorithm::SDLS:
            this->steepestDescentLocalSearch();
            this->algorithm = Algorithm::SDLS;
            break;
        case Algorithm::TABU_SEARCH:
            this->tabuSearch();
            this->algorithm = Algorithm::TABU_SEARCH;
            break;
        default:
            throw std::logic_error("Invalid argument");
            break;
    }

    this->metrics.end();
    
    this->metrics.calculate(this->nfes);
}