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
  void unimplemented();
  uint16_t read_word_from_pc();
  void cpl();
  void ccf();
  void scf();
  void daa();

  // LDH INSTRUCTIONS
  void ldh_u8_a(uint8_t u8);
  void ldh_a_u8(uint8_t u8);
  void ld_hl_sp();

  // INC INSTRUCTIONS Z0H-
  void inc(uint8_t &val);
  void inc_16(uint16_t addr);

  // DEC INSTRUCTIONS Z1H-
  void dec(uint8_t &val);
  void dec_16(uint16_t addr);

  // ROTATE INSTRUCTIONS 000C
  void rlca();
  void rla();
  void rrca();
  void rra();

  // ADD INSTRUCTIONS
  void add_hl(uint16_t r16);
  void add(uint8_t val);
  void add_sp(signed char val);
  // NOTE: half carry can be confusing because it feels like it depends on the
  // order of addition
  // https://gbdev.gg8.se/wiki/articles/ADC
  void adc(uint8_t val);

  // SUB INSTRUCTIONS
  // TODO: add sub/sbc unit tests. don't feel like it rn
  // all the rest of the ALU too
  void sub(uint8_t val);
  void sbc(uint8_t val);

  // AND INSTRUCTIONS
  // NOTE: "and" is a keyword in c++ :sob:
  void and_(uint8_t val);

  // XOR INSTRUCTION
  // NOTE: "xor" too :sob:
  void xor_(uint8_t val);

  // OR INSTRUCTION
  void or_(uint8_t val);

  // CP INSTRUCTIONS
  void cp(uint8_t val);

  // JR INSTRUCTIONS
  void jr(bool condition);

  // JP INSTRUCTIONS
  void jp(bool condition, uint16_t val);

  // POP INSTRUCTIONS
  void pop(uint16_t &r16);

  // PUSH INSTRUCTIONS
  void push(uint16_t val);

  // RET INSTRUCTIONS
  // TODO: implement diff cycles for taken/untaken
  // ret is diff from ret taken/not taken for some reason
  void ret(bool condition);
  void reti();

  // CALL INSTRUCTIONS
  void call(bool condition);

  // RST INSTRUCTIONS
  void rst(uint16_t addr);

public:
  Opcodes(MMU *_mmu, Registers *_reg);
  void opcode_00();
  void opcode_01();
  void opcode_02();
  void opcode_03();
  void opcode_04();
  void opcode_05();
  void opcode_06();
  void opcode_07();
  void opcode_08();
  void opcode_09();
  void opcode_0a();
  void opcode_0b();
  void opcode_0c();
  void opcode_0d();
  void opcode_0e();
  void opcode_0f();
  void opcode_10();
  void opcode_11();
  void opcode_12();
  void opcode_13();
  void opcode_14();
  void opcode_15();
  void opcode_16();
  void opcode_17();
  void opcode_18();
  void opcode_19();
  void opcode_1a();
  void opcode_1b();
  void opcode_1c();
  void opcode_1d();
  void opcode_1e();
  void opcode_1f();
  void opcode_20();
  void opcode_21();
  void opcode_22();
  void opcode_23();
  void opcode_24();
  void opcode_25();
  void opcode_26();
  void opcode_27();
  void opcode_28();
  void opcode_29();
  void opcode_2a();
  void opcode_2b();
  void opcode_2c();
  void opcode_2d();
  void opcode_2e();
  void opcode_2f();
  void opcode_30();
  void opcode_31();
  void opcode_32();
  void opcode_33();
  void opcode_34();
  void opcode_35();
  void opcode_36();
  void opcode_37();
  void opcode_38();
  void opcode_39();
  void opcode_3a();
  void opcode_3b();
  void opcode_3c();
  void opcode_3d();
  void opcode_3e();
  void opcode_3f();
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
  void opcode_80();
  void opcode_81();
  void opcode_82();
  void opcode_83();
  void opcode_84();
  void opcode_85();
  void opcode_86();
  void opcode_87();
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
