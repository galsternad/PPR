#pragma once

#include <chrono>
#include <string>
#include <time.h>

struct Runtime {
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;
    double duration;
};

struct CpuTime {
    struct timespec start;
    struct timespec end;
    double duration;
};

struct EnergyConsumption {
    uint64_t start;
    uint64_t end;
    uint64_t consumed;
};

class Metrics {
    private:
        // time in milliseconds
        Runtime runtime;
        CpuTime cpuTime;
        EnergyConsumption energyConsumption;
        uint32_t speed;

        const uint64_t getEnergyFromFile(const std::string& path) const;
    
    public:
        Metrics() : 
            runtime(),
            cpuTime(),
            energyConsumption(),
            speed(0) { }

        Metrics(
            Runtime runtime,
            CpuTime cpuTime,
            EnergyConsumption energyConsumption,
            uint32_t speed = 0
        ) : runtime(), cpuTime(), 
            energyConsumption(), speed(speed) { }

        inline const Runtime getRuntime() const { return this->runtime; }
        inline const CpuTime getCpuTime() const { return this->cpuTime; }
        inline const EnergyConsumption getEnergyConsumption() const { return this->energyConsumption; }
        inline const uint32_t getSpeed() const { return this->speed; }
        uint32_t calculateSpeed(uint32_t nfes);

        const std::string toString() const;
        
        void start();
        void end();
        void calculate(uint32_t nfes);
};