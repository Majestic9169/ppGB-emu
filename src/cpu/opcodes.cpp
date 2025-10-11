/*
 * Opcodes
 * - all opcodes stored as class members
 * - include helper functions and switch statements
 * - can access mmu and registers
 * - the goal is to have all opcode functions be single lines,
 *   and if that's not possible then introduce helper instructions
 * - and use instructions anyway when there are flags to be set
 * - for the sake of readibility
 */

#include "../../include/cpu/opcodes.hpp"
#include <cstdint>

Opcodes::Opcodes(MMU *_mmu, Registers *_reg) : mmu{_mmu}, reg{_reg} {}

void Opcodes::unimplemented() {
  std::cout << RED << "[!] unsupported opcode encountered\n" << COLOR_RESET;
  exit(3);
}
uint16_t Opcodes::read_word_from_pc() {
  uint16_t val = mmu->read_word(reg->pc++);
  reg->pc++;
  return val;
}
void Opcodes::cpl() {
  reg->a = ~reg->a;
  reg->f.n = 1;
  reg->f.h = 1;
}
void Opcodes::ccf() {
  reg->f.c = !reg->f.c;
  reg->f.n = 0;
  reg->f.h = 0;
}
void Opcodes::scf() {
  reg->f.c = 1;
  reg->f.n = 0;
  reg->f.h = 0;
}
void Opcodes::daa() {
  uint8_t adj = 0;
  if (reg->f.n) {
    if (reg->f.h) {
      adj += 0x6;
    }
    if (reg->f.c) {
      adj += 0x60;
    }
    reg->a = reg->a - adj;
  } else {
    if (reg->f.h || ((reg->a & 0x0f) > 0x09)) {
      // just the addition of 0x06 can not trigger a carry unless a was
      // greater than 0x99 anyway which is we can relegate the carry to be
      // dependent only on if adj>=0x60
      adj += 0x6;
    }
    if (reg->f.c || (reg->a > 0x99)) {
      adj += 0x60;
    }
    reg->f.c = adj >= 0x60 ? 1 : 0;
    reg->a = reg->a + adj;
  }
  reg->set_z(reg->a);
  reg->f.h = 0;
}
// LDH INSTRUCTIONS
void Opcodes::ldh_u8_a(uint8_t u8) { mmu->write_byte(0xff00 + u8, reg->a); }
void Opcodes::ldh_a_u8(uint8_t u8) { reg->a = mmu->read_byte(0xff00 + u8); }
void Opcodes::ld_hl_sp() {
  signed char e8 = mmu->read_byte(reg->pc++);
  uint8_t res1 = (reg->sp & 0x000F) + (e8 & 0x0F);
  if (res1 >= 0x10) {
    reg->f.h = 1;
  } else {
    reg->f.h = 0;
  }
  uint16_t res2 = (reg->sp & 0xFF) + (e8 & 0xFF);
  if (res2 >= 0x100) {
    reg->f.c = 1;
  } else {
    reg->f.c = 0;
  }
  reg->hl = reg->sp + e8;
  reg->f.z = 0;
  reg->f.n = 0;
}
// INC INSTRUCTIONS Z0H-
void Opcodes::inc(uint8_t &val) {
  reg->set_h(val);
  val++;
  reg->set_z(val);
  reg->f.n = 0;
}
void Opcodes::inc_16(uint16_t addr) {
  uint8_t val = mmu->read_byte(addr);
  reg->set_h(val);
  val++;
  mmu->write_byte(addr, val);
  reg->set_z(val);
  reg->f.n = 0;
}
// DEC INSTRUCTIONS Z1H-
void Opcodes::dec(uint8_t &val) {
  if ((val & 0x0f) == 0x00) {
    reg->f.h = 1;
  } else {
    reg->f.h = 0;
  }
  val--;
  reg->set_z(val);
  reg->f.n = 1;
}
void Opcodes::dec_16(uint16_t addr) {
  uint8_t val = mmu->read_byte(addr);
  if ((val & 0x0f) == 0x00) {
    reg->f.h = 1;
  } else {
    reg->f.h = 0;
  }
  val--;
  mmu->write_byte(addr, val);
  reg->set_z(val);
  reg->f.n = 1;
}
// ROTATE INSTRUCTIONS 000C
void Opcodes::rlca() {
  reg->f.z = 0;
  reg->f.n = 0;
  reg->f.h = 0;
  reg->f.c = (reg->a & 0x80) >> 7;
  reg->a = (reg->a << 1) | reg->f.c;
}
void Opcodes::rla() {
  reg->f.z = 0;
  reg->f.n = 0;
  reg->f.h = 0;
  uint8_t tmp = 0 + reg->f.c;
  reg->f.c = (reg->a & 0x80) >> 7;
  reg->a = (reg->a << 1) | tmp;
}
void Opcodes::rrca() {
  reg->f.z = 0;
  reg->f.n = 0;
  reg->f.h = 0;
  reg->f.c = reg->a & 0x01;
  reg->a = (reg->a >> 1) | (reg->f.c << 7);
}
void Opcodes::rra() {
  reg->f.z = 0;
  reg->f.n = 0;
  reg->f.h = 0;
  uint8_t tmp = 0 + reg->f.c;
  reg->f.c = reg->a & 0x01;
  reg->a = (reg->a >> 1) | (tmp << 7);
}
// ADD INSTRUCTIONS
void Opcodes::add_hl(uint16_t r16) {
  reg->f.n = 0;
  // check overflow from bit 11
  if (((reg->hl & 0x0fff) + (r16 & 0x0fff)) & 0xf000) {
    reg->f.h = 1;
  } else {
    reg->f.h = 0;
  }
  // check overflow from bit 15
  uint32_t res = reg->hl + r16;
  if (res & 0xffff0000) {
    reg->f.c = 1;
  } else {
    reg->f.c = 0;
  }
  reg->hl = reg->hl + r16;
}
void Opcodes::add(uint8_t val) {
  reg->set_h(reg->a, val);
  reg->set_c(reg->a, val);
  reg->f.n = 0;
  reg->a = reg->a + val;
  reg->set_z(reg->a);
}
void Opcodes::add_sp(signed char val) {
  uint8_t res = (reg->sp & 0x0f) + (val & 0x0f);
  if ((res & 0xf0)) {
    reg->f.h = 1;
  } else {
    reg->f.h = 0;
  }
  uint16_t res2 = (reg->sp & 0x00ff) + uint8_t(val);
  if ((res2 & 0xff00)) {
    reg->f.c = 1;
  } else {
    reg->f.c = 0;
  }
  reg->sp += val;
  reg->f.z = 0;
  reg->f.n = 0;
}
// NOTE: half carry can be confusing because it feels like it depends on the
// order of addition
// https://gbdev.gg8.se/wiki/articles/ADC
void Opcodes::adc(uint8_t val) {
  reg->f.h = ((reg->a & 0x0f) + (val & 0x0f) + (reg->f.c)) > 0x0F ? 1 : 0;
  uint16_t res = reg->a + val + reg->f.c;
  reg->f.c = res > 0xFF ? 1 : 0;
  reg->a = res & 0x00FF;
  reg->set_z(reg->a);
  reg->f.n = 0;
}
// SUB INSTRUCTIONS
// TODO: add sub/sbc unit tests. don't feel like it rn
// all the rest of the ALU too
// NOTE: turns out my SBC was wrong for like 5 months
// always test your code children
void Opcodes::sub(uint8_t val) {
  // help my subs are unionizing
  if ((val & 0x0f) > (reg->a & 0x0f)) {
    reg->f.h = 1;
  } else {
    reg->f.h = 0;
  }
  if (val > reg->a) {
    reg->f.c = 1;
  } else {
    reg->f.c = 0;
  }
  reg->a = reg->a - val;
  reg->f.n = 1;
  reg->set_z(reg->a);
};
void Opcodes::sbc(uint8_t val) {
  if (reg->f.c + (val & 0x0f) > (reg->a & 0x0f)) {
    reg->f.h = 1;
  } else {
    reg->f.h = 0;
  }
  uint8_t res = reg->a - val - reg->f.c;
  if (uint16_t(val) + reg->f.c > reg->a) {
    reg->f.c = 1;
  } else {
    reg->f.c = 0;
  }
  reg->a = res;
  reg->f.n = 1;
  reg->set_z(reg->a);
};
// AND INSTRUCTIONS
// NOTE: "and" is a keyword in c++ :sob:
void Opcodes::and_(uint8_t val) {
  reg->a = reg->a & val;
  reg->set_z(reg->a);
  reg->f.n = 0;
  reg->f.h = 1;
  reg->f.c = 0;
}
// XOR INSTRUCTION
// NOTE: "xor" too :sob:
void Opcodes::xor_(uint8_t val) {
  reg->a = reg->a ^ val;
  reg->set_z(reg->a);
  reg->f.n = 0;
  reg->f.h = 0;
  reg->f.c = 0;
}
// OR INSTRUCTION
void Opcodes::or_(uint8_t val) {
  reg->a = reg->a | val;
  reg->set_z(reg->a);
  reg->f.n = 0;
  reg->f.h = 0;
  reg->f.c = 0;
}
// CP INSTRUCTIONS
void Opcodes::cp(uint8_t val) {
  uint8_t res = reg->a - val;
  if ((val & 0x0f) > (reg->a & 0x0f)) {
    reg->f.h = 1;
  } else {
    reg->f.h = 0;
  }
  if (val > reg->a) {
    reg->f.c = 1;
  } else {
    reg->f.c = 0;
  }
  reg->f.n = 1;
  reg->set_z(res);
}
// JR INSTRUCTIONS
void Opcodes::jr(bool condition) {
  // TODO: add cycles diff for taken/untaken whenever i add clock
  int8_t i8 = mmu->read_byte(reg->pc++);
  if (condition) {
    reg->pc = reg->pc + i8;
  }
}
// JP INSTRUCTIONS
void Opcodes::jp(bool condition) {
  uint16_t val = read_word_from_pc();
  if (condition) {
    reg->pc = val;
  }
}
// POP INSTRUCTIONS
void Opcodes::pop(uint16_t &r16) {
  r16 = mmu->read_word(reg->sp);
  reg->af = reg->af & 0xfff0;
  reg->sp += 2;
}
// PUSH INSTRUCTIONS
void Opcodes::push(uint16_t val) {
  reg->sp = reg->sp - 2;
  mmu->write_word(reg->sp, val);
}
// RET INSTRUCTIONS
// TODO: implement diff cycles for taken/untaken
// ret is diff from ret taken/not taken for some reason
void Opcodes::ret(bool condition) {
  if (condition == true) {
    pop(reg->pc);
  }
}
void Opcodes::reti() {
  opcode_fb();
  ret(true);
}
// CALL INSTRUCTIONS
void Opcodes::call(bool condition) {
  uint16_t addr = read_word_from_pc();
  if (condition) {
    push(reg->pc);
    reg->pc = addr;
  }
}
// RST INSTRUCTIONS
void Opcodes::rst(uint16_t addr) {
  push(reg->pc);
  reg->pc = addr;
}

