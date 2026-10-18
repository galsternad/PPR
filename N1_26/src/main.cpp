#include <ctime>
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
    Solver* solver = new Solver(8, time(nullptr), 1, 16);

    /*
    std::cout << "OPTIMIZED SDLS:\n";
    solver->steepestDescentLocalSearch();

    std::cout << solver->toString() << std::endl;
    std::cout << solver->getBestSequence().toString() << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    
    std::cout << "NAIVE:\n";
    solver->steepestDescentLocalSearchNaive();

    std::cout << solver->toString() << std::endl;
    std::cout << solver->getSequence().toString() << std::endl;
    std::cout << std::endl;
    
    std::cout << "TABU SEARCH:\n";
    solver->solve();
    
    std::cout << solver->toString() << std::endl;
    std::cout << solver->getBestSequence().toString() << std::endl;
    std::cout << std::endl;
    */

    auto currSeq = solver->generateNewSequence();
    auto binSeq = currSeq.toBinaryString();
    std::cout << binSeq << std::endl;
    auto hexSeq = currSeq.toHexadecimalString(binSeq);
    // std::cout << hexSeq;

    delete solver;
}