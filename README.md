# Monitor Application

add to readme:
-(to quit the app use q or Q or ctrl+C)
-screenshots


The Monitor Application is a command-line utility that monitors CPU, memory, and disk usage on your system. It provides a user-friendly interface with visual tables displaying real-time usage statistics.

## verfication overview

- [X] `README.md` with build instructions
- [X] Builds on Linux VM (Ubuntu)
- [X] Used one of the allowed programming languages C++
- [X] Monitor CPU
	- [X] Current total usage
	- [X] Current usage per application
	- [X] Current free
- [X] Monitor Memory
	- [X] Current total usage
	- [X] Current total free
	- [X] Current usage per application
- [X] Monitor Disk
	- [X] Available devices
	- [X] Available partitions
	- [X] Read rate per application
	- [X] Write rate per application
- [X] Real-time stats collection 2-seconds refresh
- [X] UI
	- [X] Terminal-based UI

## Dependencies

- C++11
- ncurses library

## How to Build

To build the Monitor Application, ensure you have `g++` and the `ncurses` library installed on your system. Then, use the following command to compile the project:

```bash
sudo g++ -o monitor main.cpp src/cpu_monitor.cpp src/memory_monitor.cpp src/disk_monitor.cpp -lncurses -Iinclude
```
## How to Run

After building the application, you can run it with the following command:
```bash
./monitor
```

The application will display three separate sections for CPU, memory, and disk monitor data in visual tables. The data will be updated every **2 seconds**.

## File Structure
```
.
├── include
│   ├── cpu_monitor.h
│   ├── disk_monitor.h
│   └── memory_monitor.h
├── src
│   ├── cpu_monitor.cpp
│   ├── disk_monitor.cpp
│   └── memory_monitor.cpp
├── main.cpp
├── README.md
└── monitor (Executable)
```

include: Contains header files for the CPU, memory, and disk monitors.
src: Contains source files for the CPU, memory, and disk monitors.
monitor: The compiled executable of the Monitor Application.
