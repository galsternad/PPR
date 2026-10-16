#include "solver.hpp"
#include <chrono>
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

    ss << "sequence length: " << this->L << "\n";
    ss << "seed: " << this->seed << "\n";
    ss << "max evaluations: " << this->nfesLmt << "\n";
    ss << "threads: " << this->threads << "\n"; 
    ss << "number of evaluations: " << this->nfes << "\n";
    ss << "runtime: " << this->runtime << "ms\n";
    ss << "cpu time: " << this->cpuTime << "\n";
    ss << "energy consumption: " << this->energyConsumption << "\n";
    ss << "speed: " << this->calculateSpeed() << " nfes/s\n";

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

void Solver::createThreadsSDLS(
    std::vector<std::thread>& threads,
    std::vector<NeighbourResult>& results
) {
    for(unsigned int t = 0; t < this->threads; t++) {
        const unsigned int chunkSize = this->L / this->threads;
        unsigned int start = t * chunkSize;
        unsigned int end = (t == this->threads - 1) 
            ? this->L
            : start + chunkSize;

        threads.emplace_back([&, t, start, end]() {
            results[t] = evaluateNeighboursSDLS(start, end);
        });
    }

    for(auto& t : threads) {
        t.join();
    }
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

std::vector<int> Solver::getFlippedVector(const std::vector<int>& S, unsigned int index) const {
    auto vec = S;
    vec[index] *= -1;

    return vec;
}

NeighbourResult Solver::evaluateNeighboursSDLS(unsigned int start, unsigned int end) {
    unsigned int bestIndex = 0;
    auto bestEnergy = std::numeric_limits<unsigned int>::max();
    
    for(unsigned int i = start; i < end; i++) {
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
    this->bestSequence = this->currentSequence;
    this->T = Calculator::calculateT(this->currentSequence.getS());        
    
    while(this->nfes < this->nfesLmt) {
        bool canImprove = true;
        
        while(canImprove) {
            bool canEvaluateNeighbourhood = this->nfes + this->L <= this->nfesLmt;

            if(!canEvaluateNeighbourhood) {
                break;
            }

            auto bestIndex = 0U;
            auto bestEnergy = std::numeric_limits<unsigned int>::max();
            std::vector<std::thread> threads;
            std::vector<NeighbourResult> results(this->threads);
            this->createThreadsSDLS(threads, results);

            this->nfes += this->L;

            for(const auto& result : results) {
                if(result.energy < bestEnergy) {
                    bestEnergy = result.energy;
                    bestIndex = result.index;
                }
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

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

    this->runtime = std::chrono::system_clock::to_time_t(end) - std::chrono::system_clock::to_time_t(start);
}

void Solver::createThreadsTabu(
    std::vector<std::thread>& threads,
    std::vector<NeighbourResult>& results,
    unsigned int k,
    const Sequence& runBestSequence,
    const std::vector<int>& M
) {
    for(unsigned int t = 0; t < this->threads; t++) {
        const unsigned int chunkSize = this->L / this->threads;
        unsigned int start = t * chunkSize;
        unsigned int end = (t == this->threads - 1)
            ? this->L
            : start + chunkSize;

        threads.emplace_back([&, t, start, end, k]() {
            results[t] = evaluateNeighboursTabu(
                start,
                end,
                k,
                runBestSequence,
                M  
            );
        });
    }

    for(auto& t : threads) {
        t.join();
    }
}

NeighbourResult Solver::evaluateNeighboursTabu(
    unsigned int start,
    unsigned int end,
    unsigned int k,
    const Sequence& runBestSequence,
    const std::vector<int>& M
) {
    auto bestIndex = 0U;
    auto bestEnergy = std::numeric_limits<unsigned int>::max();
    auto foundMove = false;
    
    for(unsigned int i = start; i < end; i++) {
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
    }

    return { bestIndex, bestEnergy, foundMove };
}

void Solver::tabuSearch() {
    this->currentSequence = this->generateNewSequence();
    this->bestSequence = this->currentSequence;
    this->T = Calculator::calculateT(this->currentSequence.getS());
    
    while(this->nfes < this->nfesLmt) {
        std::vector<int> M(this->L, 0);
        auto runBestSequence = this->currentSequence;
        unsigned int minValue = this->L / 2;
        unsigned int maxValue = 3 * this->L / 2;
        unsigned int maxIters = generator.generateRandomInt(minValue, maxValue);
        unsigned int minTabu = maxIters / 10;
        unsigned int extraTabu = maxIters / 50;

        if(extraTabu == 0) {
            ++extraTabu;
        }
        
        for(unsigned int k = 0; k < maxIters; k++) {
            bool canEvaluateNeighbourhood = this->nfes + this->L <= this->nfesLmt;
            bool foundMove = false;
        
            if(!canEvaluateNeighbourhood) {
                break;
            }
            
            auto bestEnergy = std::numeric_limits<unsigned int>::max();
            auto bestIndex = 0U;

            // threading
            std::vector<std::thread> threads;
            std::vector<NeighbourResult> results(this->threads);
            this->createThreadsTabu(
                threads,
                results,
                k,
                runBestSequence,
                M
            );

            this->nfes += this->L;

            for(const auto& result : results) {
                if(result.foundMove && result.energy < bestEnergy) {
                    bestEnergy = result.energy;
                    bestIndex = result.index;
                    foundMove = true;
                }
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
    
            std::cout << "MF: " << newMeritFactor << ", PSL: " << newPeakSidelobeLevel << std::endl;

            if(this->currentSequence.getEnergy() < runBestSequence.getEnergy()) {
                runBestSequence = this->currentSequence;
            }
    
            if(this->currentSequence.getEnergy() < this->bestSequence.getEnergy()) {
                this->bestSequence = newSequence;
            }
        }

        bool canRestart = this->nfes + this->L + 1 <= this->nfesLmt;

        if(!canRestart) {
            break;
        }

        this->currentSequence = this->generateNewSequence();
        this->T = Calculator::calculateT(this->currentSequence.getS());
    }
}

void Solver::solve() {
    auto start = std::chrono::system_clock::now();
    
    this->tabuSearch();

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;

    this->runtime = elapsed.count();
}

unsigned int Solver::calculateSpeed() const {
    // runtime in ms => * 1000
    return (this->nfes / this->runtime) * 1000;
}
