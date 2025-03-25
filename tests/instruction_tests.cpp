#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"
#include "../include/cpu.h"
#include "../include/display.h"
#include <fstream>

TEST_CASE("BOOT REGISTER TEST") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer);

  REQUIRE(gb.read_byte(0xFF50) == 0xFF);

  gb.write_reg(&CPU::REGISTERS::b, 0x02);
  gb.write_reg(&CPU::REGISTERS::a, 0x01);
  gb.ADD_A_r8(&CPU::REGISTERS::b);
  REQUIRE(gb.read_reg(&CPU::REGISTERS::a) == 0x03);

  ROM.close();
}

TEST_CASE("ADD TESTS") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer);

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

  Display display(gb.framebuffer);

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

  Display display(gb.framebuffer);

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

  Display display(gb.framebuffer);

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
