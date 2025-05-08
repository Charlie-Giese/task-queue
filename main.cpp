#include "task_queue.hpp"

int main() {

    TaskQueue queue(4);

    queue.enqueue([] {
        std::cout << "Hello from thread " << std::this_thread::get_id() << "\n";
    });

    queue.enqueue([] {
        std::cout << "Doing some work...\n";
    });

    return 0;
}
