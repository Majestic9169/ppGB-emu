/*
 * cb_opcodes
 * cb prefixed opcodes
 * - how do i handle this?
 * - i'll make this a class like opcodes is
 */

#ifndef CB_OPCODES_H
#define CB_OPCODES_H

#include "mmu.hpp"
#include "reg.hpp"
#include <cstdint>

class CB_Opcodes {
private:
  MMU *mmu;
  Registers *reg;
  // RL INSTRUCTIONS
  // rotate THROUGH carry
  void rl(uint8_t &r8) {
    reg->f.n = 0;
    reg->f.h = 0;
    uint8_t tmp = 0 + reg->f.c;
    reg->f.c = (r8 & 0x80) >> 7;
    r8 = (r8 << 1) | tmp;
    reg->set_z(r8);
  }
  // rotate and set top bit to carry
  void rlc(uint8_t &r8) {
    reg->f.n = 0;
    reg->f.h = 0;
    reg->f.c = (r8 & 0x80) >> 7;
    r8 = (r8 << 1) | reg->f.c;
    reg->set_z(r8);
  }
  // RR INSTRUCTIONS
  // rotate and set top bit to carry
  void rrc(uint8_t &r8) {
    reg->f.n = 0;
    reg->f.h = 0;
    reg->f.c = r8 & 0x01;
    r8 = (r8 >> 1) | (reg->f.c << 7);
    reg->set_z(r8);
  }
  // rotate right through carry
  void rr(uint8_t &r8) {
    reg->f.n = 0;
    reg->f.h = 0;
    uint8_t tmp = 0 + reg->f.c;
    reg->f.c = r8 & 0x01;
    r8 = (r8 >> 1) | (tmp << 7);
    reg->set_z(r8);
  }
  // SHIFT INSTRUCTIONS
  void sla(uint8_t &val) {
    reg->f.c = (val & 0x80) >> 7;
    val = val << 1;
    reg->f.n = 0;
    reg->f.h = 0;
    reg->set_z(val);
  }
  void sra(uint8_t &val) {
    reg->f.c = (val & 0x01);
    // c++ didn't sign extend for me, fair because unsigned char
    val = (val >> 1) | (val & 0x80);
    reg->f.n = 0;
    reg->f.h = 0;
    reg->set_z(val);
  }
  void srl(uint8_t &val) {
    reg->f.c = (val & 0x01);
    val = (val >> 1);
    reg->f.n = 0;
    reg->f.h = 0;
    reg->set_z(val);
  }
  // SWAP INSTRUCTIONS
  void swap(uint8_t &val) {
    uint8_t high = (val & 0xf0);
    uint8_t low = (val & 0x0f);
    val = (low << 4) | (high >> 4);
    reg->set_z(val);
    reg->f.n = 0;
    reg->f.h = 0;
    reg->f.c = 0;
  }

public:
  CB_Opcodes(MMU *_mmu, Registers *_reg) : mmu{_mmu}, reg{_reg} {}
  // RLC B
  void cb_opcode_00() { rlc(reg->b); };
  // RLC C
  void cb_opcode_01() { rlc(reg->c); };
  // RLC D
  void cb_opcode_02() { rlc(reg->d); };
  // RLC E
  void cb_opcode_03() { rlc(reg->e); };
  // RLC H
  void cb_opcode_04() { rlc(reg->h); };
  // RLC L
  void cb_opcode_05() { rlc(reg->l); };
  // i made mmu->read_byte() return a reference for this to fit one line
  // if anything breaks you know which opcode to blame
  // RLC (HL)
  void cb_opcode_06() { rlc(mmu->read_byte(reg->hl)); };
  // RLC A
  void cb_opcode_07() { rlc(reg->a); };
  // RRC B
  void cb_opcode_08() { rrc(reg->b); };
  // RRC C
  void cb_opcode_09() { rrc(reg->c); };
  // RRC D
  void cb_opcode_0a() { rrc(reg->d); };
  // RRC E
  void cb_opcode_0b() { rrc(reg->e); };
  // RRC H
  void cb_opcode_0c() { rrc(reg->h); };
  // RRC L
  void cb_opcode_0d() { rrc(reg->l); };
  // RRC (HL)
  void cb_opcode_0e() { rrc(mmu->read_byte(reg->hl)); };
  // RRC A
  void cb_opcode_0f() { rrc(reg->a); };
  // RL B
  void cb_opcode_10() { rl(reg->b); };
  // RL C
  void cb_opcode_11() { rl(reg->c); };
  // RL D
  void cb_opcode_12() { rl(reg->d); };
  // RL E
  void cb_opcode_13() { rl(reg->e); };
  // RL H
  void cb_opcode_14() { rl(reg->h); };
  // RL L
  void cb_opcode_15() { rl(reg->l); };
  // RL (HL)
  void cb_opcode_16() { rl(mmu->read_byte(reg->hl)); };
  // RL A
  void cb_opcode_17() { rl(reg->a); };
  // RR B
  void cb_opcode_18() { rr(reg->b); };
  // RR C
  void cb_opcode_19() { rr(reg->c); };
  // RR D
  void cb_opcode_1a() { rr(reg->d); };
  // RR E
  void cb_opcode_1b() { rr(reg->e); };
  // RR H
  void cb_opcode_1c() { rr(reg->h); };
  // RR L
  void cb_opcode_1d() { rr(reg->l); };
  // RR (HL)
  void cb_opcode_1e() { rr(mmu->read_byte(reg->hl)); };
  // RR A
  void cb_opcode_1f() { rr(reg->a); };
  // SLA B
  void cb_opcode_20() { sla(reg->b); };
  // SLA C
  void cb_opcode_21() { sla(reg->c); };
  // SLA D
  void cb_opcode_22() { sla(reg->d); };
  // SLA E
  void cb_opcode_23() { sla(reg->e); };
  // SLA H
  void cb_opcode_24() { sla(reg->h); };
  // SLA L
  void cb_opcode_25() { sla(reg->l); };
  // SLA (HL)
  void cb_opcode_26() { sla(mmu->read_byte(reg->hl)); };
  // SLA A
  void cb_opcode_27() { sla(reg->a); };
  // SRA B
  void cb_opcode_28() { sra(reg->b); };
  // SRA C
  void cb_opcode_29() { sra(reg->c); };
  // SRA D
  void cb_opcode_2a() { sra(reg->d); };
  // SRA E
  void cb_opcode_2b() { sra(reg->e); };
  // SRA H
  void cb_opcode_2c() { sra(reg->h); };
  // SRA L
  void cb_opcode_2d() { sra(reg->l); };
  // SRA (HL)
  void cb_opcode_2e() { sra(mmu->read_byte(reg->hl)); };
  // SRA A
  void cb_opcode_2f() { sra(reg->a); };
  // SWAP B
  void cb_opcode_30() { swap(reg->b); };
  // SWAP C
  void cb_opcode_31() { swap(reg->c); };
  // SWAP D
  void cb_opcode_32() { swap(reg->d); };
  // SWAP E
  void cb_opcode_33() { swap(reg->e); };
  // SWAP H
  void cb_opcode_34() { swap(reg->h); };
  // SWAP L
  void cb_opcode_35() { swap(reg->l); };
  // SWAP (HL)
  void cb_opcode_36() { swap(mmu->read_byte(reg->hl)); };
  // SWAP A
  void cb_opcode_37() { swap(reg->a); };
  // SRL B
  void cb_opcode_38() { srl(reg->b); };
  // SRL C
  void cb_opcode_39() { srl(reg->c); };
  // SRL D
  void cb_opcode_3a() { srl(reg->d); };
  // SRL E
  void cb_opcode_3b() { srl(reg->e); };
  // SRL H
  void cb_opcode_3c() { srl(reg->h); };
  // SRL L
  void cb_opcode_3d() { srl(reg->l); };
  // SRL (HL)
  void cb_opcode_3e() { srl(mmu->read_byte(reg->hl)); };
  // SRL A
  void cb_opcode_3f() { srl(reg->a); };
  // void cb_opcode_40();
  // void cb_opcode_41();
  // void cb_opcode_42();
  // void cb_opcode_43();
  // void cb_opcode_44();
  // void cb_opcode_45();
  // void cb_opcode_46();
  // void cb_opcode_47();
  // void cb_opcode_48();
  // void cb_opcode_49();
  // void cb_opcode_4a();
  // void cb_opcode_4b();
  // void cb_opcode_4c();
  // void cb_opcode_4d();
  // void cb_opcode_4e();
  // void cb_opcode_4f();
  // void cb_opcode_50();
  // void cb_opcode_51();
  // void cb_opcode_52();
  // void cb_opcode_53();
  // void cb_opcode_54();
  // void cb_opcode_55();
  // void cb_opcode_56();
  // void cb_opcode_57();
  // void cb_opcode_58();
  // void cb_opcode_59();
  // void cb_opcode_5a();
  // void cb_opcode_5b();
  // void cb_opcode_5c();
  // void cb_opcode_5d();
  // void cb_opcode_5e();
  // void cb_opcode_5f();
  // void cb_opcode_60();
  // void cb_opcode_61();
  // void cb_opcode_62();
  // void cb_opcode_63();
  // void cb_opcode_64();
  // void cb_opcode_65();
  // void cb_opcode_66();
  // void cb_opcode_67();
  // void cb_opcode_68();
  // void cb_opcode_69();
  // void cb_opcode_6a();
  // void cb_opcode_6b();
  // void cb_opcode_6c();
  // void cb_opcode_6d();
  // void cb_opcode_6e();
  // void cb_opcode_6f();
  // void cb_opcode_70();
  // void cb_opcode_71();
  // void cb_opcode_72();
  // void cb_opcode_73();
  // void cb_opcode_74();
  // void cb_opcode_75();
  // void cb_opcode_76();
  // void cb_opcode_77();
  // void cb_opcode_78();
  // void cb_opcode_79();
  // void cb_opcode_7a();
  // void cb_opcode_7b();
  // void cb_opcode_7c();
  // void cb_opcode_7d();
  // void cb_opcode_7e();
  // void cb_opcode_7f();
  // void cb_opcode_80();
  // void cb_opcode_81();
  // void cb_opcode_82();
  // void cb_opcode_83();
  // void cb_opcode_84();
  // void cb_opcode_85();
  // void cb_opcode_86();
  // void cb_opcode_87();
  // void cb_opcode_88();
  // void cb_opcode_89();
  // void cb_opcode_8a();
  // void cb_opcode_8b();
  // void cb_opcode_8c();
  // void cb_opcode_8d();
  // void cb_opcode_8e();
  // void cb_opcode_8f();
  // void cb_opcode_90();
  // void cb_opcode_91();
  // void cb_opcode_92();
  // void cb_opcode_93();
  // void cb_opcode_94();
  // void cb_opcode_95();
  // void cb_opcode_96();
  // void cb_opcode_97();
  // void cb_opcode_98();
  // void cb_opcode_99();
  // void cb_opcode_9a();
  // void cb_opcode_9b();
  // void cb_opcode_9c();
  // void cb_opcode_9d();
  // void cb_opcode_9e();
  // void cb_opcode_9f();
  // void cb_opcode_a0();
  // void cb_opcode_a1();
  // void cb_opcode_a2();
  // void cb_opcode_a3();
  // void cb_opcode_a4();
  // void cb_opcode_a5();
  // void cb_opcode_a6();
  // void cb_opcode_a7();
  // void cb_opcode_a8();
  // void cb_opcode_a9();
  // void cb_opcode_aa();
  // void cb_opcode_ab();
  // void cb_opcode_ac();
  // void cb_opcode_ad();
  // void cb_opcode_ae();
  // void cb_opcode_af();
  // void cb_opcode_b0();
  // void cb_opcode_b1();
  // void cb_opcode_b2();
  // void cb_opcode_b3();
  // void cb_opcode_b4();
  // void cb_opcode_b5();
  // void cb_opcode_b6();
  // void cb_opcode_b7();
  // void cb_opcode_b8();
  // void cb_opcode_b9();
  // void cb_opcode_ba();
  // void cb_opcode_bb();
  // void cb_opcode_bc();
  // void cb_opcode_bd();
  // void cb_opcode_be();
  // void cb_opcode_bf();
  // void cb_opcode_c0();
  // void cb_opcode_c1();
  // void cb_opcode_c2();
  // void cb_opcode_c3();
  // void cb_opcode_c4();
  // void cb_opcode_c5();
  // void cb_opcode_c6();
  // void cb_opcode_c7();
  // void cb_opcode_c8();
  // void cb_opcode_c9();
  // void cb_opcode_ca();
  // void cb_opcode_cb();
  // void cb_opcode_cc();
  // void cb_opcode_cd();
  // void cb_opcode_ce();
  // void cb_opcode_cf();
  // void cb_opcode_d0();
  // void cb_opcode_d1();
  // void cb_opcode_d2();
  // void cb_opcode_d3();
  // void cb_opcode_d4();
  // void cb_opcode_d5();
  // void cb_opcode_d6();
  // void cb_opcode_d7();
  // void cb_opcode_d8();
  // void cb_opcode_d9();
  // void cb_opcode_da();
  // void cb_opcode_db();
  // void cb_opcode_dc();
  // void cb_opcode_dd();
  // void cb_opcode_de();
  // void cb_opcode_df();
  // void cb_opcode_e0();
  // void cb_opcode_e1();
  // void cb_opcode_e2();
  // void cb_opcode_e3();
  // void cb_opcode_e4();
  // void cb_opcode_e5();
  // void cb_opcode_e6();
  // void cb_opcode_e7();
  // void cb_opcode_e8();
  // void cb_opcode_e9();
  // void cb_opcode_ea();
  // void cb_opcode_eb();
  // void cb_opcode_ec();
  // void cb_opcode_ed();
  // void cb_opcode_ee();
  // void cb_opcode_ef();
  // void cb_opcode_f0();
  // void cb_opcode_f1();
  // void cb_opcode_f2();
  // void cb_opcode_f3();
  // void cb_opcode_f4();
  // void cb_opcode_f5();
  // void cb_opcode_f6();
  // void cb_opcode_f7();
  // void cb_opcode_f8();
  // void cb_opcode_f9();
  // void cb_opcode_fa();
  // void cb_opcode_fb();
  // void cb_opcode_fc();
  // void cb_opcode_fd();
  // void cb_opcode_fe();
  // void cb_opcode_ff();
};

#endif
