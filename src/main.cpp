#include <iostream>
#include <thread>
#include <chrono>

#include "spinlock/spinlock_tas.h"
#include "spinlock/spinlock_ttas.h"


template<typename Spinlock>
void test_spinlock_increment(int iterations)
{
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
}

void test_spinlock()
{
    test_spinlock_increment<spinlock_tas>(1'000'000);
    test_spinlock_increment<spinlock_ttas>(1'000'000);
}

void register_snapshot_demo();

int main()
{
    test_spinlock();

    return 0;
}