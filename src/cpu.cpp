#include "../include/cpu.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
// #include <ios>
#include <iostream>
#include <sys/types.h>

const uint8_t RESET_FLAG = 0;
const uint8_t Z_FLAG = 0b10000000;
const uint8_t N_FLAG = 0b01000000;
const uint8_t H_FLAG = 0b00100000;
const uint8_t C_FLAG = 0b00010000;

CPU::CPU(std::ifstream &ROM_)
    : ROM(ROM_), clock_m(0), IME(false), // Interrupts disabled initially
      mode(OAM),                         // Start in OAM mode
      can_render(true), ppu_clock(0), interrupt_flags(0) {

  // Initialize registers with default values
  reg.a = 0x01;
  reg.f = 0xB0;
  reg.b = 0x00;
  reg.c = 0x13;
  reg.d = 0x00;
  reg.e = 0xD8;
  reg.h = 0x01;
  reg.l = 0x4D;
  reg.sp = 0xFFFE;
  reg.pc = 0x0100; // Start at 0x100 (skip boot ROM)

  // Initialize framebuffer
  for (int i = 0; i < framebuffer.size(); i++) {
    framebuffer[i] = ROM.colors[3];
  }

  // Initialize interrupt enable register
  write_byte(0xFFFF, 0x00); // All interrupts disabled initially
}

uint8_t CPU::read_byte(uint16_t addr) {
  // clock_m += 1;
  return ROM.read_byte(addr);
}

uint16_t CPU::read_word(uint16_t addr) {
  // clock_m += 1;
  uint8_t low = ROM.read_byte(addr);
  uint8_t high = ROM.read_byte(addr + 1);
  return (high << 8) | low;
}

void CPU::write_byte(uint16_t addr, uint8_t val) {
  // clock_m += 1;
  ROM.write_byte(addr, val);
}

void CPU::write_word(uint16_t addr, uint16_t val) {
  write_byte(addr, val & 0xFF);   // Write low byte first
  write_byte(addr + 1, val >> 8); // Write high byte second
}

uint8_t CPU::read_reg(CPU::R8_PTR r) {
  // clock_m += 1;
  return reg.*r;
}

void CPU::write_reg(CPU::R8_PTR r, uint8_t val) {
  // clock_m += 1;
  reg.*r = val;
}

uint16_t CPU::read_reg(CPU::R16_PTR r) {
  // clock_m += 1;
  return reg.*r;
}

void CPU::write_reg(CPU::R16_PTR r, uint16_t val) {
  // clock_m += 1;
  reg.*r = val;
}

bool CPU::flag_value(uint8_t f, FLAG_MODE mode) {
  if (mode == REG_F)
    return (reg.f & (1 << f)) != 0;
  else if (mode == LCD_CONTROL) {
    return (read_byte(0xFF40) & (1 << f)) != 0;
  } else if (mode == LCD_STATUS) {
    return (read_byte(0xFF41) & (1 << f)) != 0;
  } else {
    std::cout << "ERROR: Unsupported Mode " << mode << std::endl;
    exit(5);
  }
}
void CPU::flag_value(uint8_t f, bool set, FLAG_MODE mode) {
  if (mode == REG_F) {
    if (set) {
      reg.f |= (1 << f);
    } else {
      reg.f &= ~(1 << f);
    }
  } else if (mode == LCD_CONTROL) {
    if (set) {
      write_byte(0xFF40, read_byte(0xFF40) | (1 << f));
    } else {
      write_byte(0xFF40, read_byte(0xFF40) & ~(1 << f));
    }
  } else if (mode == LCD_STATUS) {
    if (set) {
      write_byte(0xFF41, read_byte(0xFF41) | (1 << f));
    } else {
      write_byte(0xFF41, read_byte(0xFF41) & ~(1 << f));
    }
  }
}

void CPU::reset_flags() { reg.f = 0; }

void CPU::set_z_flag(uint test) {
  if (test == 0) {
    flag_value(Z, 1);
  } else {
    flag_value(Z, 0);
  }
}

void CPU::set_h_flag(uint test) {
  if (test > 0xf) {
    flag_value(H, 1);
  } else {
    flag_value(H, 0);
  }
}

void CPU::set_c_flag(uint test) {
  if (test > 0xff) {
    flag_value(C, 1);
  } else {
    flag_value(C, 0);
  }
}
