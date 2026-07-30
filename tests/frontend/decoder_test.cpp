#include "rv/frontend/decoder.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("ECALL", "[decoder]") {
    constexpr rv::Word raw{0x00000073};
    CAPTURE(raw);

    const auto result = rv::Decoder::decode(raw);
    REQUIRE(result.has_value());

    CHECK(result->op == rv::Opcode::Ecall);
    CHECK(result->fmt == rv::Format::I);
    CHECK(result->rd == rv::reg(0));
    CHECK(result->rs1 == rv::reg(0));
    CHECK(result->rs2 == rv::reg(0));
    CHECK(result->imm == 0);
    CHECK(result->raw == raw);
}

TEST_CASE("EBREAK", "[decoder]") {
    constexpr rv::Word raw{0x00100073};
    CAPTURE(raw);

    const auto result = rv::Decoder::decode(raw);
    REQUIRE(result.has_value());

    CHECK(result->op == rv::Opcode::Ebreak);
    CHECK(result->fmt == rv::Format::I);
    CHECK(result->rd == rv::reg(0));
    CHECK(result->rs1 == rv::reg(0));
    CHECK(result->rs2 == rv::reg(0));
    CHECK(result->imm == 0);
    CHECK(result->raw == raw);
}

TEST_CASE("SYSTEM INSTRUCTION LENIENCY/BOUNDARY CHECKS", "[decoder]") {
    SECTION("MRET Instruction") {
        constexpr rv::Word raw{0x30200073};
        CAPTURE(raw);

        const auto result = rv::Decoder::decode(raw);
        REQUIRE(!result.has_value());

        CHECK(result.error() == rv::DecodeError::IllegalInstruction);
    }
    SECTION("ECALL/EBREAK Boundary Check") {
        constexpr rv::Word raw{0x00200073};
        CAPTURE(raw);

        const auto result = rv::Decoder::decode(raw);
        REQUIRE(!result.has_value());

        CHECK(result.error() == rv::DecodeError::IllegalInstruction);
    }
    SECTION("CSR Instructions") {
        constexpr rv::Word raw{0x00002073};
        CAPTURE(raw);

        const auto result = rv::Decoder::decode(raw);
        REQUIRE(!result.has_value());
        CHECK(result.error() == rv::DecodeError::IllegalInstruction);
    }

}

TEST_CASE("RV32I Instruction Suite Test", "[decoder]") {
    
}