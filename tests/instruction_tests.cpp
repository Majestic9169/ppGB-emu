#include <cstdint>
#include <ios>
#include <iostream>
#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"
#include "../include/cpu.h"
#include "../include/display.h"
#include <fstream>

TEST_CASE("BOOT REGISTER TEST") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer, &gb);

  REQUIRE(gb.read_byte(0xFF50) == 0xFF);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::f) == 0xB0);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::a) == 0x01);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::c) == 0x13);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::b) == 0x00);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::e) == 0xD8);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::d) == 0x00);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::l) == 0x4D);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::h) == 0x01);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::sp) == 0xFFFE);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::pc) == 0x100);

  ROM.close();
}

TEST_CASE("ADD TESTS") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer, &gb);

  gb.write_reg(&CPU::REGISTERS::b, 0x02);
  gb.write_reg(&CPU::REGISTERS::a, 0x01);
  gb.ADD_A_r8(&CPU::REGISTERS::b);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::a) == 0x03);
  REQUIRE(gb.flag_value(Z) == 0);
  REQUIRE(gb.flag_value(N) == 0);
  REQUIRE(gb.flag_value(H) == 0);
  REQUIRE(gb.flag_value(C) == 0);

  gb.write_reg(&CPU::REGISTERS::b, 0x0a);
  gb.write_reg(&CPU::REGISTERS::a, 0x06);
  gb.ADD_A_r8(&CPU::REGISTERS::b);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::a) == 0x10);
  REQUIRE(gb.flag_value(Z) == 0);
  REQUIRE(gb.flag_value(N) == 0);
  REQUIRE(gb.flag_value(H) == 1);
  REQUIRE(gb.flag_value(C) == 0);

  gb.write_reg(&CPU::REGISTERS::b, 0xa0);
  gb.write_reg(&CPU::REGISTERS::a, 0x60);
  gb.ADD_A_r8(&CPU::REGISTERS::b);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::a) == 0x00);
  REQUIRE(gb.flag_value(Z) == 1);
  REQUIRE(gb.flag_value(N) == 0);
  REQUIRE(gb.flag_value(H) == 0);
  REQUIRE(gb.flag_value(C) == 1);

  gb.write_reg(&CPU::REGISTERS::hl, 0xf0f0);
  gb.write_reg(&CPU::REGISTERS::de, 0x001f);
  gb.ADD_HL_r16(&CPU::REGISTERS::de);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::hl) == 0xf10f);
  REQUIRE(gb.flag_value(N) == 0);
  REQUIRE(gb.flag_value(H) == 1);
  REQUIRE(gb.flag_value(C) == 0);

  gb.write_reg(&CPU::REGISTERS::hl, 0xf000);
  gb.write_reg(&CPU::REGISTERS::sp, 0x1000);
  gb.ADD_HL_SP();
  REQUIRE(gb.read_reg(&CPU::REGISTERS::hl) == 0x0000);
  REQUIRE(gb.flag_value(N) == 0);
  REQUIRE(gb.flag_value(H) == 0);
  REQUIRE(gb.flag_value(C) == 1);

  ROM.close();
}

TEST_CASE("XOR TESTS") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer, &gb);

  gb.write_reg(&CPU::REGISTERS::b, 0xf1);
  gb.write_reg(&CPU::REGISTERS::a, 0xf1);
  gb.XOR_A_r8(&CPU::REGISTERS::b);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::a) == 0x00);
  REQUIRE(gb.flag_value(Z) == 1);
  REQUIRE(gb.flag_value(N) == 0);
  REQUIRE(gb.flag_value(H) == 0);
  REQUIRE(gb.flag_value(C) == 0);

  ROM.close();
}

TEST_CASE("DEC TESTS") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer, &gb);

  gb.write_reg(&CPU::REGISTERS::b, 0xf1);
  gb.DEC_r8(&CPU::REGISTERS::b);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::b) == 0xf0);
  REQUIRE(gb.flag_value(Z) == 0);
  REQUIRE(gb.flag_value(N) == 1);
  REQUIRE(gb.flag_value(H) == 0);

  gb.write_reg(&CPU::REGISTERS::b, 0xf0);
  gb.DEC_r8(&CPU::REGISTERS::b);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::b) == 0xef);
  REQUIRE(gb.flag_value(Z) == 0);
  REQUIRE(gb.flag_value(N) == 1);
  REQUIRE(gb.flag_value(H) == 1);

  ROM.close();
}

