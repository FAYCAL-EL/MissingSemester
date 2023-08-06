#include <iostream>
#include <vector>
#include <ncurses.h>
#include "include/cpu_monitor.h"
#include "include/memory_monitor.h"
#include "include/disk_monitor.h"
#include <iomanip>
#include <string>
#include <unistd.h> 

// Function to display a line separator for the table
void printSeparator(WINDOW* win, int y, int x, int width) {
    mvwprintw(win, y, x, "%-*s", width, std::string(width, '-').c_str());
}

// Function to print a row in the table
void printTableRow(WINDOW* win, int y, int x, const std::string& left, const std::string& right) {
    const int width = getmaxx(win);
    mvwprintw(win, y, x, "%-*s%-*s", width / 2, left.c_str(), width / 2, right.c_str());
}
 // Variables to hold the retrieved data
    double totalCPUUsage, freeCPU;
    std::vector<CPUData> cpuDataList;
    double totalMemoryUsage, freeMemory;
    std::vector<MemoryData> memoryDataList;
    std::vector<std::string> devices, partitions;
    std::vector<DiskData> diskDataList;


int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLUE); // Pair 1: White text on Blue background
        init_pair(2, COLOR_RED, COLOR_GREEN); // Pair 2: Yellow text on Black background
    }


    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    WINDOW* cpuWin = newwin(maxY, maxX / 4, 0, 0);
    WINDOW* memoryWin = newwin(maxY, maxX / 4, 0, maxX / 4);
    WINDOW* diskWin = newwin(maxY, maxX / 2, 0,  maxX / 2 + 1 / 5);

    box(cpuWin, 0, 0);
    box(memoryWin, 0, 0);
    box(diskWin, 0, 0);

    refresh();
    wrefresh(cpuWin);
    wrefresh(memoryWin);
    wrefresh(diskWin);

    CPUMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    DiskMonitor diskMonitor;

nodelay(stdscr, TRUE);
// added :
    while (true) {
        int ch = getch();

        // Check if the user pressed 'q' or 'Q' to quit
        if (ch == 'q' || ch == 'Q') {
            break;
        }
  // Retrieve CPU data
    cpuMonitor.retrieveOverallCPUUsage(totalCPUUsage, freeCPU);
    cpuMonitor.retrievePerProcessCPUUsage(cpuDataList);

    // Retrieve memory data
    memoryMonitor.retrieveOverallMemoryUsage(totalMemoryUsage, freeMemory);
    memoryMonitor.retrievePerProcessMemoryUsage(memoryDataList);

    // Retrieve disk data
    diskMonitor.retrieveDiskInfo(devices, partitions);
    diskMonitor.retrieveDiskActivity(diskDataList);


// Print CPU data
int y = 1;
	wattron(cpuWin, COLOR_PAIR(1));
        printTableRow(cpuWin, y++, 1, "----- CPU Usage -----", "");
        wattroff(cpuWin, COLOR_PAIR(1));
printTableRow(cpuWin, y++, 1, "Total CPU Usage:", std::to_string(totalCPUUsage) + "%");
printTableRow(cpuWin, y++, 1, "Free CPU:", std::to_string(freeCPU) + "%");
printSeparator(cpuWin, y++, 1, maxX / 3);
        wattron(cpuWin, COLOR_PAIR(2));
        printTableRow(cpuWin, y++, 1, "Per Process CPU Usage:", "");
        wattroff(cpuWin, COLOR_PAIR(2));
for (const auto& data : cpuDataList) {
    printTableRow(cpuWin, y++, 1, data.processName + ":", std::to_string(data.cpuUsage) + "%");
}
printSeparator(cpuWin, y++, 1, maxX / 3);

    // Print Memory data
    y = 1;
	wattron(memoryWin, COLOR_PAIR(1));
        printTableRow(memoryWin, y++, 1, "----- Memory Usage -----", "");
        wattroff(memoryWin, COLOR_PAIR(1));

    printTableRow(memoryWin, y++, 1, "Total Memory Usage:", std::to_string(totalMemoryUsage) + " MB");
    printTableRow(memoryWin, y++, 1, "Free Memory:", std::to_string(freeMemory) + " MB");
    printSeparator(memoryWin, y++, 1, maxX / 3);
	wattron(memoryWin, COLOR_PAIR(2));
        printTableRow(memoryWin, y++, 1, "Per Process Memory Usage:", "");
        wattroff(memoryWin, COLOR_PAIR(2));
    for (const auto& data : memoryDataList) {
        printTableRow(memoryWin, y++, 1, data.processName + ":", std::to_string(data.memoryUsage) + " MB");
    }
    printSeparator(memoryWin, y++, 1, maxX / 3);



// Print Disk data
y = 1;

        wattron(diskWin, COLOR_PAIR(1));
	printTableRow(diskWin, y++, 1, "----- Disk Info -----", "");
        wattroff(diskWin, COLOR_PAIR(1));


        wattron(diskWin, COLOR_PAIR(2));
	printTableRow(diskWin, y++, 1, "Devices:", "");
        wattroff(diskWin, COLOR_PAIR(2));

std::string devicesStr;
for (const auto& device : devices) {
    devicesStr += device + "  "; // Add two spaces to separate each device name
}
printTableRow(diskWin, y++, 1, devicesStr, ""); // Print all devices in one row

printSeparator(diskWin, y++, 1, maxX / 3);
        wattron(diskWin, COLOR_PAIR(2));
	printTableRow(diskWin, y++, 1, "Partitions:", "");
        wattroff(diskWin, COLOR_PAIR(2));
std::string partitionsStr;
int partitionsPerLine = 10;
int partitionCount = 0;

for (const auto& partition : partitions) {
    partitionsStr += partition + "  "; 
    partitionCount++;

    if (partitionCount == partitionsPerLine) {
        printTableRow(diskWin, y++, 1, partitionsStr, ""); // Print the current line of partitions
        partitionsStr.clear(); // Clear the string for the next line
        partitionCount = 0; // Reset the partition count
    }
}
if (!partitionsStr.empty()) {
    printTableRow(diskWin, y++, 1, partitionsStr, "");
}

printSeparator(diskWin, y++, 1, maxX / 3);
        wattron(diskWin, COLOR_PAIR(2));
	printTableRow(diskWin, y++, 1, "Disk Activity:", "");
        wattroff(diskWin, COLOR_PAIR(2));
for (const auto& data : diskDataList) {
    std::string info = "Device: " + data.device + ", Read Rate: " + std::to_string(data.readRate) + " KB/s, Write Rate: " + std::to_string(data.writeRate) + " KB/s";
    printTableRow(diskWin, y++, 1, info, "");
}

printSeparator(diskWin, y++, 1, maxX / 3);


    wrefresh(cpuWin);
    wrefresh(memoryWin);
    wrefresh(diskWin);

// added :
sleep(2);
}
    getch();
    endwin();
    return 0;
}
