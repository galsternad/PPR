#include "sequence.hpp"

std::vector<bool> Sequence::toBinaryVector() const {
    std::vector<bool> result = std::vector<bool>(this->sequence.size());
    
    for(unsigned int i = 0; i < this->sequence.size(); i++) {
        if(this->sequence[i] == -1) {
            result[i] = true;
            continue;
        }
        if(this->sequence[i] == 1) {
            result[i] = false;
        }
    }

    return result;
}

std::string Sequence::toBinaryString() const {
    std::string result = "";

    for(auto it : this->sequence) {
        result += it;
    }

    return result;
}

// TODO:
// binary to hex
std::string Sequence::toHexadecimalString(std::string binaryString) const {
    return "";
}