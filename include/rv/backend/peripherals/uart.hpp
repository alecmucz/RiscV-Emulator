#pragma once
#include "rv/backend/device.hpp"

namespace rv {

class Uart final : public Device {
public:
    [[nodiscard]] std::expected<Word, MemFault> read(Addr offset) override;
    [[nodiscard]] std::expected<void, MemFault> write(Addr offset, Word v) override;
};

}  // namespace rv
