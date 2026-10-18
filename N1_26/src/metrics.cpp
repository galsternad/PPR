#include "metrics.hpp"
#include <chrono>
#include <ctime>
#include <fstream>

const unsigned int Metrics::getEnergyFromFile(const std::string& path) const {
    std::string result = "";
    std::string str = "";
    std::ifstream file(path);

    while(std::getline(file, str)) {
        result += str;
    }

    return std::stoul(result);
}

unsigned int Metrics::calculateSpeed(unsigned int nfes) {
    unsigned int time = this->runtime.duration;

    if(this->runtime.duration == 0) {
        time = 1;
    }
    
    // runtime in ms => * 1000
    return (unsigned int)((nfes / time) * 1000);
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

void Metrics::calculate(unsigned int nfes = 0) {
    this->energyConsumption.consumed = (unsigned int)((this->energyConsumption.end - this->energyConsumption.start) / 1000);

    this->cpuTime.duration = (unsigned int)
        ((this->cpuTime.end.tv_sec - this->cpuTime.start.tv_sec) * 1000.0 +
        (this->cpuTime.end.tv_nsec - this->cpuTime.start.tv_nsec) / 1000000.0);

    this->runtime.duration = (unsigned int)((this->runtime.end - this->runtime.start).count() / 1000000);

    this->speed = this->calculateSpeed(nfes);
}