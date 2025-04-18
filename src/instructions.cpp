#include "../include/cpu.h"
#include <cstdint>
#include <iostream>
#include <sys/types.h>

#define H_TEST(a, b) ((a & 0x0F) + (b & 0x0F))

/* ====================================
 *          CONTROL/MISC
 * ====================================
 */
// NOP - 0x00
void CPU::NOP() {
  // no operation
  clock_m = 1;
}

void CPU::HALT() {
  // Check if there are any pending interrupts
  uint8_t IE = read_byte(0xFFFF);
  uint8_t IF = read_byte(0xFF0F);
  uint8_t pending = IF & IE & 0x1F;
  
  // If interrupts are disabled and there are pending interrupts,
  // this is a "halt bug" - the CPU will skip the next instruction
  if (!IME && pending) {
    // Don't halt, but don't increment PC either (will be done by the instruction decoder)
    halted = false;
    return;
  }
  
  // Normal halt behavior
  halted = true;
  clock_m += 1; // Add a cycle for the halt
}

/* ====================================
 *          LOAD/STORE/MOVE
 * ====================================
 */
// LD r8, r8
void CPU::LD_r8_r8(CPU::R8_PTR r1, CPU::R8_PTR r2) {
  if (r1 == r2) {
    NOP();
  } else {
    write_reg(r1, read_reg(r2));
    clock_m -= 1;
  }
}

// LD r8, n8
void CPU::LD_r8_n8(CPU::R8_PTR r, uint8_t val) { write_reg(r, val); }

// LD (r16), A
void CPU::LD_r16_r8(CPU::R16_PTR r1, CPU::R8_PTR r2) {
  uint8_t val = read_reg(r2);
  write_byte(read_reg(r1), val);
}

// LD A, (r16)
void CPU::LD_r8_r16(CPU::R8_PTR r1, CPU::R16_PTR r2) {
  uint8_t val = read_byte(read_reg(r2));
  write_reg(r1, val);
}

// LD r16, n16
void CPU::LD_r16_n16(CPU::R16_PTR r, uint16_t n16) { write_reg(r, n16); }

// LD [HL], r8
void CPU::LD_HL_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r);
  write_byte(reg.hl, val);
}

// LD [HL], n8
void CPU::LD_HL_n8(uint8_t n) { write_byte(reg.hl, n); }

// LD r8, [HL]
void CPU::LD_r8_HL(CPU::R8_PTR r) {
  uint8_t val = read_byte(reg.hl);
  write_reg(r, val);
}

// LD [n16], A
void CPU::LD_n16_A(uint16_t n16) { write_byte(n16, reg.a); }

// LD A, [n16]
void CPU::LD_A_n16(uint16_t n16) { write_reg(&REGISTERS::a, read_byte(n16)); };

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
void CPU::LDH_n16_A(uint8_t n8) {
  uint8_t val = read_reg(&CPU::REGISTERS::a);
  write_byte(0xff00 + n8, val);
}

// LDH [C], A
void CPU::LDH_C_A() {
  uint8_t val = read_reg(&CPU::REGISTERS::a);
  write_byte(reg.c + 0xff00, val);
}

// LDH A, [n16]
void CPU::LDH_A_n16(uint8_t n8) {
  uint8_t val = read_byte(0xff00 + n8);
  write_reg(&CPU::REGISTERS::a, val);
}

// LDH A, [C]
void CPU::LDH_A_C() {
  uint8_t val = read_byte(reg.c);
  write_reg(&CPU::REGISTERS::a, val);
}

// PUSH r16
void CPU::PUSH_r16(CPU::R16_PTR r) {
  DEC_SP();
  write_byte(reg.sp, (read_reg(r) & 0xFF00) >> 8);
  DEC_SP();
  write_byte(reg.sp, read_reg(r) & 0x00FF);
}

// PUSH AF
void CPU::PUSH_AF() {
  DEC_SP();
  write_byte(reg.sp, reg.a);
  DEC_SP();
  uint8_t f_value = 0;
  // if (flag_value(Z))
  //   f_value |= 0x80;
  // if (flag_value(N))
  //   f_value |= 0x40;
  // if (flag_value(H))
  //   f_value |= 0x20;
  // if (flag_value(C))
  //   f_value |= 0x10;
  // write_byte(reg.sp, f_value);
  write_byte(reg.sp, (uint8_t)reg.f);
}
// POP r16
void CPU::POP_r16(CPU::R16_PTR r) {
  uint8_t low = read_byte(reg.sp);
  INC_SP();
  uint8_t high = read_byte(reg.sp);
  INC_SP();
  write_reg(r, ((uint16_t)high << 8) | low);
}

