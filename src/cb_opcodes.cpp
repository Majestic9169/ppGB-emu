#include "../include/cpu.h"
#include <cstdint>
#include <cstdio>
#include <ctime>

const char *cb_disasm[256] = {
    "RLC B",       // 0x00
    "RLC C",       // 0x01
    "RLC D",       // 0x02
    "RLC E",       // 0x03
    "RLC H",       // 0x04
    "RLC L",       // 0x05
    "RLC (HL)",    // 0x06
    "RLC A",       // 0x07
    "RRC B",       // 0x08
    "RRC C",       // 0x09
    "RRC D",       // 0x0a
    "RRC E",       // 0x0b
    "RRC H",       // 0x0c
    "RRC L",       // 0x0d
    "RRC (HL)",    // 0x0e
    "RRC A",       // 0x0f
    "RL B",        // 0x10
    "RL C",        // 0x11
    "RL D",        // 0x12
    "RL E",        // 0x13
    "RL H",        // 0x14
    "RL L",        // 0x15
    "RL (HL)",     // 0x16
    "RL A",        // 0x17
    "RR B",        // 0x18
    "RR C",        // 0x19
    "RR D",        // 0x1a
    "RR E",        // 0x1b
    "RR H",        // 0x1c
    "RR L",        // 0x1d
    "RR (HL)",     // 0x1e
    "RR A",        // 0x1f
    "SLA B",       // 0x20
    "SLA C",       // 0x21
    "SLA D",       // 0x22
    "SLA E",       // 0x23
    "SLA H",       // 0x24
    "SLA L",       // 0x25
    "SLA (HL)",    // 0x26
    "SLA A",       // 0x27
    "SRA B",       // 0x28
    "SRA C",       // 0x29
    "SRA D",       // 0x2a
    "SRA E",       // 0x2b
    "SRA H",       // 0x2c
    "SRA L",       // 0x2d
    "SRA (HL)",    // 0x2e
    "SRA A",       // 0x2f
    "SWAP B",      // 0x30
    "SWAP C",      // 0x31
    "SWAP D",      // 0x32
    "SWAP E",      // 0x33
    "SWAP H",      // 0x34
    "SWAP L",      // 0x35
    "SWAP (HL)",   // 0x36
    "SWAP A",      // 0x37
    "SRL B",       // 0x38
    "SRL C",       // 0x39
    "SRL D",       // 0x3a
    "SRL E",       // 0x3b
    "SRL H",       // 0x3c
    "SRL L",       // 0x3d
    "SRL (HL)",    // 0x3e
    "SRL A",       // 0x3f
    "BIT 0, B",    // 0x40
    "BIT 0, C",    // 0x41
    "BIT 0, D",    // 0x42
    "BIT 0, E",    // 0x43
    "BIT 0, H",    // 0x44
    "BIT 0, L",    // 0x45
    "BIT 0, (HL)", // 0x46
    "BIT 0, A",    // 0x47
    "BIT 1, B",    // 0x48
    "BIT 1, C",    // 0x49
    "BIT 1, D",    // 0x4a
    "BIT 1, E",    // 0x4b
    "BIT 1, H",    // 0x4c
    "BIT 1, L",    // 0x4d
    "BIT 1, (HL)", // 0x4e
    "BIT 1, A",    // 0x4f
    "BIT 2, B",    // 0x50
    "BIT 2, C",    // 0x51
    "BIT 2, D",    // 0x52
    "BIT 2, E",    // 0x53
    "BIT 2, H",    // 0x54
    "BIT 2, L",    // 0x55
    "BIT 2, (HL)", // 0x56
    "BIT 2, A",    // 0x57
    "BIT 3, B",    // 0x58
    "BIT 3, C",    // 0x59
    "BIT 3, D",    // 0x5a
    "BIT 3, E",    // 0x5b
    "BIT 3, H",    // 0x5c
    "BIT 3, L",    // 0x5d
    "BIT 3, (HL)", // 0x5e
    "BIT 3, A",    // 0x5f
    "BIT 4, B",    // 0x60
    "BIT 4, C",    // 0x61
    "BIT 4, D",    // 0x62
    "BIT 4, E",    // 0x63
    "BIT 4, H",    // 0x64
    "BIT 4, L",    // 0x65
    "BIT 4, (HL)", // 0x66
    "BIT 4, A",    // 0x67
    "BIT 5, B",    // 0x68
    "BIT 5, C",    // 0x69
    "BIT 5, D",    // 0x6a
    "BIT 5, E",    // 0x6b
    "BIT 6, H",    // 0x6c
    "BIT 6, L",    // 0x6d
    "BIT 5, (HL)", // 0x6e
    "BIT 5, A",    // 0x6f
    "BIT 6, B",    // 0x70
    "BIT 6, C",    // 0x71
    "BIT 6, D",    // 0x72
    "BIT 6, E",    // 0x73
    "BIT 6, H",    // 0x74
    "BIT 6, L",    // 0x75
    "BIT 6, (HL)", // 0x76
    "BIT 6, A",    // 0x77
    "BIT 7, B",    // 0x78
    "BIT 7, C",    // 0x79
    "BIT 7, D",    // 0x7a
    "BIT 7, E",    // 0x7b
    "BIT 7, H",    // 0x7c
    "BIT 7, L",    // 0x7d
    "BIT 7, (HL)", // 0x7e
    "BIT 7, A",    // 0x7f
    "RES 0, B",    // 0x80
    "RES 0, C",    // 0x81
    "RES 0, D",    // 0x82
    "RES 0, E",    // 0x83
    "RES 0, H",    // 0x84
    "RES 0, L",    // 0x85
    "RES 0, (HL)", // 0x86
    "RES 0, A",    // 0x87
    "RES 1, B",    // 0x88
    "RES 1, C",    // 0x89
    "RES 1, D",    // 0x8a
    "RES 1, E",    // 0x8b
    "RES 1, H",    // 0x8c
    "RES 1, L",    // 0x8d
    "RES 1, (HL)", // 0x8e
    "RES 1, A",    // 0x8f
    "RES 2, B",    // 0x90
    "RES 2, C",    // 0x91
    "RES 2, D",    // 0x92
    "RES 2, E",    // 0x93
    "RES 2, H",    // 0x94
    "RES 2, L",    // 0x95
    "RES 2, (HL)", // 0x96
    "RES 2, A",    // 0x97
    "RES 3, B",    // 0x98
    "RES 3, C",    // 0x99
    "RES 3, D",    // 0x9a
    "RES 3, E",    // 0x9b
    "RES 3, H",    // 0x9c
    "RES 3, L",    // 0x9d
    "RES 3, (HL)", // 0x9e
    "RES 3, A",    // 0x9f
    "RES 4, B",    // 0xa0
    "RES 4, C",    // 0xa1
    "RES 4, D",    // 0xa2
    "RES 4, E",    // 0xa3
    "RES 4, H",    // 0xa4
    "RES 4, L",    // 0xa5
    "RES 4, (HL)", // 0xa6
    "RES 4, A",    // 0xa7
    "RES 5, B",    // 0xa8
    "RES 5, C",    // 0xa9
    "RES 5, D",    // 0xaa
    "RES 5, E",    // 0xab
    "RES 5, H",    // 0xac
    "RES 5, L",    // 0xad
    "RES 5, (HL)", // 0xae
    "RES 5, A",    // 0xaf
    "RES 6, B",    // 0xb0
    "RES 6, C",    // 0xb1
    "RES 6, D",    // 0xb2
    "RES 6, E",    // 0xb3
    "RES 6, H",    // 0xb4
    "RES 6, L",    // 0xb5
    "RES 6, (HL)", // 0xb6
    "RES 6, A",    // 0xb7
    "RES 7, B",    // 0xb8
    "RES 7, C",    // 0xb9
    "RES 7, D",    // 0xba
    "RES 7, E",    // 0xbb
    "RES 7, H",    // 0xbc
    "RES 7, L",    // 0xbd
    "RES 7, (HL)", // 0xbe
    "RES 7, A",    // 0xbf
    "SET 0, B",    // 0xc0
    "SET 0, C",    // 0xc1
    "SET 0, D",    // 0xc2
    "SET 0, E",    // 0xc3
    "SET 0, H",    // 0xc4
    "SET 0, L",    // 0xc5
    "SET 0, (HL)", // 0xc6
    "SET 0, A",    // 0xc7
    "SET 1, B",    // 0xc8
    "SET 1, C",    // 0xc9
    "SET 1, D",    // 0xca
    "SET 1, E",    // 0xcb
    "SET 1, H",    // 0xcc
    "SET 1, L",    // 0xcd
    "SET 1, (HL)", // 0xce
    "SET 1, A",    // 0xcf
    "SET 2, B",    // 0xd0
    "SET 2, C",    // 0xd1
    "SET 2, D",    // 0xd2
    "SET 2, E",    // 0xd3
    "SET 2, H",    // 0xd4
    "SET 2, L",    // 0xd5
    "SET 2, (HL)", // 0xd6
    "SET 2, A",    // 0xd7
    "SET 3, B",    // 0xd8
    "SET 3, C",    // 0xd9
    "SET 3, D",    // 0xda
    "SET 3, E",    // 0xdb
    "SET 3, H",    // 0xdc
    "SET 3, L",    // 0xdd
    "SET 3, (HL)", // 0xde
    "SET 3, A",    // 0xdf
    "SET 4, B",    // 0xe0
    "SET 4, C",    // 0xe1
    "SET 4, D",    // 0xe2
    "SET 4, E",    // 0xe3
    "SET 4, H",    // 0xe4
    "SET 4, L",    // 0xe5
    "SET 4, (HL)", // 0xe6
    "SET 4, A",    // 0xe7
    "SET 5, B",    // 0xe8
    "SET 5, C",    // 0xe9
    "SET 5, D",    // 0xea
    "SET 5, E",    // 0xeb
    "SET 5, H",    // 0xec
    "SET 5, L",    // 0xed
    "SET 5, (HL)", // 0xee
    "SET 5, A",    // 0xef
    "SET 6, B",    // 0xf0
    "SET 6, C",    // 0xf1
    "SET 6, D",    // 0xf2
    "SET 6, E",    // 0xf3
    "SET 6, H",    // 0xf4
    "SET 6, L",    // 0xf5
    "SET 6, (HL)", // 0xf6
    "SET 6, A",    // 0xf7
    "SET 7, B",    // 0xf8
    "SET 7, C",    // 0xf9
    "SET 7, D",    // 0xfa
    "SET 7, E",    // 0xfb
    "SET 7, H",    // 0xfc
    "SET 7, L",    // 0xfd
    "SET 7, (HL)", // 0xfe
    "SET 7, A",    // 0xff
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
    break;
  case 0x01: {
  } break;
  case 0x02:
    break;
  case 0x03:
    break;
  case 0x04:
    break;
  case 0x05:
    break;
  case 0x06: {
  } break;
  case 0x07:
    break;
  case 0x08: {
  } break;
  case 0x09:
    break;
  case 0x0A:
    break;
  case 0x0B:
    break;
  case 0x0C:
    break;
  case 0x0D:
    break;
  case 0x0E: {
  } break;
  case 0x0F:
    break;
  case 0x10:
    break;
  case 0x11: {
  } break;
  case 0x12:
    break;
  case 0x13:
    break;
  case 0x14:
    break;
  case 0x15:
    break;
  case 0x16: {
  } break;
  case 0x18: {
  } break;
  case 0x19:
    break;
  case 0x1A:
    break;
  case 0x1B:
    break;
  case 0x1C:
    break;
  case 0x1D:
    break;
  case 0x1E: {
  } break;
  case 0x20: {
  } break;
  case 0x21: {
  } break;
  case 0x22:
    break;
  case 0x23:
    break;
  case 0x24:
    break;
  case 0x25:
    break;
  case 0x26: {
  } break;
  case 0x27: {
  } break;
  case 0x28: {
  } break;
  case 0x29:
    break;
  case 0x2A:
    break;
  case 0x2B:
    break;
  case 0x2C:
    break;
  case 0x2D:
    break;
  case 0x2E: {
  } break;
  case 0x2F: {
  } break;
  case 0x30: {
  } break;
  case 0x31: {
  } break;
  case 0x32:
    break;
  case 0x33:
    break;
  case 0x34:
    break;
  case 0x35:
    break;
  case 0x36: {
  } break;
  case 0x38: {
  } break;
  case 0x39:
    break;
  case 0x3A:
    break;
  case 0x3B:
    break;
  case 0x3C:
    break;
  case 0x3D:
    break;
  case 0x3E: {
  } break;
  case 0x3F: {
  } break;
  case 0x40:
    break;
  case 0x41:
    break;
  case 0x42:
    break;
  case 0x43:
    break;
  case 0x44:
    break;
  case 0x45:
    break;
  case 0x46:
    break;
  case 0x47:
    break;
  case 0x48:
    break;
  case 0x49:
    break;
  case 0x4A:
    break;
  case 0x4B:
    break;
  case 0x4C:
    break;
  case 0x4D:
    break;
  case 0x4E:
    break;
  case 0x4F:
    break;
  case 0x50:
    break;
  case 0x51:
    break;
  case 0x52:
    break;
  case 0x53:
    break;
  case 0x54:
    break;
  case 0x55:
    break;
  case 0x56:
    break;
  case 0x57:
    break;
  case 0x58:
    break;
  case 0x59:
    break;
  case 0x5A:
    break;
  case 0x5B:
    break;
  case 0x5C:
    break;
  case 0x5D:
    break;
  case 0x5E:
    break;
  case 0x5F:
    break;
  case 0x60:
    break;
  case 0x61:
    break;
  case 0x62:
    break;
  case 0x63:
    break;
  case 0x64:
    break;
  case 0x65:
    break;
  case 0x66:
    break;
  case 0x67:
    break;
  case 0x68:
    break;
  case 0x69:
    break;
  case 0x6A:
    break;
  case 0x6B:
    break;
  case 0x6C:
    break;
  case 0x6D:
    break;
  case 0x6E:
    break;
  case 0x6F:
    break;
  case 0x70:
    break;
  case 0x71:
    break;
  case 0x72:
    break;
  case 0x73:
    break;
  case 0x74:
    break;
  case 0x75:
    break;
  case 0x77:
    break;
  case 0x78:
    break;
  case 0x79:
    break;
  case 0x7A:
    break;
  case 0x7B:
    break;
  case 0x7C:
    break;
  case 0x7D:
    break;
  case 0x7E:
    break;
  case 0x7F:
    break;
  case 0x80:
    break;
  case 0x81:
    break;
  case 0x82:
    break;
  case 0x83:
    break;
  case 0x84:
    break;
  case 0x85:
    break;
  case 0x86:
    break;
  case 0x87:
    break;
  case 0x88:
    break;
  case 0x89:
    break;
  case 0x8A:
    break;
  case 0x8B:
    break;
  case 0x8C:
    break;
  case 0x8D:
    break;
  case 0x8E:
    break;
  case 0x8F:
    break;
  case 0x90:
    break;
  case 0x91:
    break;
  case 0x92:
    break;
  case 0x93:
    break;
  case 0x94:
    break;
  case 0x95:
    break;
  case 0x96:
    break;
  case 0x97:
    break;
  case 0x98:
    break;
  case 0x99:
    break;
  case 0x9A:
    break;
  case 0x9B:
    break;
  case 0x9C:
    break;
  case 0x9D:
    break;
  case 0x9E:
    break;
  case 0x9F:
    break;
  case 0xa0:
    break;
  case 0xa1:
    break;
  case 0xa2:
    break;
  case 0xa3:
    break;
  case 0xa4:
    break;
  case 0xa5:
    break;
  case 0xa6:
    break;
  case 0xa7:
    break;
  case 0xa8:
    break;
  case 0xa9:
    break;
  case 0xaa:
    break;
  case 0xab:
    break;
  case 0xac:
    break;
  case 0xad:
    break;
  case 0xae:
    break;
  case 0xaf:
    break;
  case 0xb0:
    break;
  case 0xb1:
    break;
  case 0xb2:
    break;
  case 0xb3:
    break;
  case 0xb4:
    break;
  case 0xb5:
    break;
  case 0xb6:
    break;
  case 0xb7:
    break;
  case 0xb8:
    break;
  case 0xb9:
    break;
  case 0xbA:
    break;
  case 0xbB:
    break;
  case 0xbC:
    break;
  case 0xbD:
    break;
  case 0xbE:
    break;
  case 0xbF:
    break;
  case 0xc0:
    break;
  case 0xc1:
    break;
  case 0xc2: {
  } break;
  case 0xc3: {
  } break;
  case 0xc4: {
  } break;
  case 0xc5:
    break;
  case 0xc6: {
  } break;
  case 0xc7:
    break;
  case 0xc8:
    break;
  case 0xc9:
    break;
  case 0xca: {
  } break;
  case 0xcb:
    break;
  case 0xcc:
    break;
  case 0xcd: {
  } break;
  case 0xce: {
  } break;
  case 0xE0: {
  } break;
  case 0xE2:
    break;
  case 0xE6: {
  } break;
  case 0xE9:
    break;
  case 0xEA: {
  } break;
  case 0xf0: {
  } break;
  case 0xf3: {
  } break;
  case 0xFA: {
  } break;
  case 0xfb: {
  } break;
  case 0xfe: {
  } break;
  case 0xFF:
    break;
  default:
    printf("ERROR: DEFAULT 0x%02X\n", curr_pc);
    printf("ERROR 0x%04X\n", read_byte(curr_pc));
    exit(1);
    break;
  }
}