TEST_CASE("CP TESTS") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer, &gb);

  gb.write_reg(&CPU::REGISTERS::a, 0x94);
  gb.CP_A_n8(0x94);
  REQUIRE(gb.flag_value(Z) == 1);
  REQUIRE(gb.flag_value(N) == 1);
  REQUIRE(gb.flag_value(H) == 0);
  REQUIRE(gb.flag_value(C) == 0);

  gb.write_reg(&CPU::REGISTERS::a, 0x84);
  gb.CP_A_n8(0x94);
  REQUIRE(gb.flag_value(Z) == 0);
  REQUIRE(gb.flag_value(N) == 1);
  REQUIRE(gb.flag_value(H) == 0);
  REQUIRE(gb.flag_value(C) == 1);

  gb.write_reg(&CPU::REGISTERS::a, 0x93);
  gb.CP_A_n8(0x94);
  REQUIRE(gb.flag_value(Z) == 0);
  REQUIRE(gb.flag_value(N) == 1);
  REQUIRE(gb.flag_value(H) == 1);
  REQUIRE(gb.flag_value(C) == 1);

  ROM.close();
}

TEST_CASE("LOAD TESTS") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer, &gb);

  SECTION("LD r8, r8") {
    gb.write_reg(&CPU::REGISTERS::b, 0x42);
    gb.write_reg(&CPU::REGISTERS::c, 0x24);
    gb.LD_r8_r8(&CPU::REGISTERS::b, &CPU::REGISTERS::c);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::b) == 0x24);
  }

  SECTION("LD r8, n8") {
    gb.LD_r8_n8(&CPU::REGISTERS::a, 0x42);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::a) == 0x42);
  }

  SECTION("LD [HL], r8") {
    gb.write_reg(&CPU::REGISTERS::hl, 0xC000);
    gb.write_reg(&CPU::REGISTERS::b, 0x42);
    gb.LD_HL_r8(&CPU::REGISTERS::b);
    REQUIRE(gb.read_byte(0xC000) == 0x42);
  }

  SECTION("LD [HL], n8") {
    gb.write_reg(&CPU::REGISTERS::hl, 0xC000);
    gb.LD_HL_n8(0x42);
    REQUIRE(gb.read_byte(0xC000) == 0x42);
  }

  ROM.close();
}

TEST_CASE("ADC TESTS") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer, &gb);

  SECTION("ADC A, r8 without carry") {
    gb.flag_value(C, 0);
    gb.write_reg(&CPU::REGISTERS::a, 0x42);
    gb.write_reg(&CPU::REGISTERS::b, 0x13);
    gb.ADC_A_r8(&CPU::REGISTERS::b);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::a) == 0x55);
    REQUIRE(gb.flag_value(Z) == 0);
    REQUIRE(gb.flag_value(N) == 0);
    REQUIRE(gb.flag_value(H) == 0);
    REQUIRE(gb.flag_value(C) == 0);
  }

  SECTION("ADC A, r8 with carry") {
    gb.write_reg(&CPU::REGISTERS::a, 0x42);
    gb.write_reg(&CPU::REGISTERS::b, 0x13);
    gb.flag_value(C, 1);
    gb.ADC_A_r8(&CPU::REGISTERS::b);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::a) == 0x56);
    REQUIRE(gb.flag_value(Z) == 0);
    REQUIRE(gb.flag_value(N) == 0);
    REQUIRE(gb.flag_value(H) == 0);
    REQUIRE(gb.flag_value(C) == 0);
  }

  ROM.close();
}

TEST_CASE("BITWISE TESTS") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer, &gb);

  SECTION("AND A, r8") {
    gb.write_reg(&CPU::REGISTERS::a, 0xF0);
    gb.write_reg(&CPU::REGISTERS::b, 0x0F);
    gb.AND_A_r8(&CPU::REGISTERS::b);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::a) == 0x00);
    REQUIRE(gb.flag_value(Z) == 1);
    REQUIRE(gb.flag_value(N) == 0);
    REQUIRE(gb.flag_value(H) == 1);
    REQUIRE(gb.flag_value(C) == 0);
  }

  SECTION("OR A, r8") {
    gb.write_reg(&CPU::REGISTERS::a, 0xF0);
    gb.write_reg(&CPU::REGISTERS::b, 0x0F);
    gb.OR_A_r8(&CPU::REGISTERS::b);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::a) == 0xFF);
    REQUIRE(gb.flag_value(Z) == 0);
    REQUIRE(gb.flag_value(N) == 0);
    REQUIRE(gb.flag_value(H) == 0);
    REQUIRE(gb.flag_value(C) == 0);
  }

  ROM.close();
}

