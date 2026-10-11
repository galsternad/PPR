#include <bits/stdc++.h>
#include <numeric>
#include <ostream>
#include <vector>

#include "sequence_generator.hpp"

const std::vector<std::vector<int>> testArrays = {
    { 1, -1, 1, 1, 1 },
    { -1, 1, 1, 1, -1, 1 },
    { 1, -1, 1, 1, -1, -1, -1 },
    { -1, 1, 1, 1, -1, 1, 1, -1 },
    { 1, 1, 1, 1, -1, 1, 1, -1, -1 },
    { 1, 1, 1, 1, -1, -1, 1, 1, -1, 1 },
    { 1, 1, 1, -1, -1, -1, 1, -1, -1, 1, -1 },
    { -1, -1, -1, 1, 1, 1, -1, 1, 1, -1, 1, 1 }
};

std::vector<int> calculateC(const std::vector<int>& S) {
    std::vector<int> result;
    
    int L = S.size();

    for(int k = 0; k < L; k++) {
        int sum = 0;

        for(int i = 0; i < L - k; i++) {
            sum += S[i] * S[i + k];
        }
    
        result.push_back(sum);
    }

    return result;
}

int calculatePeakSidelobeLevel(const std::vector<int>& C) {
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

std::vector<int> squareC(const std::vector<int>& C) {
    std::vector<int> cSquared(C);
    
    for(int k = 1; k < C.size(); k++) {
        cSquared[k] *= C[k];
    }

    return cSquared;
}

float calculateMeritFactor(const std::vector<int>& S, const std::vector<int>& C) {
    float result = 0.0f;
    unsigned int L = S.size();
    std::vector<int> squaredC = squareC(C);

    int sumC = std::accumulate(squaredC.begin() + 1, squaredC.end(), 0);

    result = (float)(L * L) / (float)(2 * sumC);

    return result;
}

int main() {
    for(auto it : testArrays) {
        std::vector<int> S = it;
        std::vector<int> C = calculateC(S);

        int peakSidelobeLevel = calculatePeakSidelobeLevel(C);
        float meritFactor = calculateMeritFactor(S, C);
    
        std::cout << "PSL: " << peakSidelobeLevel << ", MF: "<< meritFactor << std::endl;
    }

    SequenceGenerator* generator = new SequenceGenerator(time(nullptr));
    auto h = generator->generateSequence(10);

    for(auto i : h) {
        std::cout << i;
    }
 
    delete generator;
}