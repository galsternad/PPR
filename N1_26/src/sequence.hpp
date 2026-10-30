#include <cstdint>
#include <limits>
#include <vector>
#include <string>
#include <limits.h>

class Sequence {
    private:
        std::vector<int> S;
        std::vector<int> C;
        double meritFactor;
        uint32_t peakSidelobeLevel;
        uint32_t energy;
    
    public:
        Sequence() :
            S(std::vector<int> { }),
            C(std::vector<int> { }),
            meritFactor(0),
            peakSidelobeLevel(0),
            energy(std::numeric_limits<uint32_t>::max()) { }

        Sequence(const std::vector<int>& S) :
            S(S),
            C(std::vector<int> { }),
            meritFactor(0),
            peakSidelobeLevel(0),
            energy(std::numeric_limits<uint32_t>::max()) { }

        Sequence(
            const std::vector<int>& S,
            const std::vector<int>& C,
            const double meritFactor,
            const uint32_t peakSidelobeLevel,
            const uint32_t energy
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

        inline uint32_t getPeakSidelobeLevel() const { return this->peakSidelobeLevel; }
        inline void setPeakSidelobeLevel(uint32_t peakSidelobeLevel) { this->peakSidelobeLevel = peakSidelobeLevel; }

        inline uint32_t getEnergy() const { return this->energy; }
        inline void setEnergy(uint32_t energy) { this->energy = energy; }

        const std::string toString() const;
        
        static std::string toBinaryString(const std::vector<int>& S);
        static std::string toHexadecimalString(const std::string& binaryString);
};