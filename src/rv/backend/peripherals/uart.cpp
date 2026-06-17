#include "rv/backend/peripherals/uart.hpp"
#include <cstdio>

namespace rv {

std::expected<Word, MemFault> Uart::read(Addr offset) {
    (void)offset;
    return Word{0};
}

std::expected<void, MemFault> Uart::write(Addr offset, Word v) {
    if (offset == 0) std::putchar(static_cast<int>(v & 0xFFu));
    return {};
}

}  // namespace rv
