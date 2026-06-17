#include <cstdio>
#include "rv/backend/memory.hpp"
#include "rv/backend/hart.hpp"
#include "rv/loader/elf_loader.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::fprintf(stderr, "usage: %s <program.elf>\n", argv[0]);
        return 1;
    }

    rv::Bus bus;
    const auto entry = rv::ElfLoader::load(argv[1], bus);
    if (!entry) {
        std::fprintf(stderr, "failed to load %s\n", argv[1]);
        return 1;
    }

    rv::Hart hart(bus, *entry);
    hart.run();
    return 0;
}
