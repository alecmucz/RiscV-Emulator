#include "rv/frontend/decoder.hpp"
#include <cassert>

int main() {
    const auto inst = rv::Decoder::decode(0x00100093u);

    (void)inst;
    return 0;
}
