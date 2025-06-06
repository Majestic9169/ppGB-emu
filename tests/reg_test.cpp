#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"
#include "../include/reg.hpp"

TEST_CASE("register endianness check", "[reg]") {
  Registers reg{};
  reg.b = 0xca;
  reg.c = 0xfe;
  REQUIRE(reg.bc == 0xcafe);
}
