#include "calculator.hpp"
#include <numeric>

std::vector<std::vector<int>> Calculator::calculateT(const std::vector<int>& S) {
    unsigned int L = S.size();
    std::vector<std::vector<int>> result(L);

    for(unsigned int k = 0; k < L; k++) {
        std::vector<int> t(L - k);

        for(unsigned int i = 0; i < L - k; i++) {
            t[i] = S[i] * S[i + k];
        }

        result[k] = t;
    }

    return result;
}

std::vector<int> Calculator::calculateC(const std::vector<int>& S) {
    unsigned int L = S.size();
    std::vector<int> result(L);
    
    for(unsigned int k = 0; k < L; k++) {
        int sum = 0;

        for(unsigned int i = 0; i < L - k; i++) {
            sum += S[i] * S[i + k];
        }
    
        result[k] = sum;
    }

    return result;
}

std::vector<int> Calculator::calculateSquaredC(const std::vector<int>& C) {
    std::vector<int> result(C);
    
    for(unsigned int k = 1; k < C.size(); k++) {
        result[k] *= C[k];
    }

    return result;
}

unsigned int Calculator::calculateEnergy(const std::vector<int>& C) {
    if(C.size() < 2) {
        return 0;
    }

    if(C.size() == 2) {
        return C[1] * C[1];
    }

    std::vector<int> squaredAutocorrelations = calculateSquaredC(C);

    return std::accumulate(squaredAutocorrelations.begin() + 1, squaredAutocorrelations.end(), 0U);
}

unsigned int Calculator::calculatePeakSidelobeLevel(const std::vector<int>& C) {
    if(C.size() < 2) {
        return 0;
    }
    
    if(C.size() == 2) {
        return abs(C[1]);
    }
    
    unsigned int max = abs(C[1]);
    
    for(unsigned int k = 2; k < C.size(); k++) {
        if(abs(C[k]) > max) {
            max = abs(C[k]);
        }
    }

    return max;
}

double Calculator::calculateMeritFactor(const std::vector<int>& S, const std::vector<int>& C) {
    unsigned int L = S.size();
    unsigned int energy = calculateEnergy(C);
    
    if(energy == 0) {
        return 0.0;
    }
    
    return (double)(L * L) / (double)(2 * energy);
}

double Calculator::calculateMeritFactor(
    const std::vector<int> &S, 
    const std::vector<int> &C, 
    unsigned int energy
) { 
    unsigned int L = S.size();

    if(energy == 0) {
        return 0;
    }

    return (double)(L * L) / (double)(2 * energy);
}