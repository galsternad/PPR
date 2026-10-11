#include <bits/stdc++.h>
#include <numeric>
#include <ostream>
#include <vector>

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

std::vector<int> calculateC(std::vector<int> S) {
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

int calculatePeakSidelobeLevel(std::vector<int> C) {
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

void squareC(std::vector<int>& C) {
    for(int k = 1; k < C.size(); k++) {
        C[k] *= C[k];
    }
}

float calculateMeritFactor(std::vector<int> S, std::vector<int> C) {
    float result = 0.0f;
    unsigned int L = S.size();
    squareC(C);
    int sumC = std::accumulate(C.begin() + 1, C.end(), 0);

    result = (float)(L * L) / (float)(2 * sumC);

    return result;
}

int main() {
    for(auto it : testArrays) {
        std::vector<int> S = it;
        std::vector<int> C = calculateC(S);

        /*
        for(auto i : C) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        */

        int peakSidelobeLevel = calculatePeakSidelobeLevel(C);
        float meritFactor = calculateMeritFactor(S, C);
    
        std::cout << "PSL: " << peakSidelobeLevel << std::endl;
        std::cout << "MF: " << meritFactor << std::endl;
    }
}