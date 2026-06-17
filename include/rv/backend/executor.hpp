#pragma once
#include <expected>
#include "rv/frontend/instruction.hpp"
#include "rv/backend/cpu_state.hpp"
#include "rv/backend/memory.hpp"

namespace rv {

enum class Trap {
    EnvironmentCall,
    Breakpoint,
    IllegalInstruction,
    LoadFault,
    StoreFault,
};

class Executor {
public:
    [[nodiscard]] std::expected<void, Trap>
    execute(const Instruction& inst, CpuState& st, Bus& bus) const noexcept;
};

}  // namespace rv
