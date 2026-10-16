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
    Solver* solver = new Solver(600, time(nullptr), 1000000, 16);

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
    */

    std::cout << "TABU SEARCH:\n";
    solver->solve();

    std::cout << solver->toString() << std::endl;
    std::cout << solver->getBestSequence().toString() << std::endl;
    std::cout << std::endl;

    delete solver;
}