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
      is_paused(opts_->debug_enabled() ? false : false) {};

// step
int Gameboy::gb_step() {

  int cpu_cycles{0};
  int int_cycles{0};

  // interrupt handling
  uint8_t old_if = mmu.read_byte(0xff0f);

  // cycles just executed
  cpu_cycles += cpu.cpu_step();
  // move ppu ahead those many cycles
  for (int i = 0; i < cpu_cycles; i++)
    ppu.ppu_step();

  // the same old interrupt handling
  int_cycles += cpu.check_interrupts(old_if);
  // move ppu ahead those many more cycles
  for (int i = 0; i < int_cycles; i++)
    ppu.ppu_step();

  if (cli_opts->debug_enabled()) {
    if (cpu.reg.pc == 0x022F) {
      printf("Back in init at 0x022F, SP=%04x, IE=%02x, IF=%02x\n", cpu.reg.sp,
             mmu.read_byte(0xFFFF), mmu.read_byte(0xFF0F));
    }
    cpu.print_reg();
    ppu.print_debug();
  }

  return int_cycles + cpu_cycles;
}

// application loop
void Gameboy::run() {
  int curr_cycles{0};

  while (1) {
    // auto frame_start{SDL_GetTicks64()};

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
          std::cout << "Toggling Pause\n";
          is_paused = !is_paused;
        } break;
        case SDLK_n: {
          std::cout << "Stepping\n";
          gb_step();
        } break;
        }
      }
    }

    if (!is_paused) {
      while (curr_cycles < CYCLES_PER_FRAME) {
        curr_cycles += gb_step();
      }
      curr_cycles -= CYCLES_PER_FRAME;
    }

    ppu.Update();
  }
}
