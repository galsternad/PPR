#include "benchmark.hpp"
#include "solver.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

uint32_t Benchmark::iteration = 0;

const std::string Benchmark::toString() const {
    std::stringstream ss;
    
    for(auto& s : results) {
        ss << s.toString() << "\n";
        ss << "-----------------------------------------------------\n";
    }

    ss << std::left << std::setw(16) << "ALGORITHM" 
       << std::left << std::setw(8)  << "THREADS" 
       << std::left << std::setw(12) << "ENERGY" 
       << std::left << std::setw(12) << "SPEED"
       << std::left << std::setw(16) << "SEED"
       << std::left << std::setw(16) << "PARALLEL MODE\n";
    ss << "---------------" << "---------------" << "---------------"
       << "---------------" << "---------------" << "---------------\n";
    for(auto& s : results) {
        switch(s.algorithm) {
            case Algorithm::RANDOM_SEARCH:
                ss << std::left << std::setw(16) << "Random search";
                break;
            case Algorithm::SDLS:
                ss << std::left << std::setw(16) << "SDLS";
                break;
            case Algorithm::TABU_SEARCH:
                ss << std::left << std::setw(16) << "Tabu search";
                break;
            default:
                break;
        }

        ss << std::left << std::setw(8) << s.threads;

        std::stringstream energy, speed;
        energy << std::fixed << std::setprecision(2) << calculateEnergyConsumptionImprovement(s) << "%";
        speed  << std::fixed << std::setprecision(2) << calculateSpeedImprovement(s) << "%";

        ss << std::left << std::setw(12) << energy.str();
        ss << std::left << std::setw(12) << speed.str();
        ss << std::left << std::setw(16) << s.seed;
        
        switch(s.parallelMode) {
            case ParallelMode::THREADS:
                ss << std::left << std::setw(16) << "Threads";
                break;
            case ParallelMode::PRODUCER_CONSUMER:
                ss << std::left << std::setw(16) << "Producer/consumer";
                break;
            default:
                break;
        }

        ss << "\n";
    }

    ss << "-----------------------------------------\n";
    ss << "BEST SEQUENCE FOUND:                     \n";
    ss << getBestResult().sequence.toString()   << "\n";
    ss << getBestResult().metrics.toString()    << "\n";
    ss << "nfesLmt:                 " << nfesLmt << "\n";
    ss << "max number of threads:   " << maxNumberOfThreads << "\n";

    return ss.str();
}

const double Benchmark::calculateEnergyConsumptionImprovement(const Result& resultObj) const {
    auto initialSolverConsumedPower = (double)this->initialResult.metrics.getEnergyConsumption().consumed;
    auto parameterSolverConsumedPower = (double)resultObj.metrics.getEnergyConsumption().consumed;

    double result = (parameterSolverConsumedPower / initialSolverConsumedPower) * 100.0;

    return result;
}

const double Benchmark::calculateSpeedImprovement(const Result& resultObj) const {
    auto initialSolverSpeed = (double)this->initialResult.metrics.getSpeed();
    auto parameterSolverSpeed = (double)resultObj.metrics.getSpeed();

    double result = (parameterSolverSpeed / initialSolverSpeed) * 100.0 ;

    return result;
}

void Benchmark::evaluateThreads(uint32_t seed, Algorithm algorithm, ParallelMode parallelMode) {
    for(int t = 1; t <= this->maxNumberOfThreads; t *= 2) {
        Solver solver(this->L, seed, this->nfesLmt, t, parallelMode);

        solver.solve(algorithm);

        auto nfes = solver.getNfes();
        auto threads = t;
        auto sequence = solver.getBestSequence();
        auto metrics = solver.getMetrics();
        auto result = Result(
            this->L,
            seed,
            nfes,
            threads,
            algorithm,
            parallelMode,
            sequence,
            metrics
        );

        if(this->iteration == 0) {
            this->initialResult = result;
        
            this->iteration++;
        }

        results.push_back(result);
    }
}

void Benchmark::evaluateAlgorithms(uint32_t seed, ParallelMode parallelMode) {
    for(int a = Algorithm::RANDOM_SEARCH; a <= Algorithm::TABU_SEARCH; a++) {
        auto algorithm = (Algorithm)(a);
        
        evaluateThreads(seed, algorithm, parallelMode);
    }
}

void Benchmark::evaluateSeeds(ParallelMode parallelMode) {
    for(int s = 0; s < this->seeds.size(); s++) {
        auto currentSeed = this->seeds[s];

        evaluateAlgorithms(currentSeed, parallelMode);
    }
}

void Benchmark::run() {
    for(int p = ParallelMode::THREADS; p <= ParallelMode::PRODUCER_CONSUMER; p++) {
        evaluateSeeds((ParallelMode) p);
    }
}

const Result Benchmark::getBestResult() const {
    auto sequence = Sequence();
    auto metrics = Metrics();
    auto result = Result();

    for(auto& s : results) {
        if(s.sequence.getEnergy() < sequence.getEnergy()) {
            result = s;
        }
    }

    return result;
}