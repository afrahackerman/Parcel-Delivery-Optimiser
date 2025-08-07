# Parcel Delivery Optimizer

This repository contains the source code, documentation, and resources for the Parcel Delivery Optimizer, a C++ console application developed for the CSE-2422 Computer Algorithms Lab (Spring-2025, 4th Semester) at the International Islamic University Chittagong.

## Overview
The Parcel Delivery Optimizer streamlines parcel delivery management with role-based functionalities (Admin, Staff, User) and implements optimization algorithms like Merge Sort, Greedy Scheduling, and Knapsack. Data is persisted in `staff.txt` and `parcels.txt` files.

## Repository Contents
- `parcelDeliveryoptimizer.cpp` and `.h` files: Full source code.
- `staff.txt` and `parcels.txt`: Sample data files.
- `pictures/`: Folder with console screenshots.
- `flowchart.png`: Lucidchart flowchart diagram.

## Setup Instructions
1. **Install Code::Blocks with MinGW Compiler**:
   - Download and install Code::Blocks from [codeblocks.org](http://www.codeblocks.org/downloads).
   - Ensure MinGW is included or install it separately from [sourceforge.net/projects/mingw](https://sourceforge.net/projects/mingw/).
   - Add MinGW's `bin` directory (e.g., `C:\MinGW\bin`) to your system's PATH environment variable.
2. **Open the Project**:
   - Launch Code::Blocks and open `parcelDeliveryoptimizer.cpp`.
3. **Build and Run**:
   - Build the project (Build > Build) and run it (Build > Run).
4. **Usage**:
   - Select a role (Admin, Staff, User) from the main menu to access respective features.

## Requirements
### Hardware
- Processor: Minimum 1.5 GHz CPU (e.g., Intel Core i3 or equivalent).
- RAM: Minimum 4 GB (recommended 8 GB).
- Storage: Minimum 1 MB free disk space (may increase with data).
- Input/Output: Keyboard and monitor with standard terminal support.

### Software
- Operating System: Windows, macOS, or Linux (with compatible compiler).
- Development Environment: Code::Blocks IDE with MinGW, or other IDEs (e.g., Visual Studio, Eclipse) with C++11 compiler.
- Libraries: Standard C++11 libraries (e.g., `iostream`, `vector`, `fstream`).

## Future Improvements
- Input validation with `cin.fail()` for non-numeric inputs.
- Unique staff ID checks to prevent duplicates.
- Staff availability checks to avoid scheduling conflicts.
- GUI replacement for console menus.
- Database storage integration.

## Contributors
- Tahasina Tasnim Afra (C233456)
- Asliraf Samaylan (C233446)
- Nusrat Jahan (C233464)

## Instructor
Ms. Miskatul Jannat Tuly, Assistant Lecturer, Department of Computer Science and Engineering, IIUC.

## Submission Date
August 7, 2025
