#pragma once

#include "../include/cpu/cb_opcodes.hpp"
#include "../include/cpu/opcodes.hpp"
#include "../include/cpu/reg.hpp"
#include "../include/ppu/ppu.hpp"

extern int argc;
extern char *argv[2];
extern Opts cli_opts;
extern MMU mmu;
extern Opcodes op;
extern PPU ppu;
extern Registers reg;
extern CB_Opcodes cb;
