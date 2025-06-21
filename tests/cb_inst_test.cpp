#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"
#include "../include/cb_opcodes.hpp"
#include "../include/cli_opts.hpp"

int argc = 2;
char *argv[2] = {strdup("./ppGB"), strdup("../roms/tetris.gb")};
Opts cli_opts{argc, argv};
MMU mmu{&cli_opts};
Registers reg{};

CB_Opcodes cb{&mmu, &reg};

TEST_CASE("RLC tests", "[flags, rlc]") {
  reg.af = 0xfff0;
  reg.b = 0x40;
  cb.cb_opcode_00();
  REQUIRE(reg.b == 0x80);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  reg.a = 0x80;
  cb.cb_opcode_07();
  REQUIRE(reg.a == 0x01);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 1);
  reg.hl = 0x8080;
  mmu.write_byte(reg.hl, 0x03);
  cb.cb_opcode_06();
  REQUIRE(mmu.read_byte(reg.hl) == 0x06);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  reg.d = 0x0;
  cb.cb_opcode_02();
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
}

TEST_CASE("RRC tests", "[flags, rrc]") {
  reg.af = 0xfff0;
  reg.b = 0x40;
  cb.cb_opcode_08();
  REQUIRE(reg.b == 0x20);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  reg.a = 0x80;
  cb.cb_opcode_0f();
  REQUIRE(reg.a == 0x40);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  reg.hl = 0x8080;
  mmu.write_byte(reg.hl, 0x03);
  cb.cb_opcode_0e();
  REQUIRE(mmu.read_byte(reg.hl) == 0x81);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 1);
  reg.d = 0x0;
  cb.cb_opcode_0a();
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
}

TEST_CASE("RL tests", "[flags, rl]") {
  reg.af = 0xfff0;
  reg.b = 0x40;
  cb.cb_opcode_10();
  REQUIRE(reg.b == 0x81);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  reg.a = 0x80;
  cb.cb_opcode_17();
  REQUIRE(reg.a == 0x00);
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 1);
  reg.hl = 0x8080;
  mmu.write_byte(reg.hl, 0x03);
  cb.cb_opcode_16();
  REQUIRE(mmu.read_byte(reg.hl) == 0x07);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  reg.d = 0x0;
  cb.cb_opcode_12();
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
}
