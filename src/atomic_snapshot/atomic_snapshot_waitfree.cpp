#include "atomic_snapshot_waitfree.h"

void wf_register_t::write(int val) noexcept
{
    data_t next = _data;
    next.value = val;
    ++next.version;
    next.snapshot = make_snapshot(*this, *_other);
    _data = next;
}

registers_snapshot
wf_register_t::make_snapshot(const wf_register_t &reg1, const wf_register_t &reg2) noexcept
{
    data_t r1_1, r2_1, r1_2, r2_2, r1_3;

    // read 1
    r1_1 = reg1.data();
    r2_1 = reg2.data();

    // read 2
    r1_2 = reg1.data();
    if (r1_1.version == r1_2.version) {
        return { r1_1.value, r2_1.value };
    }
    // so, write to register 1 occurred between read 1-2

    r2_2 = reg2.data();
    if (r2_1.version == r2_2.version) {
        return { r1_2.value, r2_2.value };
    }
    // write to register 2 occurred between read 1-2

    // read 3
    r1_3 = reg1.data();
    if (r1_2.version == r1_3.version) {
        return { r1_2.value, r2_2.value };
    }
    // write to register 1 occurred between read 2-3

    // At this point, we know that the writes to register 1 happened
    // between read 1-2 and read 2-3 (version changed).
    // We have one writer, so the writes happened sequentially.
    // So the snapshot that happens on write between read 2-3 is relevant
    // and we can use it.
    assert(r1_1.version != r1_2.version && r1_2.version != r1_3.version);
    return r1_3.snapshot;
}