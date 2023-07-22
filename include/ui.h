#ifndef UI_H
#define UI_H

#include <ncurses.h>
#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"

class UI {
public:
    UI();
    ~UI();

    void startMonitoring();

private:
    void initColors();
    void printCPUMonitorData(const CPUMonitor& cpuMonitor);
    void printMemoryMonitorData(const MemoryMonitor& memoryMonitor);
    void printDiskMonitorData(const DiskMonitor& diskMonitor);

    WINDOW* cpuWindow;
    WINDOW* memoryWindow;
    WINDOW* diskWindow;
};

#endif // UI_H
