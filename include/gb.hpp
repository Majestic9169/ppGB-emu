/*
 * Gameboy
 * - contain all components and manage their construction
 * - provide an interface for user to run game
 */

#ifndef GB_H
#define GB_H

#include "cli_opts.hpp"
#include "cpu.hpp"
#include "mmu.hpp"
#include "ppu.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

class Gameboy {
private:
  MMU mmu;
  CPU cpu;
  PPU ppu;
  // delete copy and assignment constructor
  Gameboy(const Gameboy &) = delete;
  Gameboy &operator=(const Gameboy &) = delete;

public:
  // TODO: figure out how to move Opts to gb.hpp too
  Gameboy(Opts *opts_) : mmu(opts_), cpu{opts_, &mmu}, ppu{opts_, &mmu} {};

  // application loop
  void run() {
    while (1) {
      // poll events
      SDL_Event Event;
      while (SDL_PollEvent(&Event)) {
        if (Event.type == SDL_QUIT) {
          SDL_Quit();
          // mmu.hexdump();
          return;
        }
      }

      // render
      cpu.cpu_step();
      cpu.print_reg();
      ppu.ppu_step();
      ppu.print_debug();

      // update
      ppu.Update();
    }
  }
};

#endif
