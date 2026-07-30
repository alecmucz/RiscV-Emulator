#pragma once
#include "rv/common/types.hpp"

namespace rv {

enum class Format { R, I, S, B, U, J };

enum class Opcode {
    Invalid,
    Lui, Auipc, Jal, Jalr,
    Beq, Bne, Blt, Bge, Bltu, Bgeu,
    Lb, Lh, Lw, Lbu, Lhu, Sb, Sh, Sw,
    Addi, Slti, Sltiu, Xori, Ori, Andi, Slli, Srli, Srai,
    Add, Sub, Sll, Slt, Sltu, Xor, Srl, Sra, Or, And,
    Ecall, Ebreak, Fence,
};

struct Instruction {
    Opcode op  = Opcode::Invalid;
    Format fmt{};
    Reg    rd{};
    Reg    rs1{};
    Reg    rs2{};
    SWord  imm{0}; // CRS addresses occupy I-Immediate Fields (RiscV Spec)
    Word   raw{0};
};

}  // namespace rv
