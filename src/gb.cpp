/*
 * Gameboy
 * - contain all components and manage their construction
 * - provide an interface for user to run game
 */

#include "../include/gb.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

// TODO: figure out how to move Opts to gb.hpp too
Gameboy::Gameboy(Opts *opts_)
    : cli_opts{opts_}, mmu(opts_), cpu{opts_, &mmu}, ppu{opts_, &mmu} {};

// application loop
void Gameboy::run() {
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
    ppu.ppu_step();

    // print debug
    if (cli_opts->debug_enabled()) {
      cpu.print_reg();
      ppu.print_debug();
    }

    // update
    ppu.Update();
  }
}
