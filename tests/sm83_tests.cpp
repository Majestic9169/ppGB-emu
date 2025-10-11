#include "test_globals.hpp"
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <ios>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

TEST_CASE("sm83 test suite", "[sm83, cpu]") {
  for (int i = 0; i <= 0xff; i++) {

    if (i == 0xcb)
      continue;
    char file_name[8];
    sprintf(file_name, "%02x.json", i);
    std::string file_id{file_name};
    file_id = "./sm83/v1/" + file_id;
    std::ifstream f(file_id);
    if (!f.is_open())
      continue;

    DYNAMIC_SECTION("Test " << std::hex << i << std::dec) {
      json data = json::parse(f);
      for (auto test : data) {
        // load registers
        reg.pc = test["initial"]["pc"];
        reg.sp = test["initial"]["sp"];
        reg.a = test["initial"]["a"];
        reg.b = test["initial"]["b"];
        reg.c = test["initial"]["c"];
        reg.d = test["initial"]["d"];
        reg.e = test["initial"]["e"];
        reg.set_f(test["initial"]["f"]);
        reg.h = test["initial"]["h"];
        reg.l = test["initial"]["l"];

        // load memory
        for (auto e : test["initial"]["ram"]) {
          mmu.write_byte(e[0], e[1]);
        }

        // execute opcode
        op.test();

        // assert against final
        INFO(test["name"]);
        CAPTURE(static_cast<int>(reg.a), static_cast<int>(reg.get_f()),
                static_cast<int>(reg.b), static_cast<int>(reg.c),
                static_cast<int>(reg.d), static_cast<int>(reg.e),
                static_cast<int>(reg.h), static_cast<int>(reg.l), reg.pc,
                reg.sp);
        REQUIRE(reg.a == test["final"]["a"]);
        REQUIRE(reg.b == test["final"]["b"]);
        REQUIRE(reg.c == test["final"]["c"]);
        REQUIRE(reg.d == test["final"]["d"]);
        REQUIRE(reg.e == test["final"]["e"]);
        REQUIRE(reg.get_f() == test["final"]["f"]);
        REQUIRE(reg.h == test["final"]["h"]);
        REQUIRE(reg.l == test["final"]["l"]);
        REQUIRE(reg.pc == test["final"]["pc"]);
        REQUIRE(reg.sp == test["final"]["sp"]);
        for (auto e : test["final"]["ram"]) {
          INFO("RAM check addr=" << e[0] << ", expected=" << e[1]
                                 << ", actual = "
                                 << static_cast<int>(mmu.read_byte(e[0])));
          REQUIRE(mmu.read_byte(e[0]) == e[1]);
        }
      }
    }
  }
}
