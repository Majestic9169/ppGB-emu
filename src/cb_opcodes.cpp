#include "../include/cpu.h"
#include <cstdint>
#include <cstdio>
#include <ctime>

const char *cb_disasm[256] = {
    "0x%04x: [cb00] RLC B",       // 0x00
    "0x%04x: [cb01] RLC C",       // 0x01
    "0x%04x: [cb02] RLC D",       // 0x02
    "0x%04x: [cb03] RLC E",       // 0x03
    "0x%04x: [cb04] RLC H",       // 0x04
    "0x%04x: [cb05] RLC L",       // 0x05
    "0x%04x: [cb06] RLC (HL)",    // 0x06
    "0x%04x: [cb07] RLC A",       // 0x07
    "0x%04x: [cb08] RRC B",       // 0x08
    "0x%04x: [cb09] RRC C",       // 0x09
    "0x%04x: [cb0a] RRC D",       // 0x0a
    "0x%04x: [cb0b] RRC E",       // 0x0b
    "0x%04x: [cb0c] RRC H",       // 0x0c
    "0x%04x: [cb0d] RRC L",       // 0x0d
    "0x%04x: [cb0e] RRC (HL)",    // 0x0e
    "0x%04x: [cb0f] RRC A",       // 0x0f
    "0x%04x: [cb10] RL B",        // 0x10
    "0x%04x: [cb11] RL C",        // 0x11
    "0x%04x: [cb12] RL D",        // 0x12
    "0x%04x: [cb13] RL E",        // 0x13
    "0x%04x: [cb14] RL H",        // 0x14
    "0x%04x: [cb15] RL L",        // 0x15
    "0x%04x: [cb16] RL (HL)",     // 0x16
    "0x%04x: [cb17] RL A",        // 0x17
    "0x%04x: [cb18] RR B",        // 0x18
    "0x%04x: [cb19] RR C",        // 0x19
    "0x%04x: [cb1a] RR D",        // 0x1a
    "0x%04x: [cb1b] RR E",        // 0x1b
    "0x%04x: [cb1c] RR H",        // 0x1c
    "0x%04x: [cb1d] RR L",        // 0x1d
    "0x%04x: [cb1e] RR (HL)",     // 0x1e
    "0x%04x: [cb1f] RR A",        // 0x1f
    "0x%04x: [cb20] SLA B",       // 0x20
    "0x%04x: [cb21] SLA C",       // 0x21
    "0x%04x: [cb22] SLA D",       // 0x22
    "0x%04x: [cb23] SLA E",       // 0x23
    "0x%04x: [cb24] SLA H",       // 0x24
    "0x%04x: [cb25] SLA L",       // 0x25
    "0x%04x: [cb26] SLA (HL)",    // 0x26
    "0x%04x: [cb27] SLA A",       // 0x27
    "0x%04x: [cb28] SRA B",       // 0x28
    "0x%04x: [cb29] SRA C",       // 0x29
    "0x%04x: [cb2a] SRA D",       // 0x2a
    "0x%04x: [cb2b] SRA E",       // 0x2b
    "0x%04x: [cb2c] SRA H",       // 0x2c
    "0x%04x: [cb2d] SRA L",       // 0x2d
    "0x%04x: [cb2e] SRA (HL)",    // 0x2e
    "0x%04x: [cb2f] SRA A",       // 0x2f
    "0x%04x: [cb30] SWAP B",      // 0x30
    "0x%04x: [cb31] SWAP C",      // 0x31
    "0x%04x: [cb32] SWAP D",      // 0x32
    "0x%04x: [cb33] SWAP E",      // 0x33
    "0x%04x: [cb34] SWAP H",      // 0x34
    "0x%04x: [cb35] SWAP L",      // 0x35
    "0x%04x: [cb36] SWAP (HL)",   // 0x36
    "0x%04x: [cb37] SWAP A",      // 0x37
    "0x%04x: [cb38] SRL B",       // 0x38
    "0x%04x: [cb39] SRL C",       // 0x39
    "0x%04x: [cb3a] SRL D",       // 0x3a
    "0x%04x: [cb3b] SRL E",       // 0x3b
    "0x%04x: [cb3c] SRL H",       // 0x3c
    "0x%04x: [cb3d] SRL L",       // 0x3d
    "0x%04x: [cb3e] SRL (HL)",    // 0x3e
    "0x%04x: [cb3f] SRL A",       // 0x3f
    "0x%04x: [cb40] BIT 0, B",    // 0x40
    "0x%04x: [cb41] BIT 0, C",    // 0x41
    "0x%04x: [cb42] BIT 0, D",    // 0x42
    "0x%04x: [cb43] BIT 0, E",    // 0x43
    "0x%04x: [cb44] BIT 0, H",    // 0x44
    "0x%04x: [cb45] BIT 0, L",    // 0x45
    "0x%04x: [cb46] BIT 0, (HL)", // 0x46
    "0x%04x: [cb47] BIT 0, A",    // 0x47
    "0x%04x: [cb48] BIT 1, B",    // 0x48
    "0x%04x: [cb49] BIT 1, C",    // 0x49
    "0x%04x: [cb4a] BIT 1, D",    // 0x4a
    "0x%04x: [cb4b] BIT 1, E",    // 0x4b
    "0x%04x: [cb4c] BIT 1, H",    // 0x4c
    "0x%04x: [cb4d] BIT 1, L",    // 0x4d
    "0x%04x: [cb4e] BIT 1, (HL)", // 0x4e
    "0x%04x: [cb4f] BIT 1, A",    // 0x4f
    "0x%04x: [cb50] BIT 2, B",    // 0x50
    "0x%04x: [cb51] BIT 2, C",    // 0x51
    "0x%04x: [cb52] BIT 2, D",    // 0x52
    "0x%04x: [cb53] BIT 2, E",    // 0x53
    "0x%04x: [cb54] BIT 2, H",    // 0x54
    "0x%04x: [cb55] BIT 2, L",    // 0x55
    "0x%04x: [cb56] BIT 2, (HL)", // 0x56
    "0x%04x: [cb57] BIT 2, A",    // 0x57
    "0x%04x: [cb58] BIT 3, B",    // 0x58
    "0x%04x: [cb59] BIT 3, C",    // 0x59
    "0x%04x: [cb5a] BIT 3, D",    // 0x5a
    "0x%04x: [cb5b] BIT 3, E",    // 0x5b
    "0x%04x: [cb5c] BIT 3, H",    // 0x5c
    "0x%04x: [cb5d] BIT 3, L",    // 0x5d
    "0x%04x: [cb5e] BIT 3, (HL)", // 0x5e
    "0x%04x: [cb5f] BIT 3, A",    // 0x5f
    "0x%04x: [cb60] BIT 4, B",    // 0x60
    "0x%04x: [cb61] BIT 4, C",    // 0x61
    "0x%04x: [cb62] BIT 4, D",    // 0x62
    "0x%04x: [cb63] BIT 4, E",    // 0x63
    "0x%04x: [cb64] BIT 4, H",    // 0x64
    "0x%04x: [cb65] BIT 4, L",    // 0x65
    "0x%04x: [cb66] BIT 4, (HL)", // 0x66
    "0x%04x: [cb67] BIT 4, A",    // 0x67
    "0x%04x: [cb68] BIT 5, B",    // 0x68
    "0x%04x: [cb69] BIT 5, C",    // 0x69
    "0x%04x: [cb6a] BIT 5, D",    // 0x6a
    "0x%04x: [cb6b] BIT 5, E",    // 0x6b
    "0x%04x: [cb6c] BIT 6, H",    // 0x6c
    "0x%04x: [cb6d] BIT 6, L",    // 0x6d
    "0x%04x: [cb6e] BIT 5, (HL)", // 0x6e
    "0x%04x: [cb6f] BIT 5, A",    // 0x6f
    "0x%04x: [cb70] BIT 6, B",    // 0x70
    "0x%04x: [cb71] BIT 6, C",    // 0x71
    "0x%04x: [cb72] BIT 6, D",    // 0x72
    "0x%04x: [cb73] BIT 6, E",    // 0x73
    "0x%04x: [cb74] BIT 6, H",    // 0x74
    "0x%04x: [cb75] BIT 6, L",    // 0x75
    "0x%04x: [cb76] BIT 6, (HL)", // 0x76
    "0x%04x: [cb77] BIT 6, A",    // 0x77
    "0x%04x: [cb78] BIT 7, B",    // 0x78
    "0x%04x: [cb79] BIT 7, C",    // 0x79
    "0x%04x: [cb7a] BIT 7, D",    // 0x7a
    "0x%04x: [cb7b] BIT 7, E",    // 0x7b
    "0x%04x: [cb7c] BIT 7, H",    // 0x7c
    "0x%04x: [cb7d] BIT 7, L",    // 0x7d
    "0x%04x: [cb7e] BIT 7, (HL)", // 0x7e
    "0x%04x: [cb7f] BIT 7, A",    // 0x7f
    "0x%04x: [cb80] RES 0, B",    // 0x80
    "0x%04x: [cb81] RES 0, C",    // 0x81
    "0x%04x: [cb82] RES 0, D",    // 0x82
    "0x%04x: [cb83] RES 0, E",    // 0x83
    "0x%04x: [cb84] RES 0, H",    // 0x84
    "0x%04x: [cb85] RES 0, L",    // 0x85
    "0x%04x: [cb86] RES 0, (HL)", // 0x86
    "0x%04x: [cb87] RES 0, A",    // 0x87
    "0x%04x: [cb88] RES 1, B",    // 0x88
    "0x%04x: [cb89] RES 1, C",    // 0x89
    "0x%04x: [cb8a] RES 1, D",    // 0x8a
    "0x%04x: [cb8b] RES 1, E",    // 0x8b
    "0x%04x: [cb8c] RES 1, H",    // 0x8c
    "0x%04x: [cb8d] RES 1, L",    // 0x8d
    "0x%04x: [cb8e] RES 1, (HL)", // 0x8e
    "0x%04x: [cb8f] RES 1, A",    // 0x8f
    "0x%04x: [cb90] RES 2, B",    // 0x90
    "0x%04x: [cb91] RES 2, C",    // 0x91
    "0x%04x: [cb92] RES 2, D",    // 0x92
    "0x%04x: [cb93] RES 2, E",    // 0x93
    "0x%04x: [cb94] RES 2, H",    // 0x94
    "0x%04x: [cb95] RES 2, L",    // 0x95
    "0x%04x: [cb96] RES 2, (HL)", // 0x96
    "0x%04x: [cb97] RES 2, A",    // 0x97
    "0x%04x: [cb98] RES 3, B",    // 0x98
    "0x%04x: [cb99] RES 3, C",    // 0x99
    "0x%04x: [cb9a] RES 3, D",    // 0x9a
    "0x%04x: [cb9b] RES 3, E",    // 0x9b
    "0x%04x: [cb9c] RES 3, H",    // 0x9c
    "0x%04x: [cb9d] RES 3, L",    // 0x9d
    "0x%04x: [cb9e] RES 3, (HL)", // 0x9e
    "0x%04x: [cb9f] RES 3, A",    // 0x9f
    "0x%04x: [cba0] RES 4, B",    // 0xa0
    "0x%04x: [cba1] RES 4, C",    // 0xa1
    "0x%04x: [cba2] RES 4, D",    // 0xa2
    "0x%04x: [cba3] RES 4, E",    // 0xa3
    "0x%04x: [cba4] RES 4, H",    // 0xa4
    "0x%04x: [cba5] RES 4, L",    // 0xa5
    "0x%04x: [cba6] RES 4, (HL)", // 0xa6
    "0x%04x: [cba7] RES 4, A",    // 0xa7
    "0x%04x: [cba8] RES 5, B",    // 0xa8
    "0x%04x: [cba9] RES 5, C",    // 0xa9
    "0x%04x: [cbaa] RES 5, D",    // 0xaa
    "0x%04x: [cbab] RES 5, E",    // 0xab
    "0x%04x: [cbac] RES 5, H",    // 0xac
    "0x%04x: [cbad] RES 5, L",    // 0xad
    "0x%04x: [cbae] RES 5, (HL)", // 0xae
    "0x%04x: [cbaf] RES 5, A",    // 0xaf
    "0x%04x: [cbb0] RES 6, B",    // 0xb0
    "0x%04x: [cbb1] RES 6, C",    // 0xb1
    "0x%04x: [cbb2] RES 6, D",    // 0xb2
    "0x%04x: [cbb3] RES 6, E",    // 0xb3
    "0x%04x: [cbb4] RES 6, H",    // 0xb4
    "0x%04x: [cbb5] RES 6, L",    // 0xb5
    "0x%04x: [cbb6] RES 6, (HL)", // 0xb6
    "0x%04x: [cbb7] RES 6, A",    // 0xb7
    "0x%04x: [cbb8] RES 7, B",    // 0xb8
    "0x%04x: [cbb9] RES 7, C",    // 0xb9
    "0x%04x: [cbba] RES 7, D",    // 0xba
    "0x%04x: [cbbb] RES 7, E",    // 0xbb
    "0x%04x: [cbbc] RES 7, H",    // 0xbc
    "0x%04x: [cbbd] RES 7, L",    // 0xbd
    "0x%04x: [cbbe] RES 7, (HL)", // 0xbe
    "0x%04x: [cbbf] RES 7, A",    // 0xbf
    "0x%04x: [cbc0] SET 0, B",    // 0xc0
    "0x%04x: [cbc1] SET 0, C",    // 0xc1
    "0x%04x: [cbc2] SET 0, D",    // 0xc2
    "0x%04x: [cbc3] SET 0, E",    // 0xc3
    "0x%04x: [cbc4] SET 0, H",    // 0xc4
    "0x%04x: [cbc5] SET 0, L",    // 0xc5
    "0x%04x: [cbc6] SET 0, (HL)", // 0xc6
    "0x%04x: [cbc7] SET 0, A",    // 0xc7
    "0x%04x: [cbc8] SET 1, B",    // 0xc8
    "0x%04x: [cbc9] SET 1, C",    // 0xc9
    "0x%04x: [cbca] SET 1, D",    // 0xca
    "0x%04x: [cbcb] SET 1, E",    // 0xcb
    "0x%04x: [cbcc] SET 1, H",    // 0xcc
    "0x%04x: [cbcd] SET 1, L",    // 0xcd
    "0x%04x: [cbce] SET 1, (HL)", // 0xce
    "0x%04x: [cbcf] SET 1, A",    // 0xcf
    "0x%04x: [cbd0] SET 2, B",    // 0xd0
    "0x%04x: [cbd1] SET 2, C",    // 0xd1
    "0x%04x: [cbd2] SET 2, D",    // 0xd2
    "0x%04x: [cbd3] SET 2, E",    // 0xd3
    "0x%04x: [cbd4] SET 2, H",    // 0xd4
    "0x%04x: [cbd5] SET 2, L",    // 0xd5
    "0x%04x: [cbd6] SET 2, (HL)", // 0xd6
    "0x%04x: [cbd7] SET 2, A",    // 0xd7
    "0x%04x: [cbd8] SET 3, B",    // 0xd8
    "0x%04x: [cbd9] SET 3, C",    // 0xd9
    "0x%04x: [cbda] SET 3, D",    // 0xda
    "0x%04x: [cbdb] SET 3, E",    // 0xdb
    "0x%04x: [cbdc] SET 3, H",    // 0xdc
    "0x%04x: [cbdd] SET 3, L",    // 0xdd
    "0x%04x: [cbde] SET 3, (HL)", // 0xde
    "0x%04x: [cbdf] SET 3, A",    // 0xdf
    "0x%04x: [cbe0] SET 4, B",    // 0xe0
    "0x%04x: [cbe1] SET 4, C",    // 0xe1
    "0x%04x: [cbe2] SET 4, D",    // 0xe2
    "0x%04x: [cbe3] SET 4, E",    // 0xe3
    "0x%04x: [cbe4] SET 4, H",    // 0xe4
    "0x%04x: [cbe5] SET 4, L",    // 0xe5
    "0x%04x: [cbe6] SET 4, (HL)", // 0xe6
    "0x%04x: [cbe7] SET 4, A",    // 0xe7
    "0x%04x: [cbe8] SET 5, B",    // 0xe8
    "0x%04x: [cbe9] SET 5, C",    // 0xe9
    "0x%04x: [cbea] SET 5, D",    // 0xea
    "0x%04x: [cbeb] SET 5, E",    // 0xeb
    "0x%04x: [cbec] SET 5, H",    // 0xec
    "0x%04x: [cbed] SET 5, L",    // 0xed
    "0x%04x: [cbee] SET 5, (HL)", // 0xee
    "0x%04x: [cbef] SET 5, A",    // 0xef
    "0x%04x: [cbf0] SET 6, B",    // 0xf0
    "0x%04x: [cbf1] SET 6, C",    // 0xf1
    "0x%04x: [cbf2] SET 6, D",    // 0xf2
    "0x%04x: [cbf3] SET 6, E",    // 0xf3
    "0x%04x: [cbf4] SET 6, H",    // 0xf4
    "0x%04x: [cbf5] SET 6, L",    // 0xf5
    "0x%04x: [cbf6] SET 6, (HL)", // 0xf6
    "0x%04x: [cbf7] SET 6, A",    // 0xf7
    "0x%04x: [cbf8] SET 7, B",    // 0xf8
    "0x%04x: [cbf9] SET 7, C",    // 0xf9
    "0x%04x: [cbfa] SET 7, D",    // 0xfa
    "0x%04x: [cbfb] SET 7, E",    // 0xfb
    "0x%04x: [cbfc] SET 7, H",    // 0xfc
    "0x%04x: [cbfd] SET 7, L",    // 0xfd
    "0x%04x: [cbfe] SET 7, (HL)", // 0xfe
    "0x%04x: [cbff] SET 7, A",    // 0xff
};

