#pragma once
#include <expected>
#include "rv/common/types.hpp"
#include "rv/backend/memory.hpp"

namespace rv {

class Device {
public:
    virtual ~Device() = default;
    [[nodiscard]] virtual std::expected<Word, MemFault> read(Addr offset) = 0;
    [[nodiscard]] virtual std::expected<void, MemFault> write(Addr offset, Word v) = 0;
};

}  // namespace rv
