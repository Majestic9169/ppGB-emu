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
  reg.af = 0x81f0;
  op.opcode_0f();
  REQUIRE(reg.af == 0xc010);
  reg.af = 0x0210;
  op.opcode_1f();
  REQUIRE(reg.af == 0x8100);
}

TEST_CASE("jr tests", "[flags, jr]") {
  reg.pc = 0x104;
  mmu.write_byte(reg.pc, 0xff); // -1
  op.opcode_18();
  REQUIRE(reg.pc == 0x104);
  reg.f.z = 0;
  op.opcode_28();
  REQUIRE(reg.pc == 0x105);
  reg.pc = 0x104;
  reg.f.n = 1;
  reg.f.c = 1;
  op.opcode_30();
  REQUIRE(reg.pc == 0x104);
}

TEST_CASE("add tests", "[flags, add]") {
  reg.af = 0xfff0;
  reg.hl = 0x0069;
  reg.sp = 0x0420;
  op.opcode_39();
  REQUIRE(reg.hl == 0x489);
  REQUIRE(reg.af == 0xff80);
  reg.hl = 0x00f9;
  reg.de = 0x202c;
  op.opcode_19();
  REQUIRE(reg.hl == 0x2125);
  REQUIRE(reg.af == 0xffa0);
  reg.hl = 0x8fff;
  op.opcode_29();
  REQUIRE(reg.hl == 0x1ffe);
  REQUIRE(reg.af == 0xffb0);
  reg.af = 0x6990;
  reg.b = 0x67;
  op.opcode_80();
  REQUIRE(reg.af == 0xd020);
  reg.af = 0x6990;
  reg.h = 0x1f;
  op.opcode_8c();
  REQUIRE(reg.af == 0x8920);
}

TEST_CASE("misc tests", "[flags, daa, ccf, cpl, scf]") {
  // DAA
  // 0x54 + 0x70 = 124
  reg.a = 0x54;
  reg.c = 0x70;
  op.opcode_81();
  op.opcode_27();
  REQUIRE(reg.af == 0x2410);
  // 0x42 + 0x35 = 77
  reg.a = 0x42;
  reg.d = 0x35;
  op.opcode_82();
  op.opcode_27();
  REQUIRE(reg.af == 0x7700);
  // 0x42 + 0x29 = 71
  reg.a = 0x42;
  reg.e = 0x29;
  op.opcode_83();
  op.opcode_27();
  REQUIRE(reg.af == 0x7100);
  op.opcode_37();
  REQUIRE(reg.f.c == 1);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.h == 0);
  reg.a = 0x0f;
  op.opcode_2f();
  REQUIRE(reg.a == 0xf0);
  REQUIRE(reg.f.n == 1);
  REQUIRE(reg.f.h == 1);
  reg.f.c = 0;
  op.opcode_3f();
  REQUIRE(reg.f.c == 1);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.h == 0);
}
