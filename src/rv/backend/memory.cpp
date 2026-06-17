#include "rv/backend/memory.hpp"

namespace rv {

std::expected<Word, MemFault> Bus::load32(Addr a) const noexcept {
    (void)a;
    return std::unexpected(MemFault::AccessFault);
}

std::expected<void, MemFault> Bus::store32(Addr a, Word v) noexcept {
    (void)a; (void)v;
    return std::unexpected(MemFault::AccessFault);
}

}  // namespace rv
