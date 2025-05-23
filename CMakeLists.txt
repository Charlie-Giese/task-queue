cmake_minimum_required(VERSION 3.12)
project(TaskQueue)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# enable testing
include(CTest)
enable_testing()

# Add the library target
add_library(task_queue
    src/task_queue.cpp
    include/task_queue.hpp
)

add_library(logger
    src/logger.cpp
    include/logger.hpp
)

target_include_directories(task_queue PUBLIC include)

add_subdirectory(tests)

# GoogleTest integration
include(FetchContent)
FetchContent_Declare(
    googletest
    DOWNLOAD_EXTRACT_TIMESTAMP true
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

FetchContent_MakeAvailable(googletest)

# add executable target for testing
add_executable(main
    main.cpp
)

target_link_libraries(main PRIVATE task_queue pthread logger)

# benchmarking
add_executable(throughput_benchmark benchmark/throughput_benchmark.cpp)
add_executable(latency_benchmark benchmark/latency_benchmark.cpp)
add_executable(scalability_benchmark benchmark/scalability_benchmark.cpp)
add_executable(overhead_benchmark benchmark/overhead_benchmark.cpp)

target_link_libraries(throughput_benchmark PRIVATE task_queue)
target_link_libraries(latency_benchmark PRIVATE task_queue)
target_link_libraries(scalability_benchmark PRIVATE task_queue)
target_link_libraries(overhead_benchmark PRIVATE task_queue)


# Enable all warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(task_queue PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(logger PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(main PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

