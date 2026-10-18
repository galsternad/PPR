#include <chrono>
#include <string>

struct Runtime {
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;
    unsigned int duration;
};

struct CpuTime {
    struct timespec start;
    struct timespec end;
    unsigned int duration;
};

struct EnergyConsumption {
    unsigned int start;
    unsigned int end;
    unsigned int consumed;
};

class Metrics {
    private:
        // time in milliseconds
        Runtime runtime;
        CpuTime cpuTime;
        EnergyConsumption energyConsumption;
        unsigned int speed;

        const unsigned int getEnergyFromFile(const std::string& path) const;
    
    public:
        Metrics(
            Runtime runtime = { },
            CpuTime cpuTime = { },
            EnergyConsumption energyConsumption = { },
            unsigned int speed = 0
        ) : runtime(), cpuTime(), 
            energyConsumption(), speed(speed) { }

        inline const Runtime getRuntime() const { return this->runtime; }
        inline const CpuTime getCpuTime() const { return this->cpuTime; }
        inline const EnergyConsumption getEnergyConsumption() const { return this->energyConsumption; }
        inline const unsigned int getSpeed() const { return this->speed; }

        unsigned int calculateSpeed(unsigned int nfes);
        
        void start();
        void end();
        void calculate(unsigned int nfes);
};