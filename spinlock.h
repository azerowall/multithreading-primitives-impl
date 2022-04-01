#include <atomic>

// Based on https://rigtorp.se/spinlock/

// Test-And-Set spinlock
class spinlock_tas
{
public:
    void lock() noexcept
    {
        while(_lock.test_and_set(std::memory_order_acquire));
    }

    bool try_lock() noexcept
    {
        return !_lock.test_and_set(std::memory_order_acquire);
    }

    void unlock() noexcept
    {
        _lock.clear(std::memory_order_release);
    }

private:
    std::atomic_flag _lock { ATOMIC_FLAG_INIT };
};

// Test and Test-And-Set spinlock
// The optimization is based on the fact that many cache coherence protocols
// have common features: either a single CPU core can write,
// or multiple CPU cores can read from a cache line.
// Also this implementation has optimisations to reduce power usage
// and contention on the load-store units.
class spinlock_ttas
{
public:
    void lock() noexcept
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

    bool try_lock() noexcept
    {
        return !_lock.load(std::memory_order_relaxed)
            && !_lock.exchange(std::memory_order_acquire);
    }

    void unlock() noexcept
    {
        _lock.store(false, std::memory_order_release);
    }

private:
    // Changed to atomic<bool> from atomic_flag because we need a `load` operation.
    // In C++ 20 atomic_flag has `test` method for this purpose.
    std::atomic<bool> _lock { false };
};