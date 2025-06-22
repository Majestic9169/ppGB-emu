/*
 * CPU
 * - store registers and other state variables
 * - contain processing functions like for flag setting
 * - can access mmu
 * - store opcodes and instructions in separate class
 * - can access ppu
 */

#ifndef CPU_H
#define CPU_H

#include "cb_opcodes.hpp"
#include "cli_opts.hpp"
#include "mmu.hpp"
#include "opcodes.hpp"
#include "reg.hpp"
#include <cstdint>
#include <cstdio>

class CPU {
  // NOTE: this is necessary because c++ class members are initialised in the
  // order they are declared [-Wreorder]
private:
  Opts *cli_opts;
  MMU *mmu;

public:
  Registers reg;

private:
  Opcodes op;
  CB_Opcodes cb;
  uint16_t curr_pc;
  uint16_t opcode;

  void cb_cycle() {
    // TODO: i don't like this, feels hacky
    curr_pc = reg.pc;
    uint8_t next = mmu->read_byte(reg.pc++);
    opcode = 0xcb00 | next;
    switch (next) {
      // clang-format off
      case 0x00: cb.cb_opcode_00(); break;  case 0x01: cb.cb_opcode_01(); break;
      case 0x02: cb.cb_opcode_02(); break;  case 0x03: cb.cb_opcode_03(); break;
      case 0x04: cb.cb_opcode_04(); break;  case 0x05: cb.cb_opcode_05(); break;
      case 0x06: cb.cb_opcode_06(); break;  case 0x07: cb.cb_opcode_07(); break;
      case 0x08: cb.cb_opcode_08(); break;  case 0x09: cb.cb_opcode_09(); break;
      case 0x0a: cb.cb_opcode_0a(); break;  case 0x0b: cb.cb_opcode_0b(); break;
      case 0x0c: cb.cb_opcode_0c(); break;  case 0x0d: cb.cb_opcode_0d(); break;
      case 0x0e: cb.cb_opcode_0e(); break;  case 0x0f: cb.cb_opcode_0f(); break;
      case 0x10: cb.cb_opcode_10(); break;  case 0x11: cb.cb_opcode_11(); break;
      case 0x12: cb.cb_opcode_12(); break;  case 0x13: cb.cb_opcode_13(); break;
      case 0x14: cb.cb_opcode_14(); break;  case 0x15: cb.cb_opcode_15(); break;
      case 0x16: cb.cb_opcode_16(); break;  case 0x17: cb.cb_opcode_17(); break;
      case 0x18: cb.cb_opcode_18(); break;  case 0x19: cb.cb_opcode_19(); break;
      case 0x1a: cb.cb_opcode_1a(); break;  case 0x1b: cb.cb_opcode_1b(); break;
      case 0x1c: cb.cb_opcode_1c(); break;  case 0x1d: cb.cb_opcode_1d(); break;
      case 0x1e: cb.cb_opcode_1e(); break;  case 0x1f: cb.cb_opcode_1f(); break;
      case 0x20: cb.cb_opcode_20(); break;  case 0x21: cb.cb_opcode_21(); break;
      case 0x22: cb.cb_opcode_22(); break;  case 0x23: cb.cb_opcode_23(); break;
      case 0x24: cb.cb_opcode_24(); break;  case 0x25: cb.cb_opcode_25(); break;
      case 0x26: cb.cb_opcode_26(); break;  case 0x27: cb.cb_opcode_27(); break;
      case 0x28: cb.cb_opcode_28(); break;  case 0x29: cb.cb_opcode_29(); break;
      case 0x2a: cb.cb_opcode_2a(); break;  case 0x2b: cb.cb_opcode_2b(); break;
      case 0x2c: cb.cb_opcode_2c(); break;  case 0x2d: cb.cb_opcode_2d(); break;
      case 0x2e: cb.cb_opcode_2e(); break;  case 0x2f: cb.cb_opcode_2f(); break;
      case 0x30: cb.cb_opcode_30(); break;  case 0x31: cb.cb_opcode_31(); break;
      case 0x32: cb.cb_opcode_32(); break;  case 0x33: cb.cb_opcode_33(); break;
      case 0x34: cb.cb_opcode_34(); break;  case 0x35: cb.cb_opcode_35(); break;
      case 0x36: cb.cb_opcode_36(); break;  case 0x37: cb.cb_opcode_37(); break;
      case 0x38: cb.cb_opcode_38(); break;  case 0x39: cb.cb_opcode_39(); break;
      case 0x3a: cb.cb_opcode_3a(); break;  case 0x3b: cb.cb_opcode_3b(); break;
      case 0x3c: cb.cb_opcode_3c(); break;  case 0x3d: cb.cb_opcode_3d(); break;
      case 0x3e: cb.cb_opcode_3e(); break;  case 0x3f: cb.cb_opcode_3f(); break;
      // case 0x40: cb.cb_opcode_40(); break;  case 0x41: cb.cb_opcode_41(); break;
      // case 0x42: cb.cb_opcode_42(); break;  case 0x43: cb.cb_opcode_43(); break;
      // case 0x44: cb.cb_opcode_44(); break;  case 0x45: cb.cb_opcode_45(); break;
      // case 0x46: cb.cb_opcode_46(); break;  case 0x47: cb.cb_opcode_47(); break;
      // case 0x48: cb.cb_opcode_48(); break;  case 0x49: cb.cb_opcode_49(); break;
      // case 0x4a: cb.cb_opcode_4a(); break;  case 0x4b: cb.cb_opcode_4b(); break;
      // case 0x4c: cb.cb_opcode_4c(); break;  case 0x4d: cb.cb_opcode_4d(); break;
      // case 0x4e: cb.cb_opcode_4e(); break;  case 0x4f: cb.cb_opcode_4f(); break;
      // case 0x50: cb.cb_opcode_50(); break;  case 0x51: cb.cb_opcode_51(); break;
      // case 0x52: cb.cb_opcode_52(); break;  case 0x53: cb.cb_opcode_53(); break;
      // case 0x54: cb.cb_opcode_54(); break;  case 0x55: cb.cb_opcode_55(); break;
      // case 0x56: cb.cb_opcode_56(); break;  case 0x57: cb.cb_opcode_57(); break;
      // case 0x58: cb.cb_opcode_58(); break;  case 0x59: cb.cb_opcode_59(); break;
      // case 0x5a: cb.cb_opcode_5a(); break;  case 0x5b: cb.cb_opcode_5b(); break;
      // case 0x5c: cb.cb_opcode_5c(); break;  case 0x5d: cb.cb_opcode_5d(); break;
      // case 0x5e: cb.cb_opcode_5e(); break;  case 0x5f: cb.cb_opcode_5f(); break;
      // case 0x60: cb.cb_opcode_60(); break;  case 0x61: cb.cb_opcode_61(); break;
      // case 0x62: cb.cb_opcode_62(); break;  case 0x63: cb.cb_opcode_63(); break;
      // case 0x64: cb.cb_opcode_64(); break;  case 0x65: cb.cb_opcode_65(); break;
      // case 0x66: cb.cb_opcode_66(); break;  case 0x67: cb.cb_opcode_67(); break;
      // case 0x68: cb.cb_opcode_68(); break;  case 0x69: cb.cb_opcode_69(); break;
      // case 0x6a: cb.cb_opcode_6a(); break;  case 0x6b: cb.cb_opcode_6b(); break;
      // case 0x6c: cb.cb_opcode_6c(); break;  case 0x6d: cb.cb_opcode_6d(); break;
      // case 0x6e: cb.cb_opcode_6e(); break;  case 0x6f: cb.cb_opcode_6f(); break;
      // case 0x70: cb.cb_opcode_70(); break;  case 0x71: cb.cb_opcode_71(); break;
      // case 0x72: cb.cb_opcode_72(); break;  case 0x73: cb.cb_opcode_73(); break;
      // case 0x74: cb.cb_opcode_74(); break;  case 0x75: cb.cb_opcode_75(); break;
      // case 0x76: cb.cb_opcode_76(); break;  case 0x77: cb.cb_opcode_77(); break;
      // case 0x78: cb.cb_opcode_78(); break;  case 0x79: cb.cb_opcode_79(); break;
      // case 0x7a: cb.cb_opcode_7a(); break;  case 0x7b: cb.cb_opcode_7b(); break;
      // case 0x7c: cb.cb_opcode_7c(); break;  case 0x7d: cb.cb_opcode_7d(); break;
      // case 0x7e: cb.cb_opcode_7e(); break;  case 0x7f: cb.cb_opcode_7f(); break;
      // case 0x80: cb.cb_opcode_80(); break;  case 0x81: cb.cb_opcode_81(); break;
      // case 0x82: cb.cb_opcode_82(); break;  case 0x83: cb.cb_opcode_83(); break;
      // case 0x84: cb.cb_opcode_84(); break;  case 0x85: cb.cb_opcode_85(); break;
      // case 0x86: cb.cb_opcode_86(); break;  case 0x87: cb.cb_opcode_87(); break;
      // case 0x88: cb.cb_opcode_88(); break;  case 0x89: cb.cb_opcode_89(); break;
      // case 0x8a: cb.cb_opcode_8a(); break;  case 0x8b: cb.cb_opcode_8b(); break;
      // case 0x8c: cb.cb_opcode_8c(); break;  case 0x8d: cb.cb_opcode_8d(); break;
      // case 0x8e: cb.cb_opcode_8e(); break;  case 0x8f: cb.cb_opcode_8f(); break;
      // case 0x90: cb.cb_opcode_90(); break;  case 0x91: cb.cb_opcode_91(); break;
      // case 0x92: cb.cb_opcode_92(); break;  case 0x93: cb.cb_opcode_93(); break;
      // case 0x94: cb.cb_opcode_94(); break;  case 0x95: cb.cb_opcode_95(); break;
      // case 0x96: cb.cb_opcode_96(); break;  case 0x97: cb.cb_opcode_97(); break;
      // case 0x98: cb.cb_opcode_98(); break;  case 0x99: cb.cb_opcode_99(); break;
      // case 0x9a: cb.cb_opcode_9a(); break;  case 0x9b: cb.cb_opcode_9b(); break;
      // case 0x9c: cb.cb_opcode_9c(); break;  case 0x9d: cb.cb_opcode_9d(); break;
      // case 0x9e: cb.cb_opcode_9e(); break;  case 0x9f: cb.cb_opcode_9f(); break;
      // case 0xa0: cb.cb_opcode_a0(); break;  case 0xa1: cb.cb_opcode_a1(); break;
      // case 0xa2: cb.cb_opcode_a2(); break;  case 0xa3: cb.cb_opcode_a3(); break;
      // case 0xa4: cb.cb_opcode_a4(); break;  case 0xa5: cb.cb_opcode_a5(); break;
      // case 0xa6: cb.cb_opcode_a6(); break;  case 0xa7: cb.cb_opcode_a7(); break;
      // case 0xa8: cb.cb_opcode_a8(); break;  case 0xa9: cb.cb_opcode_a9(); break;
      // case 0xaa: cb.cb_opcode_aa(); break;  case 0xab: cb.cb_opcode_ab(); break;
      // case 0xac: cb.cb_opcode_ac(); break;  case 0xad: cb.cb_opcode_ad(); break;
      // case 0xae: cb.cb_opcode_ae(); break;  case 0xaf: cb.cb_opcode_af(); break;
      // case 0xb0: cb.cb_opcode_b0(); break;  case 0xb1: cb.cb_opcode_b1(); break;
      // case 0xb2: cb.cb_opcode_b2(); break;  case 0xb3: cb.cb_opcode_b3(); break;
      // case 0xb4: cb.cb_opcode_b4(); break;  case 0xb5: cb.cb_opcode_b5(); break;
      // case 0xb6: cb.cb_opcode_b6(); break;  case 0xb7: cb.cb_opcode_b7(); break;
      // case 0xb8: cb.cb_opcode_b8(); break;  case 0xb9: cb.cb_opcode_b9(); break;
      // case 0xba: cb.cb_opcode_ba(); break;  case 0xbb: cb.cb_opcode_bb(); break;
      // case 0xbc: cb.cb_opcode_bc(); break;  case 0xbd: cb.cb_opcode_bd(); break;
      // case 0xbe: cb.cb_opcode_be(); break;  case 0xbf: cb.cb_opcode_bf(); break;
      // case 0xc0: cb.cb_opcode_c0(); break;  case 0xc1: cb.cb_opcode_c1(); break;
      // case 0xc2: cb.cb_opcode_c2(); break;  case 0xc3: cb.cb_opcode_c3(); break;
      // case 0xc4: cb.cb_opcode_c4(); break;  case 0xc5: cb.cb_opcode_c5(); break;
      // case 0xc6: cb.cb_opcode_c6(); break;  case 0xc7: cb.cb_opcode_c7(); break;
      // case 0xc8: cb.cb_opcode_c8(); break;  case 0xc9: cb.cb_opcode_c9(); break;
      // case 0xca: cb.cb_opcode_ca(); break;  case 0xcb: cb.cb_opcode_cb(); break;
      // case 0xcc: cb.cb_opcode_cc(); break;  case 0xcd: cb.cb_opcode_cd(); break;
      // case 0xce: cb.cb_opcode_ce(); break;  case 0xcf: cb.cb_opcode_cf(); break;
      // case 0xd0: cb.cb_opcode_d0(); break;  case 0xd1: cb.cb_opcode_d1(); break;
      // case 0xd2: cb.cb_opcode_d2(); break;  case 0xd3: cb.cb_opcode_d3(); break;
      // case 0xd4: cb.cb_opcode_d4(); break;  case 0xd5: cb.cb_opcode_d5(); break;
      // case 0xd6: cb.cb_opcode_d6(); break;  case 0xd7: cb.cb_opcode_d7(); break;
      // case 0xd8: cb.cb_opcode_d8(); break;  case 0xd9: cb.cb_opcode_d9(); break;
      // case 0xda: cb.cb_opcode_da(); break;  case 0xdb: cb.cb_opcode_db(); break;
      // case 0xdc: cb.cb_opcode_dc(); break;  case 0xdd: cb.cb_opcode_dd(); break;
      // case 0xde: cb.cb_opcode_de(); break;  case 0xdf: cb.cb_opcode_df(); break;
      // case 0xe0: cb.cb_opcode_e0(); break;  case 0xe1: cb.cb_opcode_e1(); break;
      // case 0xe2: cb.cb_opcode_e2(); break;  case 0xe3: cb.cb_opcode_e3(); break;
      // case 0xe4: cb.cb_opcode_e4(); break;  case 0xe5: cb.cb_opcode_e5(); break;
      // case 0xe6: cb.cb_opcode_e6(); break;  case 0xe7: cb.cb_opcode_e7(); break;
      // case 0xe8: cb.cb_opcode_e8(); break;  case 0xe9: cb.cb_opcode_e9(); break;
      // case 0xea: cb.cb_opcode_ea(); break;  case 0xeb: cb.cb_opcode_eb(); break;
      // case 0xec: cb.cb_opcode_ec(); break;  case 0xed: cb.cb_opcode_ed(); break;
      // case 0xee: cb.cb_opcode_ee(); break;  case 0xef: cb.cb_opcode_ef(); break;
      // case 0xf0: cb.cb_opcode_f0(); break;  case 0xf1: cb.cb_opcode_f1(); break;
      // case 0xf2: cb.cb_opcode_f2(); break;  case 0xf3: cb.cb_opcode_f3(); break;
      // case 0xf4: cb.cb_opcode_f4(); break;  case 0xf5: cb.cb_opcode_f5(); break;
      // case 0xf6: cb.cb_opcode_f6(); break;  case 0xf7: cb.cb_opcode_f7(); break;
      // case 0xf8: cb.cb_opcode_f8(); break;  case 0xf9: cb.cb_opcode_f9(); break;
      // case 0xfa: cb.cb_opcode_fa(); break;  case 0xfb: cb.cb_opcode_fb(); break;
      // case 0xfc: cb.cb_opcode_fc(); break;  case 0xfd: cb.cb_opcode_fd(); break;
      // case 0xfe: cb.cb_opcode_fe(); break;  case 0xff: cb.cb_opcode_ff(); break;
      // clang-format on
    }
  }

public:
  CPU(Opts *cli, MMU *_mmu)
      : cli_opts(cli), mmu(_mmu), reg{}, op{_mmu, &reg}, cb(_mmu, &reg),
        curr_pc{}, opcode{} {};

