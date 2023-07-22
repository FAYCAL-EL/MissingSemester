#include "include/cpu_monitor.h"
#include "include/memory_monitor.h"
#include "include/disk_monitor.h"
#include <iostream>

void testCPUMonitor() {
    CPUMonitor cpuMonitor;
    double totalUsage, freeCPU;
    std::vector<CPUData> cpuDataList;

    cpuMonitor.retrieveOverallCPUUsage(totalUsage, freeCPU);
    std::cout << "Total CPU Usage: " << totalUsage << "%\n";
    std::cout << "Free CPU: " << freeCPU << "%\n";

    cpuMonitor.retrievePerProcessCPUUsage(cpuDataList);
    std::cout << "Per Process CPU Usage:\n";
    for (const auto& cpuData : cpuDataList) {
        std::cout << "Process Name: " << cpuData.processName << "\n";
        std::cout << "CPU Usage: " << cpuData.cpuUsage << "%\n";
    }
}

void testMemoryMonitor() {
    MemoryMonitor memoryMonitor;
    double totalUsage, freeMemory;
    std::vector<MemoryData> memoryDataList;

    memoryMonitor.retrieveOverallMemoryUsage(totalUsage, freeMemory);
    std::cout << "Total Memory Usage: " << totalUsage << "%\n";
    std::cout << "Free Memory: " << freeMemory << "%\n";

    memoryMonitor.retrievePerProcessMemoryUsage(memoryDataList);
    std::cout << "Per Process Memory Usage:\n";
    for (const auto& memoryData : memoryDataList) {
        std::cout << "Process Name: " << memoryData.processName << "\n";
        std::cout << "Memory Usage: " << memoryData.memoryUsage << " MB\n";
    }
}

void testDiskMonitor() {
    DiskMonitor diskMonitor;
    std::vector<std::string> devices, partitions;
    std::vector<DiskData> diskDataList;

    diskMonitor.retrieveDiskInfo(devices, partitions);
    std::cout << "Devices:\n";
    for (const auto& device : devices) {
        std::cout << device << "\n";
    }

    std::cout << "Partitions:\n";
    for (const auto& partition : partitions) {
        std::cout << partition << "\n";
    }

    diskMonitor.retrieveDiskActivity(diskDataList);
    std::cout << "Disk Activity:\n";
    for (const auto& diskData : diskDataList) {
        std::cout << "Device: " << diskData.device << "\n";
        std::cout << "Read Rate: " << diskData.readRate << " KB/s\n";
        std::cout << "Write Rate: " << diskData.writeRate << " KB/s\n";
    }
}

int main() {
    std::cout << "Testing CPU Monitor:\n";
    testCPUMonitor();

    std::cout << "\nTesting Memory Monitor:\n";
    testMemoryMonitor();

    std::cout << "\nTesting Disk Monitor:\n";
    testDiskMonitor();

    return 0;
}
