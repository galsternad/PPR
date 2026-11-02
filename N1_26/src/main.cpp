#include "parser.hpp"
#include "solver.hpp"
#include "benchmark.hpp"
#include <iostream>

void printHelp() {
    std::cout << "Usage:                                            " << std::endl;
    std::cout << "path-to-program [OPTION value]                    " << std::endl;
    std::cout << "------------------------------                    " << std::endl;
    std::cout << "FLAGS:                                            " << std::endl;
    std::cout << "-L <unsigned int> (default=250)                   " << std::endl;
    std::cout << "Length of a sequence, for which we are            " << std::endl;
    std::cout << "calculating the highest MF                        " << std::endl;
    std::cout << "                                                  " << std::endl;
    std::cout << "-s, --seed <unsigned int> (default=time(nullptr)) " << std::endl;
    std::cout << "Number used for the random generation             " << std::endl;
    std::cout << "                                                  " << std::endl;
    std::cout << "-n, --nfesLmt <unsigned int> (default=1000000)    " << std::endl;
    std::cout << "Max evaluation tries for the program              " << std::endl;
    std::cout << "                                                  " << std::endl;
    std::cout << "-t <unsigned int> (default=1)                     " << std::endl;
    std::cout << "Number of threads used for the algorithm          " << std::endl;
    std::cout << "NOTE: when used with '--benchmark', it is the     " << std::endl;
    std::cout << "max number of threads used -> [1, t], step=2      " << std::endl;
    std::cout << "                                                  " << std::endl;
    std::cout << "-a <unsigned int> (default=2)                     " << std::endl;
    std::cout << "Algorithm, used to find the best sequence         " << std::endl;
    std::cout << "0 - Random search                                 " << std::endl;
    std::cout << "1 - Steepest descent local search (SDLS)          " << std::endl;
    std::cout << "2 - Tabu search                                   " << std::endl;
    std::cout << "                                                  " << std::endl;
    std::cout << "--benchmark                                       " << std::endl;
    std::cout << "Runs a benchmark of all of the algorithms         " << std::endl;
    std::cout << "                                                  " << std::endl;
}

int main(int argc, char** argv) {
    try {
        auto parser = Parser(argc, argv);
        parser.parse();
        auto arguments = parser.getArgumentsMap();

        if(arguments["--help"] == 1) {
            printHelp();
            
            return 1;
        }

        if(arguments["--benchmark"] == 1) {
            auto benchmark = new Benchmark(
                arguments["-L"],
                arguments["--seed"],
                arguments["--nfesLmt"],
                arguments["-t"]
            );

            benchmark->run();
            std::cout << benchmark->toString() << "\n";

            delete benchmark;

        }
        else {        
            auto solver = Solver(
                arguments["-L"],
                arguments["--seed"],
                arguments["--nfesLmt"],
                arguments["-t"]
            );
            
            std::string algorithm = "";
            
            switch(arguments["-a"]) {
                case RANDOM_SEARCH:
                    algorithm = "Random search";
                    break;
                case SDLS:
                    algorithm = "SDLS";
                    break;
                case TABU_SEARCH:
                    algorithm = "Tabu search";
                    break;
                default:
                    break;
            }
            
            solver.solve(arguments["-a"]);
            
            std::cout << "algorithm: " << algorithm                            << std::endl;
            std::cout << "--------------------------------------"              << std::endl;
            std::cout << solver.toString()                                     << std::endl;
        }
    }
    catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

}