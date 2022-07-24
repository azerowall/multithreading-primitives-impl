#ifndef ATOMIC_SNAPSHOT_WAITFREE_H
#define ATOMIC_SNAPSHOT_WAITFREE_H

#include <cstddef>
#include <cassert>

#include "registers_snapshot.h"

// Register with wait-free snapshot algorithm.
// Versioning + snapshotting on write
struct wf_register_data_t
{
    int value = 0;
    size_t version = 0;
    registers_snapshot snapshot = { 0, 0 };
};

struct wf_register_t
{
    using data_t = wf_register_data_t;
    std::atomic<data_t> _data;
    const wf_register_t *_other;

    wf_register_t() : _data(data_t()), _other(nullptr) {}
    void watch_other_register(const wf_register_t &other)
    {
        _other = &other;
    }

    data_t data() const noexcept
    {
        return _data;
    }

    void write(int val) noexcept;

    static registers_snapshot
    make_snapshot(const wf_register_t &reg1, const wf_register_t &reg2) noexcept;
};


#endif // ATOMIC_SNAPSHOT_WAITFREE_H