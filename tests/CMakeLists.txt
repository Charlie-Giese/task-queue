enable_testing()

add_executable(task_queue_tests
    enqueue_tests.cpp
    worker_tests.cpp
)

target_link_libraries(task_queue_tests
    PRIVATE task_queue
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(task_queue_tests)
