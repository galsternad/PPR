#include "parser.hpp"
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
    unsigned int value = 0;

    for(int i = 1; i < this->argumentCount; i++) {
        arg = this->argumentList[i];
        valueStr = this->argumentList[++i];
        value = std::stoi(valueStr);

        if(!argumentsMap.count(arg)) {
            throw std::logic_error("Invalid argument: " + arg);
        }

        if(arg == "-L") {
            argumentsMap["-L"] = value;
        }
        else if(arg == "-seed") {
            argumentsMap["-seed"] = value;
        }
        else if(arg == "-nfesLmt") {
            argumentsMap["-nfesLmt"] = value;
        }
        else if(arg == "-t") {
            argumentsMap["-t"] = value;
        }
        else if(arg == "-a") {
            argumentsMap["-a"] = value;
        }
    }
}