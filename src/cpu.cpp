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
    : reg{.a = 0x01,
          .f = 0xb0,
          .b = 0x00,
          .c = 0x13,
          .d = 0x00,
          .e = 0xd8,
          .h = 0x01,
          .l = 0x4d,
          .sp = 0xfffe,
          .pc = 0x100},
      IME(0), ROM(ROM_), clock_m(0) {}

uint8_t CPU::read_byte(uint16_t addr) {
  // clock_m += 1;
  return ROM.read_byte(addr);
}

uint16_t CPU::read_word(uint16_t addr) {
  // clock_m += 1;
  return (ROM.read_byte(addr)) + (ROM.read_byte(addr + 1) << 8);
}

void CPU::write_byte(uint16_t addr, uint8_t val) {
  // clock_m += 1;
  ROM.write_byte(addr, val);
}

void CPU::write_word(uint16_t addr, uint16_t val) {
  write_byte(addr, val & 0x00ff);
  write_byte(addr + 1, val >> 8);
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
  if (mode == 0)
    return (reg.f & (1 << f)) != 0;
  else if (mode == 1) {
    return (read_byte(0xFF40) & (1 << f)) != 0;
  } else if (mode == 2) {
    return (read_byte(0xFF41) & (1 << f)) != 0;
  } else {
    std::cout << "ERROR: Unsupported Mode " << mode << std::endl;
    exit(5);
  }
}
void CPU::flag_value(uint8_t f, bool set, FLAG_MODE mode) {
  if (mode == 0) {
    if (set) {
      reg.f |= (1 << f);
    } else {
      reg.f &= (0xff ^ (1 << f));
    }
  } else if (mode == 1) {
    if (set) {
      write_byte(0xFF40, read_byte(0xFF40) | (1 << f));
    } else {
      write_byte(0xFF40, read_byte(0xFF40) & (0xff ^ (1 << f)));
    }
  } else if (mode == 2) {
    if (set) {
      write_byte(0xFF41, read_byte(0xFF41) | (1 << f));
    } else {
      write_byte(0xFF41, read_byte(0xFF41) & (0xff ^ (1 << f)));
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
