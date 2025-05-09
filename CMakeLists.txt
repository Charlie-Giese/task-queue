cmake_minimum_required(VERSION 3.12)
project(TaskQueue)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the library target
add_library(task_queue
    src/task_queue.cpp
    include/task_queue.hpp
)

target_include_directories(task_queue PUBLIC include)

# add executable target for testing
add_executable(main
    main.cpp
)

target_link_libraries(main PRIVATE task_queue pthread)

# Enable all warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(task_queue PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(main PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

