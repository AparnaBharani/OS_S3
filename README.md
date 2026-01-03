# OS_S3

#  Project Manager & CPU Scheduler Simulator

> **A Hybrid System Programming Project** > *Combines File Management Utilities with Operating System Process Scheduling Simulations.*

---

##  Table of Contents
1. [Project Overview](#-project-overview)
2. [Features](#-features)
3. [Technical Architecture](#-technical-architecture)
    - [File Handling & System Calls](#file-handling--system-calls)
    - [Process Control Block (PCB) Simulation](#process-control-block-pcb-simulation)
4. [Algorithm Theory & Implementation](#-algorithm-theory--implementation)
5. [Installation & Usage](#-installation--usage)
6. [Future Enhancements](#-future-enhancements)

---

##  Project Overview
This project is a C-based system utility designed to demonstrate the interaction between **User-Level Applications** and **Kernel-Level Operations**. 

It serves two primary purposes:
1.  **File Management:** Provides a CLI interface to perform standard CRUD (Create, Read, Update, Delete) operations on files and directories, wrapping standard C library calls that interact with the OS file system.
2.  **OS Simulation:** Uniquely, this project treats every file operation (like creating or writing to a file) as a **"Process"**. It calculates `Burst Time` (execution time) and `Priority` for these actions and simulates how a single-core CPU would schedule them using various algorithms.

---

##  Features

### 1. File System Operations
The system mimics a basic shell, allowing users to interact with the Windows/Linux file system:
* **Directory Management:** Create project folders (Handling `_mkdir` for Windows and `mkdir` for Linux).
* **File Creation:** Initialize new text files (`.txt`).
* **I/O Operations:** * *Write:* Append text to files (simulates CPU processing time based on character count).
    * *Read:* Display file contents to `stdout`.
* **Maintenance:** Rename or Delete files safely.
* **Search:** Check for the existence of specific files.
* **Backup:** Create a full directory backup of the project.

### 2. CPU Scheduling Simulation
Every action performed in the File Manager is logged as a process. The system then visualizes how these tasks would be executed by a CPU:
* **FCFS (First-Come, First-Served):** Non-preemptive scheduling based on arrival order.
* **Round Robin (RR):** Preemptive scheduling using a user-defined Time Quantum.
* **Priority Scheduling:** Non-preemptive selection based on assigned task priority.
* **Gantt Chart Visualization:** A text-based graphical representation of the CPU timeline.

---

##  Technical Architecture

### File Handling & System Calls
The project uses conditional compilation to ensure cross-platform compatibility between Windows and Linux/Unix systems.

**Key Macros:**
```c
#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
    #define CREATE_FOLDER(name) _mkdir(name)  // Windows System Call Wrapper
#else
    #include <sys/stat.h>
    #define CREATE_FOLDER(name) mkdir(name, 0777) // Linux System Call Wrapper
#endif
