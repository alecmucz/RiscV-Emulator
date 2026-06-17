#pragma once
#include <expected>
#include <filesystem>
#include "rv/common/types.hpp"
#include "rv/backend/memory.hpp"

namespace rv {

enum class LoadError {
    NotFound,
    NotElf,
    WrongArch,
};

class ElfLoader {
public:
    [[nodiscard]] static std::expected<Addr, LoadError>
    load(const std::filesystem::path& path, Bus& bus);
};

}  // namespace rv
