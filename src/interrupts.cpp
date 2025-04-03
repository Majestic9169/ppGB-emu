#include "../include/cpu.h"
#include <cstdint>

bool helper(uint8_t test, INTERRUPTS shift) {
  return (test & (1 << shift)) != 0 ? true : false;
}

bool CPU::interrupt_check() {
  // If CPU is halted, check if we should wake up
  if (halted) {
    uint8_t IE = read_byte(0xFFFF);
    uint8_t IF = read_byte(0xFF0F);
    uint8_t pending = IF & IE & 0x1F;
    
    if (pending) {
      // Wake up from halt
      halted = false;
      
      // If interrupts are disabled, just wake up but don't process the interrupt
      if (!IME) {
        return false;
      }
    } else {
      // Still halted, increment clock
      clock_m += 1;
      return false;
    }
  }
  
  // If interrupts are disabled, don't process any
  if (!IME) {
    return false;
  }
  
  // Get interrupt enable and flag registers
  uint8_t IE = read_byte(0xFFFF);
  uint8_t IF = read_byte(0xFF0F);
  
  // Check interrupts in priority order
  if (helper(IF, INT_VBLANK) && helper(IE, INT_VBLANK)) {
    interrupt_handle(INT_VBLANK, 0x40);
    return true;
  }
  if (helper(IF, INT_LCD) && helper(IE, INT_LCD)) {
    interrupt_handle(INT_LCD, 0x48);
    return true;
  }
  if (helper(IF, INT_TIMER) && helper(IE, INT_TIMER)) {
    interrupt_handle(INT_TIMER, 0x50);
    return true;
  }
  if (helper(IF, INT_SERIAL) && helper(IE, INT_SERIAL)) {
    interrupt_handle(INT_SERIAL, 0x58);
    return true;
  }
  if (helper(IF, INT_JOYPAD) && helper(IE, INT_JOYPAD)) {
    interrupt_handle(INT_JOYPAD, 0x60);
    return true;
  }
  
  return false;
}

void CPU::interrupt_handle(INTERRUPTS interrupt, uint8_t jump_addr) {
  // Save current PC to stack
  reg.sp -= 2;
  write_word(reg.sp, reg.pc);
  
  // Jump to interrupt vector
  reg.pc = jump_addr;
  
  // Disable interrupts
  IME = false;
  
  // Clear the interrupt flag
  write_byte(0xFF0F, read_byte(0xFF0F) & ~(1 << interrupt));
  
  // Add cycles for interrupt handling
  clock_m += 20;
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
