#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"
#include "../include/reg.hpp"

TEST_CASE("register endianness check", "[reg]") {
  Registers reg{};
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
