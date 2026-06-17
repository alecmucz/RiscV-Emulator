#pragma once
#include <cstdint>

namespace rv {

using Word  = std::uint32_t;
using Addr  = std::uint32_t;
using SWord = std::int32_t;

enum class Reg : std::uint8_t {};

[[nodiscard]]
constexpr Reg reg(std::uint8_t i) noexcept { return static_cast<Reg>(i & 0x1F); }
[[nodiscard]]
constexpr std::uint8_t index(Reg r) noexcept { return static_cast<std::uint8_t>(r); }

}  // namespace rv
