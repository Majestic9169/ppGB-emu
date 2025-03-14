#include "../include/cpu.h"
#include <cstdint>
#include <sys/types.h>

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
void CPU::ADC_A_r8(CPU::R8_PTR r) {
  uint32_t tmp = reg.a + read_reg(r) + flag_value(C);
  reg.a = tmp;
  set_z_flag(reg.a);
  flag_value(N, 0);
  set_h_flag(tmp);
  set_c_flag(tmp);
}

// ADC A, [HL]
void CPU::ADC_A_HL() {
  uint32_t tmp = read_byte(reg.hl) + flag_value(C) + reg.a;
  reg.a = tmp;
  set_z_flag(reg.a);
  flag_value(N, 0);
  set_h_flag(tmp);
  set_c_flag(tmp);
}

// ADC A, n8
void CPU::ADC_A_n8(uint8_t n8) {
  uint32_t tmp = reg.a + n8 + flag_value(C);
  reg.a = tmp;
  set_z_flag(reg.a);
  flag_value(N, 0);
  set_h_flag(tmp);
  set_c_flag(tmp);
}

// ADD A, r8
void CPU::ADD_A_r8(CPU::R8_PTR r) {
  uint32_t tmp = reg.a + read_reg(r);
  reg.a = tmp;
  set_z_flag(reg.a);
  flag_value(N, 0);
  set_h_flag(tmp);
  set_c_flag(tmp);
}

// ADD A, [HL]
void CPU::ADD_A_HL() {
  uint32_t tmp = read_byte(reg.hl) + reg.a;
  reg.a = tmp;
  set_z_flag(reg.a);
  flag_value(N, 0);
  set_h_flag(tmp);
  set_c_flag(tmp);
}

// ADD A, n8
void CPU::ADD_A_n8(uint8_t n8) {
  uint32_t tmp = reg.a + n8;
  reg.a = tmp;
  set_z_flag(reg.a);
  flag_value(N, 0);
  set_h_flag(tmp);
  set_c_flag(tmp);
}

// ADD HL, r16
void CPU::ADD_HL_r16(CPU::R16_PTR r) {
  uint tmp = read_reg(r) + reg.hl;
  reg.hl = tmp;
  tmp >>= 7;
  flag_value(N, 0);
  set_h_flag(tmp);
  set_c_flag(tmp);
}

// ADD SP, e8
void CPU::ADD_SP_e8(int8_t e8) {
  uint tmp = reg.sp + e8;
  reg.sp = tmp;
  flag_value(Z, 0);
  flag_value(N, 0);
  set_h_flag(tmp);
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
  uint8_t result = reg.a - reg_value;
  set_z_flag(result);
  flag_value(N, 1);
  flag_value(H, ((reg.a & 0x0f) - (reg_value & 0x0f)) < 0);
  flag_value(C, reg.a < reg_value);
}

// CP A, [HL]
void CPU::CP_A_HL() {
  uint8_t reg_value = read_byte(reg.hl);
  uint8_t result = reg.a - reg_value;
  set_z_flag(result);
  flag_value(N, 1);
  flag_value(H, ((reg.a & 0x0f) - (reg_value & 0x0f)) < 0);
  flag_value(C, reg.a < reg_value);
}

// CP A, n8
void CPU::CP_A_n8(uint8_t n8) {
  uint8_t result = reg.a - n8;
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
  set_z_flag((uint16_t)res);
  flag_value(N, 0);
  set_h_flag(res);
}

// INC r16
void CPU::INC_SP() {
  uint16_t res = read_reg(&CPU::REGISTERS::sp) + 1;
  write_reg(&CPU::REGISTERS::sp, res);
}

// CCF
void CPU::CCF() {
  flag_value(N, 0);
  flag_value(H, 0);
  flag_value(C, flag_value(C) == 0 ? 1 : 0);
}
