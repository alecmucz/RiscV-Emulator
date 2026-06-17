#pragma once
#include <array>
#include "rv/common/types.hpp"

namespace rv {

class RegisterFile {
public:
    [[nodiscard]] Word read(Reg r) const noexcept {
        return index(r) == 0 ? 0u : regs_[index(r)];
    }
    void write(Reg r, Word v) noexcept {
        if (index(r) != 0) regs_[index(r)] = v;
    }
private:
    std::array<Word, 32> regs_{};
};

}  // namespace rv
