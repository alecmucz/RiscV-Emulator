#include "rv/frontend/decoder.hpp"

namespace rv {
namespace {
    constexpr Word bits(Word raw, Word hi, Word lo) noexcept {
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

    constexpr Opcode normalize_opcode(Word f3, Word f7) noexcept {
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

    constexpr Opcode decode_branch(Word f3) noexcept {
        switch (f3) {
            case 0x0: return Opcode::Beq;
            case 0x1: return Opcode::Bne;
            case 0x4: return Opcode::Blt;
            case 0x5: return Opcode::Bge;
            case 0x6: return Opcode::Bltu;
            case 0x7: return Opcode::Bgeu;
        }
        return Opcode::Invalid;
    }

    constexpr Opcode decode_load(Word f3) noexcept {
        switch (f3) {
            case 0x0: return Opcode::Lb;
            case 0x1: return Opcode::Lh;
            case 0x2: return Opcode::Lw;
            case 0x4: return Opcode::Lbu;
            case 0x5: return Opcode::Lhu;
        }
        return Opcode::Invalid;
    }

    constexpr Opcode decode_store(Word f3) noexcept {
        switch (f3) {
            case 0x0: return Opcode::Sb;
            case 0x1: return Opcode::Sh;
            case 0x2: return Opcode::Sw;
        }
        return Opcode::Invalid;
    }

    constexpr Opcode decode_op_imm(Word f3, Word f7) noexcept {
        switch (f3) {
            case 0x0: return Opcode::Addi;
            case 0x2: return Opcode::Slti;
            case 0x3: return Opcode::Sltiu;
            case 0x4: return Opcode::Xori;
            case 0x6: return Opcode::Ori;
            case 0x7: return Opcode::Andi;
            case 0x1: return Opcode::Slli;
            case 0x5:
                switch (f7) {
                case 0x00: return Opcode::Srli;
                case 0x20: return Opcode::Srai;
                }
                break;
        }
        return Opcode::Invalid;
    }
}

std::expected<Instruction, DecodeError> Decoder::decode(const Word raw) noexcept {
    Instruction inst{};
    inst.raw = raw;

    switch (bits(inst.raw, 6,0)) {  // Opcode
        case 0x33:  // R-type: reg-reg
            inst.fmt = Format::R;
            inst.op  = normalize_opcode(funct3(raw), funct7(raw));
            inst.rd  = rd(raw);
            inst.rs1 = rs1(raw);
            inst.rs2 = rs2(raw);
            break;

        case 0x03:  // I-type: loads
            inst.fmt = Format::I;
            inst.op  = decode_load(funct3(raw));
            inst.rd  = rd(raw);
            inst.rs1 = rs1(raw);
            inst.imm = imm_i(raw);
            break;

        case 0x13:  // I-type: reg-imm
            inst.fmt = Format::I;
            inst.op  = decode_op_imm(funct3(raw), funct7(raw));
            inst.rd  = rd(raw);
            inst.rs1 = rs1(raw);

            inst.imm = (funct3(raw) == 0x1 || funct3(raw) == 0x5)
                         ? static_cast<SWord>(bits(raw, 24, 20))
                         : imm_i(raw);
            break;

        case 0x67:  // I-type: jalr
            inst.fmt = Format::I;
            inst.op  = Opcode::Jalr;
            inst.rd  = rd(raw);
            inst.rs1 = rs1(raw);
            inst.imm = imm_i(raw);
            break;

        case 0x73:  // I-type: system
            inst.fmt = Format::I;
            if (funct3(raw) == 0) {
                switch (bits(raw, 31, 20)) {
                    case 0x000:
                        inst.op = Opcode::Ecall;
                        break;
                    case 0x001:
                        inst.op = Opcode::Ebreak;
                        break;
                    default:
                        inst.op = Opcode::Invalid;
                        break;
                }
            }
            break;

        case 0x0F:  // fence
            inst.fmt = Format::I;
            inst.op  = Opcode::Fence;
            break;

        case 0x23:  // S-type: stores
            inst.fmt = Format::S;
            inst.op  = decode_store(funct3(raw));
            inst.rs1 = rs1(raw);
            inst.rs2 = rs2(raw);
            inst.imm = imm_s(raw);
            break;

        case 0x63:  // B-type: branches
            inst.fmt = Format::B;
            inst.op  = decode_branch(funct3(raw));
            inst.rs1 = rs1(raw);
            inst.rs2 = rs2(raw);
            inst.imm = imm_b(raw);
            break;

        case 0x37:  // U-type: lui
            inst.fmt = Format::U;
            inst.op  = Opcode::Lui;
            inst.rd  = rd(raw);
            inst.imm = imm_u(raw);
            break;

        case 0x17:  // U-type: auipc
            inst.fmt = Format::U;
            inst.op  = Opcode::Auipc;
            inst.rd  = rd(raw);
            inst.imm = imm_u(raw);
            break;

        case 0x6F:  // J-type: jal
            inst.fmt = Format::J;
            inst.op  = Opcode::Jal;
            inst.rd  = rd(raw);
            inst.imm = imm_j(raw);
            break;

        default:
            return std::unexpected(DecodeError::IllegalInstruction);
    }

    if (inst.op == Opcode::Invalid)
        return std::unexpected(DecodeError::IllegalInstruction);
    return inst;
    }
}  // namespace rv
