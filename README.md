# CS3400 Campus Transport Resource Management System

Group project repository for the CS3400 data structures and algorithms project.

## Build and run

From this folder, with MinGW `g++` and `mingw32-make` on `PATH`:

```sh
mingw32-make -f Makefile.win clean all
./CS3400_Campus_System.exe
```

The Dev-C++ project is configured to use the same makefile. The makefile
compiles with C++11 and includes the Student A, B, and C modules.

## Main menu

- **1 Resource management**: linked-list vehicle records, file persistence,
  searching, and insertion/merge sorting.
- **4 Requests and activities**: Student B FIFO requests, priority requests,
  cancellation undo, processing, and activity scheduling.
- **7 Tree operations**, **8 Hash table lookup**, **9 Campus navigation**,
  **10 System analysis**: Student C modules.

Student B requests must reference a vehicle ID already present in `vehicles.txt`
or registered through Resource Management. Pending requests are stored in
`requests.txt`; campus stops and connections are stored in
`data/locations.txt`.

## Request processing rules

Regular priorities (3 Normal and 4 Low) use a linked FIFO queue. Emergency
(1) and High (2) requests use a binary min-heap, where the lower priority
number is removed first. A linked stack stores cancelled requests for LIFO
undo. Activities are represented as request records and enter the queue or
heap according to their priority.
