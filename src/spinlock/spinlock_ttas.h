#ifndef SPINLOCK_TTAS_H
#define SPINLOCK_TTAS_H

#include <atomic>

// Test and Test-And-Set spinlock
// Based on https://rigtorp.se/spinlock/
// The optimization is based on the fact that many cache coherence protocols
// have common features: either a single CPU core can write,
// or multiple CPU cores can read from a cache line.
// Also this implementation has optimisations to reduce power usage
// and contention on the load-store units.
class spinlock_ttas
{
public:
    void lock() noexcept;
    bool try_lock() noexcept;
    void unlock() noexcept;

private:
    // Changed to atomic<bool> from atomic_flag because we need a `load` operation.
    // In C++ 20 atomic_flag has `test` method for this purpose.
    std::atomic<bool> _lock { false };
};

#endif // SPINLOCK_TTAS_H