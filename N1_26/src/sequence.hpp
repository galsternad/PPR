#include <vector>
#include <string>

class Sequence {
    private:
        std::vector<int> S;
        std::vector<int> C;
        double meritFactor;
        unsigned int peakSidelobeLevel;
    
    public:
        Sequence() :
            S(std::vector<int> { }),
            C(std::vector<int> { }),
            meritFactor(0),
            peakSidelobeLevel(0) { }

        Sequence(
            const std::vector<int>& S,
            const std::vector<int>& C,
            const double meritFactor,
            const unsigned int peakSidelobeLevel
        ) : S(S),
            C(C),
            meritFactor(meritFactor), 
            peakSidelobeLevel(peakSidelobeLevel) { }

        inline const std::vector<int>& getS() const { return this->S; }
        inline void setS(const std::vector<int>& S) { this->S = S; }

        inline const std::vector<int>& getC() const { return this->C; }
        inline void setC(const std::vector<int>& C) { this->C = C; }

        inline double getMeritFactor() const { return this->meritFactor; }
        inline void setMeritFactor(double meritFactor) { this->meritFactor = meritFactor; }

        inline unsigned int getPeakSidelobeLevel() const { return this->peakSidelobeLevel; }
        inline void setPeakSidelobeLevel(unsigned int peakSidelobeLevel) { this->peakSidelobeLevel = peakSidelobeLevel; }

        const std::string toString() const;
        
        std::vector<bool> toBinaryVector() const;
        std::string toBinaryString() const;
        std::string toHexadecimalString(std::string binaryString) const;
};