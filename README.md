# Multithreaded Task Queue

A lightweight and efficient multithreaded task queue implemented in modern C++. 

## Features

- Thread-safe task queue
- Fixed-size thread pool
- Graceful shutdown
- C++17 or later
- Minimal dependencies (STL only)

## Getting Started

### Prerequisites

- C++17 compiler (e.g. `g++`, `clang++`)
- `cmake`

### Building

You can build the project using CMake:

```bash
git clone https://github.com/Charlie-Giese/task-queue.git
cd task-queue
mkdir build && cd build
cmake ..
make```

### Running Tests

You can run tests using ctest.

### Usage

```c++
TaskQueue queue(4);
queue.enqueue([]() {
    std::cout << "Task executed." << std::endl;
})
```

