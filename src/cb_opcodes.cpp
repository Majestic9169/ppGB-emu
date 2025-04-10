#include "../include/cpu.h"
#include <cstdint>
#include <cstdio>
#include <ctime>

const char *cb_disasm[256] = {
    "RLC B\n",       // 0x00
    "RLC C\n",       // 0x01
    "RLC D\n",       // 0x02
    "RLC E\n",       // 0x03
    "RLC H\n",       // 0x04
    "RLC L\n",       // 0x05
    "RLC (HL)\n",    // 0x06
    "RLC A\n",       // 0x07
    "RRC B\n",       // 0x08
    "RRC C\n",       // 0x09
    "RRC D\n",       // 0x0a
    "RRC E\n",       // 0x0b
    "RRC H\n",       // 0x0c
    "RRC L\n",       // 0x0d
    "RRC (HL)\n",    // 0x0e
    "RRC A\n",       // 0x0f
    "RL B\n",        // 0x10
    "RL C\n",        // 0x11
    "RL D\n",        // 0x12
    "RL E\n",        // 0x13
    "RL H\n",        // 0x14
    "RL L\n",        // 0x15
    "RL (HL)\n",     // 0x16
    "RL A\n",        // 0x17
    "RR B\n",        // 0x18
    "RR C\n",        // 0x19
    "RR D\n",        // 0x1a
    "RR E\n",        // 0x1b
    "RR H\n",        // 0x1c
    "RR L\n",        // 0x1d
    "RR (HL)\n",     // 0x1e
    "RR A\n",        // 0x1f
    "SLA B\n",       // 0x20
    "SLA C\n",       // 0x21
    "SLA D\n",       // 0x22
    "SLA E\n",       // 0x23
    "SLA H\n",       // 0x24
    "SLA L\n",       // 0x25
    "SLA (HL)\n",    // 0x26
    "SLA A\n",       // 0x27
    "SRA B\n",       // 0x28
    "SRA C\n",       // 0x29
    "SRA D\n",       // 0x2a
    "SRA E\n",       // 0x2b
    "SRA H\n",       // 0x2c
    "SRA L\n",       // 0x2d
    "SRA (HL)\n",    // 0x2e
    "SRA A\n",       // 0x2f
    "SWAP B\n",      // 0x30
    "SWAP C\n",      // 0x31
    "SWAP D\n",      // 0x32
    "SWAP E\n",      // 0x33
    "SWAP H\n",      // 0x34
    "SWAP L\n",      // 0x35
    "SWAP (HL)\n",   // 0x36
    "SWAP A\n",      // 0x37
    "SRL B\n",       // 0x38
    "SRL C\n",       // 0x39
    "SRL D\n",       // 0x3a
    "SRL E\n",       // 0x3b
    "SRL H\n",       // 0x3c
    "SRL L\n",       // 0x3d
    "SRL (HL)\n",    // 0x3e
    "SRL A\n",       // 0x3f
    "BIT 0, B\n",    // 0x40
    "BIT 0, C\n",    // 0x41
    "BIT 0, D\n",    // 0x42
    "BIT 0, E\n",    // 0x43
    "BIT 0, H\n",    // 0x44
    "BIT 0, L\n",    // 0x45
    "BIT 0, (HL)\n", // 0x46
    "BIT 0, A\n",    // 0x47
    "BIT 1, B\n",    // 0x48
    "BIT 1, C\n",    // 0x49
    "BIT 1, D\n",    // 0x4a
    "BIT 1, E\n",    // 0x4b
    "BIT 1, H\n",    // 0x4c
    "BIT 1, L\n",    // 0x4d
    "BIT 1, (HL)\n", // 0x4e
    "BIT 1, A\n",    // 0x4f
    "BIT 2, B\n",    // 0x50
    "BIT 2, C\n",    // 0x51
    "BIT 2, D\n",    // 0x52
    "BIT 2, E\n",    // 0x53
    "BIT 2, H\n",    // 0x54
    "BIT 2, L\n",    // 0x55
    "BIT 2, (HL)\n", // 0x56
    "BIT 2, A\n",    // 0x57
    "BIT 3, B\n",    // 0x58
    "BIT 3, C\n",    // 0x59
    "BIT 3, D\n",    // 0x5a
    "BIT 3, E\n",    // 0x5b
    "BIT 3, H\n",    // 0x5c
    "BIT 3, L\n",    // 0x5d
    "BIT 3, (HL)\n", // 0x5e
    "BIT 3, A\n",    // 0x5f
    "BIT 4, B\n",    // 0x60
    "BIT 4, C\n",    // 0x61
    "BIT 4, D\n",    // 0x62
    "BIT 4, E\n",    // 0x63
    "BIT 4, H\n",    // 0x64
    "BIT 4, L\n",    // 0x65
    "BIT 4, (HL)\n", // 0x66
    "BIT 4, A\n",    // 0x67
    "BIT 5, B\n",    // 0x68
    "BIT 5, C\n",    // 0x69
    "BIT 5, D\n",    // 0x6a
    "BIT 5, E\n",    // 0x6b
    "BIT 6, H\n",    // 0x6c
    "BIT 6, L\n",    // 0x6d
    "BIT 5, (HL)\n", // 0x6e
    "BIT 5, A\n",    // 0x6f
    "BIT 6, B\n",    // 0x70
    "BIT 6, C\n",    // 0x71
    "BIT 6, D\n",    // 0x72
    "BIT 6, E\n",    // 0x73
    "BIT 6, H\n",    // 0x74
    "BIT 6, L\n",    // 0x75
    "BIT 6, (HL)\n", // 0x76
    "BIT 6, A\n",    // 0x77
    "BIT 7, B\n",    // 0x78
    "BIT 7, C\n",    // 0x79
    "BIT 7, D\n",    // 0x7a
    "BIT 7, E\n",    // 0x7b
    "BIT 7, H\n",    // 0x7c
    "BIT 7, L\n",    // 0x7d
    "BIT 7, (HL)\n", // 0x7e
    "BIT 7, A\n",    // 0x7f
    "RES 0, B\n",    // 0x80
    "RES 0, C\n",    // 0x81
    "RES 0, D\n",    // 0x82
    "RES 0, E\n",    // 0x83
    "RES 0, H\n",    // 0x84
    "RES 0, L\n",    // 0x85
    "RES 0, (HL)\n", // 0x86
    "RES 0, A\n",    // 0x87
    "RES 1, B\n",    // 0x88
    "RES 1, C\n",    // 0x89
    "RES 1, D\n",    // 0x8a
    "RES 1, E\n",    // 0x8b
    "RES 1, H\n",    // 0x8c
    "RES 1, L\n",    // 0x8d
    "RES 1, (HL)\n", // 0x8e
    "RES 1, A\n",    // 0x8f
    "RES 2, B\n",    // 0x90
    "RES 2, C\n",    // 0x91
    "RES 2, D\n",    // 0x92
    "RES 2, E\n",    // 0x93
    "RES 2, H\n",    // 0x94
    "RES 2, L\n",    // 0x95
    "RES 2, (HL)\n", // 0x96
    "RES 2, A\n",    // 0x97
    "RES 3, B\n",    // 0x98
    "RES 3, C\n",    // 0x99
    "RES 3, D\n",    // 0x9a
    "RES 3, E\n",    // 0x9b
    "RES 3, H\n",    // 0x9c
    "RES 3, L\n",    // 0x9d
    "RES 3, (HL)\n", // 0x9e
    "RES 3, A\n",    // 0x9f
    "RES 4, B\n",    // 0xa0
    "RES 4, C\n",    // 0xa1
    "RES 4, D\n",    // 0xa2
    "RES 4, E\n",    // 0xa3
    "RES 4, H\n",    // 0xa4
    "RES 4, L\n",    // 0xa5
    "RES 4, (HL)\n", // 0xa6
    "RES 4, A\n",    // 0xa7
    "RES 5, B\n",    // 0xa8
    "RES 5, C\n",    // 0xa9
    "RES 5, D\n",    // 0xaa
    "RES 5, E\n",    // 0xab
    "RES 5, H\n",    // 0xac
    "RES 5, L\n",    // 0xad
    "RES 5, (HL)\n", // 0xae
    "RES 5, A\n",    // 0xaf
    "RES 6, B\n",    // 0xb0
    "RES 6, C\n",    // 0xb1
    "RES 6, D\n",    // 0xb2
    "RES 6, E\n",    // 0xb3
    "RES 6, H\n",    // 0xb4
    "RES 6, L\n",    // 0xb5
    "RES 6, (HL)\n", // 0xb6
    "RES 6, A\n",    // 0xb7
    "RES 7, B\n",    // 0xb8
    "RES 7, C\n",    // 0xb9
    "RES 7, D\n",    // 0xba
    "RES 7, E\n",    // 0xbb
    "RES 7, H\n",    // 0xbc
    "RES 7, L\n",    // 0xbd
    "RES 7, (HL)\n", // 0xbe
    "RES 7, A\n",    // 0xbf
    "SET 0, B\n",    // 0xc0
    "SET 0, C\n",    // 0xc1
    "SET 0, D\n",    // 0xc2
    "SET 0, E\n",    // 0xc3
    "SET 0, H\n",    // 0xc4
    "SET 0, L\n",    // 0xc5
    "SET 0, (HL)\n", // 0xc6
    "SET 0, A\n",    // 0xc7
    "SET 1, B\n",    // 0xc8
    "SET 1, C\n",    // 0xc9
    "SET 1, D\n",    // 0xca
    "SET 1, E\n",    // 0xcb
    "SET 1, H\n",    // 0xcc
    "SET 1, L\n",    // 0xcd
    "SET 1, (HL)\n", // 0xce
    "SET 1, A\n",    // 0xcf
    "SET 2, B\n",    // 0xd0
    "SET 2, C\n",    // 0xd1
    "SET 2, D\n",    // 0xd2
    "SET 2, E\n",    // 0xd3
    "SET 2, H\n",    // 0xd4
    "SET 2, L\n",    // 0xd5
    "SET 2, (HL)\n", // 0xd6
    "SET 2, A\n",    // 0xd7
    "SET 3, B\n",    // 0xd8
    "SET 3, C\n",    // 0xd9
    "SET 3, D\n",    // 0xda
    "SET 3, E\n",    // 0xdb
    "SET 3, H\n",    // 0xdc
    "SET 3, L\n",    // 0xdd
    "SET 3, (HL)\n", // 0xde
    "SET 3, A\n",    // 0xdf
    "SET 4, B\n",    // 0xe0
    "SET 4, C\n",    // 0xe1
    "SET 4, D\n",    // 0xe2
    "SET 4, E\n",    // 0xe3
    "SET 4, H\n",    // 0xe4
    "SET 4, L\n",    // 0xe5
    "SET 4, (HL)\n", // 0xe6
    "SET 4, A\n",    // 0xe7
    "SET 5, B\n",    // 0xe8
    "SET 5, C\n",    // 0xe9
    "SET 5, D\n",    // 0xea
    "SET 5, E\n",    // 0xeb
    "SET 5, H\n",    // 0xec
    "SET 5, L\n",    // 0xed
    "SET 5, (HL)\n", // 0xee
    "SET 5, A\n",    // 0xef
    "SET 6, B\n",    // 0xf0
    "SET 6, C\n",    // 0xf1
    "SET 6, D\n",    // 0xf2
    "SET 6, E\n",    // 0xf3
    "SET 6, H\n",    // 0xf4
    "SET 6, L\n",    // 0xf5
    "SET 6, (HL)\n", // 0xf6
    "SET 6, A\n",    // 0xf7
    "SET 7, B\n",    // 0xf8
    "SET 7, C\n",    // 0xf9
    "SET 7, D\n",    // 0xfa
    "SET 7, E\n",    // 0xfb
    "SET 7, H\n",    // 0xfc
    "SET 7, L\n",    // 0xfd
    "SET 7, (HL)\n", // 0xfe
    "SET 7, A\n",    // 0xff
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
  uint16_t curr_op = read_byte(reg.pc++);
  clock_m += cb_clock_m_cycles[curr_op];

  printf("0x%04X: [cb%02x] ", curr_pc - 1, curr_op);

  switch (curr_op) {
  case 0x00:
    printf(cb_disasm[0x00], curr_pc);
    RLC_r8(&REGISTERS::b);
    break;
  case 0x01:
    printf(cb_disasm[0x01], curr_pc);
    RLC_r8(&REGISTERS::c);
    break;
  case 0x02:
    printf(cb_disasm[0x02], curr_pc);
    RLC_r8(&REGISTERS::d);
    break;
  case 0x03:
    printf(cb_disasm[0x03], curr_pc);
    RLC_r8(&REGISTERS::e);
    break;
  case 0x04:
    printf(cb_disasm[0x04], curr_pc);
    RLC_r8(&REGISTERS::h);
    break;
  case 0x05:
    printf(cb_disasm[0x05], curr_pc);
    RLC_r8(&REGISTERS::l);
    break;
  case 0x06:
    printf(cb_disasm[0x06], curr_pc);
    RLC_HL();
    break;
  case 0x07:
    printf(cb_disasm[0x07], curr_pc);
    RLCA();
    break;
  case 0x08:
    printf(cb_disasm[0x08], curr_pc);
    RRC_r8(&REGISTERS::b);
    break;
  case 0x09:
    printf(cb_disasm[0x09], curr_pc);
    RRC_r8(&REGISTERS::c);
    break;
  case 0x0A:
    printf(cb_disasm[0x0a], curr_pc);
    RRC_r8(&REGISTERS::d);
    break;
  case 0x0B:
    printf(cb_disasm[0x0b], curr_pc);
    RRC_r8(&REGISTERS::e);
    break;
  case 0x0C:
    printf(cb_disasm[0x0c], curr_pc);
    RRC_r8(&REGISTERS::h);
    break;
  case 0x0D:
    printf(cb_disasm[0x0d], curr_pc);
    RRC_r8(&REGISTERS::b);
    break;
  case 0x0E:
    printf(cb_disasm[0x0e], curr_pc);
    RRC_HL();
    break;
  case 0x0F:
    printf(cb_disasm[0x0f], curr_pc);
    RRCA();
    break;
  case 0x10:
    printf(cb_disasm[0x10], curr_pc);
    RL_r8(&REGISTERS::b);
    break;
  case 0x11:
    printf(cb_disasm[0x11], curr_pc);
    RL_r8(&REGISTERS::c);
    break;
  case 0x12:
    printf(cb_disasm[0x12], curr_pc);
    RL_r8(&REGISTERS::d);
    break;
  case 0x13:
    printf(cb_disasm[0x13], curr_pc);
    RL_r8(&REGISTERS::e);
    break;
  case 0x14:
    printf(cb_disasm[0x14], curr_pc);
    RL_r8(&REGISTERS::h);
    break;
  case 0x15:
    printf(cb_disasm[0x15], curr_pc);
    RL_r8(&REGISTERS::l);
    break;
  case 0x16:
    printf(cb_disasm[0x16], curr_pc);
    RL_HL();
    break;
  case 0x17:
    printf(cb_disasm[0x17], curr_pc);
    RLA();
    break;
  case 0x18:
    printf(cb_disasm[0x18], curr_pc);
    RR_r8(&REGISTERS::b);
    break;
  case 0x19:
    printf(cb_disasm[0x19], curr_pc);
    RR_r8(&REGISTERS::c);
    break;
  case 0x1A:
    printf(cb_disasm[0x1a], curr_pc);
    RR_r8(&REGISTERS::d);
    break;
  case 0x1B:
    printf(cb_disasm[0x1b], curr_pc);
    RR_r8(&REGISTERS::e);
    break;
  case 0x1C:
    printf(cb_disasm[0x1c], curr_pc);
    RR_r8(&REGISTERS::h);
    break;
  case 0x1D:
    printf(cb_disasm[0x1d], curr_pc);
    RR_r8(&REGISTERS::b);
    break;
  case 0x1E:
    printf(cb_disasm[0x1e], curr_pc);
    RR_HL();
    break;
  case 0x1F:
    printf(cb_disasm[0x1f], curr_pc);
    RRA();
    break;
  case 0x20:
    printf(cb_disasm[0x20], curr_pc);
    SLA_r8(&REGISTERS::b);
    break;
  case 0x21:
    printf(cb_disasm[0x21], curr_pc);
    SLA_r8(&REGISTERS::c);
    break;
  case 0x22:
    printf(cb_disasm[0x22], curr_pc);
    SLA_r8(&REGISTERS::d);
    break;
  case 0x23:
    printf(cb_disasm[0x23], curr_pc);
    SLA_r8(&REGISTERS::e);
    break;
  case 0x24:
    printf(cb_disasm[0x24], curr_pc);
    SLA_r8(&REGISTERS::h);
    break;
  case 0x25:
    printf(cb_disasm[0x25], curr_pc);
    SLA_r8(&REGISTERS::l);
    break;
  case 0x26:
    printf(cb_disasm[0x26], curr_pc);
    SLA_HL();
    break;
  case 0x27:
    printf(cb_disasm[0x27], curr_pc);
    SLA_r8(&REGISTERS::a);
    break;
  case 0x28:
    printf(cb_disasm[0x28], curr_pc);
    SRA_r8(&REGISTERS::b);
    break;
  case 0x29:
    printf(cb_disasm[0x29], curr_pc);
    SRA_r8(&REGISTERS::c);
    break;
  case 0x2A:
    printf(cb_disasm[0x2a], curr_pc);
    SRA_r8(&REGISTERS::d);
    break;
  case 0x2B:
    printf(cb_disasm[0x2b], curr_pc);
    SRA_r8(&REGISTERS::e);
    break;
  case 0x2C:
    printf(cb_disasm[0x2c], curr_pc);
    SRA_r8(&REGISTERS::h);
    break;
  case 0x2D:
    printf(cb_disasm[0x2d], curr_pc);
    SRA_r8(&REGISTERS::b);
    break;
  case 0x2E:
    printf(cb_disasm[0x2e], curr_pc);
    SRA_HL();
    break;
  case 0x2F:
    printf(cb_disasm[0x2f], curr_pc);
    SRA_r8(&REGISTERS::a);
    break;
  case 0x30:
    printf(cb_disasm[0x30], curr_pc);
    SWAP_r8(&REGISTERS::b);
    break;
  case 0x31:
    printf(cb_disasm[0x31], curr_pc);
    SWAP_r8(&REGISTERS::c);
    break;
  case 0x32:
    printf(cb_disasm[0x32], curr_pc);
    SWAP_r8(&REGISTERS::d);
    break;
  case 0x33:
    printf(cb_disasm[0x33], curr_pc);
    SWAP_r8(&REGISTERS::e);
    break;
  case 0x34:
    printf(cb_disasm[0x34], curr_pc);
    SWAP_r8(&REGISTERS::h);
    break;
  case 0x35:
    printf(cb_disasm[0x35], curr_pc);
    SWAP_r8(&REGISTERS::l);
    break;
  case 0x36:
    printf(cb_disasm[0x36], curr_pc);
    SWAP_HL();
    break;
  case 0x37:
    printf(cb_disasm[0x37], curr_pc);
    SWAP_r8(&REGISTERS::a);
    break;
  case 0x38:
    printf(cb_disasm[0x38], curr_pc);
    SRL_r8(&REGISTERS::b);
    break;
  case 0x39:
    printf(cb_disasm[0x39], curr_pc);
    SRL_r8(&REGISTERS::c);
    break;
  case 0x3A:
    printf(cb_disasm[0x3a], curr_pc);
    SRL_r8(&REGISTERS::d);
    break;
  case 0x3B:
    printf(cb_disasm[0x3b], curr_pc);
    SRL_r8(&REGISTERS::e);
    break;
  case 0x3C:
    printf(cb_disasm[0x3c], curr_pc);
    SRL_r8(&REGISTERS::h);
    break;
  case 0x3D:
    printf(cb_disasm[0x3d], curr_pc);
    SRL_r8(&REGISTERS::b);
    break;
  case 0x3E:
    printf(cb_disasm[0x3e], curr_pc);
    SRL_HL();
    break;
  case 0x3F:
    printf(cb_disasm[0x3f], curr_pc);
    SRL_r8(&REGISTERS::a);
    break;
  case 0x40:
    printf(cb_disasm[0x40], curr_pc);
    BIT_u3_r8(0, &REGISTERS::b);
    break;
  case 0x41:
    printf(cb_disasm[0x41], curr_pc);
    BIT_u3_r8(0, &REGISTERS::c);
    break;
  case 0x42:
    printf(cb_disasm[0x42], curr_pc);
    BIT_u3_r8(0, &REGISTERS::d);
    break;
  case 0x43:
    printf(cb_disasm[0x43], curr_pc);
    BIT_u3_r8(0, &REGISTERS::e);
    break;
  case 0x44:
    printf(cb_disasm[0x44], curr_pc);
    BIT_u3_r8(0, &REGISTERS::h);
    break;
  case 0x45:
    printf(cb_disasm[0x45], curr_pc);
    BIT_u3_r8(0, &REGISTERS::l);
    break;
  case 0x46:
    printf(cb_disasm[0x46], curr_pc);
    BIT_u3_HL(0);
    break;
  case 0x47:
    printf(cb_disasm[0x47], curr_pc);
    BIT_u3_r8(0, &REGISTERS::a);
    break;
  case 0x48:
    printf(cb_disasm[0x48], curr_pc);
    BIT_u3_r8(1, &REGISTERS::b);
    break;
  case 0x49:
    printf(cb_disasm[0x49], curr_pc);
    BIT_u3_r8(1, &REGISTERS::c);
    break;
  case 0x4A:
    printf(cb_disasm[0x4a], curr_pc);
    BIT_u3_r8(1, &REGISTERS::d);
    break;
  case 0x4B:
    printf(cb_disasm[0x4b], curr_pc);
    BIT_u3_r8(1, &REGISTERS::e);
    break;
  case 0x4C:
    printf(cb_disasm[0x4c], curr_pc);
    BIT_u3_r8(1, &REGISTERS::h);
    break;
  case 0x4D:
    printf(cb_disasm[0x4d], curr_pc);
    BIT_u3_r8(1, &REGISTERS::l);
    break;
  case 0x4E:
    printf(cb_disasm[0x4e], curr_pc);
    BIT_u3_HL(1);
    break;
  case 0x4F:
    printf(cb_disasm[0x4f], curr_pc);
    BIT_u3_r8(1, &REGISTERS::a);
    break;
  case 0x50:
    printf(cb_disasm[0x50], curr_pc);
    BIT_u3_r8(2, &REGISTERS::b);
    break;
  case 0x51:
    printf(cb_disasm[0x51], curr_pc);
    BIT_u3_r8(2, &REGISTERS::c);
    break;
  case 0x52:
    printf(cb_disasm[0x52], curr_pc);
    BIT_u3_r8(2, &REGISTERS::d);
    break;
  case 0x53:
    printf(cb_disasm[0x53], curr_pc);
    BIT_u3_r8(2, &REGISTERS::e);
    break;
  case 0x54:
    printf(cb_disasm[0x54], curr_pc);
    BIT_u3_r8(2, &REGISTERS::h);
    break;
  case 0x55:
    printf(cb_disasm[0x55], curr_pc);
    BIT_u3_r8(2, &REGISTERS::l);
    break;
  case 0x56:
    printf(cb_disasm[0x56], curr_pc);
    BIT_u3_HL(2);
    break;
  case 0x57:
    printf(cb_disasm[0x57], curr_pc);
    BIT_u3_r8(2, &REGISTERS::a);
    break;
  case 0x58:
    printf(cb_disasm[0x58], curr_pc);
    BIT_u3_r8(3, &REGISTERS::b);
    break;
  case 0x59:
    printf(cb_disasm[0x59], curr_pc);
    BIT_u3_r8(3, &REGISTERS::c);
    break;
  case 0x5A:
    printf(cb_disasm[0x5a], curr_pc);
    BIT_u3_r8(3, &REGISTERS::d);
    break;
  case 0x5B:
    printf(cb_disasm[0x5b], curr_pc);
    BIT_u3_r8(3, &REGISTERS::e);
    break;
  case 0x5C:
    printf(cb_disasm[0x5c], curr_pc);
    BIT_u3_r8(3, &REGISTERS::h);
    break;
  case 0x5D:
    printf(cb_disasm[0x5d], curr_pc);
    BIT_u3_r8(3, &REGISTERS::l);
    break;
  case 0x5E:
    printf(cb_disasm[0x5e], curr_pc);
    BIT_u3_HL(3);
    break;
  case 0x5F:
    printf(cb_disasm[0x5f], curr_pc);
    BIT_u3_r8(3, &REGISTERS::a);
    break;
  case 0x60:
    printf(cb_disasm[0x60], curr_pc);
    BIT_u3_r8(4, &REGISTERS::b);
    break;
  case 0x61:
    printf(cb_disasm[0x61], curr_pc);
    BIT_u3_r8(4, &REGISTERS::c);
    break;
  case 0x62:
    printf(cb_disasm[0x62], curr_pc);
    BIT_u3_r8(4, &REGISTERS::d);
    break;
  case 0x63:
    printf(cb_disasm[0x63], curr_pc);
    BIT_u3_r8(4, &REGISTERS::e);
    break;
  case 0x64:
    printf(cb_disasm[0x64], curr_pc);
    BIT_u3_r8(4, &REGISTERS::h);
    break;
  case 0x65:
    printf(cb_disasm[0x65], curr_pc);
    BIT_u3_r8(4, &REGISTERS::l);
    break;
  case 0x66:
    printf(cb_disasm[0x66], curr_pc);
    BIT_u3_HL(4);
    break;
  case 0x67:
    printf(cb_disasm[0x67], curr_pc);
    BIT_u3_r8(4, &REGISTERS::a);
    break;
  case 0x68:
    printf(cb_disasm[0x68], curr_pc);
    BIT_u3_r8(5, &REGISTERS::b);
    break;
  case 0x69:
    printf(cb_disasm[0x69], curr_pc);
    BIT_u3_r8(5, &REGISTERS::c);
    break;
  case 0x6A:
    printf(cb_disasm[0x6a], curr_pc);
    BIT_u3_r8(5, &REGISTERS::d);
    break;
  case 0x6B:
    printf(cb_disasm[0x6b], curr_pc);
    BIT_u3_r8(5, &REGISTERS::e);
    break;
  case 0x6C:
    printf(cb_disasm[0x6c], curr_pc);
    BIT_u3_r8(5, &REGISTERS::h);
    break;
  case 0x6D:
    printf(cb_disasm[0x6d], curr_pc);
    BIT_u3_r8(5, &REGISTERS::l);
    break;
  case 0x6E:
    printf(cb_disasm[0x6e], curr_pc);
    BIT_u3_HL(5);
    break;
  case 0x6F:
    printf(cb_disasm[0x6f], curr_pc);
    BIT_u3_r8(5, &REGISTERS::a);
    break;
  case 0x70:
    printf(cb_disasm[0x70], curr_pc);
    BIT_u3_r8(6, &REGISTERS::b);
    break;
  case 0x71:
    printf(cb_disasm[0x71], curr_pc);
    BIT_u3_r8(6, &REGISTERS::c);
    break;
  case 0x72:
    printf(cb_disasm[0x72], curr_pc);
    BIT_u3_r8(6, &REGISTERS::d);
    break;
  case 0x73:
    printf(cb_disasm[0x73], curr_pc);
    BIT_u3_r8(6, &REGISTERS::e);
    break;
  case 0x74:
    printf(cb_disasm[0x74], curr_pc);
    BIT_u3_r8(6, &REGISTERS::h);
    break;
  case 0x75:
    printf(cb_disasm[0x75], curr_pc);
    BIT_u3_r8(6, &REGISTERS::l);
    break;
  case 0x76:
    printf(cb_disasm[0x76], curr_pc);
    BIT_u3_HL(6);
    break;
  case 0x77:
    printf(cb_disasm[0x77], curr_pc);
    BIT_u3_r8(6, &REGISTERS::a);
    break;
  case 0x78:
    printf(cb_disasm[0x78], curr_pc);
    BIT_u3_r8(7, &REGISTERS::b);
    break;
  case 0x79:
    printf(cb_disasm[0x79], curr_pc);
    BIT_u3_r8(7, &REGISTERS::c);
    break;
  case 0x7A:
    printf(cb_disasm[0x7a], curr_pc);
    BIT_u3_r8(7, &REGISTERS::d);
    break;
  case 0x7B:
    printf(cb_disasm[0x7b], curr_pc);
    BIT_u3_r8(7, &REGISTERS::e);
    break;
  case 0x7C:
    printf(cb_disasm[0x7c], curr_pc);
    BIT_u3_r8(7, &REGISTERS::h);
    break;
  case 0x7D:
    printf(cb_disasm[0x7d], curr_pc);
    BIT_u3_r8(7, &REGISTERS::l);
    break;
  case 0x7E:
    printf(cb_disasm[0x7e], curr_pc);
    BIT_u3_HL(7);
    break;
  case 0x7F:
    printf(cb_disasm[0x7f], curr_pc);
    BIT_u3_r8(7, &REGISTERS::a);
    break;
  case 0x80:
    printf(cb_disasm[0x80], curr_pc);
    RES_u3_r8(0, &REGISTERS::b);
    break;
  case 0x81:
    printf(cb_disasm[0x81], curr_pc);
    RES_u3_r8(0, &REGISTERS::c);
    break;
  case 0x82:
    printf(cb_disasm[0x82], curr_pc);
    RES_u3_r8(0, &REGISTERS::d);
    break;
  case 0x83:
    printf(cb_disasm[0x83], curr_pc);
    RES_u3_r8(0, &REGISTERS::e);
    break;
  case 0x84:
    printf(cb_disasm[0x84], curr_pc);
    RES_u3_r8(0, &REGISTERS::h);
    break;
  case 0x85:
    printf(cb_disasm[0x85], curr_pc);
    RES_u3_r8(0, &REGISTERS::l);
    break;
  case 0x86:
    printf(cb_disasm[0x86], curr_pc);
    RES_u3_HL(0);
    break;
  case 0x87:
    printf(cb_disasm[0x87], curr_pc);
    RES_u3_r8(0, &REGISTERS::a);
    break;
  case 0x88:
    printf(cb_disasm[0x88], curr_pc);
    RES_u3_r8(1, &REGISTERS::b);
    break;
  case 0x89:
    printf(cb_disasm[0x89], curr_pc);
    RES_u3_r8(1, &REGISTERS::c);
    break;
  case 0x8A:
    printf(cb_disasm[0x8a], curr_pc);
    RES_u3_r8(1, &REGISTERS::d);
    break;
  case 0x8B:
    printf(cb_disasm[0x8b], curr_pc);
    RES_u3_r8(1, &REGISTERS::e);
    break;
  case 0x8C:
    printf(cb_disasm[0x8c], curr_pc);
    RES_u3_r8(1, &REGISTERS::h);
    break;
  case 0x8D:
    printf(cb_disasm[0x8d], curr_pc);
    RES_u3_r8(1, &REGISTERS::l);
    break;
  case 0x8E:
    printf(cb_disasm[0x8e], curr_pc);
    RES_u3_HL(1);
    break;
  case 0x8F:
    printf(cb_disasm[0x8f], curr_pc);
    RES_u3_r8(1, &REGISTERS::a);
    break;
  case 0x90:
    printf(cb_disasm[0x90], curr_pc);
    RES_u3_r8(2, &REGISTERS::b);
    break;
  case 0x91:
    printf(cb_disasm[0x91], curr_pc);
    RES_u3_r8(2, &REGISTERS::c);
    break;
  case 0x92:
    printf(cb_disasm[0x92], curr_pc);
    RES_u3_r8(2, &REGISTERS::d);
    break;
  case 0x93:
    printf(cb_disasm[0x93], curr_pc);
    RES_u3_r8(2, &REGISTERS::e);
    break;
  case 0x94:
    printf(cb_disasm[0x94], curr_pc);
    RES_u3_r8(2, &REGISTERS::h);
    break;
  case 0x95:
    printf(cb_disasm[0x95], curr_pc);
    RES_u3_r8(2, &REGISTERS::l);
    break;
  case 0x96:
    printf(cb_disasm[0x96], curr_pc);
    RES_u3_HL(2);
    break;
  case 0x97:
    printf(cb_disasm[0x97], curr_pc);
    RES_u3_r8(2, &REGISTERS::a);
    break;
  case 0x98:
    printf(cb_disasm[0x98], curr_pc);
    RES_u3_r8(3, &REGISTERS::b);
    break;
  case 0x99:
    printf(cb_disasm[0x99], curr_pc);
    RES_u3_r8(3, &REGISTERS::c);
    break;
  case 0x9A:
    printf(cb_disasm[0x9a], curr_pc);
    RES_u3_r8(3, &REGISTERS::d);
    break;
  case 0x9B:
    printf(cb_disasm[0x9b], curr_pc);
    RES_u3_r8(3, &REGISTERS::e);
    break;
  case 0x9C:
    printf(cb_disasm[0x9c], curr_pc);
    RES_u3_r8(3, &REGISTERS::h);
    break;
  case 0x9D:
    printf(cb_disasm[0x9d], curr_pc);
    RES_u3_r8(3, &REGISTERS::l);
    break;
  case 0x9E:
    printf(cb_disasm[0x9e], curr_pc);
    RES_u3_HL(3);
    break;
  case 0x9F:
    printf(cb_disasm[0x9f], curr_pc);
    RES_u3_r8(3, &REGISTERS::a);
    break;
  case 0xA0:
    printf(cb_disasm[0xa0], curr_pc);
    RES_u3_r8(4, &REGISTERS::b);
    break;
  case 0xA1:
    printf(cb_disasm[0xa1], curr_pc);
    RES_u3_r8(4, &REGISTERS::c);
    break;
  case 0xA2:
    printf(cb_disasm[0xa2], curr_pc);
    RES_u3_r8(4, &REGISTERS::d);
    break;
  case 0xA3:
    printf(cb_disasm[0xa3], curr_pc);
    RES_u3_r8(4, &REGISTERS::e);
    break;
  case 0xA4:
    printf(cb_disasm[0xa4], curr_pc);
    RES_u3_r8(4, &REGISTERS::h);
    break;
  case 0xA5:
    printf(cb_disasm[0xa5], curr_pc);
    RES_u3_r8(4, &REGISTERS::l);
    break;
  case 0xA6:
    printf(cb_disasm[0xa6], curr_pc);
    RES_u3_HL(4);
    break;
  case 0xA7:
    printf(cb_disasm[0xa7], curr_pc);
    RES_u3_r8(4, &REGISTERS::a);
    break;
  case 0xA8:
    printf(cb_disasm[0xa8], curr_pc);
    RES_u3_r8(5, &REGISTERS::b);
    break;
  case 0xA9:
    printf(cb_disasm[0xa9], curr_pc);
    RES_u3_r8(5, &REGISTERS::c);
    break;
  case 0xAA:
    printf(cb_disasm[0xaa], curr_pc);
    RES_u3_r8(5, &REGISTERS::d);
    break;
  case 0xAB:
    printf(cb_disasm[0xab], curr_pc);
    RES_u3_r8(5, &REGISTERS::e);
    break;
  case 0xAC:
    printf(cb_disasm[0xac], curr_pc);
    RES_u3_r8(5, &REGISTERS::h);
    break;
  case 0xAD:
    printf(cb_disasm[0xad], curr_pc);
    RES_u3_r8(5, &REGISTERS::l);
    break;
  case 0xAE:
    printf(cb_disasm[0xae], curr_pc);
    RES_u3_HL(5);
    break;
  case 0xAF:
    printf(cb_disasm[0xaf], curr_pc);
    RES_u3_r8(5, &REGISTERS::a);
    break;
  case 0xB0:
    printf(cb_disasm[0xb0], curr_pc);
    RES_u3_r8(6, &REGISTERS::b);
    break;
  case 0xB1:
    printf(cb_disasm[0xb1], curr_pc);
    RES_u3_r8(6, &REGISTERS::c);
    break;
  case 0xB2:
    printf(cb_disasm[0xb2], curr_pc);
    RES_u3_r8(6, &REGISTERS::d);
    break;
  case 0xB3:
    printf(cb_disasm[0xb3], curr_pc);
    RES_u3_r8(6, &REGISTERS::e);
    break;
  case 0xB4:
    printf(cb_disasm[0xb4], curr_pc);
    RES_u3_r8(6, &REGISTERS::h);
    break;
  case 0xB5:
    printf(cb_disasm[0xb5], curr_pc);
    RES_u3_r8(6, &REGISTERS::l);
    break;
  case 0xB6:
    printf(cb_disasm[0xb6], curr_pc);
    RES_u3_HL(6);
    break;
  case 0xB7:
    printf(cb_disasm[0xb7], curr_pc);
    RES_u3_r8(6, &REGISTERS::a);
    break;
  case 0xB8:
    printf(cb_disasm[0xb8], curr_pc);
    RES_u3_r8(7, &REGISTERS::b);
    break;
  case 0xB9:
    printf(cb_disasm[0xb9], curr_pc);
    RES_u3_r8(7, &REGISTERS::c);
    break;
  case 0xBA:
    printf(cb_disasm[0xba], curr_pc);
    RES_u3_r8(7, &REGISTERS::d);
    break;
  case 0xBB:
    printf(cb_disasm[0xbb], curr_pc);
    RES_u3_r8(7, &REGISTERS::e);
    break;
  case 0xBC:
    printf(cb_disasm[0xbc], curr_pc);
    RES_u3_r8(7, &REGISTERS::h);
    break;
  case 0xBD:
    printf(cb_disasm[0xbd], curr_pc);
    RES_u3_r8(7, &REGISTERS::l);
    break;
  case 0xBE:
    printf(cb_disasm[0xbe], curr_pc);
    RES_u3_HL(7);
    break;
  case 0xBF:
    printf(cb_disasm[0xbf], curr_pc);
    RES_u3_r8(7, &REGISTERS::a);
    break;
  case 0xC0:
    printf(cb_disasm[0xc0], curr_pc);
    SET_u3_r8(0, &REGISTERS::b);
    break;
  case 0xC1:
    printf(cb_disasm[0xc1], curr_pc);
    SET_u3_r8(0, &REGISTERS::c);
    break;
  case 0xC2:
    printf(cb_disasm[0xc2], curr_pc);
    SET_u3_r8(0, &REGISTERS::d);
    break;
  case 0xC3:
    printf(cb_disasm[0xc3], curr_pc);
    SET_u3_r8(0, &REGISTERS::e);
    break;
  case 0xC4:
    printf(cb_disasm[0xc4], curr_pc);
    SET_u3_r8(0, &REGISTERS::h);
    break;
  case 0xC5:
    printf(cb_disasm[0xc5], curr_pc);
    SET_u3_r8(0, &REGISTERS::l);
    break;
  case 0xC6:
    printf(cb_disasm[0xc6], curr_pc);
    SET_u3_HL(0);
    break;
  case 0xC7:
    printf(cb_disasm[0xc7], curr_pc);
    SET_u3_r8(0, &REGISTERS::a);
    break;
  case 0xC8:
    printf(cb_disasm[0xc8], curr_pc);
    SET_u3_r8(1, &REGISTERS::b);
    break;
  case 0xC9:
    printf(cb_disasm[0xc9], curr_pc);
    SET_u3_r8(1, &REGISTERS::c);
    break;
  case 0xCA:
    printf(cb_disasm[0xca], curr_pc);
    SET_u3_r8(1, &REGISTERS::d);
    break;
  case 0xCB:
    printf(cb_disasm[0xcb], curr_pc);
    SET_u3_r8(1, &REGISTERS::e);
    break;
  case 0xCC:
    printf(cb_disasm[0xcc], curr_pc);
    SET_u3_r8(1, &REGISTERS::h);
    break;
  case 0xCD:
    printf(cb_disasm[0xcd], curr_pc);
    SET_u3_r8(1, &REGISTERS::l);
    break;
  case 0xCE:
    printf(cb_disasm[0xce], curr_pc);
    SET_u3_HL(1);
    break;
  case 0xCF:
    printf(cb_disasm[0xcf], curr_pc);
    SET_u3_r8(1, &REGISTERS::a);
    break;
  case 0xD0:
    printf(cb_disasm[0xd0], curr_pc);
    SET_u3_r8(2, &REGISTERS::b);
    break;
  case 0xD1:
    printf(cb_disasm[0xd1], curr_pc);
    SET_u3_r8(2, &REGISTERS::c);
    break;
  case 0xD2:
    printf(cb_disasm[0xd2], curr_pc);
    SET_u3_r8(2, &REGISTERS::d);
    break;
  case 0xD3:
    printf(cb_disasm[0xd3], curr_pc);
    SET_u3_r8(2, &REGISTERS::e);
    break;
  case 0xD4:
    printf(cb_disasm[0xd4], curr_pc);
    SET_u3_r8(2, &REGISTERS::h);
    break;
  case 0xD5:
    printf(cb_disasm[0xd5], curr_pc);
    SET_u3_r8(2, &REGISTERS::l);
    break;
  case 0xD6:
    printf(cb_disasm[0xd6], curr_pc);
    SET_u3_HL(2);
    break;
  case 0xD7:
    printf(cb_disasm[0xd7], curr_pc);
    SET_u3_r8(2, &REGISTERS::a);
    break;
  case 0xD8:
    printf(cb_disasm[0xd8], curr_pc);
    SET_u3_r8(3, &REGISTERS::b);
    break;
  case 0xD9:
    printf(cb_disasm[0xd9], curr_pc);
    SET_u3_r8(3, &REGISTERS::c);
    break;
  case 0xDA:
    printf(cb_disasm[0xda], curr_pc);
    SET_u3_r8(3, &REGISTERS::d);
    break;
  case 0xDB:
    printf(cb_disasm[0xdb], curr_pc);
    SET_u3_r8(3, &REGISTERS::e);
    break;
  case 0xDC:
    printf(cb_disasm[0xdc], curr_pc);
    SET_u3_r8(3, &REGISTERS::h);
    break;
  case 0xDD:
    printf(cb_disasm[0xdd], curr_pc);
    SET_u3_r8(3, &REGISTERS::l);
    break;
  case 0xDE:
    printf(cb_disasm[0xde], curr_pc);
    SET_u3_HL(3);
    break;
  case 0xDF:
    printf(cb_disasm[0xdf], curr_pc);
    SET_u3_r8(3, &REGISTERS::a);
    break;
  case 0xE0:
    printf(cb_disasm[0xe0], curr_pc);
    SET_u3_r8(4, &REGISTERS::b);
    break;
  case 0xE1:
    printf(cb_disasm[0xe1], curr_pc);
    SET_u3_r8(4, &REGISTERS::c);
    break;
  case 0xE2:
    printf(cb_disasm[0xe2], curr_pc);
    SET_u3_r8(4, &REGISTERS::d);
    break;
  case 0xE3:
    printf(cb_disasm[0xe3], curr_pc);
    SET_u3_r8(4, &REGISTERS::e);
    break;
  case 0xE4:
    printf(cb_disasm[0xe4], curr_pc);
    SET_u3_r8(4, &REGISTERS::h);
    break;
  case 0xE5:
    printf(cb_disasm[0xe5], curr_pc);
    SET_u3_r8(4, &REGISTERS::l);
    break;
  case 0xE6:
    printf(cb_disasm[0xe6], curr_pc);
    SET_u3_HL(4);
    break;
  case 0xE7:
    printf(cb_disasm[0xe7], curr_pc);
    SET_u3_r8(4, &REGISTERS::a);
    break;
  case 0xE8:
    printf(cb_disasm[0xe8], curr_pc);
    SET_u3_r8(5, &REGISTERS::b);
    break;
  case 0xE9:
    printf(cb_disasm[0xe9], curr_pc);
    SET_u3_r8(5, &REGISTERS::c);
    break;
  case 0xEA:
    printf(cb_disasm[0xea], curr_pc);
    SET_u3_r8(5, &REGISTERS::d);
    break;
  case 0xEB:
    printf(cb_disasm[0xeb], curr_pc);
    SET_u3_r8(5, &REGISTERS::e);
    break;
  case 0xEC:
    printf(cb_disasm[0xec], curr_pc);
    SET_u3_r8(5, &REGISTERS::h);
    break;
  case 0xED:
    printf(cb_disasm[0xed], curr_pc);
    SET_u3_r8(5, &REGISTERS::l);
    break;
  case 0xEE:
    printf(cb_disasm[0xee], curr_pc);
    SET_u3_HL(5);
    break;
  case 0xEF:
    printf(cb_disasm[0xef], curr_pc);
    SET_u3_r8(5, &REGISTERS::a);
    break;
  case 0xF0:
    printf(cb_disasm[0xf0], curr_pc);
    SET_u3_r8(6, &REGISTERS::b);
    break;
  case 0xF1:
    printf(cb_disasm[0xf1], curr_pc);
    SET_u3_r8(6, &REGISTERS::c);
    break;
  case 0xF2:
    printf(cb_disasm[0xf2], curr_pc);
    SET_u3_r8(6, &REGISTERS::d);
    break;
  case 0xF3:
    printf(cb_disasm[0xf3], curr_pc);
    SET_u3_r8(6, &REGISTERS::e);
    break;
  case 0xF4:
    printf(cb_disasm[0xf4], curr_pc);
    SET_u3_r8(6, &REGISTERS::h);
    break;
  case 0xF5:
    printf(cb_disasm[0xf5], curr_pc);
    SET_u3_r8(6, &REGISTERS::l);
    break;
  case 0xF6:
    printf(cb_disasm[0xf6], curr_pc);
    SET_u3_HL(6);
    break;
  case 0xF7:
    printf(cb_disasm[0xf7], curr_pc);
    SET_u3_r8(6, &REGISTERS::a);
    break;
  case 0xF8:
    printf(cb_disasm[0xf8], curr_pc);
    SET_u3_r8(7, &REGISTERS::b);
    break;
  case 0xF9:
    printf(cb_disasm[0xf9], curr_pc);
    SET_u3_r8(7, &REGISTERS::c);
    break;
  case 0xFA:
    printf(cb_disasm[0xfa], curr_pc);
    SET_u3_r8(7, &REGISTERS::d);
    break;
  case 0xFB:
    printf(cb_disasm[0xfb], curr_pc);
    SET_u3_r8(7, &REGISTERS::e);
    break;
  case 0xFC:
    printf(cb_disasm[0xfc], curr_pc);
    SET_u3_r8(7, &REGISTERS::h);
    break;
  case 0xFD:
    printf(cb_disasm[0xfd], curr_pc);
    SET_u3_r8(7, &REGISTERS::l);
    break;
  case 0xFE:
    printf(cb_disasm[0xfe], curr_pc);
    SET_u3_HL(7);
    break;
  case 0xFF:
    printf(cb_disasm[0xff], curr_pc);
    SET_u3_r8(7, &REGISTERS::a);
    break;
  default:
    printf("ERROR: DEFAULT 0x%02X\n", curr_pc);
    printf("ERROR 0x%04X\n", read_byte(curr_pc));
    exit(1);
    break;
  }
}
