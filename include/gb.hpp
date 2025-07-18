/*
 * Gameboy
 * - contain all components and manage their construction
 * - provide an interface for user to run game
 */

#ifndef GB_H
#define GB_H

#include "cli_opts.hpp"
#include "cpu/cpu.hpp"
#include "mmu/mmu.hpp"
#include "ppu/ppu.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

class Gameboy {
private:
  Opts *cli_opts;
  MMU mmu;
  CPU cpu;
  PPU ppu;
  // delete copy and assignment constructor
  Gameboy(const Gameboy &) = delete;
  Gameboy &operator=(const Gameboy &) = delete;

public:
  // TODO: figure out how to move Opts to gb.hpp too
  Gameboy(Opts *opts_);

  // application loop
  void run();
};

#endif
