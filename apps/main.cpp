#include <charconv>
#include <print>
#include <string_view>

#include <rv/frontend/decoder.hpp>
#include <rv/frontend/instruction_formatter.hpp>

namespace {
    std::expected<rv::Word, std::string_view> parse_word(std::string_view text) {
        constexpr int hex_base = 16;
        constexpr std::string_view hex_prefix = "0x";

        if (text.starts_with(hex_prefix) || text.starts_with("0X")) {
            text.remove_prefix(hex_prefix.size());
        }

        rv::Word word{};
        const char* begin = text.data();
        const char* end = begin + text.size();

        const auto [parsed_until, error] = std::from_chars(begin, end, word, hex_base);

        if (error != std::errc{} || parsed_until != end) {
            return std::unexpected("not a valid hex word");
        }

        return word;
    }
}

int main(int argc, char** argv) {
    rv::Addr pc = 0;
    for (int i = 1; i < argc; ++i) {
        auto word = parse_word(argv[i]);
        if (!word) {
            std::println("{:08x}:           <bad input '{}'>", pc, argv[i]);
            pc += 4;
            continue;
        }

        auto inst = rv::Decoder::decode(*word);
        if (inst) {
            std::println("{:08x}:  {:08x}  {}", pc, *word, *inst);
        } else {
            std::println("{:08x}:  {:08x}  <decode error>", pc, *word);
        }
        pc += 4;
    }
}