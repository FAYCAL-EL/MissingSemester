#ifndef DISK_MONITOR_H
#define DISK_MONITOR_H

#include <vector>
#include <string>

 struct DiskData {
     std::string device;
     std::string partition;
     double readRate;
     double writeRate;
 };

class DiskMonitor {
public:
    void retrieveDiskInfo(std::vector<std::string>& devices, std::vector<std::string>& partitions) const;
    void retrieveDiskActivity(std::vector<DiskData>& diskDataList) const;
};

#endif // DISK_MONITOR_H
