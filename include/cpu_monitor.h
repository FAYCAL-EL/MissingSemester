#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <vector>
#include <string>

struct CPUData {
    std::string processName;
    double cpuUsage;
};

class CPUMonitor {
public:
    void retrieveOverallCPUUsage(double& totalUsage, double& freeCPU) const;
    void retrievePerProcessCPUUsage(std::vector<CPUData>& cpuDataList) const;
};

#endif // CPU_MONITOR_H
