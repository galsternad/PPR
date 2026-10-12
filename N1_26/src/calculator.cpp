#include "calculator.hpp"

std::vector<int> Calculator::naiveCalculateC(const std::vector<int>& S) {
    int L = S.size();
    std::vector<int> result(L);
    

    for(int k = 0; k < L; k++) {
        int sum = 0;

        for(int i = 0; i < L - k; i++) {
            sum += S[i] * S[i + k];
        }
    
        result[k] = sum;
    }

    return result;
}

int Calculator::calculatePeakSidelobeLevel(const std::vector<int>& C) {
    if(C.size() < 2) {
        return 0;
    }
    
    if(C.size() == 2) {
        return abs(C[1]);
    }
    
    int max = abs(C[1]);
    
    for(int k = 2; k < C.size(); k++) {
        if(abs(C[k]) > max) {
            max = abs(C[k]);
        }
    }

    return max;
}

std::vector<int> Calculator::squareC(const std::vector<int>& C) {
    std::vector<int> cSquared(C);
    
    for(int k = 1; k < C.size(); k++) {
        cSquared[k] *= C[k];
    }

    return cSquared;
}

float Calculator::calculateMeritFactor(const std::vector<int>& S, const std::vector<int>& C) {
    float result = 0.0f;
    unsigned int L = S.size();
    std::vector<int> squaredC = squareC(C);

    int sumC = std::accumulate(squaredC.begin() + 1, squaredC.end(), 0);

    result = (float)(L * L) / (float)(2 * sumC);

    return result;
}