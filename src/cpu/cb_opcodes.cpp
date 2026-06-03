/*
 * cb_opcodes
 * cb prefixed opcodes
 * - how do i handle this?
 * - i'll make this a class like opcodes is
 */

#include "../../include/cpu/cb_opcodes.hpp"
#include <cstdint>

// RL INSTRUCTIONS
// rotate THROUGH carry
void CB_Opcodes::rl(uint8_t &r8) {
  reg->f.n = 0;
  reg->f.h = 0;
  uint8_t tmp = 0 + reg->f.c;
  reg->f.c = (r8 & 0x80) >> 7;
  r8 = (r8 << 1) | tmp;
  reg->set_z(r8);
}
// rotate and set top bit to carry
void CB_Opcodes::rlc(uint8_t &r8) {
  reg->f.n = 0;
  reg->f.h = 0;
  reg->f.c = (r8 & 0x80) >> 7;
  r8 = (r8 << 1) | reg->f.c;
  reg->set_z(r8);
}
// RR INSTRUCTIONS
// rotate and set top bit to carry
void CB_Opcodes::rrc(uint8_t &r8) {
  reg->f.n = 0;
  reg->f.h = 0;
  reg->f.c = r8 & 0x01;
  r8 = (r8 >> 1) | (reg->f.c << 7);
  reg->set_z(r8);
}
// rotate right through carry
void CB_Opcodes::rr(uint8_t &r8) {
  reg->f.n = 0;
  reg->f.h = 0;
  uint8_t tmp = 0 + reg->f.c;
  reg->f.c = r8 & 0x01;
  r8 = (r8 >> 1) | (tmp << 7);
  reg->set_z(r8);
}
// SHIFT INSTRUCTIONS
void CB_Opcodes::sla(uint8_t &val) {
  reg->f.c = (val & 0x80) >> 7;
  val = val << 1;
  reg->f.n = 0;
  reg->f.h = 0;
  reg->set_z(val);
}
void CB_Opcodes::sra(uint8_t &val) {
  reg->f.c = (val & 0x01);
  // c++ didn't sign extend for me, fair because unsigned char
  val = (val >> 1) | (val & 0x80);
  reg->f.n = 0;
  reg->f.h = 0;
  reg->set_z(val);
}
void CB_Opcodes::srl(uint8_t &val) {
  reg->f.c = (val & 0x01);
  val = (val >> 1);
  reg->f.n = 0;
  reg->f.h = 0;
  reg->set_z(val);
}
// SWAP INSTRUCTIONS
void CB_Opcodes::swap(uint8_t &val) {
  uint8_t high = (val & 0xf0);
  uint8_t low = (val & 0x0f);
  val = (low << 4) | (high >> 4);
  reg->set_z(val);
  reg->f.n = 0;
  reg->f.h = 0;
  reg->f.c = 0;
}
// BIT INSTRUCTIONS
void CB_Opcodes::bit(uint8_t u3, uint8_t &r8) {
  reg->set_z(r8 & (1 << u3));
  reg->f.n = 0;
  reg->f.h = 1;
}
// RES INSTRUCTIONS
void CB_Opcodes::res(uint8_t u3, uint8_t &r8) {
  uint8_t mask = ~(1 << u3);
  r8 = r8 & mask;
}
// SET INSTRUCTIONS
void CB_Opcodes::set(uint8_t u3, uint8_t &r8) {
  uint8_t mask = (1 << u3);
  r8 = r8 | mask;
}

