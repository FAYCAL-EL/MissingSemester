#include "../include/memory_monitor.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
    void MemoryMonitor::retrieveOverallMemoryUsage(double& totalUsage, double& freeMemory) const {
        std::ifstream file("/proc/meminfo");
        std::string line;
        unsigned long memTotalKB = 0, memFreeKB = 0, buffersKB = 0, cachedKB = 0;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string memLabel;
            unsigned long valueKB;

            if (iss >> memLabel >> valueKB) {
                if (memLabel == "MemTotal:")
                    memTotalKB = valueKB;
                else if (memLabel == "MemFree:")
                    memFreeKB = valueKB;
                else if (memLabel == "Buffers:")
                    buffersKB = valueKB;
                else if (memLabel == "Cached:")
                    cachedKB = valueKB;
            }
        }

        file.close();

        // Calculate current total memory usage and free memory
        // Note: The "Buffers" and "Cached" memory can be considered as reclaimable
        //       if needed, so we add them to the "free memory" calculation.
        double totalMemory = static_cast<double>(memTotalKB) / 1024.0;
        double reclaimableMemory = static_cast<double>(buffersKB + cachedKB) / 1024.0;
        double usedMemory = totalMemory - (static_cast<double>(memFreeKB) / 1024.0) - reclaimableMemory;

        totalUsage = usedMemory;
        freeMemory = totalMemory - usedMemory;
    }
void MemoryMonitor::retrievePerProcessMemoryUsage(std::vector<MemoryData>& memoryDataList) const {
    std::vector<int> pids;
    std::string dirPath = "/proc";
    DIR* dir = opendir(dirPath.c_str());
    if (dir == nullptr) {
        perror("opendir");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            std::string processDir = entry->d_name;
            if (std::all_of(processDir.begin(), processDir.end(), ::isdigit)) {
                int pid = std::stoi(processDir);
                pids.push_back(pid);
            }
        }
    }
    closedir(dir);

    memoryDataList.clear();

    for (int pid : pids) {
        std::string statPath = "/proc/" + std::to_string(pid) + "/status";
        std::ifstream file(statPath);
        if (!file.is_open()) {
            continue;
        }

        std::string line;
        std::string processName;
        unsigned long rss;

        while (std::getline(file, line)) {
            if (line.compare(0, 6, "Name:\t") == 0) {
                processName = line.substr(6);
            }
            else if (line.compare(0, 5, "VmRSS") == 0) {
                std::istringstream iss(line);
                std::string label;
                iss >> label >> rss;
                break;
            }
        }

        file.close();

        double memoryUsage = static_cast<double>(rss) / 1024.0; // Convert to MB
        memoryDataList.push_back({processName, memoryUsage});
    }

    // Sort the memory data based on memory usage in descending order
    std::sort(memoryDataList.begin(), memoryDataList.end(), [](const MemoryData& a, const MemoryData& b) {
        return a.memoryUsage > b.memoryUsage;
    });
}
