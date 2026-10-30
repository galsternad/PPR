#include "benchmark.hpp"
#include <iomanip>

uint32_t Benchmark::iteration = 0;

const std::string Benchmark::toString() const {
    std::stringstream ss;
    
    ss << std::left << std::setw(16) << "ALGORITHM" 
       << std::left << std::setw(8) << "THREADS" 
       << std::left << std::setw(12) << "ENERGY" 
       << std::left << std::setw(12) << "SPEED"
       << std::left << "SEED\n";
    ss << "---------------" << "---------------" << "---------------\n";

    for(auto& s : solvers) {
        switch(s.getAlgorithmUsed()) {
            case RANDOM_SEARCH:
                ss << std::left << std::setw(16) << "Random search";
                break;
            case SDLS:
                ss << std::left << std::setw(16) << "SDLS";
                break;
            case TABU_SEARCH:
                ss << std::left << std::setw(16) << "Tabu search";
                break;
            default:
                break;
        }

        ss << std::left << std::setw(8) << s.getThreads();

        std::stringstream energy, speed;
        energy << std::fixed << std::setprecision(2) << calculateEnergyConsumptionImprovement(s) << "%";
        speed  << std::fixed << std::setprecision(2) << calculateSpeedImprovement(s) << "%";

        ss << std::left << std::setw(12) << energy.str();
        ss << std::left << std::setw(12) << speed.str();
        ss << std::left << s.getSeed();
        ss << "\n";
    }

    ss << "-----------------------------------------\n";
    ss << "BEST SEQUENCE FOUND:                     \n";
    ss << getBestResult().sequence.toString()   << "\n";
    ss << getBestResult().metrics.toString()    << "\n";

    return ss.str();
}

const double Benchmark::calculateEnergyConsumptionImprovement(const Solver &solver) const {
    auto initialSolverConsumedPower = (double)this->initialSolver.getMetrics().getEnergyConsumption().consumed;
    auto parameterSolverConsumedPower = (double)solver.getMetrics().getEnergyConsumption().consumed;

    double result = (parameterSolverConsumedPower / initialSolverConsumedPower) * 100.0;

    return result;
}

const double Benchmark::calculateSpeedImprovement(const Solver &solver) const {
    auto initialSolverSpeed = this->initialSolver.getMetrics().getSpeed();
    auto parameterSolverSpeed = solver.getMetrics().getSpeed();
    std::cout << initialSolverSpeed << ", " << parameterSolverSpeed << std::endl;

    double result = (parameterSolverSpeed / initialSolverSpeed) * 100.0 ;

    return result;
}

void Benchmark::evaluateThreads(uint32_t seed, uint32_t algorithm) {
    for(int t = 1; t <= this->maxNumberOfThreads; t *= 2) {
        this->solver = Solver(this->L, seed, this->nfesLmt, t);

        this->solver.solve(algorithm);

        // vzamemo prve rezultate, s katerimi primerjamo pohitritve
        if(this->iteration == 0) {
            this->initialSolver = this->solver;
        
            this->iteration++;
        }

        solvers.push_back(this->solver);
    }
}

void Benchmark::evaluateAlgorithms(uint32_t seed) {
    for(int a = 1; a < 3; a++) {
        evaluateThreads(seed, a);
    }
}

void Benchmark::run() {
    // seeds
    for(int s = 0; s < this->seeds.size(); s++) {
        auto currentSeed = this->seeds[s];

        this->evaluateAlgorithms(currentSeed);
    }
}

const BestResult Benchmark::getBestResult() const {
    auto sequence = Sequence();
    auto metrics = Metrics();

    for(auto& s : solvers) {
        if(s.getBestSequence().getEnergy() < sequence.getEnergy()) {
            sequence = s.getBestSequence();
            metrics = s.getMetrics();
        }
    }

    return { sequence, metrics };
}