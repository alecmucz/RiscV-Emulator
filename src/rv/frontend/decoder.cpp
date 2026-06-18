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

    [[nodiscard]] constexpr Reg  rd(Word w)     { return reg(bits(w, 11, 7));  }
    [[nodiscard]] constexpr Reg  rs1(Word w)    { return reg(bits(w, 19, 15)); }
    [[nodiscard]] constexpr Reg  rs2(Word w)    { return reg(bits(w, 24, 20)); }
    [[nodiscard]] constexpr Word funct3(Word w) { return bits(w, 14, 12); }
    [[nodiscard]] constexpr Word funct7(Word w) { return bits(w, 31, 25); }

    constexpr SWord imm_i(Word raw) noexcept {
        return sext(bits(raw, 31, 20), 12);
    }

    constexpr SWord imm_s(Word raw) noexcept {
        Word imm = (bits(raw, 31, 25) << 5) |
                    bits(raw, 11, 7);
        return sext(imm, 12);
    }

    constexpr SWord imm_b(Word raw) noexcept {
        Word imm = (bits(raw, 31, 31) << 12) |
                   (bits(raw, 7, 7)   << 11) |
                   (bits(raw, 30, 25) << 5)  |
                   (bits(raw, 11, 8)  << 1);
        return sext(imm, 13);
    }

    constexpr SWord imm_u(Word raw) noexcept {
        return static_cast<SWord>(raw & 0xFFFFF000);
    }

    constexpr SWord imm_j(Word raw) noexcept {
        Word imm = (bits(raw, 31, 31) << 20) |
                   (bits(raw, 19, 12) << 12) |
                   (bits(raw, 20, 20) << 11) |
                   (bits(raw, 30, 21) << 1);
        return sext(imm, 21);
    }

    constexpr Opcode funct_to_op(Word f3, Word f7) noexcept {
        switch(f3) {
            case 0x0:
                switch (f7) {
                case 0x00: return Opcode::Add;
                case 0x20: return Opcode::Sub;
                }
                break;
            case 0x1:
                return Opcode::Sll;
            case 0x2:
                return Opcode::Slt;
            case 0x3:
                return Opcode::Sltu;
            case 0x4:
                return Opcode::Xor;
            case 0x5:
                switch (f7) {
                    case 0x00: return Opcode::Srl;
                    case 0x20: return Opcode::Sra;
                }
                break;
            case 0x6:
                return Opcode::Or;
            case 0x7:
                return Opcode::And;
        }
        return Opcode::Invalid;
    }
}

std::expected<Instruction, DecodeError> Decoder::decode(const Word raw) noexcept {
    Instruction inst{};
    inst.raw = raw;

    const Word op_field = bits(inst.raw, 6,0);
`
    switch (op_field) {
        case 0x33:  // R-Type
            inst.op = funct_to_op(funct3(raw), funct7(raw));
            inst.fmt = Format::R;
            inst.rd = rd(raw);
            inst.rs1 = rs1(raw);
            inst.rs2 = rs2(raw);
            break;

        case 0x03:  // I-Type
        case 0x13:
        case 0x67:
            inst.rd = rd(raw);
            break;

        case 0x23:  // S-Type
            break;

        case 0x63:  // B-Type
            break;

        case 0x17:  // U-Type
        case 0x37:
            inst.rd = rd(raw);
            break;

        case 0x6F:  // J-Type
            inst.rd = rd(raw);
            break;

        default:
            return std::unexpected(DecodeError::IllegalInstruction);
    }

    if (inst.op == Opcode::Invalid)
        return std::unexpected(DecodeError::IllegalInstruction);
    return inst;
    }
}  // namespace rv
