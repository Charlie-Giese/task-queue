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
make
```

### Running Tests

You can run tests using ctest.

### Usage

```c++
TaskQueue queue(4);
queue.enqueue([]() {
    std::cout << "Task executed." << std::endl;
})
```

## 🧪 Benchmarking

This project includes a set of micro-benchmarks to evaluate the performance of the task queue implementation across various dimensions:

### Benchmarks

- **Latency**: Measures the time from task submission to execution.
    
- **Scalability**: Measures throughput (tasks/second) as the number of worker threads increases.
    
- **Overhead**: Compares raw task execution time vs. time with the task queue to assess framework overhead.


### Running Benchmarks

Use the provided script to build and run all benchmarks:

```python3
python3 run_benchmarks.py
```
Results will be printed to the console and saved to `benchmark_results.txt`.

### Example Output
```yaml
=== latency_benchmark ===
Average task latency: 42.3 µs

=== scalability_benchmark ===
Threads: 1  | Tasks/sec: 5123
Threads: 2  | Tasks/sec: 9610
Threads: 4  | Tasks/sec: 18520
...

=== overhead_benchmark ===
Direct execution avg: 15.2 µs
With queue avg:       43.8 µs

```
### Notes

- All benchmarks are run with `Release` build type.
    
- Results may vary depending on system load and hardware.
    
- You can modify parameters like number of tasks or threads in each benchmark source file.
