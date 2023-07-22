#include "../include/disk_monitor.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

void DiskMonitor::retrieveDiskInfo(std::vector<std::string>& devices, std::vector<std::string>& partitions) const {
    devices.clear();
    partitions.clear();

    // Use lsblk utility to obtain disk information
    std::string command = "lsblk -n -o NAME,MOUNTPOINT --nodeps";
    std::string output;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        perror("popen");
        return;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }

    pclose(pipe);

    std::istringstream iss(output);
    std::string line;
    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string device, mountpoint;
        lineStream >> device >> mountpoint;

        if (mountpoint.empty()) {
            devices.push_back(device);
        } else {
            partitions.push_back(device);
        }
    }
}

void DiskMonitor::retrieveDiskActivity(std::vector<DiskData>& diskDataList) const {
    diskDataList.clear();

    // Use system APIs or utilities to monitor disk activity
    // Implement the necessary logic to track read/write rates per application/process
    // This implementation depends on the specific system APIs or utilities you choose to use
    // You may need to read from system files or use system tools like iostat or /proc/diskstats

    // Example: Retrieve disk activity using /proc/diskstats
    std::ifstream file("/proc/diskstats");
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int major, minor;
        std::string device;
        unsigned long long readsCompleted, writesCompleted;
        unsigned long long readSectors, writeSectors;

        iss >> major >> minor >> device >> readsCompleted >> readSectors >> writesCompleted >> writeSectors;

        DiskData diskData;
        diskData.device = device;
        diskData.readRate = static_cast<double>(readSectors) / 1024.0; // Convert to KB
        diskData.writeRate = static_cast<double>(writeSectors) / 1024.0; // Convert to KB

        diskDataList.push_back(diskData);
    }
    file.close();
}

