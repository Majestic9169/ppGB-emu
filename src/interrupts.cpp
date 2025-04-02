#include "../include/cpu.h"
#include <cstdint>

bool helper(uint8_t test, INTERRUPTS shift) {
  return (test & (1 << shift)) != 0 ? true : false;
}

bool CPU::interrupt_check() {
  uint8_t IE = read_byte(0xFFFF);
  uint8_t IF = read_byte(0xFF0F);
  if (!IME) {
    return false;
  }
  if (helper(IF, INT_VBLANK) && helper(IE, INT_VBLANK)) {
    return true;
  }
  if (helper(IF, INT_LCD) && helper(IE, INT_LCD)) {
    return true;
  }
  if (helper(IF, INT_TIMER) && helper(IE, INT_TIMER)) {
    return true;
  }
  if (helper(IF, INT_JOYPAD) && helper(IE, INT_JOYPAD)) {
    return true;
  }
  return false;
}

void CPU::interrupt_handle(INTERRUPTS interrupt, uint8_t jump_addr) {
  reg.sp -= 2;
  write_word(reg.sp, reg.pc);
  reg.pc = jump_addr;
  IME = false;
  write_byte(0xFF0F, read_byte(0xFF0F) & ~(1 << interrupt));
  clock_m += 20;
}
