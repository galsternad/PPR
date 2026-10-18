#include <limits>
#include <vector>
#include <string>
#include <limits.h>

class Sequence {
    private:
        std::vector<int> S;
        std::vector<int> C;
        double meritFactor;
        unsigned int peakSidelobeLevel;
        unsigned int energy;
    
    public:
        Sequence() :
            S(std::vector<int> { }),
            C(std::vector<int> { }),
            meritFactor(0),
            peakSidelobeLevel(0),
            energy(std::numeric_limits<unsigned int>::max()) { }

        Sequence(const std::vector<int>& S) :
            S(S),
            C(std::vector<int> { }),
            meritFactor(0),
            peakSidelobeLevel(0),
            energy(std::numeric_limits<unsigned int>::max()) { }

        Sequence(
            const std::vector<int>& S,
            const std::vector<int>& C,
            const double meritFactor,
            const unsigned int peakSidelobeLevel,
            const unsigned int energy
        ) : S(S),
            C(C),
            meritFactor(meritFactor), 
            peakSidelobeLevel(peakSidelobeLevel),
            energy(energy) { }

        inline const std::vector<int>& getS() const { return this->S; }
        inline void setS(const std::vector<int>& S) { this->S = S; }

        inline const std::vector<int>& getC() const { return this->C; }
        inline void setC(const std::vector<int>& C) { this->C = C; }

        inline double getMeritFactor() const { return this->meritFactor; }
        inline void setMeritFactor(double meritFactor) { this->meritFactor = meritFactor; }

        inline unsigned int getPeakSidelobeLevel() const { return this->peakSidelobeLevel; }
        inline void setPeakSidelobeLevel(unsigned int peakSidelobeLevel) { this->peakSidelobeLevel = peakSidelobeLevel; }

        inline unsigned int getEnergy() const { return this->energy; }
        inline void setEnergy(unsigned int energy) { this->energy = energy; }

        const std::string toString() const;
        
        static std::string toBinaryString(const std::vector<int>& S);
        static std::string toHexadecimalString(const std::string& binaryString);
};