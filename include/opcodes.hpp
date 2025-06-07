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

#include "mmu.hpp"
#include "reg.hpp"
#include <cstdint>

class Opcodes {
private:
  MMU *mmu;
  Registers *reg;
  // MISC INSTRUCTIONS
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
  // ADD INSTRUCTIONS -0HC
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
  // JR INSTRUCTIONS
  void jr(bool condition) {
    // TODO: add cycles diff for taken/untaken whenever i add clock
    int8_t i8 = mmu->read_byte(reg->pc++);
    if (condition) {
      reg->pc = reg->pc + i8;
    }
  }

public:
  Opcodes(MMU *_mmu, Registers *_reg) : mmu{_mmu}, reg{_reg} {}
  // NOP
  void opcode_00() { return; }
  // LD BC, u16
  void opcode_01() { reg->bc = mmu->read_word(reg->pc++); }
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
  void opcode_11() { reg->de = mmu->read_word(reg->pc++); };
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
  void opcode_20() { jr(reg->f.n && reg->f.z); };
  // LD HL, u16
  void opcode_21() { reg->hl = mmu->read_word(reg->pc++); };
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
  void opcode_30() { jr(reg->f.n && reg->f.c); };
  // LD SP, u16
  void opcode_31() { reg->sp = mmu->read_word(reg->pc++); };
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
  void opcode_40();
  void opcode_41();
  void opcode_42();
  void opcode_43();
  void opcode_44();
  void opcode_45();
  void opcode_46();
  void opcode_47();
  void opcode_48();
  void opcode_49();
  void opcode_4a();
  void opcode_4b();
  void opcode_4c();
  void opcode_4d();
  void opcode_4e();
  void opcode_4f();
  void opcode_50();
  void opcode_51();
  void opcode_52();
  void opcode_53();
  void opcode_54();
  void opcode_55();
  void opcode_56();
  void opcode_57();
  void opcode_58();
  void opcode_59();
  void opcode_5a();
  void opcode_5b();
  void opcode_5c();
  void opcode_5d();
  void opcode_5e();
  void opcode_5f();
  void opcode_60();
  void opcode_61();
  void opcode_62();
  void opcode_63();
  void opcode_64();
  void opcode_65();
  void opcode_66();
  void opcode_67();
  void opcode_68();
  void opcode_69();
  void opcode_6a();
  void opcode_6b();
  void opcode_6c();
  void opcode_6d();
  void opcode_6e();
  void opcode_6f();
  void opcode_70();
  void opcode_71();
  void opcode_72();
  void opcode_73();
  void opcode_74();
  void opcode_75();
  void opcode_76();
  void opcode_77();
  void opcode_78();
  void opcode_79();
  void opcode_7a();
  void opcode_7b();
  void opcode_7c();
  void opcode_7d();
  void opcode_7e();
  void opcode_7f();
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
  void opcode_88();
  void opcode_89();
  void opcode_8a();
  void opcode_8b();
  void opcode_8c();
  void opcode_8d();
  void opcode_8e();
  void opcode_8f();
  void opcode_90();
  void opcode_91();
  void opcode_92();
  void opcode_93();
  void opcode_94();
  void opcode_95();
  void opcode_96();
  void opcode_97();
  void opcode_98();
  void opcode_99();
  void opcode_9a();
  void opcode_9b();
  void opcode_9c();
  void opcode_9d();
  void opcode_9e();
  void opcode_9f();
  void opcode_a0();
  void opcode_a1();
  void opcode_a2();
  void opcode_a3();
  void opcode_a4();
  void opcode_a5();
  void opcode_a6();
  void opcode_a7();
  void opcode_a8();
  void opcode_a9();
  void opcode_aa();
  void opcode_ab();
  void opcode_ac();
  void opcode_ad();
  void opcode_ae();
  void opcode_af();
  void opcode_b0();
  void opcode_b1();
  void opcode_b2();
  void opcode_b3();
  void opcode_b4();
  void opcode_b5();
  void opcode_b6();
  void opcode_b7();
  void opcode_b8();
  void opcode_b9();
  void opcode_ba();
  void opcode_bb();
  void opcode_bc();
  void opcode_bd();
  void opcode_be();
  void opcode_bf();
  void opcode_c0();
  void opcode_c1();
  void opcode_c2();
  void opcode_c3();
  void opcode_c4();
  void opcode_c5();
  void opcode_c6();
  void opcode_c7();
  void opcode_c8();
  void opcode_c9();
  void opcode_ca();
  void opcode_cb();
  void opcode_cc();
  void opcode_cd();
  void opcode_ce();
  void opcode_cf();
  void opcode_d0();
  void opcode_d1();
  void opcode_d2();
  void opcode_d3();
  void opcode_d4();
  void opcode_d5();
  void opcode_d6();
  void opcode_d7();
  void opcode_d8();
  void opcode_d9();
  void opcode_da();
  void opcode_db();
  void opcode_dc();
  void opcode_dd();
  void opcode_de();
  void opcode_df();
  void opcode_e0();
  void opcode_e1();
  void opcode_e2();
  void opcode_e3();
  void opcode_e4();
  void opcode_e5();
  void opcode_e6();
  void opcode_e7();
  void opcode_e8();
  void opcode_e9();
  void opcode_ea();
  void opcode_eb();
  void opcode_ec();
  void opcode_ed();
  void opcode_ee();
  void opcode_ef();
  void opcode_f0();
  void opcode_f1();
  void opcode_f2();
  void opcode_f3();
  void opcode_f4();
  void opcode_f5();
  void opcode_f6();
  void opcode_f7();
  void opcode_f8();
  void opcode_f9();
  void opcode_fa();
  void opcode_fb();
  void opcode_fc();
  void opcode_fd();
  void opcode_fe();
  void opcode_ff();
};

#endif