// POP AF
void CPU::POP_AF() {
  reg.f = read_byte(reg.sp) & 0xF0;
  INC_SP();
  reg.a = read_byte(reg.sp);
  INC_SP();
  flag_value(Z, (reg.f & (1 << 7)) != 0, REG_F);
  flag_value(N, (reg.f & (1 << 6)) != 0, REG_F);
  flag_value(H, (reg.f & (1 << 5)) != 0, REG_F);
  flag_value(C, (reg.f & (1 << 4)) != 0, REG_F);
}

/* ====================================
 *          ARTHMETIC UNITS
 * ====================================
 */

// ADC A, r8
void CPU::ADC_A_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r);
  uint8_t carry = flag_value(C);
  uint16_t tmp = reg.a + val + carry;
  uint8_t test_h = H_TEST(reg.a, val) + carry;
  reg.a = tmp;
  set_z_flag(reg.a);
  flag_value(N, 0);
  set_h_flag(test_h);
  set_c_flag(tmp);
}

// ADC A, [HL]
void CPU::ADC_A_HL() {
  uint8_t val = read_byte(reg.hl);
  uint8_t carry = flag_value(C);
  uint16_t tmp = reg.a + val + carry;
  uint8_t test_h = H_TEST(reg.a, val) + carry;
  reg.a = tmp;
  set_z_flag(reg.a);
  flag_value(N, 0);
  set_h_flag(test_h);
  set_c_flag(tmp);
}

// ADC A, n8
void CPU::ADC_A_n8(uint8_t n8) {
  uint8_t val = n8;
  uint8_t carry = flag_value(C);
  uint16_t tmp = reg.a + val + carry;
  uint8_t test_h = H_TEST(reg.a, val) + carry;
  reg.a = tmp;
  set_z_flag(reg.a);
  flag_value(N, 0);
  set_h_flag(test_h);
  set_c_flag(tmp);
}

// ADD A, r8
void CPU::ADD_A_r8(CPU::R8_PTR r) {
  uint32_t tmp = reg.a + read_reg(r);
  uint8_t test_h = H_TEST(reg.a, read_reg(r));
  reg.a = tmp;
  set_z_flag(reg.a);
  flag_value(N, 0);
  set_h_flag(test_h);
  set_c_flag(tmp);
}

// ADD A, [HL]
void CPU::ADD_A_HL() {
  uint32_t tmp = read_byte(reg.hl) + reg.a;
  uint8_t test_h = H_TEST(reg.a, read_byte(reg.hl));
  reg.a = tmp;
  set_z_flag(reg.a);
  flag_value(N, 0);
  set_h_flag(test_h);
  set_c_flag(tmp);
}

// ADD A, n8
void CPU::ADD_A_n8(uint8_t n8) {
  uint32_t tmp = reg.a + n8;
  uint8_t test_h = H_TEST(reg.a, n8);
  reg.a = tmp;
  set_z_flag(reg.a);
  flag_value(N, 0);
  set_h_flag(test_h);
  set_c_flag(tmp);
}

// ADD HL, r16
void CPU::ADD_HL_r16(CPU::R16_PTR r) {
  uint tmp = read_reg(r) + reg.hl;
  if (((read_reg(r) & 0x00ff) + (reg.hl & 0x00ff)) > 0x00ff) {
    flag_value(H, 1);
  } else {
    flag_value(H, 0);
  }
  reg.hl = tmp;
  flag_value(N, 0);
  if (tmp > 0xffff) {
    flag_value(C, 1);
  } else {
    flag_value(C, 0);
  }
}

