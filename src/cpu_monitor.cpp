#include "../include/cpu_monitor.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unistd.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>
#include <string>
#include <vector>


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
    long clock_ticks_per_second = sysconf(_SC_CLK_TCK);

    // Static vectors to keep track of previous values
    static std::vector<long> prevTotalTime(pids.size(), 0);
    static std::vector<double> prevCpuUsage(pids.size(), 0.0);

    for (size_t i = 0; i < pids.size(); ++i) {
        std::string statPath = "/proc/" + std::to_string(pids[i]) + "/stat";
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
        int ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt, majflt, cmajflt;
        unsigned long utime, stime, cutime, cstime;
        long priority, nice, num_threads, itrealvalue, starttime;

        iss >> stat >> processName >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt
            >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads
            >> itrealvalue >> starttime;

        long totalTime = utime + stime + cutime + cstime;
        double elapsedTime = clock_ticks_per_second;
        double cpuUsage = 0.0;

        // Check if we have previous data for this process
        if (prevTotalTime[i] != 0) {
            // Calculate CPU usage as the difference between the current and previous data
            long timeDiff = totalTime - prevTotalTime[i];
            double elapsedDiff = elapsedTime - prevCpuUsage[i];
            if (elapsedDiff > 0.0) {
                cpuUsage = (timeDiff / elapsedDiff) * 100.0;
            }
        }

        // Update previous values for the next iteration
        prevTotalTime[i] = totalTime;
        prevCpuUsage[i] = elapsedTime;

        cpuDataList.push_back({processName, cpuUsage});
    }

    // Sort the CPU data based on CPU usage in descending order
    std::sort(cpuDataList.begin(), cpuDataList.end(), [](const CPUData& a, const CPUData& b) {
        return a.cpuUsage > b.cpuUsage;
    });
}
