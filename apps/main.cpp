#include "rv/frontend/decoder.hpp"
#include <cstdio>

using namespace rv;

static const char *op_name(Opcode o) {
    switch (o) {
        case Opcode::Lui: return "lui";
        case Opcode::Auipc: return "auipc";
        case Opcode::Jal: return "jal";
        case Opcode::Jalr: return "jalr";
        case Opcode::Beq: return "beq";
        case Opcode::Bne: return "bne";
        case Opcode::Blt: return "blt";
        case Opcode::Bge: return "bge";
        case Opcode::Bltu: return "bltu";
        case Opcode::Bgeu: return "bgeu";
        case Opcode::Lb: return "lb";
        case Opcode::Lh: return "lh";
        case Opcode::Lw: return "lw";
        case Opcode::Lbu: return "lbu";
        case Opcode::Lhu: return "lhu";
        case Opcode::Sb: return "sb";
        case Opcode::Sh: return "sh";
        case Opcode::Sw: return "sw";
        case Opcode::Addi: return "addi";
        case Opcode::Slti: return "slti";
        case Opcode::Sltiu: return "sltiu";
        case Opcode::Xori: return "xori";
        case Opcode::Ori: return "ori";
        case Opcode::Andi: return "andi";
        case Opcode::Slli: return "slli";
        case Opcode::Srli: return "srli";
        case Opcode::Srai: return "srai";
        case Opcode::Add: return "add";
        case Opcode::Sub: return "sub";
        case Opcode::Sll: return "sll";
        case Opcode::Slt: return "slt";
        case Opcode::Sltu: return "sltu";
        case Opcode::Xor: return "xor";
        case Opcode::Srl: return "srl";
        case Opcode::Sra: return "sra";
        case Opcode::Or: return "or";
        case Opcode::And: return "and";
        case Opcode::Ecall: return "ecall";
        case Opcode::Ebreak: return "ebreak";
        case Opcode::Fence: return "fence";
        default: return "INVALID";
    }
}


int main() {
    const uint32_t cases[] = {
        0x00100093u,
        0x002081b3u,
        0x40208133u,
        0x00832283u,
        0x00532623u,
        0x00208463u,
        0x12345537u,
        0x010000efu,
        0x00311113u,
        0x40315113u,
        0x00000073u,
        0x00100073u,
        0xffffffffu,
    };

    for (const auto &c : cases) {
        auto r = Decoder::decode(c);
        std::printf("0x%08x -> ", c);
        if (!r) { std::printf("DecodeError\n"); continue; }
        const auto &i = *r;
        std::printf("%-6s rd=x%-2u rs1=x%-2u rs2=x%-2u imm=%d\n",
                    op_name(i.op), index(i.rd), index(i.rs1), index(i.rs2), i.imm);
    }
    return 0;

    }
