#pragma once
#include <expected>
#include "rv/common/types.hpp"

namespace rv {

enum class MemFault {
    Misaligned,
    AccessFault,
};

class Bus {
public:
    [[nodiscard]] std::expected<Word, MemFault> load32(Addr a) const noexcept;
    [[nodiscard]] std::expected<void, MemFault> store32(Addr a, Word v) noexcept;
};

}  // namespace rv