TEST_CASE("BIT MANIPULATION TESTS") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer, &gb);

  SECTION("BIT u3, r8") {
    gb.write_reg(&CPU::REGISTERS::b, 0x80);
    gb.BIT_u3_r8(7, &CPU::REGISTERS::b);
    REQUIRE(gb.flag_value(Z) == 0);
    REQUIRE(gb.flag_value(N) == 0);
    REQUIRE(gb.flag_value(H) == 1);

    gb.BIT_u3_r8(0, &CPU::REGISTERS::b);
    REQUIRE(gb.flag_value(Z) == 1);
  }

  SECTION("SET u3, r8") {
    gb.write_reg(&CPU::REGISTERS::b, 0x00);
    gb.SET_u3_r8(7, &CPU::REGISTERS::b);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::b) == 0x80);
  }

  SECTION("RES u3, r8") {
    gb.write_reg(&CPU::REGISTERS::b, 0xFF);
    gb.RES_u3_r8(7, &CPU::REGISTERS::b);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::b) == 0x7F);
  }

  ROM.close();
}

TEST_CASE("ROTATE AND SHIFT TESTS") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer, &gb);

  SECTION("RL r8") {
    gb.write_reg(&CPU::REGISTERS::b, 0x80);
    gb.flag_value(C, 1);
    gb.RL_r8(&CPU::REGISTERS::b);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::b) == 0x01);
    REQUIRE(gb.flag_value(C) == 1);
    REQUIRE(gb.flag_value(Z) == 0);
  }

  SECTION("RR r8") {
    gb.write_reg(&CPU::REGISTERS::b, 0x01);
    gb.flag_value(C, 1);
    gb.RR_r8(&CPU::REGISTERS::b);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::b) == 0x80);
    REQUIRE(gb.flag_value(C) == 1);
    REQUIRE(gb.flag_value(Z) == 0);
  }

  SECTION("SLA r8") {
    gb.write_reg(&CPU::REGISTERS::b, 0x80);
    gb.SLA_r8(&CPU::REGISTERS::b);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::b) == 0x00);
    REQUIRE(gb.flag_value(C) == 1);
    REQUIRE(gb.flag_value(Z) == 1);
  }

  SECTION("SRA r8") {
    gb.write_reg(&CPU::REGISTERS::b, 0x81);
    gb.SRA_r8(&CPU::REGISTERS::b);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::b) == 0xC0);
    REQUIRE(gb.flag_value(C) == 1);
    REQUIRE(gb.flag_value(Z) == 0);
  }

  ROM.close();
}

TEST_CASE("JUMP AND CALL TESTS") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer, &gb);

  SECTION("JP n16") {
    gb.JP_n16(0x1234);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::pc) == 0x1234);
  }

  SECTION("JP CC, n16") {
    gb.flag_value(Z, 1);
    gb.JP_CC_n16(true, 0x1234);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::pc) == 0x1234);

    gb.JP_CC_n16(false, 0x5678);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::pc) == 0x1234);
  }

  SECTION("CALL n16") {
    uint16_t old_sp = gb.read_reg(&CPU::REGISTERS::sp);
    gb.write_reg(&CPU::REGISTERS::pc, 0x1000);
    gb.CALL_n16(0x1234);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::pc) == 0x1234);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::sp) == old_sp - 2);
    gb.RET();
    REQUIRE(gb.read_reg(&CPU::REGISTERS::pc) == 0x1000);
  }

  ROM.close();
}

