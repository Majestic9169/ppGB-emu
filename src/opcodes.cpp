#include "../include/ANSI-color-codes.h"
#include "../include/cpu.h"
#include <cstdint>
#include <cstdio>
#include <ctime>

const char *disasm[256] = {
    "NOP\n",                      // 0x00
    "LD BC, 0x%04X\n",            // 0x01
    "LD (BC), A\n",               // 0x02
    "INC BC\n",                   // 0x03
    "INC B\n",                    // 0x04
    "DEC B\n",                    // 0x05
    "LD B, 0x%02X\n",             // 0x06
    "RLCA\n",                     // 0x07
    "LD (0x%04X), SP\n",          // 0x08
    "ADD HL, BC\n",               // 0x09
    "LD A, (BC)\n",               // 0x0a
    "DEC BC\n",                   // 0x0b
    "INC C\n",                    // 0x0c
    "DEC C\n",                    // 0x0d
    "LD C, 0x%02X\n",             // 0x0e
    "RRCA\n",                     // 0x0f
    "STOP\n",                     // 0x10
    "LD DE, 0x%04X\n",            // 0x11
    "LD (DE), A\n",               // 0x12
    "INC DE\n",                   // 0x13
    "INC D\n",                    // 0x14
    "DEC D\n",                    // 0x15
    "LD D, 0x%02X\n",             // 0x16
    "RLA\n",                      // 0x17
    "JR 0x%02X\n",                // 0x18
    "ADD HL, DE\n",               // 0x19
    "LD A, (DE)\n",               // 0x1a
    "DEC DE\n",                   // 0x1b
    "INC E\n",                    // 0x1c
    "DEC E\n",                    // 0x1d
    "LD E, 0x%02X\n",             // 0x1e
    "RRA\n",                      // 0x1f
    "JR NZ, 0x%02X\n",            // 0x20
    "LD HL, 0x%04X\n",            // 0x21
    "LDI (HL), A\n",              // 0x22
    "INC HL\n",                   // 0x23
    "INC H\n",                    // 0x24
    "DEC H\n",                    // 0x25
    "LD H, 0x%02X\n",             // 0x26
    "DAA\n",                      // 0x27
    "JR Z, 0x%02X\n",             // 0x28
    "ADD HL, HL\n",               // 0x29
    "LDI A, (HL)\n",              // 0x2a
    "DEC HL\n",                   // 0x2b
    "INC L\n",                    // 0x2c
    "DEC L\n",                    // 0x2d
    "LD L, 0x%02X\n",             // 0x2e
    "CPL\n",                      // 0x2f
    "JR NC, 0x%02X\n",            // 0x30
    "LD SP, 0x%04X\n",            // 0x31
    "LDD (HL), A\n",              // 0x32
    "INC SP\n",                   // 0x33
    "INC (HL)\n",                 // 0x34
    "DEC (HL)\n",                 // 0x35
    "LD (HL), 0x%02X\n",          // 0x36
    "SCF\n",                      // 0x37
    "JR C, 0x%02X\n",             // 0x38
    "ADD HL, SP\n",               // 0x39
    "LDD A, (HL)\n",              // 0x3a
    "DEC SP\n",                   // 0x3b
    "INC A\n",                    // 0x3c
    "DEC A\n",                    // 0x3d
    "LD A, 0x%02X\n",             // 0x3e
    "CCF\n",                      // 0x3f
    "LD B, B\n",                  // 0x40
    "LD B, C\n",                  // 0x41
    "LD B, D\n",                  // 0x42
    "LD B, E\n",                  // 0x43
    "LD B, H\n",                  // 0x44
    "LD B, L\n",                  // 0x45
    "LD B, (HL)\n",               // 0x46
    "LD B, A\n",                  // 0x47
    "LD C, B\n",                  // 0x48
    "LD C, C\n",                  // 0x49
    "LD C, D\n",                  // 0x4a
    "LD C, E\n",                  // 0x4b
    "LD C, H\n",                  // 0x4c
    "LD C, L\n",                  // 0x4d
    "LD C, (HL)\n",               // 0x4e
    "LD C, A\n",                  // 0x4f
    "LD D, B\n",                  // 0x50
    "LD D, C\n",                  // 0x51
    "LD D, D\n",                  // 0x52
    "LD D, E\n",                  // 0x53
    "LD D, H\n",                  // 0x54
    "LD D, L\n",                  // 0x55
    "LD D, (HL)\n",               // 0x56
    "LD D, A\n",                  // 0x57
    "LD E, B\n",                  // 0x58
    "LD E, C\n",                  // 0x59
    "LD E, D\n",                  // 0x5a
    "LD E, E\n",                  // 0x5b
    "LD E, H\n",                  // 0x5c
    "LD E, L\n",                  // 0x5d
    "LD E, (HL)\n",               // 0x5e
    "LD E, A\n",                  // 0x5f
    "LD H, B\n",                  // 0x60
    "LD H, C\n",                  // 0x61
    "LD H, D\n",                  // 0x62
    "LD H, E\n",                  // 0x63
    "LD H, H\n",                  // 0x64
    "LD H, L\n",                  // 0x65
    "LD H, (HL)\n",               // 0x66
    "LD H, A\n",                  // 0x67
    "LD L, B\n",                  // 0x68
    "LD L, C\n",                  // 0x69
    "LD L, D\n",                  // 0x6a
    "LD L, E\n",                  // 0x6b
    "LD L, H\n",                  // 0x6c
    "LD L, L\n",                  // 0x6d
    "LD L, (HL)\n",               // 0x6e
    "LD L, A\n",                  // 0x6f
    "LD (HL), B\n",               // 0x70
    "LD (HL), C\n",               // 0x71
    "LD (HL), D\n",               // 0x72
    "LD (HL), E\n",               // 0x73
    "LD (HL), H\n",               // 0x74
    "LD (HL), L\n",               // 0x75
    "HALT\n",                     // 0x76
    "LD (HL), A\n",               // 0x77
    "LD A, B\n",                  // 0x78
    "LD A, C\n",                  // 0x79
    "LD A, D\n",                  // 0x7a
    "LD A, E\n",                  // 0x7b
    "LD A, H\n",                  // 0x7c
    "LD A, L\n",                  // 0x7d
    "LD A, (HL)\n",               // 0x7e
    "LD A, A\n",                  // 0x7f
    "ADD A, B\n",                 // 0x80
    "ADD A, C\n",                 // 0x81
    "ADD A, D\n",                 // 0x82
    "ADD A, E\n",                 // 0x83
    "ADD A, H\n",                 // 0x84
    "ADD A, L\n",                 // 0x85
    "ADD A, (HL)\n",              // 0x86
    "ADD A\n",                    // 0x87
    "ADC B\n",                    // 0x88
    "ADC C\n",                    // 0x89
    "ADC D\n",                    // 0x8a
    "ADC E\n",                    // 0x8b
    "ADC H\n",                    // 0x8c
    "ADC L\n",                    // 0x8d
    "ADC (HL)\n",                 // 0x8e
    "ADC A\n",                    // 0x8f
    "SUB B\n",                    // 0x90
    "SUB C\n",                    // 0x91
    "SUB D\n",                    // 0x92
    "SUB E\n",                    // 0x93
    "SUB H\n",                    // 0x94
    "SUB L\n",                    // 0x95
    "SUB (HL)\n",                 // 0x96
    "SUB A\n",                    // 0x97
    "SBC B\n",                    // 0x98
    "SBC C\n",                    // 0x99
    "SBC D\n",                    // 0x9a
    "SBC E\n",                    // 0x9b
    "SBC H\n",                    // 0x9c
    "SBC L\n",                    // 0x9d
    "SBC (HL)\n",                 // 0x9e
    "SBC A\n",                    // 0x9f
    "AND B\n",                    // 0xa0
    "AND C\n",                    // 0xa1
    "AND D\n",                    // 0xa2
    "AND E\n",                    // 0xa3
    "AND H\n",                    // 0xa4
    "AND L\n",                    // 0xa5
    "AND (HL)\n",                 // 0xa6
    "AND A\n",                    // 0xa7
    "XOR B\n",                    // 0xa8
    "XOR C\n",                    // 0xa9
    "XOR D\n",                    // 0xaa
    "XOR E\n",                    // 0xab
    "XOR H\n",                    // 0xac
    "XOR L\n",                    // 0xad
    "XOR (HL)\n",                 // 0xae
    "XOR A\n",                    // 0xaf
    "OR B\n",                     // 0xb0
    "OR C\n",                     // 0xb1
    "OR D\n",                     // 0xb2
    "OR E\n",                     // 0xb3
    "OR H\n",                     // 0xb4
    "OR L\n",                     // 0xb5
    "OR (HL)\n",                  // 0xb6
    "OR A\n",                     // 0xb7
    "CP B\n",                     // 0xb8
    "CP C\n",                     // 0xb9
    "CP D\n",                     // 0xba
    "CP E\n",                     // 0xbb
    "CP H\n",                     // 0xbc
    "CP L\n",                     // 0xbd
    "CP (HL)\n",                  // 0xbe
    "CP A\n",                     // 0xbf
    "RET NZ\n",                   // 0xc0
    "POP BC\n",                   // 0xc1
    "JP NZ, 0x%04X\n",            // 0xc2
    "JP 0x%04X\n",                // 0xc3
    "CALL NZ, 0x%04X\n",          // 0xc4
    "PUSH BC\n",                  // 0xc5
    "ADD A, 0x%02X\n",            // 0xc6
    "RST 0x00\n",                 // 0xc7
    "RET Z\n",                    // 0xc8
    "RET\n",                      // 0xc9
    "JP Z, 0x%04X\n",             // 0xca
    "CB %02X\n",                  // 0xcb
    "CALL Z, 0x%04X\n",           // 0xcc
    "CALL 0x%04X\n",              // 0xcd
    "ADC 0x%02X\n",               // 0xce
    "RST 0x08\n",                 // 0xcf
    "RET NC\n",                   // 0xd0
    "POP DE\n",                   // 0xd1
    "JP NC, 0x%04X\n",            // 0xd2
    "UNKNOWN\n",                  // 0xd3
    "CALL NC, 0x%04X\n",          // 0xd4
    "PUSH DE\n",                  // 0xd5
    "SUB 0x%02X\n",               // 0xd6
    "RST 0x10\n",                 // 0xd7
    "RET C\n",                    // 0xd8
    "RETI\n",                     // 0xd9
    "JP C, 0x%04X\n",             // 0xda
    "UNKNOWN\n",                  // 0xdb
    "CALL C, 0x%04X\n",           // 0xdc
    "UNKNOWN\n",                  // 0xdd
    "SBC 0x%02X\n",               // 0xde
    "RST 0x18\n",                 // 0xdf
    "LDH (0xFF00 + 0x%02X), A\n", // 0xe0
    "POP HL\n",                   // 0xe1
    "LDH (0xFF00 + C), A\n",      // 0xe2
    "UNKNOWN\n",                  // 0xe3
    "UNKNOWN\n",                  // 0xe4
    "PUSH HL\n",                  // 0xe5
    "AND 0x%02X\n",               // 0xe6
    "RST 0x20\n",                 // 0xe7
    "ADD SP,0x%02X\n",            // 0xe8
    "JP HL\n",                    // 0xe9
    "LD (0x%04X), A\n",           // 0xea
    "UNKNOWN\n",                  // 0xeb
    "UNKNOWN\n",                  // 0xec
    "UNKNOWN\n",                  // 0xed
    "XOR 0x%02X\n",               // 0xee
    "RST 0x28\n",                 // 0xef
    "LDH A, (0xFF00 + 0x%02X)\n", // 0xf0
    "POP AF\n",                   // 0xf1
    "LDH A, (0xFF00 + C)\n",      // 0xf2
    "DI\n",                       // 0xf3
    "UNKNOWN\n",                  // 0xf4
    "PUSH AF\n",                  // 0xf5
    "OR 0x%02X\n",                // 0xf6
    "RST 0x30\n",                 // 0xf7
    "LD HL, SP+0x%02X\n",         // 0xf8
    "LD SP, HL\n",                // 0xf9
    "LD A, (0x%04X)\n",           // 0xfa
    "EI\n",                       // 0xfb
    "UNKNOWN\n",                  // 0xfc
    "UNKNOWN\n",                  // 0xfd
    "CP 0x%02X\n",                // 0xfe
    "RST 0x38\n",                 // 0xff
};

