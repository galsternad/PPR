#include "parser.hpp"
#include <cstdint>
#include <stdexcept>
#include <string>

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
        
        valueStr = this->argumentList[++i];
        value = std::stoi(valueStr);

        if(arg == "-L") {
            argumentsMap["-L"] = value;
        }
        else if(arg == "--seed" || arg == "-s") {
            argumentsMap["--seed"] = value;
        }
        else if(arg == "--nfesLmt" || arg == "-n") {
            argumentsMap["--nfesLmt"] = value;
        }
        else if(arg == "-t") {
            argumentsMap["-t"] = value;
        }
        else if(arg == "-a") {
            argumentsMap["-a"] = value;
        }
    }
}