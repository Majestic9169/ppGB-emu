#include "../include/cpu.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <cstdint>

bool helper(uint8_t test, INTERRUPTS shift) {
  return (test & (1 << shift)) != 0 ? true : false;
}

bool CPU::interrupt_check() {
  uint8_t IE = read_byte(0xFFFF);
  uint8_t IF = read_byte(0xFF0F);
  uint8_t pending = IF & IE & 0x1F;
  if (halted) {
    if (pending) {
      halted = false;
      if (!IME) {
        return false;
      }
    } else {
      clock_m += 1;
    }
  }
  if (!IME) {
    return false;
  }
  if (helper(IF, INT_VBLANK) && helper(IE, INT_VBLANK)) {
    // interrupt_handle(INT_VBLANK, 0x40);
    // return true;
  }
  if (helper(IF, INT_LCD) && helper(IE, INT_LCD)) {
    // interrupt_handle(INT_LCD, 0x48);
    // return true;
  }
  if (helper(IF, INT_TIMER) && helper(IE, INT_TIMER)) {
    // interrupt_handle(INT_TIMER, 0x50);
    // return true;
  }
  if (helper(IF, INT_JOYPAD) && helper(IE, INT_JOYPAD)) {
    // interrupt_handle(INT_JOYPAD, 0x60);
    // SDL_Event event;
    // while (SDL_PollEvent(&event)) {
    //   switch (event.type) {
    //   case SDL_EVENT_KEY_DOWN:
    //     switch (event.key.key) {
    //     case SDLK_RIGHT:
    //       key_down(JOYPAD_RIGHT);
    //       break;
    //     case SDLK_LEFT:
    //       key_down(JOYPAD_LEFT);
    //       break;
    //     case SDLK_UP:
    //       key_down(JOYPAD_UP);
    //       break;
    //     case SDLK_DOWN:
    //       key_down(JOYPAD_DOWN);
    //       break;
    //     case SDLK_Z:
    //       key_down(JOYPAD_A);
    //       break;
    //     case SDLK_X:
    //       key_down(JOYPAD_B);
    //       break;
    //     case SDLK_SPACE:
    //       key_down(JOYPAD_START);
    //       break;
    //     case SDLK_RETURN:
    //       key_down(JOYPAD_SELECT);
    //       break;
    //     }
    //     break;
    //
    //   case SDL_EVENT_KEY_UP:
    //     switch (event.key.key) {
    //     case SDLK_RIGHT:
    //       key_up(JOYPAD_RIGHT);
    //       break;
    //     case SDLK_LEFT:
    //       key_up(JOYPAD_LEFT);
    //       break;
    //     case SDLK_UP:
    //       key_up(JOYPAD_UP);
    //       break;
    //     case SDLK_DOWN:
    //       key_up(JOYPAD_DOWN);
    //       break;
    //     case SDLK_Z:
    //       key_up(JOYPAD_A);
    //       break;
    //     case SDLK_X:
    //       key_up(JOYPAD_B);
    //       break;
    //     case SDLK_SPACE:
    //       key_up(JOYPAD_START);
    //       break;
    //     case SDLK_RETURN:
    //       key_up(JOYPAD_SELECT);
    //       break;
    //     }
    //     break;
    //   }
    // }
    // return true;
  }
  return false;
}

void CPU::key_down(JOYPAD_BUTTON butt) {
  uint8_t joypad = read_byte(0xFF00);
  uint8_t prev_state = joypad;

  // Clear the corresponding bit (buttons are active low)
  joypad &= ~butt;
  write_byte(0xFF00, joypad);

  // Check if this button press should trigger an interrupt
  if ((prev_state & butt) && !(joypad & butt)) {
    uint8_t IF = read_byte(0xFF0F);
    IF |= (1 << INT_JOYPAD);
    write_byte(0xFF0F, IF);
  }
}

void CPU::key_up(JOYPAD_BUTTON butt) {
  uint8_t joypad = read_byte(0xFF00);
  // Set the corresponding bit (buttons are active low)
  joypad |= butt;
  write_byte(0xFF00, joypad);
}
