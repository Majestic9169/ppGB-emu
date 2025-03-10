#include "../include/cpu.h"
#include <cstdint>

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
  uint32_t tmp = reg.sp + e8;
  reg.hl = tmp;
  clock_m += 2;
  reset_flags();
  set_h_flag(tmp);
  set_c_flag(tmp);
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

/* ====================================
 *          ARTHMETIC UNITS
 * ====================================
 */

// ADC A, r8
void CPU::ADC_A_r8(CPU::R_PTR r) {
  uint32_t tmp = reg.a + read_reg(r) + flag_value(C);
  reg.a = tmp;
  flag_value(Z, reg.a == 0 ? 0 : 1);
  flag_value(N, 0);
  set_h_flag(tmp);
  set_c_flag(tmp);
}

// ADC A, [HL]
void CPU::ADC_A_HL() {
  uint32_t tmp = read_byte(reg.hl) + flag_value(C) + reg.a;
  reg.a = tmp;
  flag_value(Z, reg.a == 0 ? 0 : 1);
  flag_value(N, 0);
  set_h_flag(tmp);
  set_c_flag(tmp);
}

// ADC A, n8
void CPU::ADC_A_n8(uint8_t n8) {
  uint32_t tmp = reg.a + n8 + flag_value(C);
  reg.a = tmp;
  flag_value(Z, reg.a == 0 ? 0 : 1);
  flag_value(N, 0);
  set_h_flag(tmp);
  set_c_flag(tmp);
}
