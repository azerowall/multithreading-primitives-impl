#include <iostream>
#include <thread>
#include <chrono>

#include "spinlock/spinlock_tas.h"
#include "spinlock/spinlock_ttas.h"
#include "atomic_snapshot/atomic_snapshot_lockfree.h"
#include "atomic_snapshot/atomic_snapshot_waitfree.h"


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

void register_snapshot_demo()
{
    {
        std::cout << "Lock-free registers snapshots" << std::endl;

        lf_register_t reg1, reg2;

        problem(reg1, reg2);
    }
    {
        std::cout << "Wait-free registers snapshots" << std::endl;

        wf_register_t reg1, reg2;
        reg1.watch_other_register(reg2);
        reg2.watch_other_register(reg1);

        problem(reg1, reg2);
    }
}

int main()
{
    // test_spinlock();
    register_snapshot_demo();

    return 0;
}