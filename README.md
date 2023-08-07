# Monitor Application

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

The application will display three separate sections for CPU, memory, and disk monitor data in visual tables. The data will be updated every **2 seconds**.To quit the application, you can use one of the following methods:

    Press q or Q while the application is running.
    Use Ctrl+C in the terminal to terminate the application.

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
├── screenshots
└── monitor (Executable)
```

include: Contains header files for the CPU, memory, and disk monitors.
src: Contains source files for the CPU, memory, and disk monitors.
monitor: The compiled executable of the Monitor Application.

## Screenshots

![Screenshot 1](screenshots/image1.png)
![Screenshot 2](screenshots/image2.png)
