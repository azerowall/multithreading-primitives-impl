#include <iostream>
#include <thread>
#include <chrono>

#include "spinlock.h"


template<typename Spinlock>
void test_spinlock(int iterations) {
    uint32_t counter = 0;
    Spinlock lock;

    auto task = [&counter, &lock, iterations]() {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            lock.lock();
            ++counter;
            lock.unlock();
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Total: " << (end - start).count() << std::endl;
    };
    std::thread t1(task);
    std::thread t2(task);

    t1.join();
    t2.join();

    std::cout << "Counter: " << counter << std::endl;
    // assert(counter == iterations * 2);
}


int main()
{
    test_spinlock<spinlock_tas>(1'000'000);
    test_spinlock<spinlock_ttas>(1'000'000);

    return 0;
}