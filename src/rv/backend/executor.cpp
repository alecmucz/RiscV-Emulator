#include "rv/backend/executor.hpp"

namespace rv {

std::expected<void, Trap>
Executor::execute(const Instruction& inst, CpuState& st, Bus& bus) const noexcept {
    (void)inst; (void)st; (void)bus;
    return std::unexpected(Trap::IllegalInstruction);
}

}  // namespace rv
