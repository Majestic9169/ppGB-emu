#include "../include/cpu.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

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
      ROM(ROM_), clock_m(0) {}

void CPU::print_reg() {
  std::cout << std::hex << std::uppercase << std::setfill('0');

  std::cout << "Registers:\n";
  std::cout << "A  = 0x" << std::setw(2) << static_cast<int>(reg.a)
            << std::endl;
  std::cout << "F  = 0x" << std::setw(2) << static_cast<int>(reg.f)
            << std::endl;
  std::cout << "AF = 0x" << std::setw(4) << reg.af << std::endl;
  std::cout << "B  = 0x" << std::setw(2) << static_cast<int>(reg.b)
            << std::endl;
  std::cout << "C  = 0x" << std::setw(2) << static_cast<int>(reg.c)
            << std::endl;
  std::cout << "BC = 0x" << std::setw(4) << reg.bc << std::endl;
  std::cout << "D  = 0x" << std::setw(2) << static_cast<int>(reg.d)
            << std::endl;
  std::cout << "E  = 0x" << std::setw(2) << static_cast<int>(reg.e)
            << std::endl;
  std::cout << "DE = 0x" << std::setw(4) << reg.de << std::endl;
  std::cout << "H  = 0x" << std::setw(2) << static_cast<int>(reg.h)
            << std::endl;
  std::cout << "L  = 0x" << std::setw(2) << static_cast<int>(reg.l)
            << std::endl;
  std::cout << "HL = 0x" << std::setw(4) << reg.hl << std::endl;
  std::cout << "SP = 0x" << std::setw(4) << reg.sp << std::endl;
  std::cout << "PC = 0x" << std::setw(4) << reg.pc << std::endl;

  std::cout << std::dec;
}

uint8_t CPU::read_byte(uint16_t addr) {
  clock_m += 1;
  return ROM.read_byte(addr);
}

uint16_t CPU::read_word(uint16_t addr) {
  clock_m += 1;
  return (ROM.read_byte(addr) << 8) + (ROM.read_byte(addr + 1));
}

void CPU::write_byte(uint16_t addr, uint8_t val) { ROM.write_byte(addr, val); }

void CPU::write_word(uint16_t addr, uint16_t val) {
  write_byte(addr, val >> 8);
  write_byte(addr + 1, val & 0x00ff);
}

// NOP - 0x00
void CPU::NOP() {
  // no operation
  clock_m = 1;
}

// LD BC, u16 - 0x01
void CPU::LD_16() {
  uint16_t u16 = read_word(reg.pc);
  reg.b = u16 >> 8;
  reg.c = u16 & 0x00ff;
}
