/*
 * Gameboy
 * - contain all components and manage their construction
 * - provide an interface for user to run game
 */

#include "../include/gb.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <cstdint>

// TODO: figure out how to move Opts to gb.hpp too
Gameboy::Gameboy(Opts *opts_)
    : cli_opts{opts_}, mmu(opts_), cpu{opts_, &mmu}, ppu{opts_, &mmu},
      is_paused(opts_->debug_enabled() ? true : false) {};

// step
void Gameboy::gb_step() {

  uint8_t old_if = mmu.read_byte(0xff0f);
  cpu.cpu_step();
  ppu.ppu_step();
  cpu.check_interrupts(old_if);

  if (cli_opts->debug_enabled()) {
    cpu.print_reg();
    ppu.print_debug();
  }
}

// application loop
void Gameboy::run() {
  while (1) {
    // poll events
    SDL_Event Event;
    while (SDL_PollEvent(&Event)) {
      if (Event.type == SDL_QUIT) {
        SDL_Quit();
        mmu.hexdump();
        return;
      }
      if (Event.type == SDL_KEYDOWN) {
        switch (Event.key.keysym.sym) {
        case SDLK_r: {
          is_paused = !is_paused;
          std::cout << "Toggling Pause\n";
        } break;
        case SDLK_n: {
          std::cout << "Stepping\n";
          gb_step();
        } break;
        default: {
        }
        }
      }
    }

    if (!is_paused) {
      gb_step();
    }

    // update
    if (ppu.lx == 1) {
      ppu.Update();
    }
  }
}
