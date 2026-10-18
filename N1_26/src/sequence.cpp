#include "sequence.hpp"
#include <iostream>
#include <sstream>
#include <unordered_map>

const std::string Sequence::toString() const {
    std::stringstream ss;

    auto bin = Sequence::toBinaryString(this->S);
    auto hex = Sequence::toHexadecimalString(bin);

    ss << "sequence: " << hex << "\n";
    ss << "MF: " << this->meritFactor << "\n"; 
    ss << "PSL: " << this->peakSidelobeLevel << "\n";

    return ss.str();
}

std::string Sequence::toBinaryString(const std::vector<int>& S) {
    std::string result = "";

    for(auto i : S) {
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
std::string Sequence::toHexadecimalString(const std::string& binaryString) {
    const std::string prefix = "0x";
    std::string hexString = "";

    const std::unordered_map<std::string, std::string> hexDictionary = {
        {"0000", "0"}, {"0001", "1"}, {"0010", "2"}, {"0011", "3"},
        {"0100", "4"}, {"0101", "5"}, {"0110", "6"}, {"0111", "7"},
        {"1000", "8"}, {"1001", "9"}, {"1010", "A"}, {"1011", "B"},
        {"1100", "C"}, {"1101", "D"}, {"1110", "E"}, {"1111", "F"}
    };

    int remainder = binaryString.length() % 4;

    for(int i = binaryString.length() - 4; i >= 0; i -= 4) {
        auto sub = binaryString.substr(i, 4);
        auto hexValue = hexDictionary.at(sub);

        hexString.insert(0, hexValue);
    }

    if(remainder != 0) {
        auto sub = binaryString.substr(0, remainder);

        for(int i = 0; i < 4 - remainder; i++) {
            sub.insert(0, "0");
        }

        auto hexValue = hexDictionary.at(sub);

        hexString.insert(0, hexValue);
    }

    hexString.insert(0, prefix);

    return hexString;
}