/*
 * Gameboy
 * - contain all components and manage their construction
 * - provide an interface for user to run game
 */

#include "../include/gb.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <cstdio>

// TODO: figure out how to move Opts to gb.hpp too
Gameboy::Gameboy(Opts *opts_)
    : cli_opts{opts_}, mmu(opts_), cpu{opts_, &mmu}, ppu{opts_, &mmu},
      is_paused(opts_->debug_enabled() ? true : false) {
  // HACK: for gameboy-doctor
  // initialise registers to value after bootrom
  cpu.reg.af = 0x01b0;
  cpu.reg.bc = 0x0013;
  cpu.reg.de = 0x00d8;
  cpu.reg.hl = 0x014d;
  cpu.reg.sp = 0xfffe;
  cpu.reg.pc = 0x0100;
};

// step
void Gameboy::gb_step(FILE *fp) {
  cpu.print_reg(fp);
  // render
  cpu.cpu_step();
  // ppu.ppu_step();
  // print debug
  if (cli_opts->debug_enabled()) {
    ppu.print_debug();
  }
}

// application loop
void Gameboy::run() {
  FILE *fp = fopen("rom.log", "w");
  while (1) {
    // poll events
    SDL_Event Event;
    while (SDL_PollEvent(&Event)) {
      if (Event.type == SDL_QUIT) {
        SDL_Quit();
        fclose(fp);
        // mmu.hexdump();
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
          gb_step(fp);
        } break;
        default: {
        }
        }
      }
    }
    if (!is_paused) {
      gb_step(fp);
    }
    // update
    // ppu.Update();
  }
}
