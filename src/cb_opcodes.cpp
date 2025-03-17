#include "../include/cpu.h"
#include <cstdint>
#include <cstdio>
#include <ctime>

const int ASN = 0;
const int ASM = 0;

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
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RLC_r8(&REGISTERS::b);
    break;
  case 0x01:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RLC_r8(&REGISTERS::c);
    break;
  case 0x02:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RLC_r8(&REGISTERS::d);
    break;
  case 0x03:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RLC_r8(&REGISTERS::e);
    break;
  case 0x04:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RLC_r8(&REGISTERS::h);
    break;
  case 0x05:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RLC_r8(&REGISTERS::l);
    break;
  case 0x06:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RLC_HL();
    break;
  case 0x07:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RLCA();
    break;
  case 0x08:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RRC_r8(&REGISTERS::b);
    break;
  case 0x09:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RRC_r8(&REGISTERS::c);
    break;
  case 0x0A:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RRC_r8(&REGISTERS::d);
    break;
  case 0x0B:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RRC_r8(&REGISTERS::e);
    break;
  case 0x0C:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RRC_r8(&REGISTERS::h);
    break;
  case 0x0D:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RRC_r8(&REGISTERS::b);
    break;
  case 0x0E:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RRC_HL();
    break;
  case 0x0F:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RRCA();
    break;
  case 0x10:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RL_r8(&REGISTERS::b);
    break;
  case 0x11:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RL_r8(&REGISTERS::c);
    break;
  case 0x12:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RL_r8(&REGISTERS::d);
    break;
  case 0x13:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RL_r8(&REGISTERS::e);
    break;
  case 0x14:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RL_r8(&REGISTERS::h);
    break;
  case 0x15:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RL_r8(&REGISTERS::l);
    break;
  case 0x16:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RL_HL();
    break;
  case 0x17:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RLA();
    break;
  case 0x18:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RR_r8(&REGISTERS::b);
    break;
  case 0x19:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RR_r8(&REGISTERS::c);
    break;
  case 0x1A:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RR_r8(&REGISTERS::d);
    break;
  case 0x1B:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RR_r8(&REGISTERS::e);
    break;
  case 0x1C:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RR_r8(&REGISTERS::h);
    break;
  case 0x1D:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RR_r8(&REGISTERS::b);
    break;
  case 0x1E:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RR_HL();
    break;
  case 0x1F:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    RRA();
    break;
  case 0x20:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SLA_r8(&REGISTERS::b);
    break;
  case 0x21:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SLA_r8(&REGISTERS::c);
    break;
  case 0x22:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SLA_r8(&REGISTERS::d);
    break;
  case 0x23:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SLA_r8(&REGISTERS::e);
    break;
  case 0x24:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SLA_r8(&REGISTERS::h);
    break;
  case 0x25:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SLA_r8(&REGISTERS::l);
    break;
  case 0x26:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SLA_HL();
    break;
  case 0x27:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SLA_r8(&REGISTERS::a);
    break;
  case 0x28:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRA_r8(&REGISTERS::b);
    break;
  case 0x29:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRA_r8(&REGISTERS::c);
    break;
  case 0x2A:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRA_r8(&REGISTERS::d);
    break;
  case 0x2B:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRA_r8(&REGISTERS::e);
    break;
  case 0x2C:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRA_r8(&REGISTERS::h);
    break;
  case 0x2D:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRA_r8(&REGISTERS::b);
    break;
  case 0x2E:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRA_HL();
    break;
  case 0x2F:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRA_r8(&REGISTERS::a);
    break;
  case 0x30:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SWAP_r8(&REGISTERS::b);
    break;
  case 0x31:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SWAP_r8(&REGISTERS::c);
    break;
  case 0x32:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SWAP_r8(&REGISTERS::d);
    break;
  case 0x33:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SWAP_r8(&REGISTERS::e);
    break;
  case 0x34:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SWAP_r8(&REGISTERS::h);
    break;
  case 0x35:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SWAP_r8(&REGISTERS::l);
    break;
  case 0x36:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SWAP_HL();
    break;
  case 0x37:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SWAP_r8(&REGISTERS::a);
    break;
  case 0x38:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRL_r8(&REGISTERS::b);
    break;
  case 0x39:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRL_r8(&REGISTERS::c);
    break;
  case 0x3A:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRL_r8(&REGISTERS::d);
    break;
  case 0x3B:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRL_r8(&REGISTERS::e);
    break;
  case 0x3C:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRL_r8(&REGISTERS::h);
    break;
  case 0x3D:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRL_r8(&REGISTERS::b);
    break;
  case 0x3E:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRL_HL();
    break;
  case 0x3F:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    SRL_r8(&REGISTERS::a);
    break;
  case 0x40:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(0, &REGISTERS::b);
    break;
  case 0x41:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(0, &REGISTERS::c);
    break;
  case 0x42:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(0, &REGISTERS::d);
    break;
  case 0x43:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(0, &REGISTERS::e);
    break;
  case 0x44:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(0, &REGISTERS::h);
    break;
  case 0x45:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(0, &REGISTERS::l);
    break;
  case 0x46:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_HL(0);
    break;
  case 0x47:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(0, &REGISTERS::a);
    break;
  case 0x48:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(1, &REGISTERS::b);
    break;
  case 0x49:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(1, &REGISTERS::c);
    break;
  case 0x4A:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(1, &REGISTERS::d);
    break;
  case 0x4B:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(1, &REGISTERS::e);
    break;
  case 0x4C:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(1, &REGISTERS::h);
    break;
  case 0x4D:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(1, &REGISTERS::l);
    break;
  case 0x4E:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_HL(1);
    break;
  case 0x4F:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(1, &REGISTERS::a);
    break;
  case 0x50:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(2, &REGISTERS::b);
    break;
  case 0x51:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(2, &REGISTERS::c);
    break;
  case 0x52:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(2, &REGISTERS::d);
    break;
  case 0x53:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(2, &REGISTERS::e);
    break;
  case 0x54:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(2, &REGISTERS::h);
    break;
  case 0x55:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(2, &REGISTERS::l);
    break;
  case 0x56:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_HL(2);
    break;
  case 0x57:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(2, &REGISTERS::a);
    break;
  case 0x58:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(3, &REGISTERS::b);
    break;
  case 0x59:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(3, &REGISTERS::c);
    break;
  case 0x5A:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(3, &REGISTERS::d);
    break;
  case 0x5B:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(3, &REGISTERS::e);
    break;
  case 0x5C:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(3, &REGISTERS::h);
    break;
  case 0x5D:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(3, &REGISTERS::l);
    break;
  case 0x5E:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_HL(3);
    break;
  case 0x5F:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(3, &REGISTERS::a);
    break;
  case 0x60:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(4, &REGISTERS::b);
    break;
  case 0x61:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(4, &REGISTERS::c);
    break;
  case 0x62:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(4, &REGISTERS::d);
    break;
  case 0x63:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(4, &REGISTERS::e);
    break;
  case 0x64:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(4, &REGISTERS::h);
    break;
  case 0x65:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(4, &REGISTERS::l);
    break;
  case 0x66:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_HL(4);
    break;
  case 0x67:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(4, &REGISTERS::a);
    break;
  case 0x68:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(5, &REGISTERS::b);
    break;
  case 0x69:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(5, &REGISTERS::c);
    break;
  case 0x6A:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(5, &REGISTERS::d);
    break;
  case 0x6B:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(5, &REGISTERS::e);
    break;
  case 0x6C:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(5, &REGISTERS::h);
    break;
  case 0x6D:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(5, &REGISTERS::l);
    break;
  case 0x6E:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_HL(5);
    break;
  case 0x6F:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    BIT_u3_r8(5, &REGISTERS::a);
    break;
  case 0x70:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x71:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x72:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x73:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x74:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x75:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x77:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x78:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x79:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x7A:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x7B:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x7C:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x7D:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x7E:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x7F:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x80:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x81:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x82:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x83:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x84:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x85:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x86:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x87:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x88:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x89:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x8A:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x8B:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x8C:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x8D:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x8E:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x8F:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x90:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x91:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x92:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x93:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x94:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x95:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x96:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x97:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x98:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x99:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x9A:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x9B:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x9C:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x9D:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x9E:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0x9F:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xa0:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xa1:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xa2:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xa3:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xa4:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xa5:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xa6:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xa7:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xa8:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xa9:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xaa:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xab:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xac:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xad:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xae:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xaf:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xb0:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xb1:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xb2:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xb3:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xb4:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xb5:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xb6:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xb7:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xb8:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xb9:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xbA:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xbB:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xbC:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xbD:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xbE:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xbF:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xc0:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xc1:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xc2:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xc3:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xc4:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xc5:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xc6:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xc7:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xc8:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xc9:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xca:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xcb:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xcc:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xcd:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xce:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xE0:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xE2:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xE6:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xE9:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xEA:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xf0:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xf3:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xFA:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xfb:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xfe:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  case 0xFF:
    printf(cb_disasm[ASM], curr_pc);
    clock_m += cb_clock_m_cycles[ASN];
    break;
  default:
    printf("ERROR: DEFAULT 0x%02X\n", curr_pc);
    printf("ERROR 0x%04X\n", read_byte(curr_pc));
    exit(1);
    break;
  }
}
