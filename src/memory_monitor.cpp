#include "../include/memory_monitor.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unistd.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void MemoryMonitor::retrieveOverallMemoryUsage(double& totalUsage, double& freeMemory) const {
    std::ifstream file("/proc/meminfo");
    std::string line;

    // Read the first two lines for total and free memory
    std::getline(file, line);
    std::getline(file, line);
    file.close();

    std::istringstream iss(line);
    std::string memLabel;
    unsigned long total, free, available, buffers, cached;

    iss >> memLabel >> total >> memLabel >> free >> memLabel >> available >> memLabel >> buffers >> memLabel >> cached;

    totalUsage = 100.0 - ((free * 100.0) / total);
    freeMemory = (free * 100.0) / total;
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
