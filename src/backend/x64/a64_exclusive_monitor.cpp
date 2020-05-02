/* This file is part of the dynarmic project.
 * Copyright (c) 2018 MerryMage
 * SPDX-License-Identifier: 0BSD
 */

#include <algorithm>

#include <dynarmic/A64/exclusive_monitor.h>
#include "common/assert.h"
#include "common/common_types.h"

namespace Dynarmic {
namespace A64 {

static constexpr u64 INVALID_ADDRESS = 0xDEADDEADDEADDEADull;

ExclusiveMonitor::ExclusiveMonitor(size_t processor_count) : state(processor_count) {
    Unlock();
}

size_t ExclusiveMonitor::GetProcessorCount() const {
    return state.size();
}

bool ExclusiveMonitor::CheckAndClear(size_t processor_id, VAddr address, size_t size) {
    State& s = state[processor_id];
    if (s.address != address) {
        Unlock();
        return false;
    }

    for (State& other_s : state) {
        if (other_s.address == address) {
            other_s.address = INVALID_ADDRESS;
        }
    }
    return true;
}

void ExclusiveMonitor::Clear() {
    Lock();
    for (State& s : state) {
        s.address = INVALID_ADDRESS;
    }
    Unlock();
}

void ExclusiveMonitor::ClearProcessor(size_t processor_id) {
    Lock();
    state[processor_id].address = INVALID_ADDRESS;
    Unlock();
}

void ExclusiveMonitor::Lock() {
    while (is_locked.test_and_set(std::memory_order_acquire)) {}
}

void ExclusiveMonitor::Unlock() {
    is_locked.clear(std::memory_order_release);
}

} // namespace A64
} // namespace Dynarmic
