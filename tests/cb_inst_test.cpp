#include "test_globals.hpp"
#include <catch2/catch_test_macros.hpp>

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

TEST_CASE("SLA tests", "[flags, sla]") {
  reg.af = 0xfff0;
  reg.b = 0x40;
  cb.cb_opcode_20();
  REQUIRE(reg.b == 0x80);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  reg.c = 0xff;
  cb.cb_opcode_21();
  REQUIRE(reg.c == 0xfe);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 1);
  reg.hl = 0x8080;
  mmu.write_byte(reg.hl, 0x80);
  cb.cb_opcode_26();
  REQUIRE(mmu.read_byte(reg.hl) == 0x00);
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 1);
}

TEST_CASE("SRA tests", "[flags, sra]") {
  reg.af = 0xfff0;
  reg.b = 0x40;
  cb.cb_opcode_28();
  REQUIRE(reg.b == 0x20);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  reg.c = 0xff;
  cb.cb_opcode_29();
  REQUIRE(reg.c == 0xff);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 1);
  reg.hl = 0x8080;
  mmu.write_byte(reg.hl, 0x80);
  cb.cb_opcode_2e();
  REQUIRE(mmu.read_byte(reg.hl) == 0xc0);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
}

TEST_CASE("SWAP tests", "[flags, swap]") {
  reg.af = 0xfff0;
  reg.b = 0x40;
  cb.cb_opcode_30();
  REQUIRE(reg.b == 0x04);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  reg.c = 0xff;
  cb.cb_opcode_31();
  REQUIRE(reg.c == 0xff);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  reg.hl = 0x8080;
  mmu.write_byte(reg.hl, 0xfe);
  cb.cb_opcode_36();
  REQUIRE(mmu.read_byte(reg.hl) == 0xef);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  reg.d = 0xf1;
  cb.cb_opcode_32();
  REQUIRE(reg.d == 0x1f);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  reg.h = 0x00;
  cb.cb_opcode_34();
  REQUIRE(reg.h == 0x00);
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.c == 0);
  REQUIRE(reg.hl == 0x0080);
}
