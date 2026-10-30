#include <vector>
#include <bits/stdc++.h>


class Calculator {
    public:
        static std::vector<std::vector<int>> calculateT(const std::vector<int>& S);
        static std::vector<int> calculateC(const std::vector<int>& S);
        static std::vector<int> calculateSquaredC(const std::vector<int>& C);
        static uint32_t calculateEnergy(const std::vector<int>& C);
        static uint32_t calculatePeakSidelobeLevel(const std::vector<int>& C);
        static double calculateMeritFactor(const std::vector<int>& S, const std::vector<int>& C);
        static double calculateMeritFactor(const std::vector<int>& S, const std::vector<int>& C, uint32_t energy);
};