#include "parser.hpp"
#include "solver.hpp"
#include "benchmark.hpp"
#include <iostream>



int main(int argc, char** argv) {
    try {
        auto parser = Parser(argc, argv);
        parser.parse();
        auto arguments = parser.getArgumentsMap();

        if(arguments["--help"] == 1) {
            parser.printHelp();
            
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
            Solver solver(
                arguments["-L"],
                arguments["--seed"],
                arguments["--nfesLmt"],
                arguments["-t"],
                ParallelMode::PRODUCER_CONSUMER
            );
            
            std::string algorithm = "";
            
            switch(arguments["-a"]) {
                case Algorithm::RANDOM_SEARCH:
                    algorithm = "Random search";
                    break;
                case Algorithm::SDLS:
                    algorithm = "SDLS";
                    break;
                case Algorithm::TABU_SEARCH:
                    algorithm = "Tabu search";
                    break;
                default:
                    break;
            }
            
            solver.solve((Algorithm)arguments["-a"]);
            
            std::cout << "algorithm: " << algorithm                            << std::endl;
            std::cout << "--------------------------------------"              << std::endl;
            std::cout << solver.toString()                                     << std::endl;
        }
    }
    catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

}