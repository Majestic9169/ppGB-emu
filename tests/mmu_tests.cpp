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

TEST_CASE("write_word functioning", "[mmu, 16]") {
  mmu.write_word(0x1010, 0xbeef);
  REQUIRE(mmu.read_word(0x1010) == 0xbeef);
  REQUIRE(mmu.read_byte(0x1010) == 0xef);
  REQUIRE(mmu.read_byte(0x1011) == 0xbe);
}
