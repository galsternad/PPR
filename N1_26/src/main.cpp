#include <vector>
#include "solver.hpp"
#include "parser.hpp"

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

int main(int argc, char** argv) {
    auto parser = new Parser(argc, argv);
    auto solver = new Solver();

    try {
        parser->parse();

        auto arguments = parser->getArgumentsMap();
    
        solver->setL(arguments["-L"]);
        solver->setSeed(arguments["-seed"]);
        solver->setNfesLmt(arguments["-nfesLmt"]);
        solver->setThreads(arguments["-t"]);
    
        solver->solve(arguments["-a"]);
        
        std::cout << solver->toString() << std::endl;

        delete parser;
        delete solver;
    }
    catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}