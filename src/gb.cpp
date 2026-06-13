/*
 * Gameboy
 * - contain all components and manage their construction
 * - provide an interface for user to run game
 */

#include "../include/gb.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
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
    // cpu.print_reg();
    // ppu.print_debug();
    // printf("JOYP: %02X\n", mmu.read_byte(0xff00));
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
          // clang-format off
        case SDLK_RIGHT: mmu.joyp.setButton(JOYP_REG::BUTTON_TYPE::DPAD, 0); break;
        case SDLK_LEFT: mmu.joyp.setButton(JOYP_REG::BUTTON_TYPE::DPAD, 1); break;
        case SDLK_UP: mmu.joyp.setButton(JOYP_REG::BUTTON_TYPE::DPAD, 2); break;
        case SDLK_DOWN: mmu.joyp.setButton(JOYP_REG::BUTTON_TYPE::DPAD, 3); break;
        case SDLK_z : mmu.joyp.setButton(JOYP_REG::BUTTON_TYPE::BUTT, 0); break;
        case SDLK_x: mmu.joyp.setButton(JOYP_REG::BUTTON_TYPE::BUTT, 1); break;
        case SDLK_RSHIFT: mmu.joyp.setButton(JOYP_REG::BUTTON_TYPE::BUTT, 2); break;
        case SDLK_RETURN: mmu.joyp.setButton(JOYP_REG::BUTTON_TYPE::BUTT, 3); break;
          // clang-format on
        }
        mmu.interrupt_flag.ReqJoypad();
      } else if (Event.type == SDL_KEYUP) {
        switch (Event.key.keysym.sym) {
          // clang-format off
        case SDLK_RIGHT: mmu.joyp.clearButton(JOYP_REG::BUTTON_TYPE::DPAD, 0); break;
        case SDLK_LEFT: mmu.joyp.clearButton(JOYP_REG::BUTTON_TYPE::DPAD, 1); break;
        case SDLK_UP: mmu.joyp.clearButton(JOYP_REG::BUTTON_TYPE::DPAD, 2); break;
        case SDLK_DOWN: mmu.joyp.clearButton(JOYP_REG::BUTTON_TYPE::DPAD, 3); break;
        case SDLK_z : mmu.joyp.clearButton(JOYP_REG::BUTTON_TYPE::BUTT, 0); break;
        case SDLK_x: mmu.joyp.clearButton(JOYP_REG::BUTTON_TYPE::BUTT, 1); break;
        case SDLK_RSHIFT: mmu.joyp.clearButton(JOYP_REG::BUTTON_TYPE::BUTT, 2); break;
        case SDLK_RETURN: mmu.joyp.clearButton(JOYP_REG::BUTTON_TYPE::BUTT, 3); break;
          // clang-format on
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
