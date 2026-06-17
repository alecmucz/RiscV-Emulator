#include "rv/loader/elf_loader.hpp"

namespace rv {

std::expected<Addr, LoadError>
ElfLoader::load(const std::filesystem::path& path, Bus& bus) {
    (void)path; (void)bus;
    return std::unexpected(LoadError::NotFound);
}

}  // namespace rv
