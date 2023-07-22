#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <vector>
#include <string>

 struct MemoryData {
     std::string processName;
     double memoryUsage;
 };

class MemoryMonitor {
public:
    void retrieveOverallMemoryUsage(double& totalUsage, double& freeMemory) const;
    void retrievePerProcessMemoryUsage(std::vector<MemoryData>& memoryDataList) const;
};

#endif // MEMORY_MONITOR_H