const uint8_t clock_m_cycles[256] = {
    1, 3, 2, 2, 1, 1, 2, 2, 5, 2, 2, 2, 1, 1, 2, 2, // 0x0_
    1, 3, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, // 0x1_
    0, 3, 2, 2, 1, 1, 2, 1, 0, 2, 2, 2, 1, 1, 2, 1, // 0x2_
    2, 3, 2, 2, 3, 3, 3, 1, 0, 2, 2, 2, 1, 1, 2, 1, // 0x3_
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0x4_
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0x5_
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0x6_
    2, 2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, // 0x7_
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0x8_
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0x9_
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0xa_
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0xb_
    0, 3, 0, 3, 0, 4, 2, 4, 0, 1, 0, 0, 0, 3, 2, 4, // 0xc_
    0, 3, 0, 0, 0, 4, 2, 4, 0, 4, 0, 0, 0, 0, 2, 4, // 0xd_
    3, 3, 2, 0, 0, 4, 2, 4, 4, 1, 4, 0, 0, 0, 2, 4, // 0xe_
    3, 3, 2, 1, 0, 4, 2, 4, 3, 2, 4, 1, 0, 0, 2, 4  // 0xf_
};

void CPU::INSTRUCTION_DECODER() {
  uint16_t curr_pc = reg.pc;
  uint16_t curr_op = read_byte(reg.pc++);

  clock_m += clock_m_cycles[curr_op];

  if (curr_op != 0xcb) {
    printf(COLOR_RESET CYN "0x%04X:" BYEL " [%02x] " RED, curr_pc, curr_op);
  }

  uint16_t val;

  switch (curr_op) {
  case 0x00:
    NOP();
    break;
  case 0x01:
    val = read_word_from_pc();
    LD_r16_n16(&REGISTERS::bc, val);
    break;
  case 0x02:
    LD_r16_r8(&REGISTERS::bc, &REGISTERS::a);
    break;
  case 0x03:
    INC_r16(&REGISTERS::bc);
    break;
  case 0x04:
    INC_r8(&REGISTERS::b);
    break;
  case 0x05:
    DEC_r8(&REGISTERS::b);
    break;
  case 0x06:
    val = read_byte_from_pc();
    LD_r8_n8(&REGISTERS::b, val);
    break;
  case 0x07:
    RLCA();
    break;
  case 0x08:
    val = read_word_from_pc();
    LD_n16_SP(val);
    break;
  case 0x09:
    ADD_HL_r16(&REGISTERS::bc);
    break;
  case 0x0A:
    LD_r8_r16(&REGISTERS::a, &REGISTERS::bc);
    break;
  case 0x0B:
    DEC_r16(&REGISTERS::bc);
    break;
  case 0x0C:
    INC_r8(&REGISTERS::c);
    break;
  case 0x0D:
    DEC_r8(&REGISTERS::c);
    break;
  case 0x0E:
    val = read_byte(reg.pc++);
    LD_r8_n8(&REGISTERS::c, val);
    break;
  case 0x0F:
    RRCA();
    break;
  case 0x10:
    exit(1);
    break;
  case 0x11:
    val = read_word(reg.pc++);
    reg.pc++;
    LD_r16_n16(&REGISTERS::de, val);
    break;
  case 0x12:
    LD_r16_r8(&REGISTERS::de, &REGISTERS::a);
    break;
  case 0x13:
    INC_r16(&REGISTERS::de);
    break;
  case 0x14:
    INC_r8(&REGISTERS::d);
    break;
  case 0x15:
    DEC_r8(&REGISTERS::d);
    break;
  case 0x16:
    val = read_byte(reg.pc++);
    LD_r8_n8(&REGISTERS::d, val);
    break;
  case 0x17:
    break;
  case 0x18:
    val = read_byte(reg.pc++);
    JR_n16(val);
    break;
  case 0x19:
    ADD_HL_r16(&REGISTERS::de);
    break;
  case 0x1A:
    LD_r8_r16(&REGISTERS::a, &REGISTERS::de);
    break;
  case 0x1B:
    DEC_r16(&REGISTERS::de);
    break;
  case 0x1C:
    INC_r8(&REGISTERS::e);
    break;
  case 0x1D:
    DEC_r8(&REGISTERS::e);
    break;
  case 0x1E:
    val = read_byte(reg.pc++);
    LD_r8_n8(&REGISTERS::e, val);
    break;
  case 0x1F:
    break;
  case 0x20:
    val = read_byte(reg.pc++);
    JR_CC_n16(flag_value(N) & flag_value(Z), val);
    break;
  case 0x21:
    val = read_word(reg.pc++);
    reg.pc++;
    LD_r16_n16(&REGISTERS::hl, val);
    break;
  case 0x22:
    LD_r16_r8(&REGISTERS::hl, &REGISTERS::a);
    break;
  case 0x23:
    INC_r16(&REGISTERS::hl);
    break;
  case 0x24:
    INC_r8(&REGISTERS::h);
    break;
  case 0x25:
    DEC_r8(&REGISTERS::h);
    break;
  case 0x26:
    val = read_byte(reg.pc++);
    LD_r8_n8(&REGISTERS::h, val);
    break;
  case 0x27:
    DAA();
    break;
  case 0x28:
    val = read_byte(reg.pc++);
    JR_CC_n16(flag_value(Z), val);
    break;
  case 0x29:
    ADD_HL_r16(&REGISTERS::hl);
    break;
  case 0x2A:
    LD_r8_r16(&REGISTERS::a, &REGISTERS::hl);
    break;
  case 0x2B:
    DEC_r16(&REGISTERS::hl);
    break;
  case 0x2C:
    INC_r8(&REGISTERS::l);
    break;
  case 0x2D:
    DEC_r8(&REGISTERS::l);
    break;
  case 0x2E:
    val = read_byte(reg.pc++);
    LD_r8_n8(&REGISTERS::l, val);
    break;
  case 0x2F:
    CPL();
    break;
  case 0x30:
    val = read_byte(reg.pc++);
    JR_CC_n16(flag_value(N) & flag_value(C), val);
    break;
  case 0x31:
    val = read_word(reg.pc++);
    reg.pc++;
    LD_SP_n16(val);
    break;
  case 0x32:
    LD_HLD_A();
    break;
  case 0x33:
    INC_SP();
    break;
  case 0x34:
    INC_HL();
    break;
  case 0x35:
    DEC_HL();
    break;
  case 0x36:
    val = read_byte(reg.pc++);
    LD_HL_n8(val);
    break;
  case 0x37:
    break;
  case 0x38:
    val = read_byte(reg.pc++);
    JR_CC_n16(flag_value(C), val);
    break;
  case 0x39:
    ADD_HL_SP();
    break;
  case 0x3A:
    LD_A_HLD();
    break;
  case 0x3B:
    DEC_SP();
    break;
  case 0x3C:
    INC_r8(&REGISTERS::a);
    break;
  case 0x3D:
    DEC_r8(&REGISTERS::a);
    break;
  case 0x3E:
    val = read_byte(reg.pc++);
    LD_r8_n8(&REGISTERS::a, val);
    break;
  case 0x3F:
    CCF();
    break;
  case 0x40:
    LD_r8_r8(&REGISTERS::b, &REGISTERS::b);
    break;
  case 0x41:
    LD_r8_r8(&REGISTERS::b, &REGISTERS::c);
    break;
  case 0x42:
    LD_r8_r8(&REGISTERS::b, &REGISTERS::d);
    break;
  case 0x43:
    LD_r8_r8(&REGISTERS::b, &REGISTERS::e);
    break;
  case 0x44:
    LD_r8_r8(&REGISTERS::b, &REGISTERS::h);
    break;
  case 0x45:
    LD_r8_r8(&REGISTERS::b, &REGISTERS::l);
    break;
  case 0x46:
    LD_r8_HL(&REGISTERS::b);
    break;
  case 0x47:
    LD_r8_r8(&REGISTERS::b, &REGISTERS::a);
    break;
  case 0x48:
    LD_r8_r8(&REGISTERS::c, &REGISTERS::b);
    break;
  case 0x49:
    LD_r8_r8(&REGISTERS::c, &REGISTERS::c);
    break;
  case 0x4A:
    LD_r8_r8(&REGISTERS::c, &REGISTERS::d);
    break;
  case 0x4B:
    LD_r8_r8(&REGISTERS::c, &REGISTERS::e);
    break;
  case 0x4C:
    LD_r8_r8(&REGISTERS::c, &REGISTERS::h);
    break;
  case 0x4D:
    LD_r8_r8(&REGISTERS::c, &REGISTERS::l);
    break;
  case 0x4E:
    LD_r8_HL(&REGISTERS::c);
    break;
  case 0x4F:
    LD_r8_r8(&REGISTERS::c, &REGISTERS::a);
    break;
  case 0x50:
    LD_r8_r8(&REGISTERS::d, &REGISTERS::b);
    break;
  case 0x51:
    LD_r8_r8(&REGISTERS::d, &REGISTERS::c);
    break;
  case 0x52:
    LD_r8_r8(&REGISTERS::d, &REGISTERS::d);
    break;
  case 0x53:
    LD_r8_r8(&REGISTERS::d, &REGISTERS::e);
    break;
  case 0x54:
    LD_r8_r8(&REGISTERS::d, &REGISTERS::h);
    break;
  case 0x55:
    LD_r8_r8(&REGISTERS::d, &REGISTERS::l);
    break;
  case 0x56:
    LD_r8_HL(&REGISTERS::d);
    break;
  case 0x57:
    LD_r8_r8(&REGISTERS::d, &REGISTERS::a);
    break;
  case 0x58:
    LD_r8_r8(&REGISTERS::e, &REGISTERS::b);
    break;
  case 0x59:
    LD_r8_r8(&REGISTERS::e, &REGISTERS::c);
    break;
  case 0x5A:
    LD_r8_r8(&REGISTERS::e, &REGISTERS::d);
    break;
  case 0x5B:
    LD_r8_r8(&REGISTERS::e, &REGISTERS::e);
    break;
  case 0x5C:
    LD_r8_r8(&REGISTERS::e, &REGISTERS::h);
    break;
  case 0x5D:
    LD_r8_r8(&REGISTERS::e, &REGISTERS::l);
    break;
  case 0x5E:
    LD_r8_HL(&REGISTERS::e);
    break;
  case 0x5F:
    LD_r8_r8(&REGISTERS::e, &REGISTERS::a);
    break;
  case 0x60:
    LD_r8_r8(&REGISTERS::h, &REGISTERS::b);
    break;
  case 0x61:
    LD_r8_r8(&REGISTERS::h, &REGISTERS::c);
    break;
  case 0x62:
    LD_r8_r8(&REGISTERS::h, &REGISTERS::d);
    break;
  case 0x63:
    LD_r8_r8(&REGISTERS::h, &REGISTERS::e);
    break;
  case 0x64:
    LD_r8_r8(&REGISTERS::h, &REGISTERS::h);
    break;
  case 0x65:
    LD_r8_r8(&REGISTERS::h, &REGISTERS::l);
    break;
  case 0x66:
    LD_r8_HL(&REGISTERS::h);
    break;
  case 0x67:
    LD_r8_r8(&REGISTERS::h, &REGISTERS::a);
    break;
  case 0x68:
    LD_r8_r8(&REGISTERS::l, &REGISTERS::b);
    break;
  case 0x69:
    LD_r8_r8(&REGISTERS::l, &REGISTERS::c);
    break;
  case 0x6A:
    LD_r8_r8(&REGISTERS::l, &REGISTERS::d);
    break;
  case 0x6B:
    LD_r8_r8(&REGISTERS::l, &REGISTERS::e);
    break;
  case 0x6C:
    LD_r8_r8(&REGISTERS::l, &REGISTERS::h);
    break;
  case 0x6D:
    LD_r8_r8(&REGISTERS::l, &REGISTERS::l);
    break;
  case 0x6E:
    LD_r8_HL(&REGISTERS::l);
    break;
  case 0x6F:
    LD_r8_r8(&REGISTERS::l, &REGISTERS::a);
    break;
  case 0x70:
    LD_HL_r8(&REGISTERS::b);
    break;
  case 0x71:
    LD_HL_r8(&REGISTERS::c);
    break;
  case 0x72:
    LD_HL_r8(&REGISTERS::d);
    break;
  case 0x73:
    LD_HL_r8(&REGISTERS::e);
    break;
  case 0x74:
    LD_HL_r8(&REGISTERS::h);
    break;
  case 0x75:
    LD_HL_r8(&REGISTERS::l);
    break;
  case 0x76:
    break;
  case 0x77:
    LD_HL_r8(&REGISTERS::a);
    break;
  case 0x78:
    LD_r8_r8(&REGISTERS::a, &REGISTERS::b);
    break;
  case 0x79:
    LD_r8_r8(&REGISTERS::a, &REGISTERS::c);
    break;
  case 0x7A:
    LD_r8_r8(&REGISTERS::a, &REGISTERS::d);
    break;
  case 0x7B:
    LD_r8_r8(&REGISTERS::a, &REGISTERS::e);
    break;
  case 0x7C:
    LD_r8_r8(&REGISTERS::a, &REGISTERS::h);
    break;
  case 0x7D:
    LD_r8_r8(&REGISTERS::a, &REGISTERS::l);
    break;
  case 0x7E:
    LD_r8_HL(&REGISTERS::a);
    break;
  case 0x7F:
    LD_r8_r8(&REGISTERS::a, &REGISTERS::a);
    break;
  case 0x80:
    ADD_A_r8(&REGISTERS::b);
    break;
  case 0x81:
    ADD_A_r8(&REGISTERS::c);
    break;
  case 0x82:
    ADD_A_r8(&REGISTERS::d);
    break;
  case 0x83:
    ADD_A_r8(&REGISTERS::e);
    break;
  case 0x84:
    ADD_A_r8(&REGISTERS::h);
    break;
  case 0x85:
    ADD_A_r8(&REGISTERS::l);
    break;
  case 0x86:
    ADD_A_HL();
    break;
  case 0x87:
    ADD_A_r8(&REGISTERS::a);
    break;
  case 0x88:
    ADC_A_r8(&REGISTERS::b);
    break;
  case 0x89:
    ADC_A_r8(&REGISTERS::c);
    break;
  case 0x8A:
    ADC_A_r8(&REGISTERS::d);
    break;
  case 0x8B:
    ADC_A_r8(&REGISTERS::e);
    break;
  case 0x8C:
    ADC_A_r8(&REGISTERS::h);
    break;
  case 0x8D:
    ADC_A_r8(&REGISTERS::l);
    break;
  case 0x8E:
    ADC_A_HL();
    break;
  case 0x8F:
    ADC_A_r8(&REGISTERS::a);
    break;
  case 0x90:
    SUB_A_r8(&REGISTERS::b);
    break;
  case 0x91:
    SUB_A_r8(&REGISTERS::c);
    break;
  case 0x92:
    SUB_A_r8(&REGISTERS::d);
    break;
  case 0x93:
    SUB_A_r8(&REGISTERS::e);
    break;
  case 0x94:
    SUB_A_r8(&REGISTERS::h);
    break;
  case 0x95:
    SUB_A_r8(&REGISTERS::l);
    break;
  case 0x96:
    SUB_A_HL();
    break;
  case 0x97:
    SUB_A_r8(&REGISTERS::a);
    break;
  case 0x98:
    SBC_A_r8(&REGISTERS::b);
    break;
  case 0x99:
    SBC_A_r8(&REGISTERS::c);
    break;
  case 0x9A:
    SBC_A_r8(&REGISTERS::d);
    break;
  case 0x9B:
    SBC_A_r8(&REGISTERS::e);
    break;
  case 0x9C:
    SBC_A_r8(&REGISTERS::h);
    break;
  case 0x9D:
    SBC_A_r8(&REGISTERS::l);
    break;
  case 0x9E:
    SBC_A_HL();
    break;
  case 0x9F:
    SBC_A_r8(&REGISTERS::a);
    break;
  case 0xa0:
    AND_A_r8(&REGISTERS::b);
    break;
  case 0xa1:
    AND_A_r8(&REGISTERS::c);
    break;
  case 0xa2:
    AND_A_r8(&REGISTERS::d);
    break;
  case 0xa3:
    AND_A_r8(&REGISTERS::e);
    break;
  case 0xa4:
    AND_A_r8(&REGISTERS::h);
    break;
  case 0xa5:
    AND_A_r8(&REGISTERS::l);
    break;
  case 0xa6:
    AND_A_HL();
    break;
  case 0xa7:
    AND_A_r8(&REGISTERS::a);
    break;
  case 0xa8:
    XOR_A_r8(&REGISTERS::b);
    break;
  case 0xa9:
    XOR_A_r8(&REGISTERS::c);
    break;
  case 0xaa:
    XOR_A_r8(&REGISTERS::d);
    break;
  case 0xab:
    XOR_A_r8(&REGISTERS::e);
    break;
  case 0xac:
    XOR_A_r8(&REGISTERS::h);
    break;
  case 0xad:
    XOR_A_r8(&REGISTERS::l);
    break;
  case 0xae:
    XOR_A_HL();
    break;
  case 0xaf:
    XOR_A_r8(&REGISTERS::a);
    break;
  case 0xb0:
    OR_A_r8(&REGISTERS::b);
    break;
  case 0xb1:
    OR_A_r8(&REGISTERS::c);
    break;
  case 0xb2:
    OR_A_r8(&REGISTERS::d);
    break;
  case 0xb3:
    OR_A_r8(&REGISTERS::e);
    break;
  case 0xb4:
    OR_A_r8(&REGISTERS::h);
    break;
  case 0xb5:
    OR_A_r8(&REGISTERS::l);
    break;
  case 0xb6:
    OR_A_HL();
    break;
  case 0xb7:
    OR_A_r8(&REGISTERS::a);
    break;
  case 0xb8:
    CP_A_r8(&REGISTERS::b);
    break;
  case 0xb9:
    CP_A_r8(&REGISTERS::c);
    break;
  case 0xbA:
    CP_A_r8(&REGISTERS::d);
    break;
  case 0xbB:
    CP_A_r8(&REGISTERS::e);
    break;
  case 0xbC:
    CP_A_r8(&REGISTERS::h);
    break;
  case 0xbD:
    CP_A_r8(&REGISTERS::l);
    break;
  case 0xbE:
    CP_A_HL();
    break;
  case 0xbF:
    CP_A_r8(&REGISTERS::a);
    break;
  case 0xc0:
    RET_CC(flag_value(N) & flag_value(Z));
    break;
  case 0xc1:
    POP_r16(&REGISTERS::bc);
    break;
  case 0xc2:
    val = read_word_from_pc();
    JP_CC_n16(flag_value(N) & flag_value(Z), val);
    break;
  case 0xc3:
    val = read_word_from_pc();
    JP_n16(val);
    break;
  case 0xc4:
    val = read_word_from_pc();
    CALL_CC_n16(flag_value(N) & flag_value(Z), val);
    break;
  case 0xc5:
    PUSH_r16(&REGISTERS::bc);
    break;
  case 0xc6:
    val = read_byte_from_pc();
    ADD_A_n8(val);
    break;
  case 0xc7:
    CALL_n16(0x00);
    break;
  case 0xc8:
    RET_CC(flag_value(Z));
    break;
  case 0xc9:
    RET();
    break;
  case 0xca:
    val = read_word_from_pc();
    JP_CC_n16(flag_value(Z), val);
    break;
  case 0xcb:
    val = read_byte_from_pc();
    CB_INSTRUCTION_DECODER();
    break;
  case 0xcc:
    exit(1);
    break;
  case 0xcd:
    val = read_word_from_pc();
    CALL_n16(val);
    break;
  case 0xce:
    val = read_byte_from_pc();
    ADC_A_n8(val);
    break;
  case 0xd0:
    RET_CC(flag_value(N) & flag_value(C));
    break;
  case 0xd1:
    POP_r16(&REGISTERS::de);
    break;
  case 0xD2:
    val = read_word_from_pc();
    JP_CC_n16(flag_value(C) && flag_value(N), val);
    break;
  case 0xd5:
    PUSH_r16(&REGISTERS::de);
    break;
  case 0xd6:
    val = read_byte_from_pc();
    SUB_A_n8(val);
    break;
  case 0xd9:
    RETI();
    break;
  case 0xDA:
    val = read_word(reg.pc++);
    reg.pc++;
    JP_CC_n16(flag_value(C), val);
    break;
  case 0xE0:
    val = read_byte(reg.pc++);
    LDH_n16_A(val);
    break;
  case 0xe1:
    POP_r16(&REGISTERS::hl);
    break;
  case 0xE2:
    LDH_C_A();
    break;
  case 0xe5:
    PUSH_r16(&REGISTERS::hl);
    break;
  case 0xE6:
    val = read_byte(reg.pc++);
    AND_A_n8(val);
    break;
  case 0xE8:
    val = read_byte(reg.pc++);
    ADD_SP_e8(val);
    break;
  case 0xE9:
    break;
  case 0xEA:
    val = read_word(reg.pc++);
    reg.pc++;
    LD_n16_A(val);
    break;
  case 0xEC:
    break;
  case 0xEE:
    val = read_byte(reg.pc++);
    XOR_A_n8(val);
    break;
  case 0xEF:
    RST_vec(0x28);
    break;
  case 0xf0:
    val = read_byte(reg.pc++);
    LDH_A_n16(val);
    break;
  case 0xF1:
    POP_AF();
    break;
  case 0xf3:
    DI();
    break;
  case 0xF5:
    break;
  case 0xF8:
    val = read_byte(reg.pc++);
    LD_HL_SP_e8(val);
    break;
  case 0xF9:
    LD_SP_HL();
    break;
  case 0xFA:
    val = read_word(reg.pc++);
    reg.pc++;
    break;
  case 0xfb:
    EI();
    break;
  case 0xFC:
    break;
  case 0xFE:
    val = read_byte(reg.pc++);
    CP_A_n8(val);
    break;
  case 0xFF:
    RST_vec(0x38);
    break;
  default:
    printf("ERROR: DEFAULT 0x%02X\n", curr_pc);
    printf("ERROR 0x%04X\n", read_byte(curr_pc));
    exit(1);
    break;
  }

  printf(disasm[curr_op], curr_pc, val);
}
