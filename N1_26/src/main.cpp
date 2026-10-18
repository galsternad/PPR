#include "solver.hpp"
#include "parser.hpp"

int main(int argc, char** argv) {
    try {
        auto parser = new Parser(argc, argv);
        parser->parse();
        auto arguments = parser->getArgumentsMap();
        
        auto solver = new Solver(
            arguments["-L"],
            arguments["-seed"],
            arguments["-nfesLmt"],
            arguments["-t"]
        );

        std::string algorithm = "";
        
        switch(arguments["-a"]) {
            case 0:
                algorithm = "Random search";
                break;
            case 1:
                algorithm = "SDLS";
                break;
            case 2:
                algorithm = "Tabu search";
                break;
            default:
                break;
        }

        solver->solve(arguments["-a"]);

        std::cout << "algorithm:               " << algorithm << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        std::cout << solver->toString()                       << std::endl;
        
        delete parser;
        delete solver;
    }
    catch(const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

}