// ADD HL, SP
void CPU::ADD_HL_SP() {
  uint tmp = reg.sp + reg.hl;
  if (((reg.sp & 0x00ff) + (reg.hl & 0x00ff)) > 0x00ff) {
    flag_value(H, 1);
  } else {
    flag_value(H, 0);
  }
  reg.hl = tmp;
  flag_value(N, 0);
  if (tmp > 0xffff) {
    flag_value(C, 1);
  } else {
    flag_value(C, 0);
  }
}
// ADD SP, e8
void CPU::ADD_SP_e8(int8_t e8) {
  uint tmp = reg.sp + e8;
  uint8_t test_h = H_TEST(reg.sp, e8);
  reg.sp = tmp;
  flag_value(Z, 0);
  flag_value(N, 0);
  set_h_flag(test_h);
  set_c_flag(tmp);
}

// AND A, r8
void CPU::AND_A_r8(CPU::R8_PTR r) {
  reg.a &= read_reg(r);
  set_z_flag(reg.a);
  flag_value(N, 0);
  flag_value(H, 1);
  flag_value(C, 0);
}

// AND A, [HL]
void CPU::AND_A_HL() {
  reg.a &= read_byte(reg.hl);
  set_z_flag(reg.a);
  flag_value(N, 0);
  flag_value(H, 1);
  flag_value(C, 0);
}

// AND A, n8
void CPU::AND_A_n8(uint8_t n8) {
  reg.a &= n8;
  set_z_flag(reg.a);
  flag_value(N, 0);
  flag_value(H, 1);
  flag_value(C, 0);
}

// CP A, r8
void CPU::CP_A_r8(R8_PTR r) {
  uint8_t reg_value = read_reg(r);
  uint16_t result = (uint8_t)(reg.a - reg_value);
  set_z_flag(result);
  flag_value(N, 1);
  flag_value(H, ((reg.a & 0x0f) - (reg_value & 0x0f)) < 0);
  flag_value(C, reg.a < reg_value);
}

// CP A, [HL]
void CPU::CP_A_HL() {
  uint8_t reg_value = read_byte(reg.hl);
  uint16_t result = (uint8_t)(reg.a - reg_value);
  set_z_flag(result);
  flag_value(N, 1);
  flag_value(H, ((reg.a & 0x0f) - (reg_value & 0x0f)) < 0);
  flag_value(C, reg.a < reg_value);
}

// CP A, n8
void CPU::CP_A_n8(uint8_t n8) {
  uint16_t result = (uint8_t)(reg.a - n8);
  set_z_flag(result);
  flag_value(N, 1);
  flag_value(H, ((reg.a & 0x0f) - (n8 & 0x0f)) < 0);
  flag_value(C, reg.a < n8);
}

// CPL
void CPU::CPL() {
  reg.a = ~reg.a;
  clock_m += 1;
  flag_value(N, 1);
  flag_value(H, 1);
}

// DAA
void CPU::DAA() {
  if (flag_value(N)) {
    uint8_t adj = 0;
    if (flag_value(H)) {
      adj += 0x06;
    }
    if (flag_value(C)) {
      adj += 0x60;
      flag_value(C, 0);
    }
    reg.a -= adj;
    set_z_flag(reg.a);
    flag_value(H, 0);
  } else {
    uint8_t adj = 0;
    if (flag_value(H) || (reg.a & 0xf) > 0x9) {
      adj += 0x06;
    }
    if (flag_value(C) || reg.a > 0x99) {
      adj += 0x60;
      flag_value(C, 1);
    }
    reg.a += adj;
    set_z_flag(reg.a);
    flag_value(H, 0);
  }
}

// DEC r8
void CPU::DEC_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r) - 1;
  write_reg(r, val);
  set_z_flag(val);
  flag_value(N, 1);
  flag_value(H, (((val + 1) & 0x0f) - (0x01 & 0x0f)) < 0);
}

// DEC [HL]
void CPU::DEC_HL() {
  reg.hl -= 1;
  set_z_flag(reg.hl);
  flag_value(N, 1);
  flag_value(H, (((reg.hl + 1) & 0x0f) - (0x01 & 0x0f)) < 0);
}

// DEC r16
void CPU::DEC_r16(CPU::R16_PTR r) {
  uint16_t val = read_reg(r) - 1;
  write_reg(r, val);
  set_z_flag(val);
  flag_value(N, 1);
  flag_value(H, (((val + 1) & 0x0f) - (0x01 & 0x0f)) < 0);
}

