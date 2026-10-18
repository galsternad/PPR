#include <string>
#include <map>
#include <time.h>

class Parser {
    private:
        int argumentCount;
        char** argumentList;
        // algorithms:
        // 0 - random
        // 1 - SDLS
        // 2 - tabu
        std::map<std::string, unsigned int> argumentsMap = {
            { "-L",       250           },
            { "-seed",    time(nullptr) },
            { "-nfesLmt", 1000000       },
            { "-t",       1             },
            { "-a",       2             }
        };

    public:
        Parser(unsigned int argc, char** argv) : 
            argumentCount(argc),
            argumentList(argv) { }

        void parse();
        inline const std::map<std::string, unsigned int> getArgumentsMap() const { return this->argumentsMap; }
};