#ifndef REGISTERS_SNAPSHOT_H
#define REGISTERS_SNAPSHOT_H

#include <iostream>
#include <thread>
#include <atomic>


// The problem is to take a snapshot of two atomic registers.
// One register is modified by one thread and can be read by multiple threads.
// i.e. Single Writer - Multiple Readers.

struct registers_snapshot
{
    int first;
    int second;
};

template<typename Register>
void problem(Register &reg1, Register &reg2)
{
    std::atomic<bool> cancel = false;

    std::thread writer1([&reg1, &cancel]
    {
        int val = 1;
        while (!cancel)
        {
            reg1.write(val);
            ++val;
        }
    });
    std::thread writer2([&reg2, &cancel]
    {
        int val = 1;
        while (!cancel)
        {
            reg2.write(val);
            val += 2;
        }
    });
    std::thread reader([&reg1, &reg2, &cancel]
    {
        for (size_t i = 0; i < 10; ++i)
        {
            registers_snapshot snap = Register::make_snapshot(reg1, reg2);
            std::cout << "snapshot: (" << snap.first << ", " << snap.second << ")" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        cancel = true;
    });


    reader.join();
    writer2.join();
    writer1.join();
}

#endif // REGISTERS_SNAPSHOT_H