#pragma once
#include <format>
#include <string_view>
#include <utility>
#include <rv/frontend/instruction.hpp>

namespace rv {
    constexpr std::string_view mnemonic(const Opcode op) {
        switch (op) {
            case Opcode::Invalid: return "<invalid>";

            case Opcode::Lui:    return "lui";
            case Opcode::Auipc:  return "auipc";
            case Opcode::Jal:    return "jal";
            case Opcode::Jalr:   return "jalr";

            case Opcode::Beq:    return "beq";
            case Opcode::Bne:    return "bne";
            case Opcode::Blt:    return "blt";
            case Opcode::Bge:    return "bge";
            case Opcode::Bltu:   return "bltu";
            case Opcode::Bgeu:   return "bgeu";

            case Opcode::Lb:     return "lb";
            case Opcode::Lh:     return "lh";
            case Opcode::Lw:     return "lw";
            case Opcode::Lbu:    return "lbu";
            case Opcode::Lhu:    return "lhu";

            case Opcode::Sb:     return "sb";
            case Opcode::Sh:     return "sh";
            case Opcode::Sw:     return "sw";

            case Opcode::Addi:   return "addi";
            case Opcode::Slti:   return "slti";
            case Opcode::Sltiu:  return "sltiu";
            case Opcode::Xori:   return "xori";
            case Opcode::Ori:    return "ori";
            case Opcode::Andi:   return "andi";
            case Opcode::Slli:   return "slli";
            case Opcode::Srli:   return "srli";
            case Opcode::Srai:   return "srai";

            case Opcode::Add:    return "add";
            case Opcode::Sub:    return "sub";
            case Opcode::Sll:    return "sll";
            case Opcode::Slt:    return "slt";
            case Opcode::Sltu:   return "sltu";
            case Opcode::Xor:    return "xor";
            case Opcode::Srl:    return "srl";
            case Opcode::Sra:    return "sra";
            case Opcode::Or:     return "or";
            case Opcode::And:    return "and";

            case Opcode::Ecall:  return "ecall";
            case Opcode::Ebreak: return "ebreak";
            case Opcode::Fence:  return "fence";
        }

        std::unreachable();
    }
}

template<>
struct std::formatter<rv::Instruction> {

    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const rv::Instruction& inst, std::format_context& ctx) const {
        auto out = ctx.out();

        out = std::format_to(out, "{}", rv::mnemonic(inst.op));

        switch (inst.fmt) {
            case rv::Format::R:
                out = std::format_to(out, " x{} , x{} , x{}",
                                     rv::index(inst.rd),
                                     rv::index(inst.rs1),
                                     rv::index(inst.rs2));
                break;
            case rv::Format::I:
                out = std::format_to(out, " x{} , x{} , {}",
                                     rv::index(inst.rd),
                                     rv::index(inst.rs1),
                                     inst.imm);
                break;
            case rv::Format::S:
                out = std::format_to(out, " x{} , x{} , {}",
                                     rv::index(inst.rs1),
                                     rv::index(inst.rs2),
                                     inst.imm);
                break;
            case rv::Format::B:
                out = std::format_to(out, " x{} , x{} , {}",
                                     rv::index(inst.rs1),
                                     rv::index(inst.rs2),
                                     inst.imm);
                break;
            case rv::Format::U:
                out = std::format_to(out, " x{} , {}",
                                     rv::index(inst.rd),
                                     inst.imm);
                break;
            case rv::Format::J:
                out = std::format_to(out, " x{} , {}",
                                     rv::index(inst.rd),
                                     inst.imm);
                break;
        }

        return out;
    }
};