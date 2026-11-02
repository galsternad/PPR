#pragma once

#include <string>
#include <map>
#include <cstdint>
#include <time.h>

class Parser {
    private:
        int32_t argumentCount;
        char** argumentList;
        // algorithms:
        // 0 - random
        // 1 - SDLS
        // 2 - tabu
        std::map<std::string, uint32_t> argumentsMap = {
            { "--help",        0               },
            { "--benchmark",   0               },
            { "-L",            250             },
            { "--seed",        time(nullptr)   },
            { "--nfesLmt",     1000000         },
            { "-t",            1               },
            { "-a",            2               },
            { "-p",            0               }
        };

    public:
        Parser(uint32_t argc, char** argv) : 
            argumentCount(argc),
            argumentList(argv) { }

        void parse();
        const void printHelp() const;
        inline const std::map<std::string, uint32_t> getArgumentsMap() const { return this->argumentsMap; }
};