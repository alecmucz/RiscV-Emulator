#include "rv/backend/hart.hpp"
#include "rv/frontend/decoder.hpp"

namespace rv {

std::expected<void, Trap> Hart::step() noexcept {
    const auto word = bus_.load32(state_.pc);
    if (!word) return std::unexpected(Trap::LoadFault);

    const auto inst = Decoder::decode(*word);
    if (!inst) return std::unexpected(Trap::IllegalInstruction);

    return exec_.execute(*inst, state_, bus_);
}

void Hart::run() noexcept {

}

}  // namespace rv
