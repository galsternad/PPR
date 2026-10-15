#include "sequence.hpp"
#include <sstream>

const std::string Sequence::toString() const {
    std::stringstream ss;

    ss << "MF: " << this->meritFactor << ", PSL: " << this->peakSidelobeLevel << "."; 

    return ss.str();
}

std::vector<bool> Sequence::toBinaryVector() const {
    std::vector<bool> result = std::vector<bool>(this->S.size());
    
    for(unsigned int i = 0; i < this->S.size(); i++) {
        if(this->S[i] == -1) {
            result[i] = true;
            continue;
        }
        if(this->S[i] == 1) {
            result[i] = false;
        }
    }

    return result;
}

std::string Sequence::toBinaryString() const {
    std::string result = "";

    for(auto i : this->S) {
        switch(i) {
            case -1:
                result += "1";
                break;
            case 1:
                result += "0";
                break;
        }
    }

    return result;
}

// TODO:
// binary to hex
std::string Sequence::toHexadecimalString(std::string binaryString) const {
    return "";
}