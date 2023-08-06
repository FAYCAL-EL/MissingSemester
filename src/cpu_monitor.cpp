#include "../include/cpu_monitor.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <map>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

void CPUMonitor::retrieveOverallCPUUsage(double& totalUsage, double& freeCPU) const {
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    file.close();

    std::istringstream iss(line);
    std::string cpuLabel;
    int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

    int total = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
    int idleTotal = idle + iowait;

    totalUsage = 100.0 - ((idleTotal * 100.0) / total);
    freeCPU = (idleTotal * 100.0) / total;
}

void CPUMonitor::retrievePerProcessCPUUsage(std::vector<CPUData>& cpuDataList) const {
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

    cpuDataList.clear();

    // Map to store the previous CPU usage values for each process
    static std::map<int, std::tuple<unsigned long, unsigned long, unsigned long, unsigned long>> prevCPUValues;

    for (int pid : pids) {
        std::string statPath = "/proc/" + std::to_string(pid) + "/stat";
        std::ifstream file(statPath);
        if (!file.is_open()) {
            continue;
        }

        std::string line;
        std::getline(file, line);
        file.close();

        std::istringstream iss(line);
        std::string processName;
        std::string stat;
        unsigned long utime, stime, cutime, cstime;
        iss >> stat; // Read the process name (enclosed in parentheses)
        std::getline(iss, processName, ')'); // Extract the process name from parentheses

        // Skip the first 12 fields and read utime, stime, cutime, and cstime
        for (int i = 0; i < 12; ++i) {
            iss >> stat;
        }
        iss >> utime >> stime >> cutime >> cstime;

        // Calculate the total CPU time (including child processes)
        unsigned long totalTime = utime + stime + cutime + cstime;
        // Check if we have previous data for this process
        auto it = prevCPUValues.find(pid);
        if (it != prevCPUValues.end()) {
            // Get the previous CPU time values for this process
            unsigned long prevUtime, prevStime, prevCutime, prevCstime;
            std::tie(prevUtime, prevStime, prevCutime, prevCstime) = it->second;

            // Calculate CPU usage as the difference between the current and previous total CPU times
            double elapsedTime = sysconf(_SC_CLK_TCK); // Clock ticks per second
            double cpuUsage = ((totalTime - prevUtime - prevStime - prevCutime - prevCstime) / elapsedTime) * 100.0;

            cpuDataList.push_back({processName, cpuUsage});
        }

        // Update the previous CPU time values for this process
        prevCPUValues[pid] = std::make_tuple(utime, stime, cutime, cstime);
    }

    // Sort the CPU data based on CPU usage in descending order
    std::sort(cpuDataList.begin(), cpuDataList.end(), [](const CPUData& a, const CPUData& b) {
        return a.cpuUsage > b.cpuUsage;
    });
}
