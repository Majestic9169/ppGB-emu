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

#ifndef OPCODE_H
#define OPCODE_H

#include "../mmu/mmu.hpp"
#include "reg.hpp"
#include <cstdint>

class Opcodes {
private:
  MMU *mmu;
  Registers *reg;
  // delete copy and assignment constructor
  Opcodes(const Opcodes &) = delete;
  Opcodes operator=(const Opcodes &) = delete;

  // MISC INSTRUCTIONS
  void unimplemented() {
    std::cout << RED << "[!] unsupported opcode encountered\n" << COLOR_RESET;
    exit(3);
  }
  uint16_t read_word_from_pc() {
    uint16_t val = mmu->read_word(reg->pc++);
    reg->pc++;
    return val;
  }
  void cpl() {
    reg->a = ~reg->a;
    reg->f.n = 1;
    reg->f.h = 1;
  }
  void ccf() {
    reg->f.c = !reg->f.c;
    reg->f.n = 0;
    reg->f.h = 0;
  }
  void scf() {
    reg->f.c = 1;
    reg->f.n = 0;
    reg->f.h = 0;
  }
  void daa() {
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
  void ldh_u8_a(uint8_t u8) { mmu->write_byte(0xff00 + u8, reg->a); }
  void ldh_a_u8(uint8_t u8) { reg->a = mmu->read_byte(0xff00 + u8); }
  void ld_hl_sp() {
    opcode_e8();
    reg->hl = reg->sp;
  }
  // INC INSTRUCTIONS Z0H-
  void inc(uint8_t &val) {
    reg->set_h(val);
    val++;
    reg->set_z(val);
    reg->f.n = 0;
  }
  void inc_16(uint16_t addr) {
    uint8_t val = mmu->read_byte(addr);
    reg->set_h(val);
    val++;
    mmu->write_byte(addr, val);
    reg->set_z(val);
    reg->f.n = 0;
  }
  // DEC INSTRUCTIONS Z1H-
  void dec(uint8_t &val) {
    if ((val & 0x0f) == 0x00) {
      reg->f.h = 1;
    } else {
      reg->f.h = 0;
    }
    val--;
    reg->set_z(val);
    reg->f.n = 1;
  }
  void dec_16(uint16_t addr) {
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
  void rlca() {
    reg->f.z = 0;
    reg->f.n = 0;
    reg->f.h = 0;
    reg->f.c = (reg->a & 0x80) >> 7;
    reg->a = (reg->a << 1) | reg->f.c;
  }
  void rla() {
    reg->f.z = 0;
    reg->f.n = 0;
    reg->f.h = 0;
    uint8_t tmp = 0 + reg->f.c;
    reg->f.c = (reg->a & 0x80) >> 7;
    reg->a = (reg->a << 1) | tmp;
  }
  void rrca() {
    reg->f.z = 0;
    reg->f.n = 0;
    reg->f.h = 0;
    reg->f.c = reg->a & 0x01;
    reg->a = (reg->a >> 1) | (reg->f.c << 7);
  }
  void rra() {
    reg->f.z = 0;
    reg->f.n = 0;
    reg->f.h = 0;
    uint8_t tmp = 0 + reg->f.c;
    reg->f.c = reg->a & 0x01;
    reg->a = (reg->a >> 1) | (tmp << 7);
  }
  // ADD INSTRUCTIONS
  void add_hl(uint16_t r16) {
    reg->f.n = 0;
    // check overflow from bit 11
    if (((reg->hl & 0x00ff) + (r16 & 0x00ff)) & 0xff00) {
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
  void add(uint8_t val) {
    reg->set_h(reg->a, val);
    reg->set_c(reg->a, val);
    reg->f.n = 0;
    reg->a = reg->a + val;
    reg->set_z(reg->a);
  }
  void add_sp(signed char val) {
    uint8_t res = (reg->sp & 0x0f) + (val & 0x0f);
    if ((res & 0xf0)) {
      reg->f.h = 1;
    } else {
      reg->f.h = 0;
    }
    uint16_t res2 = reg->sp + val;
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
  void adc(uint8_t val) {
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
  void sub(uint8_t val) {
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
  void sbc(uint8_t val) {
    // WARN: not sure about this half carry logic
    if (reg->f.c + (val & 0x0f) > (reg->a & 0x0f)) {
      reg->f.h = 1;
    } else {
      reg->f.h = 0;
    }
    uint8_t res = reg->a - val - reg->f.c;
    if (val > reg->a) {
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
  void and_(uint8_t val) {
    reg->a = reg->a & val;
    reg->set_z(reg->a);
    reg->f.n = 0;
    reg->f.h = 1;
    reg->f.c = 0;
  }
  // XOR INSTRUCTION
  // NOTE: "xor" too :sob:
  void xor_(uint8_t val) {
    reg->a = reg->a ^ val;
    reg->set_z(reg->a);
    reg->f.n = 0;
    reg->f.h = 0;
    reg->f.c = 0;
  }
  // OR INSTRUCTION
  void or_(uint8_t val) {
    reg->a = reg->a | val;
    reg->set_z(reg->a);
    reg->f.n = 0;
    reg->f.h = 0;
    reg->f.c = 0;
  }
  // CP INSTRUCTIONS
  void cp(uint8_t val) {
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
  void jr(bool condition) {
    // TODO: add cycles diff for taken/untaken whenever i add clock
    int8_t i8 = mmu->read_byte(reg->pc++);
    if (condition) {
      reg->pc = reg->pc + i8;
    }
  }
  // JP INSTRUCTIONS
  void jp(bool condition, uint16_t val) {
    if (condition) {
      reg->pc = val;
    }
  }
  // POP INSTRUCTIONS
  void pop(uint16_t &r16) {
    // TODO: if r = af -> ensure lower nibble stays 0
    r16 = mmu->read_word(reg->sp);
    reg->sp += 2;
  }
  // PUSH INSTRUCTIONS
  void push(uint16_t val) {
    reg->sp = reg->sp - 2;
    mmu->write_word(reg->sp, val);
  }
  // RET INSTRUCTIONS
  // TODO: implement diff cycles for taken/untaken
  // ret is diff from ret taken/not taken for some reason
  void ret(bool condition) {
    if (condition == true) {
      pop(reg->pc);
    }
  }
  void reti() {
    opcode_fb();
    ret(true);
  }
  // CALL INSTRUCTIONS
  void call(bool condition) {
    if (condition) {
      uint16_t addr = mmu->read_word(reg->pc);
      reg->pc += 2;
      push(reg->pc);
      reg->pc = addr;
    }
  }
  // RST INSTRUCTIONS
  void rst(uint16_t addr) {
    reg->pc += 2;
    push(reg->pc);
    reg->pc = addr;
  }

public:
  Opcodes(MMU *_mmu, Registers *_reg) : mmu{_mmu}, reg{_reg} {}
  // NOP
  void opcode_00() { return; }
  // LD BC, u16
  void opcode_01() { reg->bc = read_word_from_pc(); }
  // LD (BC), A
  void opcode_02() { mmu->write_byte(reg->bc, reg->a); }
  // INC BC
  void opcode_03() { reg->bc++; }
  // INC B
  void opcode_04() { inc(reg->b); }
  // DEC B
  void opcode_05() { dec(reg->b); };
  // LD B, u8
  void opcode_06() { reg->b = mmu->read_byte(reg->pc++); };
  // RLCA
  void opcode_07() { rlca(); };
  // LD (u16), SP
  void opcode_08() { mmu->write_word(mmu->read_byte(reg->pc++), reg->sp); };
  // ADD HL, BC
  void opcode_09() { add_hl(reg->bc); };
  // LD A, (BC)
  void opcode_0a() { reg->a = mmu->read_byte(reg->bc); };
  // DEC BC
  void opcode_0b() { reg->bc--; };
  // INC C
  void opcode_0c() { inc(reg->c); };
  // DEC C
  void opcode_0d() { dec(reg->c); };
  // LD C, u8
  void opcode_0e() { reg->c = mmu->read_byte(reg->pc++); };
  // RRCA
  void opcode_0f() { rrca(); };
  // STOP
  void opcode_10() { reg->stopped = true; };
  // LD DE, u16
  void opcode_11() { reg->de = read_word_from_pc(); };
  // LD (DE), A
  void opcode_12() { mmu->write_byte(reg->de, reg->a); };
  // INC DE
  void opcode_13() { reg->de++; };
  // INC D
  void opcode_14() { inc(reg->d); }
  // DEC D
  void opcode_15() { dec(reg->d); };
  // LD D, u8
  void opcode_16() { reg->d = mmu->read_byte(reg->pc++); };
  // RLA
  void opcode_17() { rla(); };
  // JR i8
  void opcode_18() { jr(true); };
  // ADD HL, DE
  void opcode_19() { add_hl(reg->de); };
  // LD A, (BC)
  void opcode_1a() { reg->a = mmu->read_byte(reg->de); };
  // DEC DE
  void opcode_1b() { reg->de--; };
  // INC E
  void opcode_1c() { inc(reg->e); };
  // DEC E
  void opcode_1d() { dec(reg->e); };
  // LD E, u8
  void opcode_1e() { reg->e = mmu->read_byte(reg->pc++); };
  // RRA
  void opcode_1f() { rra(); };
  // JR NZ, i8
  void opcode_20() { jr(!reg->f.z); };
  // LD HL, u16
  void opcode_21() { reg->hl = read_word_from_pc(); };
  // LD (HL+), A
  void opcode_22() { mmu->write_byte(reg->hl++, reg->a); };
  // INC HL
  void opcode_23() { reg->hl++; };
  // INC H
  void opcode_24() { inc(reg->h); };
  // DEC H
  void opcode_25() { dec(reg->h); };
  // LD H, u8
  void opcode_26() { reg->h = mmu->read_byte(reg->pc++); };
  // DAA
  void opcode_27() { daa(); };
  // JR Z, i8
  void opcode_28() { jr(reg->f.z); };
  // ADD HL, HL
  void opcode_29() { add_hl(reg->hl); };
  // LD A, (HL+)
  void opcode_2a() { reg->a = mmu->read_byte(reg->hl++); };
  // DEC HL
  void opcode_2b() { reg->hl--; };
  // INC L
  void opcode_2c() { inc(reg->l); };
  // DEC L
  void opcode_2d() { dec(reg->l); };
  // LD L, u8
  void opcode_2e() { reg->l = mmu->read_byte(reg->pc++); };
  // CPL
  void opcode_2f() { cpl(); };
  // JR NC, i8
  void opcode_30() { jr(!reg->f.c); };
  // LD SP, u16
  void opcode_31() { reg->sp = read_word_from_pc(); };
  // LD (HL-), A
  void opcode_32() { mmu->write_byte(reg->hl--, reg->a); };
  // INC SP
  void opcode_33() { reg->sp++; };
  // INC (HL)
  void opcode_34() { inc_16(reg->hl); };
  // DEC (HL)
  void opcode_35() { dec_16(reg->hl); };
  // LD (HL), u8
  void opcode_36() { mmu->write_byte(reg->hl, mmu->read_byte(reg->pc++)); };
  // SCF
  void opcode_37() { scf(); };
  // JR C, i8
  void opcode_38() { jr(reg->f.c); };
  // ADD HL, SP
  void opcode_39() { add_hl(reg->sp); };
  // LD A, (HL-)
  void opcode_3a() { reg->a = mmu->read_byte(reg->hl--); };
  // DEC SP
  void opcode_3b() { reg->sp--; };
  // INC A
  void opcode_3c() { inc(reg->a); };
  // DEC A
  void opcode_3d() { dec(reg->a); };
  // LD A, u8
  void opcode_3e() { reg->a = mmu->read_byte(reg->pc++); };
  // CCF
  void opcode_3f() { ccf(); };
  // LD B, B
  void opcode_40() { return; };
  // LD B, C
  void opcode_41() { reg->b = reg->c; };
  // LD B, D
  void opcode_42() { reg->b = reg->d; };
  // LD B, E
  void opcode_43() { reg->b = reg->e; };
  // LD B, H
  void opcode_44() { reg->b = reg->h; };
  // LD B, L
  void opcode_45() { reg->b = reg->l; };
  // LD B, (HL)
  void opcode_46() { reg->b = mmu->read_byte(reg->hl); };
  // LD B, A
  void opcode_47() { reg->b = reg->a; };
  // LD C, B
  void opcode_48() { reg->c = reg->b; };
  // LD C, C
  void opcode_49() { return; };
  // LD C, D
  void opcode_4a() { reg->c = reg->d; };
  // LD C, E
  void opcode_4b() { reg->c = reg->e; };
  // LD C, H
  void opcode_4c() { reg->c = reg->h; };
  // LD C, L
  void opcode_4d() { reg->c = reg->l; };
  // LD C, (HL)
  void opcode_4e() { reg->c = mmu->read_byte(reg->hl); };
  // LD C, A
  void opcode_4f() { reg->c = reg->a; };
  // LD D, B
  void opcode_50() { reg->d = reg->b; };
  // LD D, C
  void opcode_51() { reg->d = reg->c; };
  // LD D, D
  void opcode_52() { return; };
  // LD D, E
  void opcode_53() { reg->d = reg->e; };
  // LD D, H
  void opcode_54() { reg->d = reg->h; };
  // LD D, L
  void opcode_55() { reg->d = reg->l; };
  // LD D, (HL)
  void opcode_56() { reg->d = mmu->read_byte(reg->hl); };
  // LD D, A
  void opcode_57() { reg->d = reg->a; };
  // LD E, B
  void opcode_58() { reg->e = reg->b; };
  // LD E, C
  void opcode_59() { reg->e = reg->c; };
  // LD E, D
  void opcode_5a() { reg->e = reg->d; };
  // LD E, E
  void opcode_5b() { return; };
  // LD E, H
  void opcode_5c() { reg->e = reg->h; };
  // LD E, L
  void opcode_5d() { reg->e = reg->l; };
  // LD E, (HL)
  void opcode_5e() { reg->e = mmu->read_byte(reg->hl); };
  // LD E, A
  void opcode_5f() { reg->e = reg->a; };
  // LD H, B
  void opcode_60() { reg->h = reg->b; };
  // LD H, C
  void opcode_61() { reg->h = reg->c; };
  // LD H, D
  void opcode_62() { reg->h = reg->d; };
  // LD H, E
  void opcode_63() { reg->h = reg->e; };
  // LD H, H
  void opcode_64() { return; };
  // LD H, L
  void opcode_65() { reg->h = reg->l; };
  // LD H, (HL)
  void opcode_66() { reg->h = mmu->read_byte(reg->hl); };
  // LD H, A
  void opcode_67() { reg->h = reg->a; };
  // LD L, B
  void opcode_68() { reg->l = reg->b; };
  // LD L, C
  void opcode_69() { reg->l = reg->c; };
  // LD L, D
  void opcode_6a() { reg->l = reg->d; };
  // LD L, E
  void opcode_6b() { reg->l = reg->e; };
  // LD L, H
  void opcode_6c() { reg->l = reg->h; };
  // LD L, L
  void opcode_6d() { return; };
  // LD L, (HL)
  void opcode_6e() { reg->l = mmu->read_byte(reg->hl); };
  // LD L, A
  void opcode_6f() { reg->l = reg->a; };
  // LD (HL), B
  void opcode_70() { mmu->write_byte(reg->hl, reg->b); };
  // LD (HL), C
  void opcode_71() { mmu->write_byte(reg->hl, reg->c); };
  // LD (HL), D
  void opcode_72() { mmu->write_byte(reg->hl, reg->d); };
  // LD (HL), E
  void opcode_73() { mmu->write_byte(reg->hl, reg->e); };
  // LD (HL), H
  void opcode_74() { mmu->write_byte(reg->hl, reg->h); };
  // LD (HL), L
  void opcode_75() { mmu->write_byte(reg->hl, reg->l); };
  // HALT
  // TODO: implement halt properly once IME etc are added
  // will probably need a new instruction
  void opcode_76() { reg->stopped = true; };
  // LD (HL), A
  void opcode_77() { mmu->write_byte(reg->hl, reg->a); };
  // LD A, B
  void opcode_78() { reg->a = reg->b; };
  // LD A, C
  void opcode_79() { reg->a = reg->c; };
  // LD A, D
  void opcode_7a() { reg->a = reg->d; };
  // LD A, E
  void opcode_7b() { reg->a = reg->e; };
  // LD A, H
  void opcode_7c() { reg->a = reg->h; };
  // LD A, L
  void opcode_7d() { reg->a = reg->l; };
  // LD A, (HL)
  void opcode_7e() { reg->a = mmu->read_byte(reg->hl); };
  // LD A, A
  void opcode_7f() { return; };
  // ADD A, B
  void opcode_80() { add(reg->b); };
  // ADD A, C
  void opcode_81() { add(reg->c); };
  // ADD A, D
  void opcode_82() { add(reg->d); };
  // ADD A, E
  void opcode_83() { add(reg->e); };
  // ADD A, H
  void opcode_84() { add(reg->h); };
  // ADD A, L
  void opcode_85() { add(reg->l); };
  // ADD A, (HL)
  void opcode_86() { add(mmu->read_byte(reg->hl)); };
  // ADD A, A
  void opcode_87() { add(reg->a); };
  // ADC A, B
  void opcode_88() { adc(reg->b); };
  // ADC A, c
  void opcode_89() { adc(reg->c); };
  // ADC A, D
  void opcode_8a() { adc(reg->d); };
  // ADC A, E
  void opcode_8b() { adc(reg->e); };
  // ADC A, H
  void opcode_8c() { adc(reg->h); };
  // ADC A, L
  void opcode_8d() { adc(reg->l); };
  // ADC A, (HL)
  void opcode_8e() { adc(mmu->read_byte(reg->hl)); };
  // ADC A, A
  void opcode_8f() { adc(reg->a); };
  // SUB A, B
  void opcode_90() { sub(reg->b); };
  // SUB A, C
  void opcode_91() { sub(reg->c); };
  // SUB A, D
  void opcode_92() { sub(reg->d); };
  // SUB A, E
  void opcode_93() { sub(reg->e); };
  // SUB A, H
  void opcode_94() { sub(reg->h); };
  // SUB A, L
  void opcode_95() { sub(reg->l); };
  // SUB A, (HL)
  void opcode_96() { sub(mmu->read_byte(reg->hl)); };
  // SUB A, A
  void opcode_97() { sub(reg->a); };
  // SBC A, B
  void opcode_98() { sbc(reg->b); };
  // SBC A, C
  void opcode_99() { sbc(reg->c); };
  // SBC A, D
  void opcode_9a() { sbc(reg->d); };
  // SBC A, E
  void opcode_9b() { sbc(reg->e); };
  // SBC A, H
  void opcode_9c() { sbc(reg->h); };
  // SBC A, L
  void opcode_9d() { sbc(reg->l); };
  // SBC A, (HL)
  void opcode_9e() { sbc(mmu->read_byte(reg->hl)); };
  // SBC A, A
  void opcode_9f() { sbc(reg->a); };
  // AND A, B
  void opcode_a0() { and_(reg->b); };
  // AND A, C
  void opcode_a1() { and_(reg->c); };
  // AND A, D
  void opcode_a2() { and_(reg->d); };
  // AND A, E
  void opcode_a3() { and_(reg->e); };
  // AND A, H
  void opcode_a4() { and_(reg->h); };
  // AND A, L
  void opcode_a5() { and_(reg->l); };
  // AND A, (HL)
  void opcode_a6() { and_(mmu->read_byte(reg->hl)); };
  // AND A, A
  void opcode_a7() { and_(reg->a); };
  // XOR A, B
  void opcode_a8() { xor_(reg->b); };
  // XOR A, C
  void opcode_a9() { xor_(reg->c); };
  // XOR A, D
  void opcode_aa() { xor_(reg->d); };
  // XOR A, E
  void opcode_ab() { xor_(reg->e); };
  // XOR A, H
  void opcode_ac() { xor_(reg->h); };
  // XOR A, L
  void opcode_ad() { xor_(reg->l); };
  // XOR A, (HL)
  void opcode_ae() { xor_(mmu->read_byte(reg->hl)); };
  // XOR A, A
  void opcode_af() { xor_(reg->a); };
  // OR A, B
  void opcode_b0() { or_(reg->b); };
  // OR A, C
  void opcode_b1() { or_(reg->c); };
  // OR A, D
  void opcode_b2() { or_(reg->d); };
  // OR A, E
  void opcode_b3() { or_(reg->e); };
  // OR A, H
  void opcode_b4() { or_(reg->h); };
  // OR A, L
  void opcode_b5() { or_(reg->l); };
  // OR A, (HL)
  void opcode_b6() { or_(mmu->read_byte(reg->hl)); };
  // OR A, A
  void opcode_b7() { or_(reg->a); };
  // CP A, B
  void opcode_b8() { cp(reg->b); };
  // CP A, C
  void opcode_b9() { cp(reg->c); };
  // CP A, D
  void opcode_ba() { cp(reg->d); };
  // CP A, E
  void opcode_bb() { cp(reg->e); };
  // CP A, H
  void opcode_bc() { cp(reg->h); };
  // CP A, L
  void opcode_bd() { cp(reg->l); };
  // CP A, (HL)
  void opcode_be() { cp(mmu->read_byte(reg->hl)); };
  // CP A, A
  void opcode_bf() { cp(reg->a); };
  // RET NZ
  void opcode_c0() { ret(!reg->f.z); };
  // POP BC
  void opcode_c1() { pop(reg->bc); };
  // JP NZ, u16
  void opcode_c2() { jp(!reg->f.z, mmu->read_word(reg->pc)); };
  // JP u16
  void opcode_c3() { jp(true, mmu->read_word(reg->pc)); };
  // CALL NZ, u16
  void opcode_c4() { call(!reg->f.z); };
  // PUSH BC
  void opcode_c5() { push(reg->bc); };
  // ADD A, u8
  void opcode_c6() { add(mmu->read_byte(reg->pc++)); };
  // RST 0x00
  void opcode_c7() { rst(0x00); };
  // RET Z
  void opcode_c8() { ret(reg->f.z); };
  // RET
  void opcode_c9() { ret(true); };
  // JP Z, u16
  void opcode_ca() { jp(reg->f.z, mmu->read_byte(reg->pc++)); };
  // CALL Z, u16
  void opcode_cc() { call(reg->f.z); };
  // CALL u16
  void opcode_cd() { call(true); };
  // ADC A, u8
  void opcode_ce() { adc(mmu->read_byte(reg->pc++)); };
  // RST 0x08
  void opcode_cf() { rst(0x08); };
  // RET NC
  void opcode_d0() { ret(!reg->f.c); };
  // POP DE
  void opcode_d1() { pop(reg->de); };
  // JP NC, u16
  void opcode_d2() { jp(!reg->f.c, mmu->read_word(reg->pc)); };
  void opcode_d3() { unimplemented(); };
  // CALL NC, u16
  void opcode_d4() { call(!reg->f.c); };
  // PUSH DE
  void opcode_d5() { push(reg->de); };
  // SUB A, u8
  void opcode_d6() { sub(mmu->read_byte(reg->pc++)); };
  // RST 0x10
  void opcode_d7() { rst(0x10); };
  // RET C
  void opcode_d8() { ret(reg->f.c); };
  // RETI
  void opcode_d9() { reti(); };
  // JP C, u16
  void opcode_da() { jp(reg->f.c, mmu->read_byte(reg->pc++)); };
  void opcode_db() { unimplemented(); };
  // CALL C, u16
  void opcode_dc() { call(reg->f.c); };
  void opcode_dd() { unimplemented(); };
  // SBC A, u8
  void opcode_de() { sbc(mmu->read_byte(reg->pc++)); };
  // RST 0x18
  void opcode_df() { rst(0x18); };
  // LD (FF00+u8), A
  void opcode_e0() { ldh_u8_a(mmu->read_byte(reg->pc++)); };
  // POP HL
  void opcode_e1() { pop(reg->hl); };
  // LD (FF00+C), A
  void opcode_e2() { ldh_u8_a(reg->c); };
  void opcode_e3() { unimplemented(); };
  void opcode_e4() { unimplemented(); };
  // PUSH HL
  void opcode_e5() { push(reg->hl); };
  // AND A, u8
  void opcode_e6() { and_(mmu->read_byte(reg->pc++)); };
  // RST 0x20
  void opcode_e7() { rst(0x20); };
  // ADD SP, i8
  void opcode_e8() { add_sp(mmu->read_byte(reg->pc++)); };
  // JP HL
  void opcode_e9() { reg->pc = reg->hl; };
  // LD (u16), A
  void opcode_ea() { mmu->write_byte(read_word_from_pc(), reg->a); };
  void opcode_eb() { unimplemented(); };
  void opcode_ec() { unimplemented(); };
  void opcode_ed() { unimplemented(); };
  // XOR A, u8
  void opcode_ee() { xor_(mmu->read_byte(reg->pc++)); };
  // RST 0x28
  void opcode_ef() { rst(0x28); };
  // LD A, (FF00+u8)
  void opcode_f0() { ldh_a_u8(mmu->read_byte(reg->pc++)); };
  // POP AF
  void opcode_f1() { pop(reg->af); };
  // LD A, (FF00+C)
  void opcode_f2() { ldh_a_u8(reg->c); };
  // DI
  void opcode_f3() { reg->ime = false; };
  void opcode_f4() { unimplemented(); };
  // PUSH AF
  void opcode_f5() { push(reg->af); };
  // OR A, u8
  void opcode_f6() { or_(mmu->read_byte(reg->pc++)); };
  // RST 0x30
  void opcode_f7() { rst(0x30); };
  // LD HL, SP+i8
  void opcode_f8() { ld_hl_sp(); };
  // LD SP, HL
  void opcode_f9() { reg->sp = reg->hl; };
  // LD A, (u16)
  void opcode_fa() { reg->a = mmu->read_byte(read_word_from_pc()); };
  // EI
  // TODO: implemente EI with the 1 instruction delay
  void opcode_fb() { reg->ime = true; };
  void opcode_fc() { unimplemented(); };
  void opcode_fd() { unimplemented(); };
  // CP A, u8
  void opcode_fe() { cp(mmu->read_byte(reg->pc++)); };
  // RST 0x38
  void opcode_ff() { rst(0x38); };
};

#endif