void Opcodes::call_interrupt(uint8_t addr) {
  push(reg->pc);
  reg->pc = addr;
}

// NOP
void Opcodes::opcode_00() { return; }
// LD BC, u16
void Opcodes::opcode_01() { reg->bc = read_word_from_pc(); }
// LD (BC), A
void Opcodes::opcode_02() { mmu->write_byte(reg->bc, reg->a); }
// INC BC
void Opcodes::opcode_03() { reg->bc++; }
// INC B
void Opcodes::opcode_04() { inc(reg->b); }
// DEC B
void Opcodes::opcode_05() { dec(reg->b); };
// LD B, u8
void Opcodes::opcode_06() { reg->b = mmu->read_byte(reg->pc++); };
// RLCA
void Opcodes::opcode_07() { rlca(); };
// LD (u16), SP
void Opcodes::opcode_08() {
  mmu->write_word(mmu->read_byte(reg->pc++), reg->sp);
  reg->pc++;
};
// ADD HL, BC
void Opcodes::opcode_09() { add_hl(reg->bc); };
// LD A, (BC)
void Opcodes::opcode_0a() { reg->a = mmu->read_byte(reg->bc); };
// DEC BC
void Opcodes::opcode_0b() { reg->bc--; };
// INC C
void Opcodes::opcode_0c() { inc(reg->c); };
// DEC C
void Opcodes::opcode_0d() { dec(reg->c); };
// LD C, u8
void Opcodes::opcode_0e() { reg->c = mmu->read_byte(reg->pc++); };
// RRCA
void Opcodes::opcode_0f() { rrca(); };
// STOP
void Opcodes::opcode_10() { reg->stopped = true; };
// LD DE, u16
void Opcodes::opcode_11() { reg->de = read_word_from_pc(); };
// LD (DE), A
void Opcodes::opcode_12() { mmu->write_byte(reg->de, reg->a); };
// INC DE
void Opcodes::opcode_13() { reg->de++; };
// INC D
void Opcodes::opcode_14() { inc(reg->d); }
// DEC D
void Opcodes::opcode_15() { dec(reg->d); };
// LD D, u8
void Opcodes::opcode_16() { reg->d = mmu->read_byte(reg->pc++); };
// RLA
void Opcodes::opcode_17() { rla(); };
// JR i8
void Opcodes::opcode_18() { jr(true); };
// ADD HL, DE
void Opcodes::opcode_19() { add_hl(reg->de); };
// LD A, (BC)
void Opcodes::opcode_1a() { reg->a = mmu->read_byte(reg->de); };
// DEC DE
void Opcodes::opcode_1b() { reg->de--; };
// INC E
void Opcodes::opcode_1c() { inc(reg->e); };
// DEC E
void Opcodes::opcode_1d() { dec(reg->e); };
// LD E, u8
void Opcodes::opcode_1e() { reg->e = mmu->read_byte(reg->pc++); };
// RRA
void Opcodes::opcode_1f() { rra(); };
// JR NZ, i8
void Opcodes::opcode_20() { jr(!reg->f.z); };
// LD HL, u16
void Opcodes::opcode_21() { reg->hl = read_word_from_pc(); };
// LD (HL+), A
void Opcodes::opcode_22() { mmu->write_byte(reg->hl++, reg->a); };
// INC HL
void Opcodes::opcode_23() { reg->hl++; };
// INC H
void Opcodes::opcode_24() { inc(reg->h); };
// DEC H
void Opcodes::opcode_25() { dec(reg->h); };
// LD H, u8
void Opcodes::opcode_26() { reg->h = mmu->read_byte(reg->pc++); };
// DAA
void Opcodes::opcode_27() { daa(); };
// JR Z, i8
void Opcodes::opcode_28() { jr(reg->f.z); };
// ADD HL, HL
void Opcodes::opcode_29() { add_hl(reg->hl); };
// LD A, (HL+)
void Opcodes::opcode_2a() { reg->a = mmu->read_byte(reg->hl++); };
// DEC HL
void Opcodes::opcode_2b() { reg->hl--; };
// INC L
void Opcodes::opcode_2c() { inc(reg->l); };
// DEC L
void Opcodes::opcode_2d() { dec(reg->l); };
// LD L, u8
void Opcodes::opcode_2e() { reg->l = mmu->read_byte(reg->pc++); };
// CPL
void Opcodes::opcode_2f() { cpl(); };
// JR NC, i8
void Opcodes::opcode_30() { jr(!reg->f.c); };
// LD SP, u16
void Opcodes::opcode_31() { reg->sp = read_word_from_pc(); };
// LD (HL-), A
void Opcodes::opcode_32() { mmu->write_byte(reg->hl--, reg->a); };
// INC SP
void Opcodes::opcode_33() { reg->sp++; };
// INC (HL)
void Opcodes::opcode_34() { inc_16(reg->hl); };
// DEC (HL)
void Opcodes::opcode_35() { dec_16(reg->hl); };
// LD (HL), u8
void Opcodes::opcode_36() {
  mmu->write_byte(reg->hl, mmu->read_byte(reg->pc++));
};
// SCF
void Opcodes::opcode_37() { scf(); };
// JR C, i8
void Opcodes::opcode_38() { jr(reg->f.c); };
// ADD HL, SP
void Opcodes::opcode_39() { add_hl(reg->sp); };
// LD A, (HL-)
void Opcodes::opcode_3a() { reg->a = mmu->read_byte(reg->hl--); };
// DEC SP
void Opcodes::opcode_3b() { reg->sp--; };
// INC A
void Opcodes::opcode_3c() { inc(reg->a); };
// DEC A
void Opcodes::opcode_3d() { dec(reg->a); };
// LD A, u8
void Opcodes::opcode_3e() { reg->a = mmu->read_byte(reg->pc++); };
// CCF
void Opcodes::opcode_3f() { ccf(); };
// LD B, B
void Opcodes::opcode_40() { return; };
// LD B, C
void Opcodes::opcode_41() { reg->b = reg->c; };
// LD B, D
void Opcodes::opcode_42() { reg->b = reg->d; };
// LD B, E
void Opcodes::opcode_43() { reg->b = reg->e; };
// LD B, H
void Opcodes::opcode_44() { reg->b = reg->h; };
// LD B, L
void Opcodes::opcode_45() { reg->b = reg->l; };
// LD B, (HL)
void Opcodes::opcode_46() { reg->b = mmu->read_byte(reg->hl); };
// LD B, A
void Opcodes::opcode_47() { reg->b = reg->a; };
// LD C, B
void Opcodes::opcode_48() { reg->c = reg->b; };
// LD C, C
void Opcodes::opcode_49() { return; };
// LD C, D
void Opcodes::opcode_4a() { reg->c = reg->d; };
// LD C, E
void Opcodes::opcode_4b() { reg->c = reg->e; };
// LD C, H
void Opcodes::opcode_4c() { reg->c = reg->h; };
// LD C, L
void Opcodes::opcode_4d() { reg->c = reg->l; };
// LD C, (HL)
void Opcodes::opcode_4e() { reg->c = mmu->read_byte(reg->hl); };
// LD C, A
void Opcodes::opcode_4f() { reg->c = reg->a; };
// LD D, B
void Opcodes::opcode_50() { reg->d = reg->b; };
// LD D, C
void Opcodes::opcode_51() { reg->d = reg->c; };
// LD D, D
void Opcodes::opcode_52() { return; };
// LD D, E
void Opcodes::opcode_53() { reg->d = reg->e; };
// LD D, H
void Opcodes::opcode_54() { reg->d = reg->h; };
// LD D, L
void Opcodes::opcode_55() { reg->d = reg->l; };
// LD D, (HL)
void Opcodes::opcode_56() { reg->d = mmu->read_byte(reg->hl); };
// LD D, A
void Opcodes::opcode_57() { reg->d = reg->a; };
// LD E, B
void Opcodes::opcode_58() { reg->e = reg->b; };
// LD E, C
void Opcodes::opcode_59() { reg->e = reg->c; };
// LD E, D
void Opcodes::opcode_5a() { reg->e = reg->d; };
// LD E, E
void Opcodes::opcode_5b() { return; };
// LD E, H
void Opcodes::opcode_5c() { reg->e = reg->h; };
// LD E, L
void Opcodes::opcode_5d() { reg->e = reg->l; };
// LD E, (HL)
void Opcodes::opcode_5e() { reg->e = mmu->read_byte(reg->hl); };
// LD E, A
void Opcodes::opcode_5f() { reg->e = reg->a; };
// LD H, B
void Opcodes::opcode_60() { reg->h = reg->b; };
// LD H, C
void Opcodes::opcode_61() { reg->h = reg->c; };
// LD H, D
void Opcodes::opcode_62() { reg->h = reg->d; };
// LD H, E
void Opcodes::opcode_63() { reg->h = reg->e; };
// LD H, H
void Opcodes::opcode_64() { return; };
// LD H, L
void Opcodes::opcode_65() { reg->h = reg->l; };
// LD H, (HL)
void Opcodes::opcode_66() { reg->h = mmu->read_byte(reg->hl); };
// LD H, A
void Opcodes::opcode_67() { reg->h = reg->a; };
// LD L, B
void Opcodes::opcode_68() { reg->l = reg->b; };
// LD L, C
void Opcodes::opcode_69() { reg->l = reg->c; };
// LD L, D
void Opcodes::opcode_6a() { reg->l = reg->d; };
// LD L, E
void Opcodes::opcode_6b() { reg->l = reg->e; };
// LD L, H
void Opcodes::opcode_6c() { reg->l = reg->h; };
// LD L, L
void Opcodes::opcode_6d() { return; };
// LD L, (HL)
void Opcodes::opcode_6e() { reg->l = mmu->read_byte(reg->hl); };
// LD L, A
void Opcodes::opcode_6f() { reg->l = reg->a; };
// LD (HL), B
void Opcodes::opcode_70() { mmu->write_byte(reg->hl, reg->b); };
// LD (HL), C
void Opcodes::opcode_71() { mmu->write_byte(reg->hl, reg->c); };
// LD (HL), D
void Opcodes::opcode_72() { mmu->write_byte(reg->hl, reg->d); };
// LD (HL), E
void Opcodes::opcode_73() { mmu->write_byte(reg->hl, reg->e); };
// LD (HL), H
void Opcodes::opcode_74() { mmu->write_byte(reg->hl, reg->h); };
// LD (HL), L
void Opcodes::opcode_75() { mmu->write_byte(reg->hl, reg->l); };
// HALT
// TODO: implement halt properly once IME etc are added
// will probably need a new instruction
void Opcodes::opcode_76() { reg->stopped = true; };
// LD (HL), A
void Opcodes::opcode_77() { mmu->write_byte(reg->hl, reg->a); };
// LD A, B
void Opcodes::opcode_78() { reg->a = reg->b; };
// LD A, C
void Opcodes::opcode_79() { reg->a = reg->c; };
// LD A, D
void Opcodes::opcode_7a() { reg->a = reg->d; };
// LD A, E
void Opcodes::opcode_7b() { reg->a = reg->e; };
// LD A, H
void Opcodes::opcode_7c() { reg->a = reg->h; };
// LD A, L
void Opcodes::opcode_7d() { reg->a = reg->l; };
// LD A, (HL)
void Opcodes::opcode_7e() { reg->a = mmu->read_byte(reg->hl); };
// LD A, A
void Opcodes::opcode_7f() { return; };
// ADD A, B
void Opcodes::opcode_80() { add(reg->b); };
// ADD A, C
void Opcodes::opcode_81() { add(reg->c); };
// ADD A, D
void Opcodes::opcode_82() { add(reg->d); };
// ADD A, E
void Opcodes::opcode_83() { add(reg->e); };
// ADD A, H
void Opcodes::opcode_84() { add(reg->h); };
// ADD A, L
void Opcodes::opcode_85() { add(reg->l); };
// ADD A, (HL)
void Opcodes::opcode_86() { add(mmu->read_byte(reg->hl)); };
// ADD A, A
void Opcodes::opcode_87() { add(reg->a); };
// ADC A, B
void Opcodes::opcode_88() { adc(reg->b); };
// ADC A, c
void Opcodes::opcode_89() { adc(reg->c); };
// ADC A, D
void Opcodes::opcode_8a() { adc(reg->d); };
// ADC A, E
void Opcodes::opcode_8b() { adc(reg->e); };
// ADC A, H
void Opcodes::opcode_8c() { adc(reg->h); };
// ADC A, L
void Opcodes::opcode_8d() { adc(reg->l); };
// ADC A, (HL)
void Opcodes::opcode_8e() { adc(mmu->read_byte(reg->hl)); };
// ADC A, A
void Opcodes::opcode_8f() { adc(reg->a); };
// SUB A, B
void Opcodes::opcode_90() { sub(reg->b); };
// SUB A, C
void Opcodes::opcode_91() { sub(reg->c); };
// SUB A, D
void Opcodes::opcode_92() { sub(reg->d); };
// SUB A, E
void Opcodes::opcode_93() { sub(reg->e); };
// SUB A, H
void Opcodes::opcode_94() { sub(reg->h); };
// SUB A, L
void Opcodes::opcode_95() { sub(reg->l); };
// SUB A, (HL)
void Opcodes::opcode_96() { sub(mmu->read_byte(reg->hl)); };
// SUB A, A
void Opcodes::opcode_97() { sub(reg->a); };
// SBC A, B
void Opcodes::opcode_98() { sbc(reg->b); };
// SBC A, C
void Opcodes::opcode_99() { sbc(reg->c); };
// SBC A, D
void Opcodes::opcode_9a() { sbc(reg->d); };
// SBC A, E
void Opcodes::opcode_9b() { sbc(reg->e); };
// SBC A, H
void Opcodes::opcode_9c() { sbc(reg->h); };
// SBC A, L
void Opcodes::opcode_9d() { sbc(reg->l); };
// SBC A, (HL)
void Opcodes::opcode_9e() { sbc(mmu->read_byte(reg->hl)); };
// SBC A, A
void Opcodes::opcode_9f() { sbc(reg->a); };
// AND A, B
void Opcodes::opcode_a0() { and_(reg->b); };
// AND A, C
void Opcodes::opcode_a1() { and_(reg->c); };
// AND A, D
void Opcodes::opcode_a2() { and_(reg->d); };
// AND A, E
void Opcodes::opcode_a3() { and_(reg->e); };
// AND A, H
void Opcodes::opcode_a4() { and_(reg->h); };
// AND A, L
void Opcodes::opcode_a5() { and_(reg->l); };
// AND A, (HL)
void Opcodes::opcode_a6() { and_(mmu->read_byte(reg->hl)); };
// AND A, A
void Opcodes::opcode_a7() { and_(reg->a); };
// XOR A, B
void Opcodes::opcode_a8() { xor_(reg->b); };
// XOR A, C
void Opcodes::opcode_a9() { xor_(reg->c); };
// XOR A, D
void Opcodes::opcode_aa() { xor_(reg->d); };
// XOR A, E
void Opcodes::opcode_ab() { xor_(reg->e); };
// XOR A, H
void Opcodes::opcode_ac() { xor_(reg->h); };
// XOR A, L
void Opcodes::opcode_ad() { xor_(reg->l); };
// XOR A, (HL)
void Opcodes::opcode_ae() { xor_(mmu->read_byte(reg->hl)); };
// XOR A, A
void Opcodes::opcode_af() { xor_(reg->a); };
// OR A, B
void Opcodes::opcode_b0() { or_(reg->b); };
// OR A, C
void Opcodes::opcode_b1() { or_(reg->c); };
// OR A, D
void Opcodes::opcode_b2() { or_(reg->d); };
// OR A, E
void Opcodes::opcode_b3() { or_(reg->e); };
// OR A, H
void Opcodes::opcode_b4() { or_(reg->h); };
// OR A, L
void Opcodes::opcode_b5() { or_(reg->l); };
// OR A, (HL)
void Opcodes::opcode_b6() { or_(mmu->read_byte(reg->hl)); };
// OR A, A
void Opcodes::opcode_b7() { or_(reg->a); };
// CP A, B
void Opcodes::opcode_b8() { cp(reg->b); };
// CP A, C
void Opcodes::opcode_b9() { cp(reg->c); };
// CP A, D
void Opcodes::opcode_ba() { cp(reg->d); };
// CP A, E
void Opcodes::opcode_bb() { cp(reg->e); };
// CP A, H
void Opcodes::opcode_bc() { cp(reg->h); };
// CP A, L
void Opcodes::opcode_bd() { cp(reg->l); };
// CP A, (HL)
void Opcodes::opcode_be() { cp(mmu->read_byte(reg->hl)); };
// CP A, A
void Opcodes::opcode_bf() { cp(reg->a); };
// RET NZ
void Opcodes::opcode_c0() { ret(!reg->f.z); };
// POP BC
void Opcodes::opcode_c1() { pop(reg->bc); };
// JP NZ, u16
void Opcodes::opcode_c2() { jp(!reg->f.z); };
// JP u16
void Opcodes::opcode_c3() { jp(true); };
// CALL NZ, u16
void Opcodes::opcode_c4() { call(!reg->f.z); };
// PUSH BC
void Opcodes::opcode_c5() { push(reg->bc); };
// ADD A, u8
void Opcodes::opcode_c6() { add(mmu->read_byte(reg->pc++)); };
// RST 0x00
void Opcodes::opcode_c7() { rst(0x00); };
// RET Z
void Opcodes::opcode_c8() { ret(reg->f.z); };
// RET
void Opcodes::opcode_c9() { ret(true); };
// JP Z, u16
void Opcodes::opcode_ca() { jp(reg->f.z); };
// CALL Z, u16
void Opcodes::opcode_cc() { call(reg->f.z); };
// CALL u16
void Opcodes::opcode_cd() { call(true); };
// ADC A, u8
void Opcodes::opcode_ce() { adc(mmu->read_byte(reg->pc++)); };
// RST 0x08
void Opcodes::opcode_cf() { rst(0x08); };
// RET NC
void Opcodes::opcode_d0() { ret(!reg->f.c); };
// POP DE
void Opcodes::opcode_d1() { pop(reg->de); };
// JP NC, u16
void Opcodes::opcode_d2() { jp(!reg->f.c); };
void Opcodes::opcode_d3() { unimplemented(); };
// CALL NC, u16
void Opcodes::opcode_d4() { call(!reg->f.c); };
// PUSH DE
void Opcodes::opcode_d5() { push(reg->de); };
// SUB A, u8
void Opcodes::opcode_d6() { sub(mmu->read_byte(reg->pc++)); };
// RST 0x10
void Opcodes::opcode_d7() { rst(0x10); };
// RET C
void Opcodes::opcode_d8() { ret(reg->f.c); };
// RETI
void Opcodes::opcode_d9() { reti(); };
// JP C, u16
void Opcodes::opcode_da() { jp(reg->f.c); };
void Opcodes::opcode_db() { unimplemented(); };
// CALL C, u16
void Opcodes::opcode_dc() { call(reg->f.c); };
void Opcodes::opcode_dd() { unimplemented(); };
// SBC A, u8
void Opcodes::opcode_de() { sbc(mmu->read_byte(reg->pc++)); };
// RST 0x18
void Opcodes::opcode_df() { rst(0x18); };
// LD (FF00+u8), A
void Opcodes::opcode_e0() { ldh_u8_a(mmu->read_byte(reg->pc++)); };
// POP HL
void Opcodes::opcode_e1() { pop(reg->hl); };
// LD (FF00+C), A
void Opcodes::opcode_e2() { ldh_u8_a(reg->c); };
void Opcodes::opcode_e3() { unimplemented(); };
void Opcodes::opcode_e4() { unimplemented(); };
// PUSH HL
void Opcodes::opcode_e5() { push(reg->hl); };
// AND A, u8
void Opcodes::opcode_e6() { and_(mmu->read_byte(reg->pc++)); };
// RST 0x20
void Opcodes::opcode_e7() { rst(0x20); };
// ADD SP, i8
void Opcodes::opcode_e8() { add_sp(mmu->read_byte(reg->pc++)); };
// JP HL
void Opcodes::opcode_e9() { reg->pc = reg->hl; };
// LD (u16), A
void Opcodes::opcode_ea() { mmu->write_byte(read_word_from_pc(), reg->a); };
void Opcodes::opcode_eb() { unimplemented(); };
void Opcodes::opcode_ec() { unimplemented(); };
void Opcodes::opcode_ed() { unimplemented(); };
// XOR A, u8
void Opcodes::opcode_ee() { xor_(mmu->read_byte(reg->pc++)); };
// RST 0x28
void Opcodes::opcode_ef() { rst(0x28); };
// LD A, (FF00+u8)
void Opcodes::opcode_f0() { ldh_a_u8(mmu->read_byte(reg->pc++)); };
// POP AF
void Opcodes::opcode_f1() { pop(reg->af); };
// LD A, (FF00+C)
void Opcodes::opcode_f2() { ldh_a_u8(reg->c); };
// DI
void Opcodes::opcode_f3() { reg->ime = false; };
void Opcodes::opcode_f4() { unimplemented(); };
// PUSH AF
void Opcodes::opcode_f5() { push(reg->af); };
// OR A, u8
void Opcodes::opcode_f6() { or_(mmu->read_byte(reg->pc++)); };
// RST 0x30
void Opcodes::opcode_f7() { rst(0x30); };
// LD HL, SP+i8
void Opcodes::opcode_f8() { ld_hl_sp(); };
// LD SP, HL
void Opcodes::opcode_f9() { reg->sp = reg->hl; };
// LD A, (u16)
void Opcodes::opcode_fa() { reg->a = mmu->read_byte(read_word_from_pc()); };
// EI
// TODO: implemente EI with the 1 instruction delay
void Opcodes::opcode_fb() { reg->ime = true; };
void Opcodes::opcode_fc() { unimplemented(); };
void Opcodes::opcode_fd() { unimplemented(); };
// CP A, u8
void Opcodes::opcode_fe() { cp(mmu->read_byte(reg->pc++)); };
// RST 0x38
void Opcodes::opcode_ff() { rst(0x38); };