CB_Opcodes::CB_Opcodes(MMU *_mmu, Registers *_reg) : mmu{_mmu}, reg{_reg} {}
// RLC B
void CB_Opcodes::cb_opcode_00() { rlc(reg->b); };
// RLC C
void CB_Opcodes::cb_opcode_01() { rlc(reg->c); };
// RLC D
void CB_Opcodes::cb_opcode_02() { rlc(reg->d); };
// RLC E
void CB_Opcodes::cb_opcode_03() { rlc(reg->e); };
// RLC H
void CB_Opcodes::cb_opcode_04() { rlc(reg->h); };
// RLC L
void CB_Opcodes::cb_opcode_05() { rlc(reg->l); };
// i made mmu->read_byte() return a reference for this to fit one line
// if anything breaks you know which opcode to blame
// RLC (HL)
void CB_Opcodes::cb_opcode_06() { rlc(mmu->read_byte(reg->hl)); };
// RLC A
void CB_Opcodes::cb_opcode_07() { rlc(reg->a); };
// RRC B
void CB_Opcodes::cb_opcode_08() { rrc(reg->b); };
// RRC C
void CB_Opcodes::cb_opcode_09() { rrc(reg->c); };
// RRC D
void CB_Opcodes::cb_opcode_0a() { rrc(reg->d); };
// RRC E
void CB_Opcodes::cb_opcode_0b() { rrc(reg->e); };
// RRC H
void CB_Opcodes::cb_opcode_0c() { rrc(reg->h); };
// RRC L
void CB_Opcodes::cb_opcode_0d() { rrc(reg->l); };
// RRC (HL)
void CB_Opcodes::cb_opcode_0e() { rrc(mmu->read_byte(reg->hl)); };
// RRC A
void CB_Opcodes::cb_opcode_0f() { rrc(reg->a); };
// RL B
void CB_Opcodes::cb_opcode_10() { rl(reg->b); };
// RL C
void CB_Opcodes::cb_opcode_11() { rl(reg->c); };
// RL D
void CB_Opcodes::cb_opcode_12() { rl(reg->d); };
// RL E
void CB_Opcodes::cb_opcode_13() { rl(reg->e); };
// RL H
void CB_Opcodes::cb_opcode_14() { rl(reg->h); };
// RL L
void CB_Opcodes::cb_opcode_15() { rl(reg->l); };
// RL (HL)
void CB_Opcodes::cb_opcode_16() { rl(mmu->read_byte(reg->hl)); };
// RL A
void CB_Opcodes::cb_opcode_17() { rl(reg->a); };
// RR B
void CB_Opcodes::cb_opcode_18() { rr(reg->b); };
// RR C
void CB_Opcodes::cb_opcode_19() { rr(reg->c); };
// RR D
void CB_Opcodes::cb_opcode_1a() { rr(reg->d); };
// RR E
void CB_Opcodes::cb_opcode_1b() { rr(reg->e); };
// RR H
void CB_Opcodes::cb_opcode_1c() { rr(reg->h); };
// RR L
void CB_Opcodes::cb_opcode_1d() { rr(reg->l); };
// RR (HL)
void CB_Opcodes::cb_opcode_1e() { rr(mmu->read_byte(reg->hl)); };
// RR A
void CB_Opcodes::cb_opcode_1f() { rr(reg->a); };
// SLA B
void CB_Opcodes::cb_opcode_20() { sla(reg->b); };
// SLA C
void CB_Opcodes::cb_opcode_21() { sla(reg->c); };
// SLA D
void CB_Opcodes::cb_opcode_22() { sla(reg->d); };
// SLA E
void CB_Opcodes::cb_opcode_23() { sla(reg->e); };
// SLA H
void CB_Opcodes::cb_opcode_24() { sla(reg->h); };
// SLA L
void CB_Opcodes::cb_opcode_25() { sla(reg->l); };
// SLA (HL)
void CB_Opcodes::cb_opcode_26() { sla(mmu->read_byte(reg->hl)); };
// SLA A
void CB_Opcodes::cb_opcode_27() { sla(reg->a); };
// SRA B
void CB_Opcodes::cb_opcode_28() { sra(reg->b); };
// SRA C
void CB_Opcodes::cb_opcode_29() { sra(reg->c); };
// SRA D
void CB_Opcodes::cb_opcode_2a() { sra(reg->d); };
// SRA E
void CB_Opcodes::cb_opcode_2b() { sra(reg->e); };
// SRA H
void CB_Opcodes::cb_opcode_2c() { sra(reg->h); };
// SRA L
void CB_Opcodes::cb_opcode_2d() { sra(reg->l); };
// SRA (HL)
void CB_Opcodes::cb_opcode_2e() { sra(mmu->read_byte(reg->hl)); };
// SRA A
void CB_Opcodes::cb_opcode_2f() { sra(reg->a); };
// SWAP B
void CB_Opcodes::cb_opcode_30() { swap(reg->b); };
// SWAP C
void CB_Opcodes::cb_opcode_31() { swap(reg->c); };
// SWAP D
void CB_Opcodes::cb_opcode_32() { swap(reg->d); };
// SWAP E
void CB_Opcodes::cb_opcode_33() { swap(reg->e); };
// SWAP H
void CB_Opcodes::cb_opcode_34() { swap(reg->h); };
// SWAP L
void CB_Opcodes::cb_opcode_35() { swap(reg->l); };
// SWAP (HL)
void CB_Opcodes::cb_opcode_36() { swap(mmu->read_byte(reg->hl)); };
// SWAP A
void CB_Opcodes::cb_opcode_37() { swap(reg->a); };
// SRL B
void CB_Opcodes::cb_opcode_38() { srl(reg->b); };
// SRL C
void CB_Opcodes::cb_opcode_39() { srl(reg->c); };
// SRL D
void CB_Opcodes::cb_opcode_3a() { srl(reg->d); };
// SRL E
void CB_Opcodes::cb_opcode_3b() { srl(reg->e); };
// SRL H
void CB_Opcodes::cb_opcode_3c() { srl(reg->h); };
// SRL L
void CB_Opcodes::cb_opcode_3d() { srl(reg->l); };
// SRL (HL)
void CB_Opcodes::cb_opcode_3e() { srl(mmu->read_byte(reg->hl)); };
// SRL A
void CB_Opcodes::cb_opcode_3f() { srl(reg->a); };
void CB_Opcodes::cb_opcode_40() { bit(0, reg->b); };
void CB_Opcodes::cb_opcode_41() { bit(0, reg->c); };
void CB_Opcodes::cb_opcode_42() { bit(0, reg->d); };
void CB_Opcodes::cb_opcode_43() { bit(0, reg->e); };
void CB_Opcodes::cb_opcode_44() { bit(0, reg->h); };
void CB_Opcodes::cb_opcode_45() { bit(0, reg->l); };
void CB_Opcodes::cb_opcode_46() { bit(0, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_47() { bit(0, reg->a); };
void CB_Opcodes::cb_opcode_48() { bit(1, reg->b); };
void CB_Opcodes::cb_opcode_49() { bit(1, reg->c); };
void CB_Opcodes::cb_opcode_4a() { bit(1, reg->d); };
void CB_Opcodes::cb_opcode_4b() { bit(1, reg->e); };
void CB_Opcodes::cb_opcode_4c() { bit(1, reg->h); };
void CB_Opcodes::cb_opcode_4d() { bit(1, reg->l); };
void CB_Opcodes::cb_opcode_4e() { bit(1, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_4f() { bit(1, reg->a); };
void CB_Opcodes::cb_opcode_50() { bit(2, reg->b); };
void CB_Opcodes::cb_opcode_51() { bit(2, reg->c); };
void CB_Opcodes::cb_opcode_52() { bit(2, reg->d); };
void CB_Opcodes::cb_opcode_53() { bit(2, reg->e); };
void CB_Opcodes::cb_opcode_54() { bit(2, reg->h); };
void CB_Opcodes::cb_opcode_55() { bit(2, reg->l); };
void CB_Opcodes::cb_opcode_56() { bit(2, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_57() { bit(2, reg->a); };
void CB_Opcodes::cb_opcode_58() { bit(3, reg->b); };
void CB_Opcodes::cb_opcode_59() { bit(3, reg->c); };
void CB_Opcodes::cb_opcode_5a() { bit(3, reg->d); };
void CB_Opcodes::cb_opcode_5b() { bit(3, reg->e); };
void CB_Opcodes::cb_opcode_5c() { bit(3, reg->h); };
void CB_Opcodes::cb_opcode_5d() { bit(3, reg->l); };
void CB_Opcodes::cb_opcode_5e() { bit(3, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_5f() { bit(3, reg->a); };
void CB_Opcodes::cb_opcode_60() { bit(4, reg->b); };
void CB_Opcodes::cb_opcode_61() { bit(4, reg->c); };
void CB_Opcodes::cb_opcode_62() { bit(4, reg->d); };
void CB_Opcodes::cb_opcode_63() { bit(4, reg->e); };
void CB_Opcodes::cb_opcode_64() { bit(4, reg->h); };
void CB_Opcodes::cb_opcode_65() { bit(4, reg->l); };
void CB_Opcodes::cb_opcode_66() { bit(4, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_67() { bit(4, reg->a); };
void CB_Opcodes::cb_opcode_68() { bit(5, reg->b); };
void CB_Opcodes::cb_opcode_69() { bit(5, reg->c); };
void CB_Opcodes::cb_opcode_6a() { bit(5, reg->d); };
void CB_Opcodes::cb_opcode_6b() { bit(5, reg->e); };
void CB_Opcodes::cb_opcode_6c() { bit(5, reg->h); };
void CB_Opcodes::cb_opcode_6d() { bit(5, reg->l); };
void CB_Opcodes::cb_opcode_6e() { bit(5, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_6f() { bit(5, reg->a); };
void CB_Opcodes::cb_opcode_70() { bit(6, reg->b); };
void CB_Opcodes::cb_opcode_71() { bit(6, reg->c); };
void CB_Opcodes::cb_opcode_72() { bit(6, reg->d); };
void CB_Opcodes::cb_opcode_73() { bit(6, reg->e); };
void CB_Opcodes::cb_opcode_74() { bit(6, reg->h); };
void CB_Opcodes::cb_opcode_75() { bit(6, reg->l); };
void CB_Opcodes::cb_opcode_76() { bit(6, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_77() { bit(6, reg->a); };
void CB_Opcodes::cb_opcode_78() { bit(7, reg->b); };
void CB_Opcodes::cb_opcode_79() { bit(7, reg->c); };
void CB_Opcodes::cb_opcode_7a() { bit(7, reg->d); };
void CB_Opcodes::cb_opcode_7b() { bit(7, reg->e); };
void CB_Opcodes::cb_opcode_7c() { bit(7, reg->h); };
void CB_Opcodes::cb_opcode_7d() { bit(7, reg->l); };
void CB_Opcodes::cb_opcode_7e() { bit(7, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_7f() { bit(7, reg->a); };
void CB_Opcodes::cb_opcode_80() { res(0, reg->b); };
void CB_Opcodes::cb_opcode_81() { res(0, reg->c); };
void CB_Opcodes::cb_opcode_82() { res(0, reg->d); };
void CB_Opcodes::cb_opcode_83() { res(0, reg->e); };
void CB_Opcodes::cb_opcode_84() { res(0, reg->h); };
void CB_Opcodes::cb_opcode_85() { res(0, reg->l); };
void CB_Opcodes::cb_opcode_86() { res(0, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_87() { res(0, reg->a); };
void CB_Opcodes::cb_opcode_88() { res(1, reg->b); };
void CB_Opcodes::cb_opcode_89() { res(1, reg->c); };
void CB_Opcodes::cb_opcode_8a() { res(1, reg->d); };
void CB_Opcodes::cb_opcode_8b() { res(1, reg->e); };
void CB_Opcodes::cb_opcode_8c() { res(1, reg->h); };
void CB_Opcodes::cb_opcode_8d() { res(1, reg->l); };
void CB_Opcodes::cb_opcode_8e() { res(1, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_8f() { res(1, reg->a); };
void CB_Opcodes::cb_opcode_90() { res(2, reg->b); };
void CB_Opcodes::cb_opcode_91() { res(2, reg->c); };
void CB_Opcodes::cb_opcode_92() { res(2, reg->d); };
void CB_Opcodes::cb_opcode_93() { res(2, reg->e); };
void CB_Opcodes::cb_opcode_94() { res(2, reg->h); };
void CB_Opcodes::cb_opcode_95() { res(2, reg->l); };
void CB_Opcodes::cb_opcode_96() { res(2, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_97() { res(2, reg->a); };
void CB_Opcodes::cb_opcode_98() { res(3, reg->b); };
void CB_Opcodes::cb_opcode_99() { res(3, reg->c); };
void CB_Opcodes::cb_opcode_9a() { res(3, reg->d); };
void CB_Opcodes::cb_opcode_9b() { res(3, reg->e); };
void CB_Opcodes::cb_opcode_9c() { res(3, reg->h); };
void CB_Opcodes::cb_opcode_9d() { res(3, reg->l); };
void CB_Opcodes::cb_opcode_9e() { res(3, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_9f() { res(3, reg->a); };
void CB_Opcodes::cb_opcode_a0() { res(4, reg->b); };
void CB_Opcodes::cb_opcode_a1() { res(4, reg->c); };
void CB_Opcodes::cb_opcode_a2() { res(4, reg->d); };
void CB_Opcodes::cb_opcode_a3() { res(4, reg->e); };
void CB_Opcodes::cb_opcode_a4() { res(4, reg->h); };
void CB_Opcodes::cb_opcode_a5() { res(4, reg->l); };
void CB_Opcodes::cb_opcode_a6() { res(4, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_a7() { res(4, reg->a); };
void CB_Opcodes::cb_opcode_a8() { res(5, reg->b); };
void CB_Opcodes::cb_opcode_a9() { res(5, reg->c); };
void CB_Opcodes::cb_opcode_aa() { res(5, reg->d); };
void CB_Opcodes::cb_opcode_ab() { res(5, reg->e); };
void CB_Opcodes::cb_opcode_ac() { res(5, reg->h); };
void CB_Opcodes::cb_opcode_ad() { res(5, reg->l); };
void CB_Opcodes::cb_opcode_ae() { res(5, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_af() { res(5, reg->a); };
void CB_Opcodes::cb_opcode_b0() { res(6, reg->b); };
void CB_Opcodes::cb_opcode_b1() { res(6, reg->c); };
void CB_Opcodes::cb_opcode_b2() { res(6, reg->d); };
void CB_Opcodes::cb_opcode_b3() { res(6, reg->e); };
void CB_Opcodes::cb_opcode_b4() { res(6, reg->h); };
void CB_Opcodes::cb_opcode_b5() { res(6, reg->l); };
void CB_Opcodes::cb_opcode_b6() { res(6, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_b7() { res(6, reg->a); };
void CB_Opcodes::cb_opcode_b8() { res(7, reg->b); };
void CB_Opcodes::cb_opcode_b9() { res(7, reg->c); };
void CB_Opcodes::cb_opcode_ba() { res(7, reg->d); };
void CB_Opcodes::cb_opcode_bb() { res(7, reg->e); };
void CB_Opcodes::cb_opcode_bc() { res(7, reg->h); };
void CB_Opcodes::cb_opcode_bd() { res(7, reg->l); };
void CB_Opcodes::cb_opcode_be() { res(7, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_bf() { res(7, reg->a); };
void CB_Opcodes::cb_opcode_c0() { set(0, reg->b); };
void CB_Opcodes::cb_opcode_c1() { set(0, reg->c); };
void CB_Opcodes::cb_opcode_c2() { set(0, reg->d); };
void CB_Opcodes::cb_opcode_c3() { set(0, reg->e); };
void CB_Opcodes::cb_opcode_c4() { set(0, reg->h); };
void CB_Opcodes::cb_opcode_c5() { set(0, reg->l); };
void CB_Opcodes::cb_opcode_c6() { set(0, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_c7() { set(0, reg->a); };
void CB_Opcodes::cb_opcode_c8() { set(1, reg->b); };
void CB_Opcodes::cb_opcode_c9() { set(1, reg->c); };
void CB_Opcodes::cb_opcode_ca() { set(1, reg->d); };
void CB_Opcodes::cb_opcode_cb() { set(1, reg->e); };
void CB_Opcodes::cb_opcode_cc() { set(1, reg->h); };
void CB_Opcodes::cb_opcode_cd() { set(1, reg->l); };
void CB_Opcodes::cb_opcode_ce() { set(1, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_cf() { set(1, reg->a); };
void CB_Opcodes::cb_opcode_d0() { set(2, reg->b); };
void CB_Opcodes::cb_opcode_d1() { set(2, reg->c); };
void CB_Opcodes::cb_opcode_d2() { set(2, reg->d); };
void CB_Opcodes::cb_opcode_d3() { set(2, reg->e); };
void CB_Opcodes::cb_opcode_d4() { set(2, reg->h); };
void CB_Opcodes::cb_opcode_d5() { set(2, reg->l); };
void CB_Opcodes::cb_opcode_d6() { set(2, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_d7() { set(2, reg->a); };
void CB_Opcodes::cb_opcode_d8() { set(3, reg->b); };
void CB_Opcodes::cb_opcode_d9() { set(3, reg->c); };
void CB_Opcodes::cb_opcode_da() { set(3, reg->d); };
void CB_Opcodes::cb_opcode_db() { set(3, reg->e); };
void CB_Opcodes::cb_opcode_dc() { set(3, reg->h); };
void CB_Opcodes::cb_opcode_dd() { set(3, reg->l); };
void CB_Opcodes::cb_opcode_de() { set(3, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_df() { set(3, reg->a); };
void CB_Opcodes::cb_opcode_e0() { set(4, reg->b); };
void CB_Opcodes::cb_opcode_e1() { set(4, reg->c); };
void CB_Opcodes::cb_opcode_e2() { set(4, reg->d); };
void CB_Opcodes::cb_opcode_e3() { set(4, reg->e); };
void CB_Opcodes::cb_opcode_e4() { set(4, reg->h); };
void CB_Opcodes::cb_opcode_e5() { set(4, reg->l); };
void CB_Opcodes::cb_opcode_e6() { set(4, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_e7() { set(4, reg->a); };
void CB_Opcodes::cb_opcode_e8() { set(5, reg->b); };
void CB_Opcodes::cb_opcode_e9() { set(5, reg->c); };
void CB_Opcodes::cb_opcode_ea() { set(5, reg->d); };
void CB_Opcodes::cb_opcode_eb() { set(5, reg->e); };
void CB_Opcodes::cb_opcode_ec() { set(5, reg->h); };
void CB_Opcodes::cb_opcode_ed() { set(5, reg->l); };
void CB_Opcodes::cb_opcode_ee() { set(5, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_ef() { set(5, reg->a); };
void CB_Opcodes::cb_opcode_f0() { set(6, reg->b); };
void CB_Opcodes::cb_opcode_f1() { set(6, reg->c); };
void CB_Opcodes::cb_opcode_f2() { set(6, reg->d); };
void CB_Opcodes::cb_opcode_f3() { set(6, reg->e); };
void CB_Opcodes::cb_opcode_f4() { set(6, reg->h); };
void CB_Opcodes::cb_opcode_f5() { set(6, reg->l); };
void CB_Opcodes::cb_opcode_f6() { set(6, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_f7() { set(6, reg->a); };
void CB_Opcodes::cb_opcode_f8() { set(7, reg->b); };
void CB_Opcodes::cb_opcode_f9() { set(7, reg->c); };
void CB_Opcodes::cb_opcode_fa() { set(7, reg->d); };
void CB_Opcodes::cb_opcode_fb() { set(7, reg->e); };
void CB_Opcodes::cb_opcode_fc() { set(7, reg->h); };
void CB_Opcodes::cb_opcode_fd() { set(7, reg->l); };
void CB_Opcodes::cb_opcode_fe() { set(7, mmu->read_byte(reg->hl)); };
void CB_Opcodes::cb_opcode_ff() { set(7, reg->a); };

void CB_Opcodes::test() {
  switch (mmu->read_byte(reg->pc++)) {
    // clang-format off
      case 0x00: cb_opcode_00(); break;  case 0x01: cb_opcode_01(); break;
      case 0x02: cb_opcode_02(); break;  case 0x03: cb_opcode_03(); break;
      case 0x04: cb_opcode_04(); break;  case 0x05: cb_opcode_05(); break;
      case 0x06: cb_opcode_06(); break;  case 0x07: cb_opcode_07(); break;
      case 0x08: cb_opcode_08(); break;  case 0x09: cb_opcode_09(); break;
      case 0x0a: cb_opcode_0a(); break;  case 0x0b: cb_opcode_0b(); break;
      case 0x0c: cb_opcode_0c(); break;  case 0x0d: cb_opcode_0d(); break;
      case 0x0e: cb_opcode_0e(); break;  case 0x0f: cb_opcode_0f(); break;
      case 0x10: cb_opcode_10(); break;  case 0x11: cb_opcode_11(); break;
      case 0x12: cb_opcode_12(); break;  case 0x13: cb_opcode_13(); break;
      case 0x14: cb_opcode_14(); break;  case 0x15: cb_opcode_15(); break;
      case 0x16: cb_opcode_16(); break;  case 0x17: cb_opcode_17(); break;
      case 0x18: cb_opcode_18(); break;  case 0x19: cb_opcode_19(); break;
      case 0x1a: cb_opcode_1a(); break;  case 0x1b: cb_opcode_1b(); break;
      case 0x1c: cb_opcode_1c(); break;  case 0x1d: cb_opcode_1d(); break;
      case 0x1e: cb_opcode_1e(); break;  case 0x1f: cb_opcode_1f(); break;
      case 0x20: cb_opcode_20(); break;  case 0x21: cb_opcode_21(); break;
      case 0x22: cb_opcode_22(); break;  case 0x23: cb_opcode_23(); break;
      case 0x24: cb_opcode_24(); break;  case 0x25: cb_opcode_25(); break;
      case 0x26: cb_opcode_26(); break;  case 0x27: cb_opcode_27(); break;
      case 0x28: cb_opcode_28(); break;  case 0x29: cb_opcode_29(); break;
      case 0x2a: cb_opcode_2a(); break;  case 0x2b: cb_opcode_2b(); break;
      case 0x2c: cb_opcode_2c(); break;  case 0x2d: cb_opcode_2d(); break;
      case 0x2e: cb_opcode_2e(); break;  case 0x2f: cb_opcode_2f(); break;
      case 0x30: cb_opcode_30(); break;  case 0x31: cb_opcode_31(); break;
      case 0x32: cb_opcode_32(); break;  case 0x33: cb_opcode_33(); break;
      case 0x34: cb_opcode_34(); break;  case 0x35: cb_opcode_35(); break;
      case 0x36: cb_opcode_36(); break;  case 0x37: cb_opcode_37(); break;
      case 0x38: cb_opcode_38(); break;  case 0x39: cb_opcode_39(); break;
      case 0x3a: cb_opcode_3a(); break;  case 0x3b: cb_opcode_3b(); break;
      case 0x3c: cb_opcode_3c(); break;  case 0x3d: cb_opcode_3d(); break;
      case 0x3e: cb_opcode_3e(); break;  case 0x3f: cb_opcode_3f(); break;
      case 0x40: cb_opcode_40(); break;  case 0x41: cb_opcode_41(); break;
      case 0x42: cb_opcode_42(); break;  case 0x43: cb_opcode_43(); break;
      case 0x44: cb_opcode_44(); break;  case 0x45: cb_opcode_45(); break;
      case 0x46: cb_opcode_46(); break;  case 0x47: cb_opcode_47(); break;
      case 0x48: cb_opcode_48(); break;  case 0x49: cb_opcode_49(); break;
      case 0x4a: cb_opcode_4a(); break;  case 0x4b: cb_opcode_4b(); break;
      case 0x4c: cb_opcode_4c(); break;  case 0x4d: cb_opcode_4d(); break;
      case 0x4e: cb_opcode_4e(); break;  case 0x4f: cb_opcode_4f(); break;
      case 0x50: cb_opcode_50(); break;  case 0x51: cb_opcode_51(); break;
      case 0x52: cb_opcode_52(); break;  case 0x53: cb_opcode_53(); break;
      case 0x54: cb_opcode_54(); break;  case 0x55: cb_opcode_55(); break;
      case 0x56: cb_opcode_56(); break;  case 0x57: cb_opcode_57(); break;
      case 0x58: cb_opcode_58(); break;  case 0x59: cb_opcode_59(); break;
      case 0x5a: cb_opcode_5a(); break;  case 0x5b: cb_opcode_5b(); break;
      case 0x5c: cb_opcode_5c(); break;  case 0x5d: cb_opcode_5d(); break;
      case 0x5e: cb_opcode_5e(); break;  case 0x5f: cb_opcode_5f(); break;
      case 0x60: cb_opcode_60(); break;  case 0x61: cb_opcode_61(); break;
      case 0x62: cb_opcode_62(); break;  case 0x63: cb_opcode_63(); break;
      case 0x64: cb_opcode_64(); break;  case 0x65: cb_opcode_65(); break;
      case 0x66: cb_opcode_66(); break;  case 0x67: cb_opcode_67(); break;
      case 0x68: cb_opcode_68(); break;  case 0x69: cb_opcode_69(); break;
      case 0x6a: cb_opcode_6a(); break;  case 0x6b: cb_opcode_6b(); break;
      case 0x6c: cb_opcode_6c(); break;  case 0x6d: cb_opcode_6d(); break;
      case 0x6e: cb_opcode_6e(); break;  case 0x6f: cb_opcode_6f(); break;
      case 0x70: cb_opcode_70(); break;  case 0x71: cb_opcode_71(); break;
      case 0x72: cb_opcode_72(); break;  case 0x73: cb_opcode_73(); break;
      case 0x74: cb_opcode_74(); break;  case 0x75: cb_opcode_75(); break;
      case 0x76: cb_opcode_76(); break;  case 0x77: cb_opcode_77(); break;
      case 0x78: cb_opcode_78(); break;  case 0x79: cb_opcode_79(); break;
      case 0x7a: cb_opcode_7a(); break;  case 0x7b: cb_opcode_7b(); break;
      case 0x7c: cb_opcode_7c(); break;  case 0x7d: cb_opcode_7d(); break;
      case 0x7e: cb_opcode_7e(); break;  case 0x7f: cb_opcode_7f(); break;
      case 0x80: cb_opcode_80(); break;  case 0x81: cb_opcode_81(); break;
      case 0x82: cb_opcode_82(); break;  case 0x83: cb_opcode_83(); break;
      case 0x84: cb_opcode_84(); break;  case 0x85: cb_opcode_85(); break;
      case 0x86: cb_opcode_86(); break;  case 0x87: cb_opcode_87(); break;
      case 0x88: cb_opcode_88(); break;  case 0x89: cb_opcode_89(); break;
      case 0x8a: cb_opcode_8a(); break;  case 0x8b: cb_opcode_8b(); break;
      case 0x8c: cb_opcode_8c(); break;  case 0x8d: cb_opcode_8d(); break;
      case 0x8e: cb_opcode_8e(); break;  case 0x8f: cb_opcode_8f(); break;
      case 0x90: cb_opcode_90(); break;  case 0x91: cb_opcode_91(); break;
      case 0x92: cb_opcode_92(); break;  case 0x93: cb_opcode_93(); break;
      case 0x94: cb_opcode_94(); break;  case 0x95: cb_opcode_95(); break;
      case 0x96: cb_opcode_96(); break;  case 0x97: cb_opcode_97(); break;
      case 0x98: cb_opcode_98(); break;  case 0x99: cb_opcode_99(); break;
      case 0x9a: cb_opcode_9a(); break;  case 0x9b: cb_opcode_9b(); break;
      case 0x9c: cb_opcode_9c(); break;  case 0x9d: cb_opcode_9d(); break;
      case 0x9e: cb_opcode_9e(); break;  case 0x9f: cb_opcode_9f(); break;
      case 0xa0: cb_opcode_a0(); break;  case 0xa1: cb_opcode_a1(); break;
      case 0xa2: cb_opcode_a2(); break;  case 0xa3: cb_opcode_a3(); break;
      case 0xa4: cb_opcode_a4(); break;  case 0xa5: cb_opcode_a5(); break;
      case 0xa6: cb_opcode_a6(); break;  case 0xa7: cb_opcode_a7(); break;
      case 0xa8: cb_opcode_a8(); break;  case 0xa9: cb_opcode_a9(); break;
      case 0xaa: cb_opcode_aa(); break;  case 0xab: cb_opcode_ab(); break;
      case 0xac: cb_opcode_ac(); break;  case 0xad: cb_opcode_ad(); break;
      case 0xae: cb_opcode_ae(); break;  case 0xaf: cb_opcode_af(); break;
      case 0xb0: cb_opcode_b0(); break;  case 0xb1: cb_opcode_b1(); break;
      case 0xb2: cb_opcode_b2(); break;  case 0xb3: cb_opcode_b3(); break;
      case 0xb4: cb_opcode_b4(); break;  case 0xb5: cb_opcode_b5(); break;
      case 0xb6: cb_opcode_b6(); break;  case 0xb7: cb_opcode_b7(); break;
      case 0xb8: cb_opcode_b8(); break;  case 0xb9: cb_opcode_b9(); break;
      case 0xba: cb_opcode_ba(); break;  case 0xbb: cb_opcode_bb(); break;
      case 0xbc: cb_opcode_bc(); break;  case 0xbd: cb_opcode_bd(); break;
      case 0xbe: cb_opcode_be(); break;  case 0xbf: cb_opcode_bf(); break;
      case 0xc0: cb_opcode_c0(); break;  case 0xc1: cb_opcode_c1(); break;
      case 0xc2: cb_opcode_c2(); break;  case 0xc3: cb_opcode_c3(); break;
      case 0xc4: cb_opcode_c4(); break;  case 0xc5: cb_opcode_c5(); break;
      case 0xc6: cb_opcode_c6(); break;  case 0xc7: cb_opcode_c7(); break;
      case 0xc8: cb_opcode_c8(); break;  case 0xc9: cb_opcode_c9(); break;
      case 0xca: cb_opcode_ca(); break;  case 0xcb: cb_opcode_cb(); break;
      case 0xcc: cb_opcode_cc(); break;  case 0xcd: cb_opcode_cd(); break;
      case 0xce: cb_opcode_ce(); break;  case 0xcf: cb_opcode_cf(); break;
      case 0xd0: cb_opcode_d0(); break;  case 0xd1: cb_opcode_d1(); break;
      case 0xd2: cb_opcode_d2(); break;  case 0xd3: cb_opcode_d3(); break;
      case 0xd4: cb_opcode_d4(); break;  case 0xd5: cb_opcode_d5(); break;
      case 0xd6: cb_opcode_d6(); break;  case 0xd7: cb_opcode_d7(); break;
      case 0xd8: cb_opcode_d8(); break;  case 0xd9: cb_opcode_d9(); break;
      case 0xda: cb_opcode_da(); break;  case 0xdb: cb_opcode_db(); break;
      case 0xdc: cb_opcode_dc(); break;  case 0xdd: cb_opcode_dd(); break;
      case 0xde: cb_opcode_de(); break;  case 0xdf: cb_opcode_df(); break;
      case 0xe0: cb_opcode_e0(); break;  case 0xe1: cb_opcode_e1(); break;
      case 0xe2: cb_opcode_e2(); break;  case 0xe3: cb_opcode_e3(); break;
      case 0xe4: cb_opcode_e4(); break;  case 0xe5: cb_opcode_e5(); break;
      case 0xe6: cb_opcode_e6(); break;  case 0xe7: cb_opcode_e7(); break;
      case 0xe8: cb_opcode_e8(); break;  case 0xe9: cb_opcode_e9(); break;
      case 0xea: cb_opcode_ea(); break;  case 0xeb: cb_opcode_eb(); break;
      case 0xec: cb_opcode_ec(); break;  case 0xed: cb_opcode_ed(); break;
      case 0xee: cb_opcode_ee(); break;  case 0xef: cb_opcode_ef(); break;
      case 0xf0: cb_opcode_f0(); break;  case 0xf1: cb_opcode_f1(); break;
      case 0xf2: cb_opcode_f2(); break;  case 0xf3: cb_opcode_f3(); break;
      case 0xf4: cb_opcode_f4(); break;  case 0xf5: cb_opcode_f5(); break;
      case 0xf6: cb_opcode_f6(); break;  case 0xf7: cb_opcode_f7(); break;
      case 0xf8: cb_opcode_f8(); break;  case 0xf9: cb_opcode_f9(); break;
      case 0xfa: cb_opcode_fa(); break;  case 0xfb: cb_opcode_fb(); break;
      case 0xfc: cb_opcode_fc(); break;  case 0xfd: cb_opcode_fd(); break;
      case 0xfe: cb_opcode_fe(); break;  case 0xff: cb_opcode_ff(); break;
    // clang-format on
  }
}
