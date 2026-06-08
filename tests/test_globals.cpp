#include "test_globals.hpp"
#include <catch2/catch_test_macros.hpp>

int argc = 2;
char *argv[2] = {strdup("./tests"), strdup("./roms/tetris.gb")};
Opts cli_opts{argc, argv};
MMU mmu{&cli_opts};
Registers reg{};
Opcodes op{&mmu, &reg};
CB_Opcodes cb{&mmu, &reg};
PPU ppu{&cli_opts, &mmu};
