#include <vector>
#include <bits/stdc++.h>


class Calculator {
    public:
        static std::vector<int> naiveCalculateC(const std::vector<int>& S);
        static int calculatePeakSidelobeLevel(const std::vector<int>& C);
        static std::vector<int> squareC(const std::vector<int>& C);
        static float calculateMeritFactor(const std::vector<int>& S, const std::vector<int>& C);
};