#include <vector>
#include <string>

class Sequence {
    private:
        std::vector<int> sequence;
        std::vector<int> autocorrelationVector;
        double meritFactor;
        unsigned int peakSidelobeLevel;
    
    public:
        Sequence() :
            sequence(std::vector<int> { }),
            autocorrelationVector(std::vector<int> { }),
            meritFactor(0),
            peakSidelobeLevel(0) { }

        Sequence(
            const std::vector<int>& sequence,
            const std::vector<int>& autocorrelationVector,
            const double meritFactor,
            const unsigned int peakSidelobeLevel
        ) : sequence(sequence),
            autocorrelationVector(autocorrelationVector),
            meritFactor(meritFactor), 
            peakSidelobeLevel(peakSidelobeLevel) { }

        inline const std::vector<int>& getSequence() const { return this->sequence; }
        inline void setSequence(const std::vector<int>& sequence) { this->sequence = sequence; }

        inline const std::vector<int>& getAutocorrelationVector() const { return this->autocorrelationVector; }
        inline void setAutocorrelationVector(const std::vector<int>& autocorrelationVector) { this->autocorrelationVector = autocorrelationVector; }

        inline double getMeritFactor() const { return this->meritFactor; }
        inline void setMeritFactor(double meritFactor) { this->meritFactor = meritFactor; }

        inline unsigned int getPeakSidelobeLevel() const { return this->peakSidelobeLevel; }
        inline void setPeakSidelobeLevel(unsigned int peakSidelobeLevel) { this->peakSidelobeLevel = peakSidelobeLevel; }

        std::vector<bool> toBinaryVector() const;
        std::string toBinaryString() const;
        std::string toHexadecimalString(std::string binaryString) const;
};