// DEC SP
void CPU::DEC_SP() {
  reg.sp -= 1;
  set_z_flag(reg.sp);
  flag_value(N, 1);
  flag_value(H, (((reg.sp + 1) & 0x0f) - (0x01 & 0x0f)) < 0);
}

// INC r8
void CPU::INC_r8(CPU::R8_PTR r) {
  uint res = read_reg(r) + 1;
  write_reg(r, res);
  set_z_flag((uint8_t)res);
  flag_value(N, 0);
  set_h_flag(res);
}

// INC HL
void CPU::INC_HL() {
  uint res = read_byte(reg.hl) + 1;
  write_byte(reg.hl, res);
  set_z_flag((read_byte(reg.hl)));
  flag_value(N, 0);
  set_h_flag(res);
}

// INC r16
void CPU::INC_r16(CPU::R16_PTR r) {
  uint res = read_reg(r) + 1;
  write_reg(r, res);
}

// INC SP
void CPU::INC_SP() { reg.sp += 1; }

// CCF
void CPU::CCF() {
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, flag_value(C) == 0 ? 1 : 0);
}

// SUB A, r8
void CPU::SUB_A_r8(CPU::R8_PTR r) {
  uint8_t reg_val = read_reg(r);
  flag_value(C, reg_val > reg.a);
  flag_value(H, (reg_val & 0x0f) > (reg.a & 0x0f));
  reg.a -= reg_val;
  set_z_flag(reg.a);
  flag_value(N, 1);
}

// SUB A, [HL]
void CPU::SUB_A_HL() {
  uint8_t val = read_byte(reg.hl);
  flag_value(C, val > reg.a);
  flag_value(H, (val & 0x0f) > (reg.a & 0x0f));
  write_reg(&CPU::REGISTERS::a, reg.a - val);
  set_z_flag(reg.a);
  flag_value(N, 1);
}

// SUB A, n8
void CPU::SUB_A_n8(uint8_t n8) {
  uint8_t val = n8;
  flag_value(C, val > reg.a);
  flag_value(H, (val & 0x0f) > (reg.a & 0x0f));
  reg.a -= n8;
  set_z_flag(reg.a);
  flag_value(N, 1);
}

// SBC A, n8
void CPU::SBC_A_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r);
  flag_value(C, val + flag_value(C) > reg.a);
  flag_value(H, ((val + flag_value(C)) & 0x0f) > (reg.a & 0x0f));
  reg.a = reg.a - val - flag_value(C);
  set_z_flag(reg.a);
  flag_value(N, 1);
}
// SBC A, n8
void CPU::SBC_A_n8(uint8_t n8) {
  uint8_t val = n8;
  flag_value(C, val + flag_value(C) > reg.a);
  flag_value(H, ((val + flag_value(C)) & 0x0f) > (reg.a & 0x0f));
  reg.a = reg.a - val - flag_value(C);
  set_z_flag(reg.a);
  flag_value(N, 1);
}

// SUB A, n8
void CPU::SBC_A_HL() {
  uint8_t val = read_byte(reg.hl);
  flag_value(C, (val + flag_value(C)) > reg.a);
  flag_value(H, ((val + flag_value(C)) & 0x0f) > (reg.a & 0x0f));
  reg.a = reg.a - val - flag_value(C);
  set_z_flag(reg.a);
  flag_value(N, 1);
}

// XOR A, r8
void CPU::XOR_A_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r);
  reg.a ^= val;
  set_z_flag(reg.a);
  flag_value(N, 0);
  flag_value(C, 0);
  flag_value(H, 0);
}

// XOR A, [HL]
void CPU::XOR_A_HL() {
  uint8_t val = read_byte(reg.hl);
  reg.a ^= val;
  set_z_flag(reg.a);
  flag_value(N, 0);
  flag_value(C, 0);
  flag_value(H, 0);
}

// XOR A, n8
void CPU::XOR_A_n8(uint8_t n8) {
  uint8_t val = n8;
  reg.a ^= val;
  set_z_flag(reg.a);
  flag_value(N, 0);
  flag_value(C, 0);
  flag_value(H, 0);
}

// OR A, r8
void CPU::OR_A_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r);
  reg.a |= val;
  set_z_flag(reg.a);
  flag_value(N, 0);
  flag_value(C, 0);
  flag_value(H, 0);
}

