#include "spinlock_ttas.h"


void spinlock_ttas::lock() noexcept
{
    for (;;)
    {
        if (!_lock.exchange(std::memory_order_acquire))
        {
            break;
        }
        while (_lock.load(std::memory_order_relaxed))
        {
#if defined(__GNUC__)
            __builtin_ia32_pause();
#elif defined(_MSC_VER)
            _mm_pause();
#endif
        }
    }
}

bool spinlock_ttas::try_lock() noexcept
{
    return !_lock.load(std::memory_order_relaxed)
        && !_lock.exchange(std::memory_order_acquire);
}

void spinlock_ttas::unlock() noexcept
{
    _lock.store(false, std::memory_order_release);
}