// HACK: for the sm83 tests
void Opcodes::test() {
  switch (mmu->read_byte(reg->pc++)) {
    // clang-format off
      case 0x00: opcode_00(); break;  case 0x01: opcode_01(); break;
      case 0x02: opcode_02(); break;  case 0x03: opcode_03(); break;
      case 0x04: opcode_04(); break;  case 0x05: opcode_05(); break;
      case 0x06: opcode_06(); break;  case 0x07: opcode_07(); break;
      case 0x08: opcode_08(); break;  case 0x09: opcode_09(); break;
      case 0x0a: opcode_0a(); break;  case 0x0b: opcode_0b(); break;
      case 0x0c: opcode_0c(); break;  case 0x0d: opcode_0d(); break;
      case 0x0e: opcode_0e(); break;  case 0x0f: opcode_0f(); break;
      case 0x10: opcode_10(); break;  case 0x11: opcode_11(); break;
      case 0x12: opcode_12(); break;  case 0x13: opcode_13(); break;
      case 0x14: opcode_14(); break;  case 0x15: opcode_15(); break;
      case 0x16: opcode_16(); break;  case 0x17: opcode_17(); break;
      case 0x18: opcode_18(); break;  case 0x19: opcode_19(); break;
      case 0x1a: opcode_1a(); break;  case 0x1b: opcode_1b(); break;
      case 0x1c: opcode_1c(); break;  case 0x1d: opcode_1d(); break;
      case 0x1e: opcode_1e(); break;  case 0x1f: opcode_1f(); break;
      case 0x20: opcode_20(); break;  case 0x21: opcode_21(); break;
      case 0x22: opcode_22(); break;  case 0x23: opcode_23(); break;
      case 0x24: opcode_24(); break;  case 0x25: opcode_25(); break;
      case 0x26: opcode_26(); break;  case 0x27: opcode_27(); break;
      case 0x28: opcode_28(); break;  case 0x29: opcode_29(); break;
      case 0x2a: opcode_2a(); break;  case 0x2b: opcode_2b(); break;
      case 0x2c: opcode_2c(); break;  case 0x2d: opcode_2d(); break;
      case 0x2e: opcode_2e(); break;  case 0x2f: opcode_2f(); break;
      case 0x30: opcode_30(); break;  case 0x31: opcode_31(); break;
      case 0x32: opcode_32(); break;  case 0x33: opcode_33(); break;
      case 0x34: opcode_34(); break;  case 0x35: opcode_35(); break;
      case 0x36: opcode_36(); break;  case 0x37: opcode_37(); break;
      case 0x38: opcode_38(); break;  case 0x39: opcode_39(); break;
      case 0x3a: opcode_3a(); break;  case 0x3b: opcode_3b(); break;
      case 0x3c: opcode_3c(); break;  case 0x3d: opcode_3d(); break;
      case 0x3e: opcode_3e(); break;  case 0x3f: opcode_3f(); break;
      case 0x40: opcode_40(); break;  case 0x41: opcode_41(); break;
      case 0x42: opcode_42(); break;  case 0x43: opcode_43(); break;
      case 0x44: opcode_44(); break;  case 0x45: opcode_45(); break;
      case 0x46: opcode_46(); break;  case 0x47: opcode_47(); break;
      case 0x48: opcode_48(); break;  case 0x49: opcode_49(); break;
      case 0x4a: opcode_4a(); break;  case 0x4b: opcode_4b(); break;
      case 0x4c: opcode_4c(); break;  case 0x4d: opcode_4d(); break;
      case 0x4e: opcode_4e(); break;  case 0x4f: opcode_4f(); break;
      case 0x50: opcode_50(); break;  case 0x51: opcode_51(); break;
      case 0x52: opcode_52(); break;  case 0x53: opcode_53(); break;
      case 0x54: opcode_54(); break;  case 0x55: opcode_55(); break;
      case 0x56: opcode_56(); break;  case 0x57: opcode_57(); break;
      case 0x58: opcode_58(); break;  case 0x59: opcode_59(); break;
      case 0x5a: opcode_5a(); break;  case 0x5b: opcode_5b(); break;
      case 0x5c: opcode_5c(); break;  case 0x5d: opcode_5d(); break;
      case 0x5e: opcode_5e(); break;  case 0x5f: opcode_5f(); break;
      case 0x60: opcode_60(); break;  case 0x61: opcode_61(); break;
      case 0x62: opcode_62(); break;  case 0x63: opcode_63(); break;
      case 0x64: opcode_64(); break;  case 0x65: opcode_65(); break;
      case 0x66: opcode_66(); break;  case 0x67: opcode_67(); break;
      case 0x68: opcode_68(); break;  case 0x69: opcode_69(); break;
      case 0x6a: opcode_6a(); break;  case 0x6b: opcode_6b(); break;
      case 0x6c: opcode_6c(); break;  case 0x6d: opcode_6d(); break;
      case 0x6e: opcode_6e(); break;  case 0x6f: opcode_6f(); break;
      case 0x70: opcode_70(); break;  case 0x71: opcode_71(); break;
      case 0x72: opcode_72(); break;  case 0x73: opcode_73(); break;
      case 0x74: opcode_74(); break;  case 0x75: opcode_75(); break;
      case 0x76: opcode_76(); break;  case 0x77: opcode_77(); break;
      case 0x78: opcode_78(); break;  case 0x79: opcode_79(); break;
      case 0x7a: opcode_7a(); break;  case 0x7b: opcode_7b(); break;
      case 0x7c: opcode_7c(); break;  case 0x7d: opcode_7d(); break;
      case 0x7e: opcode_7e(); break;  case 0x7f: opcode_7f(); break;
      case 0x80: opcode_80(); break;  case 0x81: opcode_81(); break;
      case 0x82: opcode_82(); break;  case 0x83: opcode_83(); break;
      case 0x84: opcode_84(); break;  case 0x85: opcode_85(); break;
      case 0x86: opcode_86(); break;  case 0x87: opcode_87(); break;
      case 0x88: opcode_88(); break;  case 0x89: opcode_89(); break;
      case 0x8a: opcode_8a(); break;  case 0x8b: opcode_8b(); break;
      case 0x8c: opcode_8c(); break;  case 0x8d: opcode_8d(); break;
      case 0x8e: opcode_8e(); break;  case 0x8f: opcode_8f(); break;
      case 0x90: opcode_90(); break;  case 0x91: opcode_91(); break;
      case 0x92: opcode_92(); break;  case 0x93: opcode_93(); break;
      case 0x94: opcode_94(); break;  case 0x95: opcode_95(); break;
      case 0x96: opcode_96(); break;  case 0x97: opcode_97(); break;
      case 0x98: opcode_98(); break;  case 0x99: opcode_99(); break;
      case 0x9a: opcode_9a(); break;  case 0x9b: opcode_9b(); break;
      case 0x9c: opcode_9c(); break;  case 0x9d: opcode_9d(); break;
      case 0x9e: opcode_9e(); break;  case 0x9f: opcode_9f(); break;
      case 0xa0: opcode_a0(); break;  case 0xa1: opcode_a1(); break;
      case 0xa2: opcode_a2(); break;  case 0xa3: opcode_a3(); break;
      case 0xa4: opcode_a4(); break;  case 0xa5: opcode_a5(); break;
      case 0xa6: opcode_a6(); break;  case 0xa7: opcode_a7(); break;
      case 0xa8: opcode_a8(); break;  case 0xa9: opcode_a9(); break;
      case 0xaa: opcode_aa(); break;  case 0xab: opcode_ab(); break;
      case 0xac: opcode_ac(); break;  case 0xad: opcode_ad(); break;
      case 0xae: opcode_ae(); break;  case 0xaf: opcode_af(); break;
      case 0xb0: opcode_b0(); break;  case 0xb1: opcode_b1(); break;
      case 0xb2: opcode_b2(); break;  case 0xb3: opcode_b3(); break;
      case 0xb4: opcode_b4(); break;  case 0xb5: opcode_b5(); break;
      case 0xb6: opcode_b6(); break;  case 0xb7: opcode_b7(); break;
      case 0xb8: opcode_b8(); break;  case 0xb9: opcode_b9(); break;
      case 0xba: opcode_ba(); break;  case 0xbb: opcode_bb(); break;
      case 0xbc: opcode_bc(); break;  case 0xbd: opcode_bd(); break;
      case 0xbe: opcode_be(); break;  case 0xbf: opcode_bf(); break;
      case 0xc0: opcode_c0(); break;  case 0xc1: opcode_c1(); break;
      case 0xc2: opcode_c2(); break;  case 0xc3: opcode_c3(); break;
      case 0xc4: opcode_c4(); break;  case 0xc5: opcode_c5(); break;
      case 0xc6: opcode_c6(); break;  case 0xc7: opcode_c7(); break;
      case 0xc8: opcode_c8(); break;  case 0xc9: opcode_c9(); break;
      case 0xca: opcode_ca(); break;  case 0xcb: break;
      case 0xcc: opcode_cc(); break;  case 0xcd: opcode_cd(); break;
      case 0xce: opcode_ce(); break;  case 0xcf: opcode_cf(); break;
      case 0xd0: opcode_d0(); break;  case 0xd1: opcode_d1(); break;
      case 0xd2: opcode_d2(); break;  case 0xd3: opcode_d3(); break;
      case 0xd4: opcode_d4(); break;  case 0xd5: opcode_d5(); break;
      case 0xd6: opcode_d6(); break;  case 0xd7: opcode_d7(); break;
      case 0xd8: opcode_d8(); break;  case 0xd9: opcode_d9(); break;
      case 0xda: opcode_da(); break;  case 0xdb: opcode_db(); break;
      case 0xdc: opcode_dc(); break;  case 0xdd: opcode_dd(); break;
      case 0xde: opcode_de(); break;  case 0xdf: opcode_df(); break;
      case 0xe0: opcode_e0(); break;  case 0xe1: opcode_e1(); break;
      case 0xe2: opcode_e2(); break;  case 0xe3: opcode_e3(); break;
      case 0xe4: opcode_e4(); break;  case 0xe5: opcode_e5(); break;
      case 0xe6: opcode_e6(); break;  case 0xe7: opcode_e7(); break;
      case 0xe8: opcode_e8(); break;  case 0xe9: opcode_e9(); break;
      case 0xea: opcode_ea(); break;  case 0xeb: opcode_eb(); break;
      case 0xec: opcode_ec(); break;  case 0xed: opcode_ed(); break;
      case 0xee: opcode_ee(); break;  case 0xef: opcode_ef(); break;
      case 0xf0: opcode_f0(); break;  case 0xf1: opcode_f1(); break;
      case 0xf2: opcode_f2(); break;  case 0xf3: opcode_f3(); break;
      case 0xf4: opcode_f4(); break;  case 0xf5: opcode_f5(); break;
      case 0xf6: opcode_f6(); break;  case 0xf7: opcode_f7(); break;
      case 0xf8: opcode_f8(); break;  case 0xf9: opcode_f9(); break;
      case 0xfa: opcode_fa(); break;  case 0xfb: opcode_fb(); break;
      case 0xfc: opcode_fc(); break;  case 0xfd: opcode_fd(); break;
      case 0xfe: opcode_fe(); break;  case 0xff: opcode_ff(); break;
    // clang-format on
  }
}
