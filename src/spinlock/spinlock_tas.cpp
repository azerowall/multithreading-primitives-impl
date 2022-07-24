#include "spinlock_tas.h"


void spinlock_tas::lock() noexcept
{
    while(_lock.test_and_set(std::memory_order_acquire));
}

bool spinlock_tas::try_lock() noexcept
{
    return !_lock.test_and_set(std::memory_order_acquire);
}

void spinlock_tas::unlock() noexcept
{
    _lock.clear(std::memory_order_release);
}