// OR A, [HL]
void CPU::OR_A_HL() {
  uint8_t val = read_byte(reg.hl);
  reg.a |= val;
  set_z_flag(reg.a);
  flag_value(N, 0);
  flag_value(C, 0);
  flag_value(H, 0);
}

// OR A, n8
void CPU::OR_A_n8(uint8_t n8) {
  uint8_t val = n8;
  reg.a |= val;
  set_z_flag(reg.a);
  flag_value(N, 0);
  flag_value(C, 0);
  flag_value(H, 0);
}

/* ====================================
 *          CONTROL/BRANCH
 * ====================================
 */

// JR n16
void CPU::JR_n16(int8_t e8) { reg.pc += e8; }

// JR cc, n16
void CPU::JR_CC_n16(bool condition, int8_t e8) {
  if (condition) {
    JR_n16(e8);
  } else {
  }
}

// RET
void CPU::RET() {
  INC_SP();
  reg.pc = read_word(reg.sp);
  INC_SP();
}

// RET CC
void CPU::RET_CC(bool condition) {
  if (condition) {
    RET();
  }
}

// RETI
void CPU::RETI() {
  // First return from the subroutine
  RET();
  
  // Then enable interrupts (this happens after the return)
  IME = true;
}

// RST vec
void CPU::RST_vec(uint8_t vec) { CALL_n16(vec); }

// JP n16
void CPU::JP_n16(uint16_t n16) { reg.pc = n16; }

// JP cc, n16
void CPU::JP_CC_n16(bool condition, uint16_t n16) {
  if (condition) {
    JP_n16(n16);
  } else {
  }
}

// JP HL
void CPU::JP_HL() { reg.pc = reg.hl; }

// CALL n16
void CPU::CALL_n16(uint16_t n16) {
  uint16_t return_addr = reg.pc;
  DEC_SP();
  write_word(reg.sp, return_addr);
  DEC_SP();
  JP_n16(n16);
}

// JP CC, n16
void CPU::CALL_CC_n16(bool condition, uint16_t n16) {
  if (condition) {
    CALL_n16(n16);
  }
}

/* ====================================
 *          MISCELLANEOUS
 * ====================================
 */
// DI
void CPU::DI() { IME = 0; }

// EI - Enable interrupts after the next instruction
void CPU::EI() { 
  // On the Game Boy, EI doesn't enable interrupts immediately
  // It sets a flag that enables interrupts after the next instruction
  // This is to prevent interrupts from occurring during the EI instruction itself
  IME = 1;
}

/* ====================================
 *     8-bit ROTATE/SHIFT bit
 * ====================================
 */

// BIT u3, r8
void CPU::BIT_u3_r8(uint8_t u3, CPU::R8_PTR r) {
  uint8_t test_byte = 1 << u3;
  if (test_byte & read_reg(r)) {
    flag_value(Z, 0);
  } else {
    flag_value(Z, 1);
  }
  flag_value(N, 0);
  flag_value(H, 1);
}

// BIT u3, [HL]
void CPU::BIT_u3_HL(uint8_t u3) {
  uint8_t test_byte = 1 << u3;
  if (test_byte & read_byte(reg.hl)) {
    flag_value(Z, 0);
  } else {
    flag_value(Z, 1);
  }
  flag_value(N, 0);
  flag_value(H, 1);
}

// RES u3, r8
void CPU::RES_u3_r8(uint8_t u3, CPU::R8_PTR r) {
  uint8_t set_byte = 1 << u3;
  set_byte = ~set_byte;
  write_reg(r, read_reg(r) & set_byte);
}

// RES u3, [HL]
void CPU::RES_u3_HL(uint8_t u3) {
  uint8_t set_byte = 1 << u3;
  set_byte = ~set_byte;
  write_byte(reg.hl, read_byte(reg.hl) & set_byte);
}

