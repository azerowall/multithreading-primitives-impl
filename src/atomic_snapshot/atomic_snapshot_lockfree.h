#ifndef ATOMIC_SNAPSHOT_LOCKFREE_H
#define ATOMIC_SNAPSHOT_LOCKFREE_H

#include <cstddef>
#include <cassert>

#include "registers_snapshot.h"


// Register with lock-free snapshot algorithm.
// Based on versioning.

struct lf_register_data_t
{
    int value = 0;
    size_t version = 0;
};

struct lf_register_t
{
    using data_t = lf_register_data_t;
    std::atomic<data_t> _data;

    lf_register_t() : _data(data_t()) {}

    data_t data() const noexcept
    {
        return _data;
    }

    void write(int val) noexcept;

    static registers_snapshot
    make_snapshot(const lf_register_t & reg1, const lf_register_t & reg2) noexcept;
};

#endif // ATOMIC_SNAPSHOT_LOCKFREE_H