  // TODO: add cpu cycle clocks array and handle when opcodes have 2-3 possible
  // values

  // fetch-decode-execute cycle
  void cpu_step() {
    curr_pc = reg.pc;
    opcode = mmu->read_byte(reg.pc++);
    switch (opcode) {
      // clang-format off
      case 0x00: op.opcode_00(); break;  case 0x01: op.opcode_01(); break;
      case 0x02: op.opcode_02(); break;  case 0x03: op.opcode_03(); break;
      case 0x04: op.opcode_04(); break;  case 0x05: op.opcode_05(); break;
      case 0x06: op.opcode_06(); break;  case 0x07: op.opcode_07(); break;
      case 0x08: op.opcode_08(); break;  case 0x09: op.opcode_09(); break;
      case 0x0a: op.opcode_0a(); break;  case 0x0b: op.opcode_0b(); break;
      case 0x0c: op.opcode_0c(); break;  case 0x0d: op.opcode_0d(); break;
      case 0x0e: op.opcode_0e(); break;  case 0x0f: op.opcode_0f(); break;
      case 0x10: op.opcode_10(); break;  case 0x11: op.opcode_11(); break;
      case 0x12: op.opcode_12(); break;  case 0x13: op.opcode_13(); break;
      case 0x14: op.opcode_14(); break;  case 0x15: op.opcode_15(); break;
      case 0x16: op.opcode_16(); break;  case 0x17: op.opcode_17(); break;
      case 0x18: op.opcode_18(); break;  case 0x19: op.opcode_19(); break;
      case 0x1a: op.opcode_1a(); break;  case 0x1b: op.opcode_1b(); break;
      case 0x1c: op.opcode_1c(); break;  case 0x1d: op.opcode_1d(); break;
      case 0x1e: op.opcode_1e(); break;  case 0x1f: op.opcode_1f(); break;
      case 0x20: op.opcode_20(); break;  case 0x21: op.opcode_21(); break;
      case 0x22: op.opcode_22(); break;  case 0x23: op.opcode_23(); break;
      case 0x24: op.opcode_24(); break;  case 0x25: op.opcode_25(); break;
      case 0x26: op.opcode_26(); break;  case 0x27: op.opcode_27(); break;
      case 0x28: op.opcode_28(); break;  case 0x29: op.opcode_29(); break;
      case 0x2a: op.opcode_2a(); break;  case 0x2b: op.opcode_2b(); break;
      case 0x2c: op.opcode_2c(); break;  case 0x2d: op.opcode_2d(); break;
      case 0x2e: op.opcode_2e(); break;  case 0x2f: op.opcode_2f(); break;
      case 0x30: op.opcode_30(); break;  case 0x31: op.opcode_31(); break;
      case 0x32: op.opcode_32(); break;  case 0x33: op.opcode_33(); break;
      case 0x34: op.opcode_34(); break;  case 0x35: op.opcode_35(); break;
      case 0x36: op.opcode_36(); break;  case 0x37: op.opcode_37(); break;
      case 0x38: op.opcode_38(); break;  case 0x39: op.opcode_39(); break;
      case 0x3a: op.opcode_3a(); break;  case 0x3b: op.opcode_3b(); break;
      case 0x3c: op.opcode_3c(); break;  case 0x3d: op.opcode_3d(); break;
      case 0x3e: op.opcode_3e(); break;  case 0x3f: op.opcode_3f(); break;
      case 0x40: op.opcode_40(); break;  case 0x41: op.opcode_41(); break;
      case 0x42: op.opcode_42(); break;  case 0x43: op.opcode_43(); break;
      case 0x44: op.opcode_44(); break;  case 0x45: op.opcode_45(); break;
      case 0x46: op.opcode_46(); break;  case 0x47: op.opcode_47(); break;
      case 0x48: op.opcode_48(); break;  case 0x49: op.opcode_49(); break;
      case 0x4a: op.opcode_4a(); break;  case 0x4b: op.opcode_4b(); break;
      case 0x4c: op.opcode_4c(); break;  case 0x4d: op.opcode_4d(); break;
      case 0x4e: op.opcode_4e(); break;  case 0x4f: op.opcode_4f(); break;
      case 0x50: op.opcode_50(); break;  case 0x51: op.opcode_51(); break;
      case 0x52: op.opcode_52(); break;  case 0x53: op.opcode_53(); break;
      case 0x54: op.opcode_54(); break;  case 0x55: op.opcode_55(); break;
      case 0x56: op.opcode_56(); break;  case 0x57: op.opcode_57(); break;
      case 0x58: op.opcode_58(); break;  case 0x59: op.opcode_59(); break;
      case 0x5a: op.opcode_5a(); break;  case 0x5b: op.opcode_5b(); break;
      case 0x5c: op.opcode_5c(); break;  case 0x5d: op.opcode_5d(); break;
      case 0x5e: op.opcode_5e(); break;  case 0x5f: op.opcode_5f(); break;
      case 0x60: op.opcode_60(); break;  case 0x61: op.opcode_61(); break;
      case 0x62: op.opcode_62(); break;  case 0x63: op.opcode_63(); break;
      case 0x64: op.opcode_64(); break;  case 0x65: op.opcode_65(); break;
      case 0x66: op.opcode_66(); break;  case 0x67: op.opcode_67(); break;
      case 0x68: op.opcode_68(); break;  case 0x69: op.opcode_69(); break;
      case 0x6a: op.opcode_6a(); break;  case 0x6b: op.opcode_6b(); break;
      case 0x6c: op.opcode_6c(); break;  case 0x6d: op.opcode_6d(); break;
      case 0x6e: op.opcode_6e(); break;  case 0x6f: op.opcode_6f(); break;
      case 0x70: op.opcode_70(); break;  case 0x71: op.opcode_71(); break;
      case 0x72: op.opcode_72(); break;  case 0x73: op.opcode_73(); break;
      case 0x74: op.opcode_74(); break;  case 0x75: op.opcode_75(); break;
      case 0x76: op.opcode_76(); break;  case 0x77: op.opcode_77(); break;
      case 0x78: op.opcode_78(); break;  case 0x79: op.opcode_79(); break;
      case 0x7a: op.opcode_7a(); break;  case 0x7b: op.opcode_7b(); break;
      case 0x7c: op.opcode_7c(); break;  case 0x7d: op.opcode_7d(); break;
      case 0x7e: op.opcode_7e(); break;  case 0x7f: op.opcode_7f(); break;
      case 0x80: op.opcode_80(); break;  case 0x81: op.opcode_81(); break;
      case 0x82: op.opcode_82(); break;  case 0x83: op.opcode_83(); break;
      case 0x84: op.opcode_84(); break;  case 0x85: op.opcode_85(); break;
      case 0x86: op.opcode_86(); break;  case 0x87: op.opcode_87(); break;
      case 0x88: op.opcode_88(); break;  case 0x89: op.opcode_89(); break;
      case 0x8a: op.opcode_8a(); break;  case 0x8b: op.opcode_8b(); break;
      case 0x8c: op.opcode_8c(); break;  case 0x8d: op.opcode_8d(); break;
      case 0x8e: op.opcode_8e(); break;  case 0x8f: op.opcode_8f(); break;
      case 0x90: op.opcode_90(); break;  case 0x91: op.opcode_91(); break;
      case 0x92: op.opcode_92(); break;  case 0x93: op.opcode_93(); break;
      case 0x94: op.opcode_94(); break;  case 0x95: op.opcode_95(); break;
      case 0x96: op.opcode_96(); break;  case 0x97: op.opcode_97(); break;
      case 0x98: op.opcode_98(); break;  case 0x99: op.opcode_99(); break;
      case 0x9a: op.opcode_9a(); break;  case 0x9b: op.opcode_9b(); break;
      case 0x9c: op.opcode_9c(); break;  case 0x9d: op.opcode_9d(); break;
      case 0x9e: op.opcode_9e(); break;  case 0x9f: op.opcode_9f(); break;
      case 0xa0: op.opcode_a0(); break;  case 0xa1: op.opcode_a1(); break;
      case 0xa2: op.opcode_a2(); break;  case 0xa3: op.opcode_a3(); break;
      case 0xa4: op.opcode_a4(); break;  case 0xa5: op.opcode_a5(); break;
      case 0xa6: op.opcode_a6(); break;  case 0xa7: op.opcode_a7(); break;
      case 0xa8: op.opcode_a8(); break;  case 0xa9: op.opcode_a9(); break;
      case 0xaa: op.opcode_aa(); break;  case 0xab: op.opcode_ab(); break;
      case 0xac: op.opcode_ac(); break;  case 0xad: op.opcode_ad(); break;
      case 0xae: op.opcode_ae(); break;  case 0xaf: op.opcode_af(); break;
      case 0xb0: op.opcode_b0(); break;  case 0xb1: op.opcode_b1(); break;
      case 0xb2: op.opcode_b2(); break;  case 0xb3: op.opcode_b3(); break;
      case 0xb4: op.opcode_b4(); break;  case 0xb5: op.opcode_b5(); break;
      case 0xb6: op.opcode_b6(); break;  case 0xb7: op.opcode_b7(); break;
      case 0xb8: op.opcode_b8(); break;  case 0xb9: op.opcode_b9(); break;
      case 0xba: op.opcode_ba(); break;  case 0xbb: op.opcode_bb(); break;
      case 0xbc: op.opcode_bc(); break;  case 0xbd: op.opcode_bd(); break;
      case 0xbe: op.opcode_be(); break;  case 0xbf: op.opcode_bf(); break;
      case 0xc0: op.opcode_c0(); break;  case 0xc1: op.opcode_c1(); break;
      case 0xc2: op.opcode_c2(); break;  case 0xc3: op.opcode_c3(); break;
      case 0xc4: op.opcode_c4(); break;  case 0xc5: op.opcode_c5(); break;
      case 0xc6: op.opcode_c6(); break;  case 0xc7: op.opcode_c7(); break;
      case 0xc8: op.opcode_c8(); break;  case 0xc9: op.opcode_c9(); break;
      case 0xca: op.opcode_ca(); break;  case 0xcb: cb_cycle(); break;
      case 0xcc: op.opcode_cc(); break;  case 0xcd: op.opcode_cd(); break;
      case 0xce: op.opcode_ce(); break;  case 0xcf: op.opcode_cf(); break;
      case 0xd0: op.opcode_d0(); break;  case 0xd1: op.opcode_d1(); break;
      case 0xd2: op.opcode_d2(); break;  case 0xd3: op.opcode_d3(); break;
      case 0xd4: op.opcode_d4(); break;  case 0xd5: op.opcode_d5(); break;
      case 0xd6: op.opcode_d6(); break;  case 0xd7: op.opcode_d7(); break;
      case 0xd8: op.opcode_d8(); break;  case 0xd9: op.opcode_d9(); break;
      case 0xda: op.opcode_da(); break;  case 0xdb: op.opcode_db(); break;
      case 0xdc: op.opcode_dc(); break;  case 0xdd: op.opcode_dd(); break;
      case 0xde: op.opcode_de(); break;  case 0xdf: op.opcode_df(); break;
      case 0xe0: op.opcode_e0(); break;  case 0xe1: op.opcode_e1(); break;
      case 0xe2: op.opcode_e2(); break;  case 0xe3: op.opcode_e3(); break;
      case 0xe4: op.opcode_e4(); break;  case 0xe5: op.opcode_e5(); break;
      case 0xe6: op.opcode_e6(); break;  case 0xe7: op.opcode_e7(); break;
      case 0xe8: op.opcode_e8(); break;  case 0xe9: op.opcode_e9(); break;
      case 0xea: op.opcode_ea(); break;  case 0xeb: op.opcode_eb(); break;
      case 0xec: op.opcode_ec(); break;  case 0xed: op.opcode_ed(); break;
      case 0xee: op.opcode_ee(); break;  case 0xef: op.opcode_ef(); break;
      case 0xf0: op.opcode_f0(); break;  case 0xf1: op.opcode_f1(); break;
      case 0xf2: op.opcode_f2(); break;  case 0xf3: op.opcode_f3(); break;
      case 0xf4: op.opcode_f4(); break;  case 0xf5: op.opcode_f5(); break;
      case 0xf6: op.opcode_f6(); break;  case 0xf7: op.opcode_f7(); break;
      case 0xf8: op.opcode_f8(); break;  case 0xf9: op.opcode_f9(); break;
      case 0xfa: op.opcode_fa(); break;  case 0xfb: op.opcode_fb(); break;
      case 0xfc: op.opcode_fc(); break;  case 0xfd: op.opcode_fd(); break;
      case 0xfe: op.opcode_fe(); break;  case 0xff: op.opcode_ff(); break;
      // clang-format on
    }
    if (cli_opts->debug_enabled()) {
      // TODO: write this to file instead of stdout
      std::printf("%s[%04x] %sopcode 0x%02x\n%s", YEL.c_str(), curr_pc,
                  GRN.c_str(), opcode, COLOR_RESET.c_str());
    }
  }
};

#endif
