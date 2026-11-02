#include "parser.hpp"
#include <cstdint>
#include <stdexcept>
#include <string>
#include <iostream>

// -L => length of a sequence
// -seed => seed
// -nfesLmt => limit of evaluations
// -t => threads
// -a => algorithm
void Parser::parse() {
    std::string arg = "";
    std::string valueStr = "";
    uint32_t value = 0;

    for(int i = 1; i < this->argumentCount; i++) {
        arg = this->argumentList[i];
        
        if(!argumentsMap.count(arg)) {
            throw std::logic_error("Invalid argument: " + arg);
        }

        if(arg == "--help") {
            argumentsMap["--help"] = 1;
            break;
        }
        
        if(arg == "--benchmark") {
            argumentsMap["--benchmark"] = 1;
            continue;
        }
        
        if(i + 1 >= this->argumentCount) {
            throw std::logic_error("Missing value for argument: " + arg);
        }

        valueStr = this->argumentList[++i];
        value = std::stoi(valueStr);

        if(arg == "-L") {
            argumentsMap["-L"] = value;
        }

        else if(arg == "--seed") {
            argumentsMap["--seed"] = value;
        }

        else if(arg == "--nfesLmt") {
            argumentsMap["--nfesLmt"] = value;
        }

        else if(arg == "-t") {
            argumentsMap["-t"] = value;
        }

        else if(arg == "-a") {
            argumentsMap["-a"] = value;
        }
        
        else if(arg == "-p") {
            argumentsMap["-p"] = value;
        }
    }
}

const void Parser::printHelp() const {
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