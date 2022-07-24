#include "atomic_snapshot_lockfree.h"

void lf_register_t::write(int val) noexcept
{
    data_t next = _data;
    next.value = val;
    ++next.version;
    _data = next;
}

registers_snapshot
lf_register_t::make_snapshot(const lf_register_t & reg1, const lf_register_t & reg2) noexcept
{
    lf_register_t::data_t r1_prev, r1_curr, r2_prev, r2_curr;

    r1_prev = reg1.data();
    r2_prev = reg2.data();
    while (true) {
        r1_curr = reg1.data();
        if (r1_prev.version == r1_curr.version)
        {
            return { r1_prev.value, r2_prev.value };
        }

        r2_curr = reg2.data();
        if (r2_prev.version == r2_curr.version)
        {
            return { r1_curr.value, r2_curr.value };
        }

        r1_prev = r1_curr;
        r2_prev = r2_curr;
    }
}