// RL r8
void CPU::RL_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r);
  bool new_carry = (val & 0x80) ? 1 : 0;
  val = (val << 1) + flag_value(C);
  write_reg(r, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// RL [HL]
void CPU::RL_HL() {
  uint8_t val = read_byte(reg.hl);
  bool new_carry = (val & 0x80) ? 1 : 0;
  val = (val << 1) + flag_value(C);
  write_byte(reg.hl, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// RLA
void CPU::RLA() {
  uint8_t val = reg.a;
  bool new_carry = (val & 0x80) ? 1 : 0;
  val = (val << 1) + flag_value(C);
  reg.a = val;
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// RLC r8
void CPU::RLC_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r);
  bool new_carry = (val & 0x80) ? 1 : 0;
  val = (val << 1) + new_carry;
  write_reg(r, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// RLC [HL]
void CPU::RLC_HL() {
  uint8_t val = read_byte(reg.hl);
  bool new_carry = (val & 0x80) ? 1 : 0;
  val = (val << 1) + new_carry;
  write_byte(reg.hl, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// RLCA
void CPU::RLCA() {
  uint8_t val = reg.a;
  bool new_carry = (val & 0x80) ? 1 : 0;
  val = (val << 1) + new_carry;
  reg.a = val;
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// RR r8
void CPU::RR_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r);
  bool new_carry = (val & 0x01);
  val = (flag_value(C) << 7) + (val >> 1);
  write_reg(r, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// RR [HL]
void CPU::RR_HL() {
  uint8_t val = read_byte(reg.hl);
  bool new_carry = (val & 0x01);
  val = (flag_value(C) << 7) + (val >> 1);
  write_byte(reg.hl, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// RRA
void CPU::RRA() {
  uint8_t val = reg.a;
  bool new_carry = (val & 0x01);
  val = (flag_value(C) << 7) + (val >> 1);
  reg.a = val;
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// RRC r8
void CPU::RRC_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r);
  bool new_carry = (val & 0x01);
  val = (new_carry << 7) + (val >> 1);
  write_reg(r, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// RRC [HL]
void CPU::RRC_HL() {
  uint8_t val = read_byte(reg.hl);
  bool new_carry = (val & 0x01);
  val = (new_carry << 7) + (val >> 1);
  write_byte(reg.hl, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// RRCA
void CPU::RRCA() {
  uint8_t val = reg.a;
  bool new_carry = (val & 0x01);
  val = (new_carry << 7) + (val >> 1);
  reg.a = val;
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// SET u3, r8
void CPU::SET_u3_r8(uint8_t u3, CPU::R8_PTR r) {
  uint8_t set_byte = 1 << u3;
  write_reg(r, read_reg(r) | set_byte);
}

// SET u3, [HL]
void CPU::SET_u3_HL(uint8_t u3) {
  uint8_t set_byte = 1 << u3;
  write_byte(reg.hl, read_byte(reg.hl) | set_byte);
}

// SLA r8
void CPU::SLA_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r);
  bool new_carry = (val & 0x80) ? 1 : 0;
  val = (val << 1);
  write_reg(r, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// SLA [HL]
void CPU::SLA_HL() {
  uint8_t val = read_byte(reg.hl);
  bool new_carry = (val & 0x80) ? 1 : 0;
  val = (val << 1);
  write_byte(reg.hl, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// SRA r8
void CPU::SRA_r8(CPU::R8_PTR r) {
  int8_t val = read_reg(r);
  bool new_carry = (val & 0x01);
  val = val >> 1;
  write_reg(r, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// SRA [HL]
void CPU::SRA_HL() {
  int8_t val = read_byte(reg.hl);
  bool new_carry = (val & 0x01);
  val = val >> 1;
  write_byte(reg.hl, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// SRL r8
void CPU::SRL_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r);
  bool new_carry = (val & 0x01);
  val = val >> 1;
  write_reg(r, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// SRL [HL]
void CPU::SRL_HL() {
  uint8_t val = read_byte(reg.hl);
  bool new_carry = (val & 0x01);
  val = val >> 1;
  write_byte(reg.hl, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, new_carry);
}

// SWAP r8
void CPU::SWAP_r8(CPU::R8_PTR r) {
  uint8_t val = read_reg(r);
  uint8_t upper = (val >> 4);
  uint8_t lower = (val & 0x0f);
  val = (lower << 3) + upper;
  write_reg(r, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, 0);
}

// SWAP [HL]
void CPU::SWAP_HL() {
  uint8_t val = read_byte(reg.hl);
  uint8_t upper = (val >> 4);
  uint8_t lower = (val & 0x0f);
  val = (lower << 3) + upper;
  write_byte(reg.hl, val);
  set_z_flag(val);
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, 0);
}
