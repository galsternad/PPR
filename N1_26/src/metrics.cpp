#include "metrics.hpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>

const std::string Metrics::toString() const {
    std::stringstream ss;

    ss << "runtime:                 " << this->runtime.duration             << " ms\n";
    ss << "CPU time:                " << this->cpuTime.duration             << " ms\n";
    ss << "energy consumption:      " << this->energyConsumption.consumed   << " mJ\n";
    ss << "speed:                   " << this->speed                        << " nfes/s";

    return ss.str();
}

const uint64_t Metrics::getEnergyFromFile(const std::string& path) const {
    std::string result = "";
    std::string str = "";
    std::ifstream file(path);

    while(std::getline(file, str)) {
        result += str;
    }

    return std::stoul(result);
}

uint32_t Metrics::calculateSpeed(uint32_t nfes) {
    double time = this->runtime.duration;
    
    // runtime in ms => * 1000
    double result = ((double)nfes / time) * 1000.0;

    return (uint32_t)result;
}


void Metrics::start() {
    this->energyConsumption.start = this->getEnergyFromFile("/sys/class/powercap/intel-rapl:0/energy_uj");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &this->cpuTime.start);
    this->runtime.start = std::chrono::steady_clock::now();
}

void Metrics::end() {
    this->energyConsumption.end = this->getEnergyFromFile("/sys/class/powercap/intel-rapl:0/energy_uj");
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &this->cpuTime.end);
    this->runtime.end = std::chrono::steady_clock::now();
}

void Metrics::calculate(uint32_t nfes = 0) {
    this->energyConsumption.consumed = (this->energyConsumption.end - this->energyConsumption.start) / 1000;

    this->cpuTime.duration =
        ((this->cpuTime.end.tv_sec - this->cpuTime.start.tv_sec) * 1000.0 +
        (this->cpuTime.end.tv_nsec - this->cpuTime.start.tv_nsec) / 1000000.0);

    this->runtime.duration = 
        (std::chrono::duration_cast<std::chrono::nanoseconds>
            (this->runtime.end - this->runtime.start)
            .count()) / 1000000.0;

    this->speed = this->calculateSpeed(nfes);
}