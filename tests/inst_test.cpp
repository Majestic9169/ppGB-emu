#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"
#include "../include/cli_opts.hpp"
#include "../include/opcodes.hpp"

int argc = 2;
char *argv[2] = {strdup("./ppGB"),
                 strdup("../../gb-test-roms/cpu_instrs/cpu_instrs.gb")};
Opts cli_opts{argc, argv};
MMU mmu{&cli_opts};
Registers reg{};

Opcodes op{&mmu, &reg};

TEST_CASE("inc tests with flag", "[flags, inc]") {
  reg.af = 0xfff0;
  reg.b = 0xff;
  op.opcode_04();
  REQUIRE(reg.b == 0);
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.h == 1);
  REQUIRE(reg.f.n == 0);
}

TEST_CASE("inc test without flag", "[flags, inc]") {
  reg.af = 0xfff0;
  reg.b = 0xbf;
  op.opcode_04();
  REQUIRE(reg.b == 0xc0);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 1);
  REQUIRE(reg.f.n == 0);
}

TEST_CASE("inc 16 test with flag", "[flags, inc, 16]") {
  reg.af = 0xfff0;
  reg.hl = 0x2010;
  mmu.write_byte(reg.hl, 0xff);
  op.opcode_34();
  REQUIRE(mmu.read_byte(reg.hl) == 0x00);
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.h == 1);
  REQUIRE(reg.f.n == 0);
}

TEST_CASE("inc 16 test without flag", "[flags, inc, 16]") {
  reg.af = 0xfff0;
  reg.hl = 0x2010;
  mmu.write_byte(reg.hl, 0xde);
  op.opcode_34();
  REQUIRE(mmu.read_byte(reg.hl) == 0xdf);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
}

TEST_CASE("dec with flag", "[flags, dec]") {
  reg.af = 0xfff0;
  reg.b = 0;
  op.opcode_05();
  REQUIRE(reg.b == 0xff);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.n == 1);
  REQUIRE(reg.f.h == 1);
}

TEST_CASE("dec without flag", "[flags, dec]") {
  reg.af = 0xff00;
  reg.b = 1;
  op.opcode_05();
  REQUIRE(reg.b == 0x00);
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.n == 1);
  REQUIRE(reg.f.h == 0);
}

TEST_CASE("dec 16 with flag", "[flags, dec, 16]") {
  reg.af = 0xff00;
  reg.hl = 0x4230;
  mmu.write_byte(reg.hl, 0x00);
  op.opcode_35();
  REQUIRE(mmu.read_byte(reg.hl) == 0xff);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.n == 1);
  REQUIRE(reg.f.h == 1);
}

TEST_CASE("dec 16 without flag", "[flags, dec, 16]") {
  reg.af = 0xff00;
  reg.hl = 0x4230;
  mmu.write_byte(reg.hl, 0x01);
  op.opcode_35();
  REQUIRE(mmu.read_byte(reg.hl) == 0x00);
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.n == 1);
  REQUIRE(reg.f.h == 0);
}

TEST_CASE("16 bit loads", "[flags, ld, 16]") {
  mmu.write_byte(0x102, 0x01);
  mmu.write_byte(0x103, 0x10);
  reg.pc = 0x102;
  reg.de = 0;
  reg.af = 0xcaf0;
  op.opcode_11();
  REQUIRE(reg.de == 0x1001);
  REQUIRE(reg.af == 0xcaf0);
  mmu.write_byte(0x102, 0xfe);
  mmu.write_byte(0x103, 0xca);
  reg.pc = 0x102;
  reg.sp = 0x1020;
  reg.af = 0xdea0;
  op.opcode_31();
  REQUIRE(reg.sp == 0xcafe);
  REQUIRE(reg.af == 0xdea0);
}

TEST_CASE("8 bit loads", "[flags, ld]") {
  reg.a = 0x69;
  reg.hl = 0x1070;
  mmu.write_byte(reg.hl, 0x00);
  op.opcode_32();
  REQUIRE(mmu.read_byte(reg.hl + 1) == 0x69);
  REQUIRE(reg.hl == 0x106f);
}

TEST_CASE("rotate tests", "[flags, rlca, rla]") {
  reg.af = 0xc0f0;
  op.opcode_07();
  REQUIRE(reg.af == 0x8110);
  reg.af = 0x81f0;
  op.opcode_17();
  REQUIRE(reg.af == 0x0310);
}
