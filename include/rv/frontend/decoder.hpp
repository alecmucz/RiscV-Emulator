#pragma once
#include <expected>
#include "rv/common/types.hpp"
#include "rv/frontend/instruction.hpp"

namespace rv {

enum class DecodeError {
    IllegalInstruction,
    Unsupported,
};

class Decoder {
public:
    [[nodiscard]]
    static std::expected<Instruction, DecodeError> decode(Word raw) noexcept;
};

}  // namespace rv
