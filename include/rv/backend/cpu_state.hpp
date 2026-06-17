#pragma once
#include "rv/common/types.hpp"
#include "rv/backend/register_file.hpp"
#include "rv/backend/csr.hpp"

namespace rv {

struct CpuState {
    Addr         pc = 0;
    RegisterFile gpr{};
    Csr          csr{};
};

}  // namespace rv