const uint8_t cb_clock_m_cycles[256] = {
    4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4, // 0x0_
    4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4, // 0x1_
    4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4, // 0x2_
    4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4, // 0x3_
    4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4, // 0x4_
    4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4, // 0x5_
    4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4, // 0x6_
    4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4, // 0x7_
    4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4, // 0x8_
    4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4, // 0x9_
    4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4, // 0xa_
    4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4, // 0xb_
    4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4, // 0xc_
    4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4, // 0xd_
    4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4, // 0xe_
    4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4  // 0xf_
};

void CPU::CB_INSTRUCTION_DECODER() {
  uint16_t curr_pc = reg.pc;

  switch (read_byte(reg.pc++)) {
  case 0x00:
    printf(cb_disasm[0x00], curr_pc);
    clock_m += cb_clock_m_cycles[0x00];
    RLC_r8(&REGISTERS::b);
    break;
  case 0x01:
    printf(cb_disasm[0x01], curr_pc);
    clock_m += cb_clock_m_cycles[0x01];
    RLC_r8(&REGISTERS::c);
    break;
  case 0x02:
    printf(cb_disasm[0x02], curr_pc);
    clock_m += cb_clock_m_cycles[0x02];
    RLC_r8(&REGISTERS::d);
    break;
  case 0x03:
    printf(cb_disasm[0x03], curr_pc);
    clock_m += cb_clock_m_cycles[0x03];
    RLC_r8(&REGISTERS::e);
    break;
  case 0x04:
    printf(cb_disasm[0x04], curr_pc);
    clock_m += cb_clock_m_cycles[0x04];
    RLC_r8(&REGISTERS::h);
    break;
  case 0x05:
    printf(cb_disasm[0x05], curr_pc);
    clock_m += cb_clock_m_cycles[0x05];
    RLC_r8(&REGISTERS::l);
    break;
  case 0x06:
    printf(cb_disasm[0x06], curr_pc);
    clock_m += cb_clock_m_cycles[0x06];
    RLC_HL();
    break;
  case 0x07:
    printf(cb_disasm[0x07], curr_pc);
    clock_m += cb_clock_m_cycles[0x07];
    RLCA();
    break;
  case 0x08:
    printf(cb_disasm[0x08], curr_pc);
    clock_m += cb_clock_m_cycles[0x08];
    RRC_r8(&REGISTERS::b);
    break;
  case 0x09:
    printf(cb_disasm[0x09], curr_pc);
    clock_m += cb_clock_m_cycles[0x09];
    RRC_r8(&REGISTERS::c);
    break;
  case 0x0A:
    printf(cb_disasm[0x0a], curr_pc);
    clock_m += cb_clock_m_cycles[0x0a];
    RRC_r8(&REGISTERS::d);
    break;
  case 0x0B:
    printf(cb_disasm[0x0b], curr_pc);
    clock_m += cb_clock_m_cycles[0x0b];
    RRC_r8(&REGISTERS::e);
    break;
  case 0x0C:
    printf(cb_disasm[0x0c], curr_pc);
    clock_m += cb_clock_m_cycles[0x0c];
    RRC_r8(&REGISTERS::h);
    break;
  case 0x0D:
    printf(cb_disasm[0x0d], curr_pc);
    clock_m += cb_clock_m_cycles[0x0d];
    RRC_r8(&REGISTERS::b);
    break;
  case 0x0E:
    printf(cb_disasm[0x0e], curr_pc);
    clock_m += cb_clock_m_cycles[0x0e];
    RRC_HL();
    break;
  case 0x0F:
    printf(cb_disasm[0x0f], curr_pc);
    clock_m += cb_clock_m_cycles[0x0f];
    RRCA();
    break;
  case 0x10:
    printf(cb_disasm[0x10], curr_pc);
    clock_m += cb_clock_m_cycles[0x10];
    RL_r8(&REGISTERS::b);
    break;
  case 0x11:
    printf(cb_disasm[0x11], curr_pc);
    clock_m += cb_clock_m_cycles[0x11];
    RL_r8(&REGISTERS::c);
    break;
  case 0x12:
    printf(cb_disasm[0x12], curr_pc);
    clock_m += cb_clock_m_cycles[0x12];
    RL_r8(&REGISTERS::d);
    break;
  case 0x13:
    printf(cb_disasm[0x13], curr_pc);
    clock_m += cb_clock_m_cycles[0x13];
    RL_r8(&REGISTERS::e);
    break;
  case 0x14:
    printf(cb_disasm[0x14], curr_pc);
    clock_m += cb_clock_m_cycles[0x14];
    RL_r8(&REGISTERS::h);
    break;
  case 0x15:
    printf(cb_disasm[0x15], curr_pc);
    clock_m += cb_clock_m_cycles[0x15];
    RL_r8(&REGISTERS::l);
    break;
  case 0x16:
    printf(cb_disasm[0x16], curr_pc);
    clock_m += cb_clock_m_cycles[0x16];
    RL_HL();
    break;
  case 0x17:
    printf(cb_disasm[0x17], curr_pc);
    clock_m += cb_clock_m_cycles[0x17];
    RLA();
    break;
  case 0x18:
    printf(cb_disasm[0x18], curr_pc);
    clock_m += cb_clock_m_cycles[0x18];
    RR_r8(&REGISTERS::b);
    break;
  case 0x19:
    printf(cb_disasm[0x19], curr_pc);
    clock_m += cb_clock_m_cycles[0x19];
    RR_r8(&REGISTERS::c);
    break;
  case 0x1A:
    printf(cb_disasm[0x1a], curr_pc);
    clock_m += cb_clock_m_cycles[0x1a];
    RR_r8(&REGISTERS::d);
    break;
  case 0x1B:
    printf(cb_disasm[0x1b], curr_pc);
    clock_m += cb_clock_m_cycles[0x1b];
    RR_r8(&REGISTERS::e);
    break;
  case 0x1C:
    printf(cb_disasm[0x1c], curr_pc);
    clock_m += cb_clock_m_cycles[0x1c];
    RR_r8(&REGISTERS::h);
    break;
  case 0x1D:
    printf(cb_disasm[0x1d], curr_pc);
    clock_m += cb_clock_m_cycles[0x1d];
    RR_r8(&REGISTERS::b);
    break;
  case 0x1E:
    printf(cb_disasm[0x1e], curr_pc);
    clock_m += cb_clock_m_cycles[0x1e];
    RR_HL();
    break;
  case 0x1F:
    printf(cb_disasm[0x1f], curr_pc);
    clock_m += cb_clock_m_cycles[0x1f];
    RRA();
    break;
  case 0x20:
    printf(cb_disasm[0x20], curr_pc);
    clock_m += cb_clock_m_cycles[0x20];
    SLA_r8(&REGISTERS::b);
    break;
  case 0x21:
    printf(cb_disasm[0x21], curr_pc);
    clock_m += cb_clock_m_cycles[0x21];
    SLA_r8(&REGISTERS::c);
    break;
  case 0x22:
    printf(cb_disasm[0x22], curr_pc);
    clock_m += cb_clock_m_cycles[0x22];
    SLA_r8(&REGISTERS::d);
    break;
  case 0x23:
    printf(cb_disasm[0x23], curr_pc);
    clock_m += cb_clock_m_cycles[0x23];
    SLA_r8(&REGISTERS::e);
    break;
  case 0x24:
    printf(cb_disasm[0x24], curr_pc);
    clock_m += cb_clock_m_cycles[0x24];
    SLA_r8(&REGISTERS::h);
    break;
  case 0x25:
    printf(cb_disasm[0x25], curr_pc);
    clock_m += cb_clock_m_cycles[0x25];
    SLA_r8(&REGISTERS::l);
    break;
  case 0x26:
    printf(cb_disasm[0x26], curr_pc);
    clock_m += cb_clock_m_cycles[0x26];
    SLA_HL();
    break;
  case 0x27:
    printf(cb_disasm[0x27], curr_pc);
    clock_m += cb_clock_m_cycles[0x27];
    SLA_r8(&REGISTERS::a);
    break;
  case 0x28:
    printf(cb_disasm[0x28], curr_pc);
    clock_m += cb_clock_m_cycles[0x28];
    SRA_r8(&REGISTERS::b);
    break;
  case 0x29:
    printf(cb_disasm[0x29], curr_pc);
    clock_m += cb_clock_m_cycles[0x29];
    SRA_r8(&REGISTERS::c);
    break;
  case 0x2A:
    printf(cb_disasm[0x2a], curr_pc);
    clock_m += cb_clock_m_cycles[0x2a];
    SRA_r8(&REGISTERS::d);
    break;
  case 0x2B:
    printf(cb_disasm[0x2b], curr_pc);
    clock_m += cb_clock_m_cycles[0x2b];
    SRA_r8(&REGISTERS::e);
    break;
  case 0x2C:
    printf(cb_disasm[0x2c], curr_pc);
    clock_m += cb_clock_m_cycles[0x2c];
    SRA_r8(&REGISTERS::h);
    break;
  case 0x2D:
    printf(cb_disasm[0x2d], curr_pc);
    clock_m += cb_clock_m_cycles[0x2d];
    SRA_r8(&REGISTERS::b);
    break;
  case 0x2E:
    printf(cb_disasm[0x2e], curr_pc);
    clock_m += cb_clock_m_cycles[0x2e];
    SRA_HL();
    break;
  case 0x2F:
    printf(cb_disasm[0x2f], curr_pc);
    clock_m += cb_clock_m_cycles[0x2f];
    SRA_r8(&REGISTERS::a);
    break;
  case 0x30:
    printf(cb_disasm[0x30], curr_pc);
    clock_m += cb_clock_m_cycles[0x30];
    SWAP_r8(&REGISTERS::b);
    break;
  case 0x31:
    printf(cb_disasm[0x31], curr_pc);
    clock_m += cb_clock_m_cycles[0x31];
    SWAP_r8(&REGISTERS::c);
    break;
  case 0x32:
    printf(cb_disasm[0x32], curr_pc);
    clock_m += cb_clock_m_cycles[0x32];
    SWAP_r8(&REGISTERS::d);
    break;
  case 0x33:
    printf(cb_disasm[0x33], curr_pc);
    clock_m += cb_clock_m_cycles[0x33];
    SWAP_r8(&REGISTERS::e);
    break;
  case 0x34:
    printf(cb_disasm[0x34], curr_pc);
    clock_m += cb_clock_m_cycles[0x34];
    SWAP_r8(&REGISTERS::h);
    break;
  case 0x35:
    printf(cb_disasm[0x35], curr_pc);
    clock_m += cb_clock_m_cycles[0x35];
    SWAP_r8(&REGISTERS::l);
    break;
  case 0x36:
    printf(cb_disasm[0x36], curr_pc);
    clock_m += cb_clock_m_cycles[0x36];
    SWAP_HL();
    break;
  case 0x37:
    printf(cb_disasm[0x37], curr_pc);
    clock_m += cb_clock_m_cycles[0x37];
    SWAP_r8(&REGISTERS::a);
    break;
  case 0x38:
    printf(cb_disasm[0x38], curr_pc);
    clock_m += cb_clock_m_cycles[0x38];
    SRL_r8(&REGISTERS::b);
    break;
  case 0x39:
    printf(cb_disasm[0x39], curr_pc);
    clock_m += cb_clock_m_cycles[0x39];
    SRL_r8(&REGISTERS::c);
    break;
  case 0x3A:
    printf(cb_disasm[0x3a], curr_pc);
    clock_m += cb_clock_m_cycles[0x3a];
    SRL_r8(&REGISTERS::d);
    break;
  case 0x3B:
    printf(cb_disasm[0x3b], curr_pc);
    clock_m += cb_clock_m_cycles[0x3b];
    SRL_r8(&REGISTERS::e);
    break;
  case 0x3C:
    printf(cb_disasm[0x3c], curr_pc);
    clock_m += cb_clock_m_cycles[0x3c];
    SRL_r8(&REGISTERS::h);
    break;
  case 0x3D:
    printf(cb_disasm[0x3d], curr_pc);
    clock_m += cb_clock_m_cycles[0x3d];
    SRL_r8(&REGISTERS::b);
    break;
  case 0x3E:
    printf(cb_disasm[0x3e], curr_pc);
    clock_m += cb_clock_m_cycles[0x3e];
    SRL_HL();
    break;
  case 0x3F:
    printf(cb_disasm[0x3f], curr_pc);
    clock_m += cb_clock_m_cycles[0x3f];
    SRL_r8(&REGISTERS::a);
    break;
  case 0x40:
    printf(cb_disasm[0x40], curr_pc);
    clock_m += cb_clock_m_cycles[0x40];
    BIT_u3_r8(0, &REGISTERS::b);
    break;
  case 0x41:
    printf(cb_disasm[0x41], curr_pc);
    clock_m += cb_clock_m_cycles[0x41];
    BIT_u3_r8(0, &REGISTERS::c);
    break;
  case 0x42:
    printf(cb_disasm[0x42], curr_pc);
    clock_m += cb_clock_m_cycles[0x42];
    BIT_u3_r8(0, &REGISTERS::d);
    break;
  case 0x43:
    printf(cb_disasm[0x43], curr_pc);
    clock_m += cb_clock_m_cycles[0x43];
    BIT_u3_r8(0, &REGISTERS::e);
    break;
  case 0x44:
    printf(cb_disasm[0x44], curr_pc);
    clock_m += cb_clock_m_cycles[0x44];
    BIT_u3_r8(0, &REGISTERS::h);
    break;
  case 0x45:
    printf(cb_disasm[0x45], curr_pc);
    clock_m += cb_clock_m_cycles[0x45];
    BIT_u3_r8(0, &REGISTERS::l);
    break;
  case 0x46:
    printf(cb_disasm[0x46], curr_pc);
    clock_m += cb_clock_m_cycles[0x46];
    BIT_u3_HL(0);
    break;
  case 0x47:
    printf(cb_disasm[0x47], curr_pc);
    clock_m += cb_clock_m_cycles[0x47];
    BIT_u3_r8(0, &REGISTERS::a);
    break;
  case 0x48:
    printf(cb_disasm[0x48], curr_pc);
    clock_m += cb_clock_m_cycles[0x48];
    BIT_u3_r8(1, &REGISTERS::b);
    break;
  case 0x49:
    printf(cb_disasm[0x49], curr_pc);
    clock_m += cb_clock_m_cycles[0x49];
    BIT_u3_r8(1, &REGISTERS::c);
    break;
  case 0x4A:
    printf(cb_disasm[0x4a], curr_pc);
    clock_m += cb_clock_m_cycles[0x4a];
    BIT_u3_r8(1, &REGISTERS::d);
    break;
  case 0x4B:
    printf(cb_disasm[0x4b], curr_pc);
    clock_m += cb_clock_m_cycles[0x4b];
    BIT_u3_r8(1, &REGISTERS::e);
    break;
  case 0x4C:
    printf(cb_disasm[0x4c], curr_pc);
    clock_m += cb_clock_m_cycles[0x4c];
    BIT_u3_r8(1, &REGISTERS::h);
    break;
  case 0x4D:
    printf(cb_disasm[0x4d], curr_pc);
    clock_m += cb_clock_m_cycles[0x4d];
    BIT_u3_r8(1, &REGISTERS::l);
    break;
  case 0x4E:
    printf(cb_disasm[0x4e], curr_pc);
    clock_m += cb_clock_m_cycles[0x4e];
    BIT_u3_HL(1);
    break;
  case 0x4F:
    printf(cb_disasm[0x4f], curr_pc);
    clock_m += cb_clock_m_cycles[0x4f];
    BIT_u3_r8(1, &REGISTERS::a);
    break;
  case 0x50:
    printf(cb_disasm[0x50], curr_pc);
    clock_m += cb_clock_m_cycles[0x50];
    BIT_u3_r8(2, &REGISTERS::b);
    break;
  case 0x51:
    printf(cb_disasm[0x51], curr_pc);
    clock_m += cb_clock_m_cycles[0x51];
    BIT_u3_r8(2, &REGISTERS::c);
    break;
  case 0x52:
    printf(cb_disasm[0x52], curr_pc);
    clock_m += cb_clock_m_cycles[0x52];
    BIT_u3_r8(2, &REGISTERS::d);
    break;
  case 0x53:
    printf(cb_disasm[0x53], curr_pc);
    clock_m += cb_clock_m_cycles[0x53];
    BIT_u3_r8(2, &REGISTERS::e);
    break;
  case 0x54:
    printf(cb_disasm[0x54], curr_pc);
    clock_m += cb_clock_m_cycles[0x54];
    BIT_u3_r8(2, &REGISTERS::h);
    break;
  case 0x55:
    printf(cb_disasm[0x55], curr_pc);
    clock_m += cb_clock_m_cycles[0x55];
    BIT_u3_r8(2, &REGISTERS::l);
    break;
  case 0x56:
    printf(cb_disasm[0x56], curr_pc);
    clock_m += cb_clock_m_cycles[0x56];
    BIT_u3_HL(2);
    break;
  case 0x57:
    printf(cb_disasm[0x57], curr_pc);
    clock_m += cb_clock_m_cycles[0x57];
    BIT_u3_r8(2, &REGISTERS::a);
    break;
  case 0x58:
    printf(cb_disasm[0x58], curr_pc);
    clock_m += cb_clock_m_cycles[0x58];
    BIT_u3_r8(3, &REGISTERS::b);
    break;
  case 0x59:
    printf(cb_disasm[0x59], curr_pc);
    clock_m += cb_clock_m_cycles[0x59];
    BIT_u3_r8(3, &REGISTERS::c);
    break;
  case 0x5A:
    printf(cb_disasm[0x5a], curr_pc);
    clock_m += cb_clock_m_cycles[0x5a];
    BIT_u3_r8(3, &REGISTERS::d);
    break;
  case 0x5B:
    printf(cb_disasm[0x5b], curr_pc);
    clock_m += cb_clock_m_cycles[0x5b];
    BIT_u3_r8(3, &REGISTERS::e);
    break;
  case 0x5C:
    printf(cb_disasm[0x5c], curr_pc);
    clock_m += cb_clock_m_cycles[0x5c];
    BIT_u3_r8(3, &REGISTERS::h);
    break;
  case 0x5D:
    printf(cb_disasm[0x5d], curr_pc);
    clock_m += cb_clock_m_cycles[0x5d];
    BIT_u3_r8(3, &REGISTERS::l);
    break;
  case 0x5E:
    printf(cb_disasm[0x5e], curr_pc);
    clock_m += cb_clock_m_cycles[0x5e];
    BIT_u3_HL(3);
    break;
  case 0x5F:
    printf(cb_disasm[0x5f], curr_pc);
    clock_m += cb_clock_m_cycles[0x5f];
    BIT_u3_r8(3, &REGISTERS::a);
    break;
  case 0x60:
    printf(cb_disasm[0x60], curr_pc);
    clock_m += cb_clock_m_cycles[0x60];
    BIT_u3_r8(4, &REGISTERS::b);
    break;
  case 0x61:
    printf(cb_disasm[0x61], curr_pc);
    clock_m += cb_clock_m_cycles[0x61];
    BIT_u3_r8(4, &REGISTERS::c);
    break;
  case 0x62:
    printf(cb_disasm[0x62], curr_pc);
    clock_m += cb_clock_m_cycles[0x62];
    BIT_u3_r8(4, &REGISTERS::d);
    break;
  case 0x63:
    printf(cb_disasm[0x63], curr_pc);
    clock_m += cb_clock_m_cycles[0x63];
    BIT_u3_r8(4, &REGISTERS::e);
    break;
  case 0x64:
    printf(cb_disasm[0x64], curr_pc);
    clock_m += cb_clock_m_cycles[0x64];
    BIT_u3_r8(4, &REGISTERS::h);
    break;
  case 0x65:
    printf(cb_disasm[0x65], curr_pc);
    clock_m += cb_clock_m_cycles[0x65];
    BIT_u3_r8(4, &REGISTERS::l);
    break;
  case 0x66:
    printf(cb_disasm[0x66], curr_pc);
    clock_m += cb_clock_m_cycles[0x66];
    BIT_u3_HL(4);
    break;
  case 0x67:
    printf(cb_disasm[0x67], curr_pc);
    clock_m += cb_clock_m_cycles[0x67];
    BIT_u3_r8(4, &REGISTERS::a);
    break;
  case 0x68:
    printf(cb_disasm[0x68], curr_pc);
    clock_m += cb_clock_m_cycles[0x68];
    BIT_u3_r8(5, &REGISTERS::b);
    break;
  case 0x69:
    printf(cb_disasm[0x69], curr_pc);
    clock_m += cb_clock_m_cycles[0x69];
    BIT_u3_r8(5, &REGISTERS::c);
    break;
  case 0x6A:
    printf(cb_disasm[0x6a], curr_pc);
    clock_m += cb_clock_m_cycles[0x6a];
    BIT_u3_r8(5, &REGISTERS::d);
    break;
  case 0x6B:
    printf(cb_disasm[0x6b], curr_pc);
    clock_m += cb_clock_m_cycles[0x6b];
    BIT_u3_r8(5, &REGISTERS::e);
    break;
  case 0x6C:
    printf(cb_disasm[0x6c], curr_pc);
    clock_m += cb_clock_m_cycles[0x6c];
    BIT_u3_r8(5, &REGISTERS::h);
    break;
  case 0x6D:
    printf(cb_disasm[0x6d], curr_pc);
    clock_m += cb_clock_m_cycles[0x6d];
    BIT_u3_r8(5, &REGISTERS::l);
    break;
  case 0x6E:
    printf(cb_disasm[0x6e], curr_pc);
    clock_m += cb_clock_m_cycles[0x6e];
    BIT_u3_HL(5);
    break;
  case 0x6F:
    printf(cb_disasm[0x6f], curr_pc);
    clock_m += cb_clock_m_cycles[0x6f];
    BIT_u3_r8(5, &REGISTERS::a);
    break;
  case 0x70:
    printf(cb_disasm[0x70], curr_pc);
    clock_m += cb_clock_m_cycles[0x70];
    BIT_u3_r8(6, &REGISTERS::b);
    break;
  case 0x71:
    printf(cb_disasm[0x71], curr_pc);
    clock_m += cb_clock_m_cycles[0x71];
    BIT_u3_r8(6, &REGISTERS::c);
    break;
  case 0x72:
    printf(cb_disasm[0x72], curr_pc);
    clock_m += cb_clock_m_cycles[0x72];
    BIT_u3_r8(6, &REGISTERS::d);
    break;
  case 0x73:
    printf(cb_disasm[0x73], curr_pc);
    clock_m += cb_clock_m_cycles[0x73];
    BIT_u3_r8(6, &REGISTERS::e);
    break;
  case 0x74:
    printf(cb_disasm[0x74], curr_pc);
    clock_m += cb_clock_m_cycles[0x74];
    BIT_u3_r8(6, &REGISTERS::h);
    break;
  case 0x75:
    printf(cb_disasm[0x75], curr_pc);
    clock_m += cb_clock_m_cycles[0x75];
    BIT_u3_r8(6, &REGISTERS::l);
    break;
  case 0x76:
    printf(cb_disasm[0x76], curr_pc);
    clock_m += cb_clock_m_cycles[0x76];
    BIT_u3_HL(6);
    break;
  case 0x77:
    printf(cb_disasm[0x77], curr_pc);
    clock_m += cb_clock_m_cycles[0x77];
    BIT_u3_r8(6, &REGISTERS::a);
    break;
  case 0x78:
    printf(cb_disasm[0x78], curr_pc);
    clock_m += cb_clock_m_cycles[0x78];
    BIT_u3_r8(7, &REGISTERS::b);
    break;
  case 0x79:
    printf(cb_disasm[0x79], curr_pc);
    clock_m += cb_clock_m_cycles[0x79];
    BIT_u3_r8(7, &REGISTERS::c);
    break;
  case 0x7A:
    printf(cb_disasm[0x7a], curr_pc);
    clock_m += cb_clock_m_cycles[0x7a];
    BIT_u3_r8(7, &REGISTERS::d);
    break;
  case 0x7B:
    printf(cb_disasm[0x7b], curr_pc);
    clock_m += cb_clock_m_cycles[0x7b];
    BIT_u3_r8(7, &REGISTERS::e);
    break;
  case 0x7C:
    printf(cb_disasm[0x7c], curr_pc);
    clock_m += cb_clock_m_cycles[0x7c];
    BIT_u3_r8(7, &REGISTERS::h);
    break;
  case 0x7D:
    printf(cb_disasm[0x7d], curr_pc);
    clock_m += cb_clock_m_cycles[0x7d];
    BIT_u3_r8(7, &REGISTERS::l);
    break;
  case 0x7E:
    printf(cb_disasm[0x7e], curr_pc);
    clock_m += cb_clock_m_cycles[0x7e];
    BIT_u3_HL(7);
    break;
  case 0x7F:
    printf(cb_disasm[0x7f], curr_pc);
    clock_m += cb_clock_m_cycles[0x7f];
    BIT_u3_r8(7, &REGISTERS::a);
    break;
  case 0x80:
    printf(cb_disasm[0x80], curr_pc);
    clock_m += cb_clock_m_cycles[0x80];
    RES_u3_r8(0, &REGISTERS::b);
    break;
  case 0x81:
    printf(cb_disasm[0x81], curr_pc);
    clock_m += cb_clock_m_cycles[0x81];
    RES_u3_r8(0, &REGISTERS::c);
    break;
  case 0x82:
    printf(cb_disasm[0x82], curr_pc);
    clock_m += cb_clock_m_cycles[0x82];
    RES_u3_r8(0, &REGISTERS::d);
    break;
  case 0x83:
    printf(cb_disasm[0x83], curr_pc);
    clock_m += cb_clock_m_cycles[0x83];
    RES_u3_r8(0, &REGISTERS::e);
    break;
  case 0x84:
    printf(cb_disasm[0x84], curr_pc);
    clock_m += cb_clock_m_cycles[0x84];
    RES_u3_r8(0, &REGISTERS::h);
    break;
  case 0x85:
    printf(cb_disasm[0x85], curr_pc);
    clock_m += cb_clock_m_cycles[0x85];
    RES_u3_r8(0, &REGISTERS::l);
    break;
  case 0x86:
    printf(cb_disasm[0x86], curr_pc);
    clock_m += cb_clock_m_cycles[0x86];
    RES_u3_HL(0);
    break;
  case 0x87:
    printf(cb_disasm[0x87], curr_pc);
    clock_m += cb_clock_m_cycles[0x87];
    RES_u3_r8(0, &REGISTERS::a);
    break;
  case 0x88:
    printf(cb_disasm[0x88], curr_pc);
    clock_m += cb_clock_m_cycles[0x88];
    RES_u3_r8(1, &REGISTERS::b);
    break;
  case 0x89:
    printf(cb_disasm[0x89], curr_pc);
    clock_m += cb_clock_m_cycles[0x89];
    RES_u3_r8(1, &REGISTERS::c);
    break;
  case 0x8A:
    printf(cb_disasm[0x8a], curr_pc);
    clock_m += cb_clock_m_cycles[0x8a];
    RES_u3_r8(1, &REGISTERS::d);
    break;
  case 0x8B:
    printf(cb_disasm[0x8b], curr_pc);
    clock_m += cb_clock_m_cycles[0x8b];
    RES_u3_r8(1, &REGISTERS::e);
    break;
  case 0x8C:
    printf(cb_disasm[0x8c], curr_pc);
    clock_m += cb_clock_m_cycles[0x8c];
    RES_u3_r8(1, &REGISTERS::h);
    break;
  case 0x8D:
    printf(cb_disasm[0x8d], curr_pc);
    clock_m += cb_clock_m_cycles[0x8d];
    RES_u3_r8(1, &REGISTERS::l);
    break;
  case 0x8E:
    printf(cb_disasm[0x8e], curr_pc);
    clock_m += cb_clock_m_cycles[0x8e];
    RES_u3_HL(1);
    break;
  case 0x8F:
    printf(cb_disasm[0x8f], curr_pc);
    clock_m += cb_clock_m_cycles[0x8f];
    RES_u3_r8(1, &REGISTERS::a);
    break;
  case 0x90:
    printf(cb_disasm[0x90], curr_pc);
    clock_m += cb_clock_m_cycles[0x90];
    RES_u3_r8(2, &REGISTERS::b);
    break;
  case 0x91:
    printf(cb_disasm[0x91], curr_pc);
    clock_m += cb_clock_m_cycles[0x91];
    RES_u3_r8(2, &REGISTERS::c);
    break;
  case 0x92:
    printf(cb_disasm[0x92], curr_pc);
    clock_m += cb_clock_m_cycles[0x92];
    RES_u3_r8(2, &REGISTERS::d);
    break;
  case 0x93:
    printf(cb_disasm[0x93], curr_pc);
    clock_m += cb_clock_m_cycles[0x93];
    RES_u3_r8(2, &REGISTERS::e);
    break;
  case 0x94:
    printf(cb_disasm[0x94], curr_pc);
    clock_m += cb_clock_m_cycles[0x94];
    RES_u3_r8(2, &REGISTERS::h);
    break;
  case 0x95:
    printf(cb_disasm[0x95], curr_pc);
    clock_m += cb_clock_m_cycles[0x95];
    RES_u3_r8(2, &REGISTERS::l);
    break;
  case 0x96:
    printf(cb_disasm[0x96], curr_pc);
    clock_m += cb_clock_m_cycles[0x96];
    RES_u3_HL(2);
    break;
  case 0x97:
    printf(cb_disasm[0x97], curr_pc);
    clock_m += cb_clock_m_cycles[0x97];
    RES_u3_r8(2, &REGISTERS::a);
    break;
  case 0x98:
    printf(cb_disasm[0x98], curr_pc);
    clock_m += cb_clock_m_cycles[0x98];
    RES_u3_r8(3, &REGISTERS::b);
    break;
  case 0x99:
    printf(cb_disasm[0x99], curr_pc);
    clock_m += cb_clock_m_cycles[0x99];
    RES_u3_r8(3, &REGISTERS::c);
    break;
  case 0x9A:
    printf(cb_disasm[0x9a], curr_pc);
    clock_m += cb_clock_m_cycles[0x9a];
    RES_u3_r8(3, &REGISTERS::d);
    break;
  case 0x9B:
    printf(cb_disasm[0x9b], curr_pc);
    clock_m += cb_clock_m_cycles[0x9b];
    RES_u3_r8(3, &REGISTERS::e);
    break;
  case 0x9C:
    printf(cb_disasm[0x9c], curr_pc);
    clock_m += cb_clock_m_cycles[0x9c];
    RES_u3_r8(3, &REGISTERS::h);
    break;
  case 0x9D:
    printf(cb_disasm[0x9d], curr_pc);
    clock_m += cb_clock_m_cycles[0x9d];
    RES_u3_r8(3, &REGISTERS::l);
    break;
  case 0x9E:
    printf(cb_disasm[0x9e], curr_pc);
    clock_m += cb_clock_m_cycles[0x9e];
    RES_u3_HL(3);
    break;
  case 0x9F:
    printf(cb_disasm[0x9f], curr_pc);
    clock_m += cb_clock_m_cycles[0x9f];
    RES_u3_r8(3, &REGISTERS::a);
    break;
  case 0xA0:
    printf(cb_disasm[0xa0], curr_pc);
    clock_m += cb_clock_m_cycles[0xa0];
    RES_u3_r8(4, &REGISTERS::b);
    break;
  case 0xA1:
    printf(cb_disasm[0xa1], curr_pc);
    clock_m += cb_clock_m_cycles[0xa1];
    RES_u3_r8(4, &REGISTERS::c);
    break;
  case 0xA2:
    printf(cb_disasm[0xa2], curr_pc);
    clock_m += cb_clock_m_cycles[0xa2];
    RES_u3_r8(4, &REGISTERS::d);
    break;
  case 0xA3:
    printf(cb_disasm[0xa3], curr_pc);
    clock_m += cb_clock_m_cycles[0xa3];
    RES_u3_r8(4, &REGISTERS::e);
    break;
  case 0xA4:
    printf(cb_disasm[0xa4], curr_pc);
    clock_m += cb_clock_m_cycles[0xa4];
    RES_u3_r8(4, &REGISTERS::h);
    break;
  case 0xA5:
    printf(cb_disasm[0xa5], curr_pc);
    clock_m += cb_clock_m_cycles[0xa5];
    RES_u3_r8(4, &REGISTERS::l);
    break;
  case 0xA6:
    printf(cb_disasm[0xa6], curr_pc);
    clock_m += cb_clock_m_cycles[0xa6];
    RES_u3_HL(4);
    break;
  case 0xA7:
    printf(cb_disasm[0xa7], curr_pc);
    clock_m += cb_clock_m_cycles[0xa7];
    RES_u3_r8(4, &REGISTERS::a);
    break;
  case 0xA8:
    printf(cb_disasm[0xa8], curr_pc);
    clock_m += cb_clock_m_cycles[0xa8];
    RES_u3_r8(5, &REGISTERS::b);
    break;
  case 0xA9:
    printf(cb_disasm[0xa9], curr_pc);
    clock_m += cb_clock_m_cycles[0xa9];
    RES_u3_r8(5, &REGISTERS::c);
    break;
  case 0xAA:
    printf(cb_disasm[0xaa], curr_pc);
    clock_m += cb_clock_m_cycles[0xaa];
    RES_u3_r8(5, &REGISTERS::d);
    break;
  case 0xAB:
    printf(cb_disasm[0xab], curr_pc);
    clock_m += cb_clock_m_cycles[0xab];
    RES_u3_r8(5, &REGISTERS::e);
    break;
  case 0xAC:
    printf(cb_disasm[0xac], curr_pc);
    clock_m += cb_clock_m_cycles[0xac];
    RES_u3_r8(5, &REGISTERS::h);
    break;
  case 0xAD:
    printf(cb_disasm[0xad], curr_pc);
    clock_m += cb_clock_m_cycles[0xad];
    RES_u3_r8(5, &REGISTERS::l);
    break;
  case 0xAE:
    printf(cb_disasm[0xae], curr_pc);
    clock_m += cb_clock_m_cycles[0xae];
    RES_u3_HL(5);
    break;
  case 0xAF:
    printf(cb_disasm[0xaf], curr_pc);
    clock_m += cb_clock_m_cycles[0xaf];
    RES_u3_r8(5, &REGISTERS::a);
    break;
  case 0xB0:
    printf(cb_disasm[0xb0], curr_pc);
    clock_m += cb_clock_m_cycles[0xb0];
    RES_u3_r8(6, &REGISTERS::b);
    break;
  case 0xB1:
    printf(cb_disasm[0xb1], curr_pc);
    clock_m += cb_clock_m_cycles[0xb1];
    RES_u3_r8(6, &REGISTERS::c);
    break;
  case 0xB2:
    printf(cb_disasm[0xb2], curr_pc);
    clock_m += cb_clock_m_cycles[0xb2];
    RES_u3_r8(6, &REGISTERS::d);
    break;
  case 0xB3:
    printf(cb_disasm[0xb3], curr_pc);
    clock_m += cb_clock_m_cycles[0xb3];
    RES_u3_r8(6, &REGISTERS::e);
    break;
  case 0xB4:
    printf(cb_disasm[0xb4], curr_pc);
    clock_m += cb_clock_m_cycles[0xb4];
    RES_u3_r8(6, &REGISTERS::h);
    break;
  case 0xB5:
    printf(cb_disasm[0xb5], curr_pc);
    clock_m += cb_clock_m_cycles[0xb5];
    RES_u3_r8(6, &REGISTERS::l);
    break;
  case 0xB6:
    printf(cb_disasm[0xb6], curr_pc);
    clock_m += cb_clock_m_cycles[0xb6];
    RES_u3_HL(6);
    break;
  case 0xB7:
    printf(cb_disasm[0xb7], curr_pc);
    clock_m += cb_clock_m_cycles[0xb7];
    RES_u3_r8(6, &REGISTERS::a);
    break;
  case 0xB8:
    printf(cb_disasm[0xb8], curr_pc);
    clock_m += cb_clock_m_cycles[0xb8];
    RES_u3_r8(7, &REGISTERS::b);
    break;
  case 0xB9:
    printf(cb_disasm[0xb9], curr_pc);
    clock_m += cb_clock_m_cycles[0xb9];
    RES_u3_r8(7, &REGISTERS::c);
    break;
  case 0xBA:
    printf(cb_disasm[0xba], curr_pc);
    clock_m += cb_clock_m_cycles[0xba];
    RES_u3_r8(7, &REGISTERS::d);
    break;
  case 0xBB:
    printf(cb_disasm[0xbb], curr_pc);
    clock_m += cb_clock_m_cycles[0xbb];
    RES_u3_r8(7, &REGISTERS::e);
    break;
  case 0xBC:
    printf(cb_disasm[0xbc], curr_pc);
    clock_m += cb_clock_m_cycles[0xbc];
    RES_u3_r8(7, &REGISTERS::h);
    break;
  case 0xBD:
    printf(cb_disasm[0xbd], curr_pc);
    clock_m += cb_clock_m_cycles[0xbd];
    RES_u3_r8(7, &REGISTERS::l);
    break;
  case 0xBE:
    printf(cb_disasm[0xbe], curr_pc);
    clock_m += cb_clock_m_cycles[0xbe];
    RES_u3_HL(7);
    break;
  case 0xBF:
    printf(cb_disasm[0xbf], curr_pc);
    clock_m += cb_clock_m_cycles[0xbf];
    RES_u3_r8(7, &REGISTERS::a);
    break;
  case 0xC0:
    printf(cb_disasm[0xc0], curr_pc);
    clock_m += cb_clock_m_cycles[0xc0];
    SET_u3_r8(0, &REGISTERS::b);
    break;
  case 0xC1:
    printf(cb_disasm[0xc1], curr_pc);
    clock_m += cb_clock_m_cycles[0xc1];
    SET_u3_r8(0, &REGISTERS::c);
    break;
  case 0xC2:
    printf(cb_disasm[0xc2], curr_pc);
    clock_m += cb_clock_m_cycles[0xc2];
    SET_u3_r8(0, &REGISTERS::d);
    break;
  case 0xC3:
    printf(cb_disasm[0xc3], curr_pc);
    clock_m += cb_clock_m_cycles[0xc3];
    SET_u3_r8(0, &REGISTERS::e);
    break;
  case 0xC4:
    printf(cb_disasm[0xc4], curr_pc);
    clock_m += cb_clock_m_cycles[0xc4];
    SET_u3_r8(0, &REGISTERS::h);
    break;
  case 0xC5:
    printf(cb_disasm[0xc5], curr_pc);
    clock_m += cb_clock_m_cycles[0xc5];
    SET_u3_r8(0, &REGISTERS::l);
    break;
  case 0xC6:
    printf(cb_disasm[0xc6], curr_pc);
    clock_m += cb_clock_m_cycles[0xc6];
    SET_u3_HL(0);
    break;
  case 0xC7:
    printf(cb_disasm[0xc7], curr_pc);
    clock_m += cb_clock_m_cycles[0xc7];
    SET_u3_r8(0, &REGISTERS::a);
    break;
  case 0xC8:
    printf(cb_disasm[0xc8], curr_pc);
    clock_m += cb_clock_m_cycles[0xc8];
    SET_u3_r8(1, &REGISTERS::b);
    break;
  case 0xC9:
    printf(cb_disasm[0xc9], curr_pc);
    clock_m += cb_clock_m_cycles[0xc9];
    SET_u3_r8(1, &REGISTERS::c);
    break;
  case 0xCA:
    printf(cb_disasm[0xca], curr_pc);
    clock_m += cb_clock_m_cycles[0xca];
    SET_u3_r8(1, &REGISTERS::d);
    break;
  case 0xCB:
    printf(cb_disasm[0xcb], curr_pc);
    clock_m += cb_clock_m_cycles[0xcb];
    SET_u3_r8(1, &REGISTERS::e);
    break;
  case 0xCC:
    printf(cb_disasm[0xcc], curr_pc);
    clock_m += cb_clock_m_cycles[0xcc];
    SET_u3_r8(1, &REGISTERS::h);
    break;
  case 0xCD:
    printf(cb_disasm[0xcd], curr_pc);
    clock_m += cb_clock_m_cycles[0xcd];
    SET_u3_r8(1, &REGISTERS::l);
    break;
  case 0xCE:
    printf(cb_disasm[0xce], curr_pc);
    clock_m += cb_clock_m_cycles[0xce];
    SET_u3_HL(1);
    break;
  case 0xCF:
    printf(cb_disasm[0xcf], curr_pc);
    clock_m += cb_clock_m_cycles[0xcf];
    SET_u3_r8(1, &REGISTERS::a);
    break;
  case 0xD0:
    printf(cb_disasm[0xd0], curr_pc);
    clock_m += cb_clock_m_cycles[0xd0];
    SET_u3_r8(2, &REGISTERS::b);
    break;
  case 0xD1:
    printf(cb_disasm[0xd1], curr_pc);
    clock_m += cb_clock_m_cycles[0xd1];
    SET_u3_r8(2, &REGISTERS::c);
    break;
  case 0xD2:
    printf(cb_disasm[0xd2], curr_pc);
    clock_m += cb_clock_m_cycles[0xd2];
    SET_u3_r8(2, &REGISTERS::d);
    break;
  case 0xD3:
    printf(cb_disasm[0xd3], curr_pc);
    clock_m += cb_clock_m_cycles[0xd3];
    SET_u3_r8(2, &REGISTERS::e);
    break;
  case 0xD4:
    printf(cb_disasm[0xd4], curr_pc);
    clock_m += cb_clock_m_cycles[0xd4];
    SET_u3_r8(2, &REGISTERS::h);
    break;
  case 0xD5:
    printf(cb_disasm[0xd5], curr_pc);
    clock_m += cb_clock_m_cycles[0xd5];
    SET_u3_r8(2, &REGISTERS::l);
    break;
  case 0xD6:
    printf(cb_disasm[0xd6], curr_pc);
    clock_m += cb_clock_m_cycles[0xd6];
    SET_u3_HL(2);
    break;
  case 0xD7:
    printf(cb_disasm[0xd7], curr_pc);
    clock_m += cb_clock_m_cycles[0xd7];
    SET_u3_r8(2, &REGISTERS::a);
    break;
  case 0xD8:
    printf(cb_disasm[0xd8], curr_pc);
    clock_m += cb_clock_m_cycles[0xd8];
    SET_u3_r8(3, &REGISTERS::b);
    break;
  case 0xD9:
    printf(cb_disasm[0xd9], curr_pc);
    clock_m += cb_clock_m_cycles[0xd9];
    SET_u3_r8(3, &REGISTERS::c);
    break;
  case 0xDA:
    printf(cb_disasm[0xda], curr_pc);
    clock_m += cb_clock_m_cycles[0xda];
    SET_u3_r8(3, &REGISTERS::d);
    break;
  case 0xDB:
    printf(cb_disasm[0xdb], curr_pc);
    clock_m += cb_clock_m_cycles[0xdb];
    SET_u3_r8(3, &REGISTERS::e);
    break;
  case 0xDC:
    printf(cb_disasm[0xdc], curr_pc);
    clock_m += cb_clock_m_cycles[0xdc];
    SET_u3_r8(3, &REGISTERS::h);
    break;
  case 0xDD:
    printf(cb_disasm[0xdd], curr_pc);
    clock_m += cb_clock_m_cycles[0xdd];
    SET_u3_r8(3, &REGISTERS::l);
    break;
  case 0xDE:
    printf(cb_disasm[0xde], curr_pc);
    clock_m += cb_clock_m_cycles[0xde];
    SET_u3_HL(3);
    break;
  case 0xDF:
    printf(cb_disasm[0xdf], curr_pc);
    clock_m += cb_clock_m_cycles[0xdf];
    SET_u3_r8(3, &REGISTERS::a);
    break;
  case 0xE0:
    printf(cb_disasm[0xe0], curr_pc);
    clock_m += cb_clock_m_cycles[0xe0];
    SET_u3_r8(4, &REGISTERS::b);
    break;
  case 0xE1:
    printf(cb_disasm[0xe1], curr_pc);
    clock_m += cb_clock_m_cycles[0xe1];
    SET_u3_r8(4, &REGISTERS::c);
    break;
  case 0xE2:
    printf(cb_disasm[0xe2], curr_pc);
    clock_m += cb_clock_m_cycles[0xe2];
    SET_u3_r8(4, &REGISTERS::d);
    break;
  case 0xE3:
    printf(cb_disasm[0xe3], curr_pc);
    clock_m += cb_clock_m_cycles[0xe3];
    SET_u3_r8(4, &REGISTERS::e);
    break;
  case 0xE4:
    printf(cb_disasm[0xe4], curr_pc);
    clock_m += cb_clock_m_cycles[0xe4];
    SET_u3_r8(4, &REGISTERS::h);
    break;
  case 0xE5:
    printf(cb_disasm[0xe5], curr_pc);
    clock_m += cb_clock_m_cycles[0xe5];
    SET_u3_r8(4, &REGISTERS::l);
    break;
  case 0xE6:
    printf(cb_disasm[0xe6], curr_pc);
    clock_m += cb_clock_m_cycles[0xe6];
    SET_u3_HL(4);
    break;
  case 0xE7:
    printf(cb_disasm[0xe7], curr_pc);
    clock_m += cb_clock_m_cycles[0xe7];
    SET_u3_r8(4, &REGISTERS::a);
    break;
  case 0xE8:
    printf(cb_disasm[0xe8], curr_pc);
    clock_m += cb_clock_m_cycles[0xe8];
    SET_u3_r8(5, &REGISTERS::b);
    break;
  case 0xE9:
    printf(cb_disasm[0xe9], curr_pc);
    clock_m += cb_clock_m_cycles[0xe9];
    SET_u3_r8(5, &REGISTERS::c);
    break;
  case 0xEA:
    printf(cb_disasm[0xea], curr_pc);
    clock_m += cb_clock_m_cycles[0xea];
    SET_u3_r8(5, &REGISTERS::d);
    break;
  case 0xEB:
    printf(cb_disasm[0xeb], curr_pc);
    clock_m += cb_clock_m_cycles[0xeb];
    SET_u3_r8(5, &REGISTERS::e);
    break;
  case 0xEC:
    printf(cb_disasm[0xec], curr_pc);
    clock_m += cb_clock_m_cycles[0xec];
    SET_u3_r8(5, &REGISTERS::h);
    break;
  case 0xED:
    printf(cb_disasm[0xed], curr_pc);
    clock_m += cb_clock_m_cycles[0xed];
    SET_u3_r8(5, &REGISTERS::l);
    break;
  case 0xEE:
    printf(cb_disasm[0xee], curr_pc);
    clock_m += cb_clock_m_cycles[0xee];
    SET_u3_HL(5);
    break;
  case 0xEF:
    printf(cb_disasm[0xef], curr_pc);
    clock_m += cb_clock_m_cycles[0xef];
    SET_u3_r8(5, &REGISTERS::a);
    break;
  case 0xF0:
    printf(cb_disasm[0xf0], curr_pc);
    clock_m += cb_clock_m_cycles[0xf0];
    SET_u3_r8(6, &REGISTERS::b);
    break;
  case 0xF1:
    printf(cb_disasm[0xf1], curr_pc);
    clock_m += cb_clock_m_cycles[0xf1];
    SET_u3_r8(6, &REGISTERS::c);
    break;
  case 0xF2:
    printf(cb_disasm[0xf2], curr_pc);
    clock_m += cb_clock_m_cycles[0xf2];
    SET_u3_r8(6, &REGISTERS::d);
    break;
  case 0xF3:
    printf(cb_disasm[0xf3], curr_pc);
    clock_m += cb_clock_m_cycles[0xf3];
    SET_u3_r8(6, &REGISTERS::e);
    break;
  case 0xF4:
    printf(cb_disasm[0xf4], curr_pc);
    clock_m += cb_clock_m_cycles[0xf4];
    SET_u3_r8(6, &REGISTERS::h);
    break;
  case 0xF5:
    printf(cb_disasm[0xf5], curr_pc);
    clock_m += cb_clock_m_cycles[0xf5];
    SET_u3_r8(6, &REGISTERS::l);
    break;
  case 0xF6:
    printf(cb_disasm[0xf6], curr_pc);
    clock_m += cb_clock_m_cycles[0xf6];
    SET_u3_HL(6);
    break;
  case 0xF7:
    printf(cb_disasm[0xf7], curr_pc);
    clock_m += cb_clock_m_cycles[0xf7];
    SET_u3_r8(6, &REGISTERS::a);
    break;
  case 0xF8:
    printf(cb_disasm[0xf8], curr_pc);
    clock_m += cb_clock_m_cycles[0xf8];
    SET_u3_r8(7, &REGISTERS::b);
    break;
  case 0xF9:
    printf(cb_disasm[0xf9], curr_pc);
    clock_m += cb_clock_m_cycles[0xf9];
    SET_u3_r8(7, &REGISTERS::c);
    break;
  case 0xFA:
    printf(cb_disasm[0xfa], curr_pc);
    clock_m += cb_clock_m_cycles[0xfa];
    SET_u3_r8(7, &REGISTERS::d);
    break;
  case 0xFB:
    printf(cb_disasm[0xfb], curr_pc);
    clock_m += cb_clock_m_cycles[0xfb];
    SET_u3_r8(7, &REGISTERS::e);
    break;
  case 0xFC:
    printf(cb_disasm[0xfc], curr_pc);
    clock_m += cb_clock_m_cycles[0xfc];
    SET_u3_r8(7, &REGISTERS::h);
    break;
  case 0xFD:
    printf(cb_disasm[0xfd], curr_pc);
    clock_m += cb_clock_m_cycles[0xfd];
    SET_u3_r8(7, &REGISTERS::l);
    break;
  case 0xFE:
    printf(cb_disasm[0xfe], curr_pc);
    clock_m += cb_clock_m_cycles[0xfe];
    SET_u3_HL(7);
    break;
  case 0xFF:
    printf(cb_disasm[0xff], curr_pc);
    clock_m += cb_clock_m_cycles[0xff];
    SET_u3_r8(7, &REGISTERS::a);
    break;
  default:
    printf("ERROR: DEFAULT 0x%02X\n", curr_pc);
    printf("ERROR 0x%04X\n", read_byte(curr_pc));
    exit(1);
    break;
  }
}
