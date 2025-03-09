#include "../include/cpu.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
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

void CPU::write_byte(uint16_t addr, uint8_t val) {
  clock_m += 1;
  ROM.write_byte(addr, val);
}

void CPU::write_word(uint16_t addr, uint16_t val) {
  write_byte(addr, val & 0x00ff);
  write_byte(addr + 1, val >> 8);
}

uint8_t CPU::read_reg(CPU::R_PTR r) {
  clock_m += 1;
  return reg.*r;
}

void CPU::write_reg(CPU::R_PTR r, uint8_t val) {
  clock_m += 1;
  reg.*r = val;
}

/* ====================================
 *          CONTROL/MISC
 * ====================================
 */
// NOP - 0x00
void CPU::NOP() {
  // no operation
  clock_m = 1;
}

/* ====================================
 *          LOAD/STORE/MOVE
 * ====================================
 */
// LD r8, r8
void CPU::LD_r8_r8(CPU::R_PTR r1, CPU::R_PTR r2) {
  if (r1 == r2) {
    NOP();
  } else {
    write_reg(r1, read_reg(r2));
    clock_m -= 1;
  }
}

// LD r8, n8
void CPU::LD_r8_n8(CPU::R_PTR r, uint8_t val) { write_reg(r, val); }

// LD (r16), A
void CPU::LD_r16_r8(uint16_t addr, CPU::R_PTR r) {
  uint8_t val = read_reg(r);
  write_byte(addr, val);
}

// LD A, (r16)
void CPU::LD_r8_r16(CPU::R_PTR r, uint16_t addr) {
  uint8_t val = read_byte(addr);
  write_reg(r, val);
}

// LD r16, n16
void CPU::LD_r16_n16(CPU::R_PTR r1, CPU::R_PTR r2, uint16_t n16) {
  write_reg(r1, n16 << 8);
  write_reg(r2, n16 & 0x00ff);
}

// LD [HL], r8
void CPU::LD_HL_r8(CPU::R_PTR r) {
  uint8_t val = read_reg(r);
  write_byte(reg.hl, val);
}

// LD [HL], n8
void CPU::LD_HL_n8(uint8_t n) { write_byte(reg.hl, n); }

// LD r8, [HL]
void CPU::LD_r8_HL(CPU::R_PTR r) {
  uint8_t val = read_byte(reg.hl);
  write_reg(r, val);
}

// LD [HLI], A
void CPU::LD_HLI_A() {
  uint8_t val = read_reg(&CPU::REGISTERS::a);
  write_byte(reg.hl, val);
  reg.hl += 1;
}

// LD [HLD], A
void CPU::LD_HLD_A() {
  uint8_t val = read_reg(&CPU::REGISTERS::a);
  write_byte(reg.hl, val);
  reg.hl -= 1;
}

// LD A, [HLD]
void CPU::LD_A_HLD() {
  uint8_t val = read_byte(reg.hl);
  write_reg(&CPU::REGISTERS::a, val);
  reg.hl -= 1;
}

// LD A, [HLI]
void CPU::LD_A_HLI() {
  uint8_t val = read_byte(reg.hl);
  write_reg(&CPU::REGISTERS::a, val);
  reg.hl += 1;
}

// LD SP, n16
void CPU::LD_SP_n16(uint16_t n16) {
  clock_m += 1;
  reg.sp = n16;
}

// LD [n16], SP
void CPU::LD_n16_SP(uint16_t n16) { write_word(n16, reg.sp); }

// LD HL, SP+e8
void CPU::LD_HL_SP_e8(int8_t e8) {
  reg.hl = reg.sp + e8;
  clock_m += 2;
  reg.f &= RESET_FLAG;
  // implement flags
}

// LD SP, HL
void CPU::LD_SP_HL() {
  reg.sp = reg.hl;
  clock_m += 2;
}

// LDH [n16], A
void CPU::LDH_n16_A(uint16_t n16) {
  uint8_t val = read_reg(&CPU::REGISTERS::a);
  write_byte(n16, val);
}

// LDH [C], A
void CPU::LDH_C_A() {
  uint8_t val = read_reg(&CPU::REGISTERS::a);
  write_byte(reg.c + 0xff00, val);
}

// LDH A, [n16]
void CPU::LDH_A_n16(uint16_t n16) {
  uint8_t val = read_byte(n16);
  write_reg(&CPU::REGISTERS::a, val);
}

// LDH A, [C]
void CPU::LDH_A_C() {
  uint8_t val = read_byte(reg.c);
  write_reg(&CPU::REGISTERS::a, val);
}
