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

TEST_CASE("inc tests with flag", "[flags]") {
  reg.af = 0xfff0;
  reg.b = 0xff;
  op.opcode_04();
  REQUIRE(reg.b == 0);
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.h == 1);
  REQUIRE(reg.f.n == 0);
}

TEST_CASE("inc test without flag", "[flags]") {
  reg.af = 0xfff0;
  reg.b = 0xbf;
  op.opcode_04();
  REQUIRE(reg.b == 0xc0);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 1);
  REQUIRE(reg.f.n == 0);
}

TEST_CASE("inc 16 test with flag", "[flags]") {
  reg.af = 0xfff0;
  reg.hl = 0x2010;
  mmu.write_byte(reg.hl, 0xff);
  op.opcode_34();
  REQUIRE(mmu.read_byte(reg.hl) == 0x00);
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.h == 1);
  REQUIRE(reg.f.n == 0);
}

TEST_CASE("inc 16 test without flag", "[flags]") {
  reg.af = 0xfff0;
  reg.hl = 0x2010;
  mmu.write_byte(reg.hl, 0xde);
  op.opcode_34();
  REQUIRE(mmu.read_byte(reg.hl) == 0xdf);
  REQUIRE(reg.f.z == 0);
  REQUIRE(reg.f.h == 0);
  REQUIRE(reg.f.n == 0);
}
