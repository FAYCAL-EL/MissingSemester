# Monitor Application

The Monitor Application is a command-line utility that monitors CPU, memory, and disk usage on your system. It provides a user-friendly interface with visual tables displaying real-time usage statistics.

## Dependencies

- C++11
- ncurses library

## How to Build

To build the Monitor Application, ensure you have `g++` and the `ncurses` library installed on your system. Then, use the following command to compile the project:

```bash
g++ -o ui_test_program main.cpp src/cpu_monitor.cpp src/memory_monitor.cpp src/disk_monitor.cpp src/ui.cpp -Iinclude -lncurses

How to Run

After building the application, you can run it with the following command:

./ui_test_program

The application will display three separate sections for CPU, memory, and disk monitor data in visual tables. The data will be updated every 5 seconds.

File Structure

.
├── include
│   ├── cpu_monitor.h
│   ├── disk_monitor.h
│   ├── memory_monitor.h
│   └── ui.h
├── src
│   ├── cpu_monitor.cpp
│   ├── disk_monitor.cpp
│   ├── memory_monitor.cpp
│   └── ui.cpp
├── main.cpp
├── README.md
└── ui_test_program (Executable)




include: Contains header files for the CPU, memory, disk monitors, and user interface.
src: Contains source files for the CPU, memory, disk monitors,and user interface.
ui_test_program: The compiled executable of the Monitor Application.

