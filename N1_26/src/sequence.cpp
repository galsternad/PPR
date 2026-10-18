#include "sequence.hpp"
#include <iostream>
#include <sstream>
#include <unordered_map>

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
    std::string hexString = "";
    std::cout<<binaryString<<std::endl;
    std::cout<<binaryString.length()<<std::endl;

    const std::unordered_map<std::string, char> hexDictionary = {
        {"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'},
        {"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'},
        {"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'},
        {"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'}
    };

    for(unsigned int i = binaryString.length())

    return hexString;
}