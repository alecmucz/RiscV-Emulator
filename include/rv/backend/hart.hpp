#pragma once
#include <expected>
#include "rv/common/types.hpp"
#include "rv/backend/cpu_state.hpp"
#include "rv/backend/memory.hpp"
#include "rv/backend/executor.hpp"

namespace rv {


class Hart {
public:
    Hart(Bus& bus, Addr entry) : bus_(bus) { state_.pc = entry; }

    [[nodiscard]] std::expected<void, Trap> step() noexcept;

    void run() noexcept;

    [[nodiscard]] const CpuState& state() const noexcept { return state_; }

private:
    Bus&     bus_;
    CpuState state_{};
    Executor exec_{};
};

}  // namespace rv
