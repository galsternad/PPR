#include <vector>
#include <bits/stdc++.h>


class Calculator {
    public:
        static std::vector<std::vector<int>> calculateT(const std::vector<int>& S);
        static std::vector<int> calculateC(const std::vector<int>& S);
        static std::vector<int> calculateSquaredC(const std::vector<int>& C);
        static unsigned int calculateEnergy(const std::vector<int>& C);
        static unsigned int calculatePeakSidelobeLevel(const std::vector<int>& C);
        static double calculateMeritFactor(const std::vector<int>& S, const std::vector<int>& C);
        static double calculateMeritFactor(const std::vector<int>& S, const std::vector<int>& C, unsigned int energy);
};