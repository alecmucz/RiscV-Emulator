#include "rv/frontend/decoder.hpp"

namespace rv {
namespace {
    constexpr Word bits(rv::Word raw, Word hi, Word lo) noexcept {
        const int width = hi - lo + 1;
        return (raw >> lo) & ((Word{1} << width) - 1);
    }

    constexpr SWord sext(Word value, int width) noexcept {
        const int shift = 32 - width;
        return static_cast<SWord>(value << shift) >> shift;
    }
}

std::expected<Instruction, DecodeError> Decoder::decode(Word raw) noexcept {
    Instruction inst{};
    inst.raw = raw;

    return inst;
    }

}  // namespace rv
