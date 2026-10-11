#include <vector>

#include "solver.hpp"

const std::vector<std::vector<int>> testArrays = {
    { 1, -1, 1, 1, 1 },
    { -1, 1, 1, 1, -1, 1 },
    { 1, -1, 1, 1, -1, -1, -1 },
    { -1, 1, 1, 1, -1, 1, 1, -1 },
    { 1, 1, 1, 1, -1, 1, 1, -1, -1 },
    { 1, 1, 1, 1, -1, -1, 1, 1, -1, 1 },
    { 1, 1, 1, -1, -1, -1, 1, -1, -1, 1, -1 },
    { -1, -1, -1, 1, 1, 1, -1, 1, 1, -1, 1, 1 }
};

int main() {
    Solver* solver = new Solver(600, time(nullptr), 100000, 1);
    solver->solveNaive();
    auto vec = solver->getSequence();

    for(auto i : vec) {
        std::cout << i;
    }
    std::cout << std::endl;
    std::cout << "PSL: " << solver->getPeakSidelobeLevel() << std::endl;
    std::cout << "MF: " << solver->getMeritFactor() << std::endl;
}
/*
int main() {
    for(auto it : testArrays) {
        std::vector<int> S = it;
        std::vector<int> C = calculateC(S);

        int peakSidelobeLevel = calculatePeakSidelobeLevel(C);
        float meritFactor = calculateMeritFactor(S, C);
    
        std::cout << "PSL: " << peakSidelobeLevel << ", MF: "<< meritFactor << std::endl;
    }

    SequenceGenerator* generator = new SequenceGenerator(time(nullptr));
    auto h = generator->generateSequence(10);

    for(auto i : h) {
        std::cout << i;
    }
 
    delete generator;
}
*/