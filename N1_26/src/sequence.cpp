#include "sequence.hpp"

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

    for(auto it : this->S) {
        switch(it) {
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