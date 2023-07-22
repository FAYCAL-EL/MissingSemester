#include "../include/ui.h"
#include <vector>
#include <thread>
#include <chrono>

UI::UI() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    initColors();
}

UI::~UI() {
    endwin();
}

void UI::initColors() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
}

void UI::printCPUMonitorData(const CPUMonitor& cpuMonitor) {
    double totalUsage, freeCPU;
    std::vector<CPUData> cpuDataList;
    cpuMonitor.retrieveOverallCPUUsage(totalUsage, freeCPU);
    cpuMonitor.retrievePerProcessCPUUsage(cpuDataList);

    wclear(cpuWindow);
    wattron(cpuWindow, COLOR_PAIR(1));
    mvwprintw(cpuWindow, 0, 0, "----- CPU Monitor -----");
    wattroff(cpuWindow, COLOR_PAIR(1));

    mvwprintw(cpuWindow, 1, 0, "Total CPU Usage: %.2f%%", totalUsage);
    mvwprintw(cpuWindow, 2, 0, "Free CPU: %.2f%%", freeCPU);

    wattron(cpuWindow, COLOR_PAIR(2));
    mvwprintw(cpuWindow, 4, 0, "%-25s | %10s", "Process Name", "CPU Usage");
    wattroff(cpuWindow, COLOR_PAIR(2));

    int row = 5;
    for (const auto& cpuData : cpuDataList) {
        mvwprintw(cpuWindow, row, 0, "%-25s | %10.2f%%", cpuData.processName.c_str(), cpuData.cpuUsage);
        row++;
    }

    wrefresh(cpuWindow);
}

void UI::printMemoryMonitorData(const MemoryMonitor& memoryMonitor) {
    double totalUsage, freeMemory;
    std::vector<MemoryData> memoryDataList;
    memoryMonitor.retrieveOverallMemoryUsage(totalUsage, freeMemory);
    memoryMonitor.retrievePerProcessMemoryUsage(memoryDataList);

    wclear(memoryWindow);
    wattron(memoryWindow, COLOR_PAIR(1));
    mvwprintw(memoryWindow, 0, 0, "----- Memory Monitor -----");
    wattroff(memoryWindow, COLOR_PAIR(1));

    mvwprintw(memoryWindow, 1, 0, "Total Memory Usage: %.2f%%", totalUsage);
    mvwprintw(memoryWindow, 2, 0, "Free Memory: %.2f%%", freeMemory);

    wattron(memoryWindow, COLOR_PAIR(2));
    mvwprintw(memoryWindow, 4, 0, "%-25s | %10s", "Process Name", "Memory Usage");
    wattroff(memoryWindow, COLOR_PAIR(2));

    int row = 5;
    for (const auto& memoryData : memoryDataList) {
        mvwprintw(memoryWindow, row, 0, "%-25s | %10.2f MB", memoryData.processName.c_str(), memoryData.memoryUsage);
        row++;
    }

    wrefresh(memoryWindow);
}

void UI::printDiskMonitorData(const DiskMonitor& diskMonitor) {
    std::vector<std::string> devices, partitions;
    std::vector<DiskData> diskDataList;
    diskMonitor.retrieveDiskInfo(devices, partitions);
    diskMonitor.retrieveDiskActivity(diskDataList);

    wclear(diskWindow);
    wattron(diskWindow, COLOR_PAIR(1));
    mvwprintw(diskWindow, 0, 0, "----- Disk Monitor -----");
    wattroff(diskWindow, COLOR_PAIR(1));

    int row = 1;
    for (const auto& device : devices) {
        mvwprintw(diskWindow, row, 0, "%s", device.c_str());
        row++;
    }

    row += 2;
    for (const auto& partition : partitions) {
        mvwprintw(diskWindow, row, 0, "%s", partition.c_str());
        row++;
    }

    wattron(diskWindow, COLOR_PAIR(2));
    mvwprintw(diskWindow, row + 2, 0, "%-25s | %12s | %12s", "Device", "Read Rate (KB/s)", "Write Rate (KB/s)");
    wattroff(diskWindow, COLOR_PAIR(2));

    row += 5;
    for (const auto& diskData : diskDataList) {
        mvwprintw(diskWindow, row, 0, "%-25s | %12.2f | %12.2f", diskData.device.c_str(), diskData.readRate, diskData.writeRate);
        row++;
    }

    wrefresh(diskWindow);
}

void UI::startMonitoring() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    cpuWindow = newwin(maxY / 3, maxX, 0, 0);
    memoryWindow = newwin(maxY / 3, maxX, maxY / 3, 0);
    diskWindow = newwin(maxY / 3, maxX, (maxY / 3) * 2, 0);

    timeout(5000); // Refresh data every 5 seconds

    while (true) {
        printCPUMonitorData(CPUMonitor());
        printMemoryMonitorData(MemoryMonitor());
        printDiskMonitorData(DiskMonitor());
        refresh();
    }
}