TEST_CASE("16-bit Tests") {
  std::ifstream ROM;
  ROM.open("../roms/tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);
  Display display(gb.framebuffer, &gb);

  SECTION("16-bit Load Tests", "[load]") {
    // Write test instruction to memory
    gb.write_byte(0x100, 0x01); // LD BC, 0x1234
    gb.write_byte(0x101, 0x34);
    gb.write_byte(0x102, 0x12);
    gb.write_reg(&CPU::REGISTERS::pc, 0x100);
    gb.INSTRUCTION_DECODER();

    REQUIRE(gb.read_reg(&CPU::REGISTERS::b) == 0x12);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::c) == 0x34);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::bc) == 0x1234);

    ROM.close();
  }

  SECTION("PUSH/POP Tests", "[stack]") {
    std::ifstream ROM;
    ROM.open("../roms/tetris.gb", std::ios::binary | std::ios::in);
    CPU gb(ROM);
    Display display(gb.framebuffer, &gb);

    // Write test instruction to memory
    gb.write_byte(0x100, 0xC5); // PUSH BC
    gb.write_reg(&CPU::REGISTERS::pc, 0x100);
    gb.write_reg(&CPU::REGISTERS::b, 0x12);
    gb.write_reg(&CPU::REGISTERS::c, 0x34);
    uint16_t old_sp = gb.read_reg(&CPU::REGISTERS::sp);
    gb.INSTRUCTION_DECODER();

    REQUIRE(gb.read_reg(&CPU::REGISTERS::bc) == 0x1234);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::sp) == old_sp - 2);
    uint16_t sp = gb.read_reg(&CPU::REGISTERS::sp);
    REQUIRE(gb.read_byte(sp) == 0x34);
    REQUIRE(gb.read_byte(sp + 1) == 0x12);

    ROM.close();
  }

  SECTION("POP BC") {
    std::ifstream ROM;
    ROM.open("../roms/tetris.gb", std::ios::binary | std::ios::in);
    CPU gb(ROM);
    Display display(gb.framebuffer, &gb);

    // Set up initial stack pointer and push test value
    gb.write_reg(&CPU::REGISTERS::sp, 0xFFFE);
    gb.write_reg(&CPU::REGISTERS::bc, 0x1234);
    gb.PUSH_r16(&CPU::REGISTERS::bc);
    gb.write_reg(&CPU::REGISTERS::bc, 0x6969);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::bc) == 0x6969);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::sp) == 0xFFFC);

    // Test POP BC
    gb.POP_r16(&CPU::REGISTERS::bc);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::bc) == 0x1234);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::sp) == 0xFFFE);
  }

  SECTION("POP DE") {
    gb.write_reg(&CPU::REGISTERS::sp, 0xFFFE);
    gb.write_reg(&CPU::REGISTERS::de, 0xFFFF);

    gb.PUSH_r16(&CPU::REGISTERS::de);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::sp) == 0xFFFC);
    REQUIRE(gb.read_byte(0xFFFC) == 0xFF);
    REQUIRE(gb.read_byte(0xFFFD) == 0xFF);

    gb.write_reg(&CPU::REGISTERS::de, 0x0000);
    gb.POP_r16(&CPU::REGISTERS::de);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::de) == 0xFFFF);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::sp) == 0xFFFE);
  }

  SECTION("POP HL") {
    gb.write_reg(&CPU::REGISTERS::sp, 0xFFFE);
    gb.write_reg(&CPU::REGISTERS::hl, 0x5AF0);

    gb.PUSH_r16(&CPU::REGISTERS::hl);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::sp) == 0xFFFC);
    REQUIRE(gb.read_byte(0xFFFC) == 0xF0);
    REQUIRE(gb.read_byte(0xFFFD) == 0x5A);

    gb.write_reg(&CPU::REGISTERS::hl, 0x0000);
    gb.POP_r16(&CPU::REGISTERS::hl);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::hl) == 0x5AF0);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::sp) == 0xFFFE);

    ROM.close();
  }

  SECTION("POP AF with flag bits") {
    std::ifstream ROM;
    ROM.open("../roms/tetris.gb", std::ios::binary | std::ios::in);
    CPU gb(ROM);
    Display display(gb.framebuffer, &gb);

    // Set up stack with value that will test flag bits
    gb.write_reg(&CPU::REGISTERS::sp, 0xFFF0);
    gb.write_reg(&CPU::REGISTERS::a, 0x5A);
    // gb.reg.f = 0xF0;
    gb.write_reg(&CPU::REGISTERS::f, 0xF0);
    gb.PUSH_AF();
    // gb.write_byte(0xFFEF, 0xF0);
    // TODO: wtf is this error how tf do i fix it
    REQUIRE(gb.read_byte(0xFFEF) == 0x5A);
    REQUIRE(gb.read_byte(0xFFEE) == 0xF0);
    gb.write_reg(&CPU::REGISTERS::af, 0x6969);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::af) == 0x6969);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::sp) == 0xFFEE);

    gb.POP_AF();
    REQUIRE(gb.read_reg(&CPU::REGISTERS::a) == 0x5A);
    REQUIRE(gb.read_reg(&CPU::REGISTERS::f) == 0xF0);
    REQUIRE(gb.flag_value(Z) == 1);
    REQUIRE(gb.flag_value(N) == 1);
    REQUIRE(gb.flag_value(H) == 1);
    REQUIRE(gb.flag_value(C) == 1);
  }

  ROM.close();
}
