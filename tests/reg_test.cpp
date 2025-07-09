#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"
#include "../include/reg.hpp"

Registers reg{};

TEST_CASE("pc init") { REQUIRE(reg.pc == 0); }

TEST_CASE("register endianness check", "[reg]") {
  reg.b = 0xca;
  reg.c = 0xfe;
  REQUIRE(reg.bc == 0xcafe);
  reg.af = 0xdead;
  REQUIRE(reg.a == 0xde);
  REQUIRE(reg.f.z == 1);
  REQUIRE(reg.f.n == 0);
  REQUIRE(reg.f.h == 1);
  REQUIRE(reg.f.c == 0);
  REQUIRE(reg.f.ignore == 0xd);
}

TEST_CASE("set flags check", "[flags]") {
  reg.f.z = 1;
  reg.set_z(0x69);
  REQUIRE(reg.f.z == 0);
  reg.f.h = 0;
  reg.set_h(0xef);
  REQUIRE(reg.f.h == 1);
  reg.f.h = 0;
  reg.set_h(0x39, 0xa8);
  REQUIRE(reg.f.h == 1);
  reg.f.c = 0;
  reg.set_c(0xbe, 0xef);
  REQUIRE(reg.f.c == 1);
}
