#ifndef SPINLOCK_TAS_H
#define SPINLOCK_TAS_H

#include <atomic>

// Test-And-Set spinlock
class spinlock_tas
{
public:
    void lock() noexcept;
    bool try_lock() noexcept;
    void unlock() noexcept;

private:
    std::atomic_flag _lock { ATOMIC_FLAG_INIT };
};

#endif // SPINLOCK_TAS_H