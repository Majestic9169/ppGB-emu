#include "../include/cpu.h"
#include <cstdint>
#include <cstdio>

const char *disasm[256] = {
    "[ASM] NOP\n",                     // 0x00
    "[ASM] LD BC, 0x%04X\n",           // 0x01
    "[ASM] LD (BC), A\n",              // 0x02
    "[ASM] INC BC\n",                  // 0x03
    "[ASM] INC B\n",                   // 0x04
    "[ASM] DEC B\n",                   // 0x05
    "[ASM] LD B, 0x%02X\n",            // 0x06
    "[ASM] RLCA\n",                    // 0x07
    "[ASM] LD (0x%04X), SP\n",         // 0x08
    "[ASM] ADD HL, BC\n",              // 0x09
    "[ASM] LD A, (BC)\n",              // 0x0a
    "[ASM] DEC BC\n",                  // 0x0b
    "[ASM] INC C\n",                   // 0x0c
    "[ASM] DEC C\n",                   // 0x0d
    "[ASM] LD C, 0x%02X\n",            // 0x0e
    "[ASM] RRCA\n",                    // 0x0f
    "[ASM] STOP\n",                    // 0x10
    "[ASM] LD DE, 0x%04X\n",           // 0x11
    "[ASM] LD (DE), A\n",              // 0x12
    "[ASM] INC DE\n",                  // 0x13
    "[ASM] INC D\n",                   // 0x14
    "[ASM] DEC D\n",                   // 0x15
    "[ASM] LD D, 0x%02X\n",            // 0x16
    "[ASM] RLA\n",                     // 0x17
    "[ASM] JR 0x%02X\n",               // 0x18
    "[ASM] ADD HL, DE\n",              // 0x19
    "[ASM] LD A, (DE)\n",              // 0x1a
    "[ASM] DEC DE\n",                  // 0x1b
    "[ASM] INC E\n",                   // 0x1c
    "[ASM] DEC E\n",                   // 0x1d
    "[ASM] LD E, 0x%02X\n",            // 0x1e
    "[ASM] RRA\n",                     // 0x1f
    "[ASM] JR NZ, 0x%02X\n",           // 0x20
    "[ASM] LD HL, 0x%04X\n",           // 0x21
    "[ASM] LDI (HL), A\n",             // 0x22
    "[ASM] INC HL\n",                  // 0x23
    "[ASM] INC H\n",                   // 0x24
    "[ASM] DEC H\n",                   // 0x25
    "[ASM] LD H, 0x%02X\n",            // 0x26
    "[ASM] DAA\n",                     // 0x27
    "[ASM] JR Z, 0x%02X\n",            // 0x28
    "[ASM] ADD HL, HL\n",              // 0x29
    "[ASM] LDI A, (HL)\n",             // 0x2a
    "[ASM] DEC HL\n",                  // 0x2b
    "[ASM] INC L\n",                   // 0x2c
    "[ASM] DEC L\n",                   // 0x2d
    "[ASM] LD L, 0x%02X\n",            // 0x2e
    "[ASM] CPL\n",                     // 0x2f
    "[ASM] JR NC, 0x%02X\n",           // 0x30
    "[ASM] LD SP, 0x%04X\n",           // 0x31
    "[ASM] LDD (HL), A\n",             // 0x32
    "[ASM] INC SP\n",                  // 0x33
    "[ASM] INC (HL)\n",                // 0x34
    "[ASM] DEC (HL)\n",                // 0x35
    "[ASM] LD (HL), 0x%02X\n",         // 0x36
    "[ASM] SCF\n",                     // 0x37
    "[ASM] JR C, 0x%02X\n",            // 0x38
    "[ASM] ADD HL, SP\n",              // 0x39
    "[ASM] LDD A, (HL)\n",             // 0x3a
    "[ASM] DEC SP\n",                  // 0x3b
    "[ASM] INC A\n",                   // 0x3c
    "[ASM] DEC A\n",                   // 0x3d
    "[ASM] LD A, 0x%02X\n",            // 0x3e
    "[ASM] CCF\n",                     // 0x3f
    "[ASM] LD B, B\n",                 // 0x40
    "[ASM] LD B, C\n",                 // 0x41
    "[ASM] LD B, D\n",                 // 0x42
    "[ASM] LD B, E\n",                 // 0x43
    "[ASM] LD B, H\n",                 // 0x44
    "[ASM] LD B, L\n",                 // 0x45
    "[ASM] LD B, (HL)\n",              // 0x46
    "[ASM] LD B, A\n",                 // 0x47
    "[ASM] LD C, B\n",                 // 0x48
    "[ASM] LD C, C\n",                 // 0x49
    "[ASM] LD C, D\n",                 // 0x4a
    "[ASM] LD C, E\n",                 // 0x4b
    "[ASM] LD C, H\n",                 // 0x4c
    "[ASM] LD C, L\n",                 // 0x4d
    "[ASM] LD C, (HL)\n",              // 0x4e
    "[ASM] LD C, A\n",                 // 0x4f
    "[ASM] LD D, B\n",                 // 0x50
    "[ASM] LD D, C\n",                 // 0x51
    "[ASM] LD D, D\n",                 // 0x52
    "[ASM] LD D, E\n",                 // 0x53
    "[ASM] LD D, H\n",                 // 0x54
    "[ASM] LD D, L\n",                 // 0x55
    "[ASM] LD D, (HL)\n",              // 0x56
    "[ASM] LD D, A\n",                 // 0x57
    "[ASM] LD E, B\n",                 // 0x58
    "[ASM] LD E, C\n",                 // 0x59
    "[ASM] LD E, D\n",                 // 0x5a
    "[ASM] LD E, E\n",                 // 0x5b
    "[ASM] LD E, H\n",                 // 0x5c
    "[ASM] LD E, L\n",                 // 0x5d
    "[ASM] LD E, (HL)\n",              // 0x5e
    "[ASM] LD E, A\n",                 // 0x5f
    "[ASM] LD H, B\n",                 // 0x60
    "[ASM] LD H, C\n",                 // 0x61
    "[ASM] LD H, D\n",                 // 0x62
    "[ASM] LD H, E\n",                 // 0x63
    "[ASM] LD H, H\n",                 // 0x64
    "[ASM] LD H, L\n",                 // 0x65
    "[ASM] LD H, (HL)\n",              // 0x66
    "[ASM] LD H, A\n",                 // 0x67
    "[ASM] LD L, B\n",                 // 0x68
    "[ASM] LD L, C\n",                 // 0x69
    "[ASM] LD L, D\n",                 // 0x6a
    "[ASM] LD L, E\n",                 // 0x6b
    "[ASM] LD L, H\n",                 // 0x6c
    "[ASM] LD L, L\n",                 // 0x6d
    "[ASM] LD L, (HL)\n",              // 0x6e
    "[ASM] LD L, A\n",                 // 0x6f
    "[ASM] LD (HL), B\n",              // 0x70
    "[ASM] LD (HL), C\n",              // 0x71
    "[ASM] LD (HL), D\n",              // 0x72
    "[ASM] LD (HL), E\n",              // 0x73
    "[ASM] LD (HL), H\n",              // 0x74
    "[ASM] LD (HL), L\n",              // 0x75
    "[ASM] HALT\n",                    // 0x76
    "[ASM] LD (HL), A\n",              // 0x77
    "[ASM] LD A, B\n",                 // 0x78
    "[ASM] LD A, C\n",                 // 0x79
    "[ASM] LD A, D\n",                 // 0x7a
    "[ASM] LD A, E\n",                 // 0x7b
    "[ASM] LD A, H\n",                 // 0x7c
    "[ASM] LD A, L\n",                 // 0x7d
    "[ASM] LD A, (HL)\n",              // 0x7e
    "[ASM] LD A, A\n",                 // 0x7f
    "[ASM] ADD A, B\n",                // 0x80
    "[ASM] ADD A, C\n",                // 0x81
    "[ASM] ADD A, D\n",                // 0x82
    "[ASM] ADD A, E\n",                // 0x83
    "[ASM] ADD A, H\n",                // 0x84
    "[ASM] ADD A, L\n",                // 0x85
    "[ASM] ADD A, (HL)\n",             // 0x86
    "[ASM] ADD A\n",                   // 0x87
    "[ASM] ADC B\n",                   // 0x88
    "[ASM] ADC C\n",                   // 0x89
    "[ASM] ADC D\n",                   // 0x8a
    "[ASM] ADC E\n",                   // 0x8b
    "[ASM] ADC H\n",                   // 0x8c
    "[ASM] ADC L\n",                   // 0x8d
    "[ASM] ADC (HL)\n",                // 0x8e
    "[ASM] ADC A\n",                   // 0x8f
    "[ASM] SUB B\n",                   // 0x90
    "[ASM] SUB C\n",                   // 0x91
    "[ASM] SUB D\n",                   // 0x92
    "[ASM] SUB E\n",                   // 0x93
    "[ASM] SUB H\n",                   // 0x94
    "[ASM] SUB L\n",                   // 0x95
    "[ASM] SUB (HL)\n",                // 0x96
    "[ASM] SUB A\n",                   // 0x97
    "[ASM] SBC B\n",                   // 0x98
    "[ASM] SBC C\n",                   // 0x99
    "[ASM] SBC D\n",                   // 0x9a
    "[ASM] SBC E\n",                   // 0x9b
    "[ASM] SBC H\n",                   // 0x9c
    "[ASM] SBC L\n",                   // 0x9d
    "[ASM] SBC (HL)\n",                // 0x9e
    "[ASM] SBC A\n",                   // 0x9f
    "[ASM] AND B\n",                   // 0xa0
    "[ASM] AND C\n",                   // 0xa1
    "[ASM] AND D\n",                   // 0xa2
    "[ASM] AND E\n",                   // 0xa3
    "[ASM] AND H\n",                   // 0xa4
    "[ASM] AND L\n",                   // 0xa5
    "[ASM] AND (HL)\n",                // 0xa6
    "[ASM] AND A\n",                   // 0xa7
    "[ASM] XOR B\n",                   // 0xa8
    "[ASM] XOR C\n",                   // 0xa9
    "[ASM] XOR D\n",                   // 0xaa
    "[ASM] XOR E\n",                   // 0xab
    "[ASM] XOR H\n",                   // 0xac
    "[ASM] XOR L\n",                   // 0xad
    "[ASM] XOR (HL)\n",                // 0xae
    "[ASM] XOR A\n",                   // 0xaf
    "[ASM] OR B\n",                    // 0xb0
    "[ASM] OR C\n",                    // 0xb1
    "[ASM] OR D\n",                    // 0xb2
    "[ASM] OR E\n",                    // 0xb3
    "[ASM] OR H\n",                    // 0xb4
    "[ASM] OR L\n",                    // 0xb5
    "[ASM] OR (HL)\n",                 // 0xb6
    "[ASM] OR A\n",                    // 0xb7
    "[ASM] CP B\n",                    // 0xb8
    "[ASM] CP C\n",                    // 0xb9
    "[ASM] CP D\n",                    // 0xba
    "[ASM] CP E\n",                    // 0xbb
    "[ASM] CP H\n",                    // 0xbc
    "[ASM] CP L\n",                    // 0xbd
    "[ASM] CP (HL)\n",                 // 0xbe
    "[ASM] CP A\n",                    // 0xbf
    "[ASM] RET NZ\n",                  // 0xc0
    "[ASM] POP BC\n",                  // 0xc1
    "[ASM] JP NZ, 0x%04X\n",           // 0xc2
    "[ASM] JP 0x%04X\n",               // 0xc3
    "[ASM] CALL NZ, 0x%04X\n",         // 0xc4
    "[ASM] PUSH BC\n",                 // 0xc5
    "[ASM] ADD A, 0x%02X\n",           // 0xc6
    "[ASM] RST 0x00\n",                // 0xc7
    "[ASM] RET Z\n",                   // 0xc8
    "[ASM] RET\n",                     // 0xc9
    "[ASM] JP Z, 0x%04X\n",            // 0xca
    "[ASM] CB %02X\n",                 // 0xcb
    "[ASM] CALL Z, 0x%04X\n",          // 0xcc
    "[ASM] CALL 0x%04X\n",             // 0xcd
    "[ASM] ADC 0x%02X\n",              // 0xce
    "[ASM] RST 0x08\n",                // 0xcf
    "[ASM] RET NC\n",                  // 0xd0
    "[ASM] POP DE\n",                  // 0xd1
    "[ASM] JP NC, 0x%04X\n",           // 0xd2
    "[ASM] UNKNOWN\n",                 // 0xd3
    "[ASM] CALL NC, 0x%04X\n",         // 0xd4
    "[ASM] PUSH DE\n",                 // 0xd5
    "[ASM] SUB 0x%02X\n",              // 0xd6
    "[ASM] RST 0x10\n",                // 0xd7
    "[ASM] RET C\n",                   // 0xd8
    "[ASM] RETI\n",                    // 0xd9
    "[ASM] JP C, 0x%04X\n",            // 0xda
    "[ASM] UNKNOWN\n",                 // 0xdb
    "[ASM] CALL C, 0x%04X\n",          // 0xdc
    "[ASM] UNKNOWN\n",                 // 0xdd
    "[ASM] SBC 0x%02X\n",              // 0xde
    "[ASM] RST 0x18\n",                // 0xdf
    "[ASM] LD (0xFF00 + 0x%02X), A\n", // 0xe0
    "[ASM] POP HL\n",                  // 0xe1
    "[ASM] LD (0xFF00 + C), A\n",      // 0xe2
    "[ASM] UNKNOWN\n",                 // 0xe3
    "[ASM] UNKNOWN\n",                 // 0xe4
    "[ASM] PUSH HL\n",                 // 0xe5
    "[ASM] AND 0x%02X\n",              // 0xe6
    "[ASM] RST 0x20\n",                // 0xe7
    "[ASM] ADD SP,0x%02X\n",           // 0xe8
    "[ASM] JP HL\n",                   // 0xe9
    "[ASM] LD (0x%04X), A\n",          // 0xea
    "[ASM] UNKNOWN\n",                 // 0xeb
    "[ASM] UNKNOWN\n",                 // 0xec
    "[ASM] UNKNOWN\n",                 // 0xed
    "[ASM] XOR 0x%02X\n",              // 0xee
    "[ASM] RST 0x28\n",                // 0xef
    "[ASM] LD A, (0xFF00 + 0x%02X)\n", // 0xf0
    "[ASM] POP AF\n",                  // 0xf1
    "[ASM] LD A, (0xFF00 + C)\n",      // 0xf2
    "[ASM] DI\n",                      // 0xf3
    "[ASM] UNKNOWN\n",                 // 0xf4
    "[ASM] PUSH AF\n",                 // 0xf5
    "[ASM] OR 0x%02X\n",               // 0xf6
    "[ASM] RST 0x30\n",                // 0xf7
    "[ASM] LD HL, SP+0x%02X\n",        // 0xf8
    "[ASM] LD SP, HL\n",               // 0xf9
    "[ASM] LD A, (0x%04X)\n",          // 0xfa
    "[ASM] EI\n",                      // 0xfb
    "[ASM] UNKNOWN\n",                 // 0xfc
    "[ASM] UNKNOWN\n",                 // 0xfd
    "[ASM] CP 0x%02X\n",               // 0xfe
    "[ASM] RST 0x38\n",                // 0xff
};

const uint8_t clock_m_cycles[256] = {
    1, 3, 2, 2, 1, 1, 2, 2, 10, 2, 2, 2, 1, 1, 2, 2, // 0x0_
    1, 3, 2, 2, 1, 1, 2, 2, 2,  2, 2, 2, 1, 1, 2, 2, // 0x1_
    0, 3, 2, 2, 1, 1, 2, 1, 0,  2, 2, 2, 1, 1, 2, 1, // 0x2_
    2, 3, 2, 2, 3, 3, 3, 1, 0,  2, 2, 2, 1, 1, 2, 1, // 0x3_
    1, 1, 1, 1, 1, 1, 2, 1, 1,  1, 1, 1, 1, 1, 2, 1, // 0x4_
    1, 1, 1, 1, 1, 1, 2, 1, 1,  1, 1, 1, 1, 1, 2, 1, // 0x5_
    1, 1, 1, 1, 1, 1, 2, 1, 1,  1, 1, 1, 1, 1, 2, 1, // 0x6_
    2, 2, 2, 2, 2, 2, 1, 2, 1,  1, 1, 1, 1, 1, 2, 1, // 0x7_
    1, 1, 1, 1, 1, 1, 2, 1, 1,  1, 1, 1, 1, 1, 2, 1, // 0x8_
    1, 1, 1, 1, 1, 1, 2, 1, 1,  1, 1, 1, 1, 1, 2, 1, // 0x9_
    1, 1, 1, 1, 1, 1, 2, 1, 1,  1, 1, 1, 1, 1, 2, 1, // 0xa_
    1, 1, 1, 1, 1, 1, 2, 1, 1,  1, 1, 1, 1, 1, 2, 1, // 0xb_
    0, 3, 0, 3, 0, 4, 2, 4, 0,  1, 0, 0, 0, 3, 2, 4, // 0xc_
    0, 3, 0, 0, 0, 4, 2, 4, 0,  4, 0, 0, 0, 0, 2, 4, // 0xd_
    3, 3, 2, 0, 0, 4, 2, 4, 4,  1, 4, 0, 0, 0, 2, 4, // 0xe_
    3, 3, 2, 1, 0, 4, 2, 4, 3,  2, 4, 1, 0, 0, 2, 4  // 0xf_
};

void CPU::INSTRUCTION_DECODER() {
  switch (reg.pc++) {
  case 0x00:
    printf("%s", disasm[0x00]);
    clock_m += clock_m_cycles[0x00];
    NOP();
    break;
  case 0x01: {
    uint16_t val = read_word(reg.pc++);
    printf(disasm[0x01], val);
    clock_m += clock_m_cycles[0x01];
    LD_r16_n16(&REGISTERS::bc, val);
  } break;
  case 0x02:
    printf("%s", disasm[0x02]);
    clock_m += clock_m_cycles[0x02];
    LD_r16_r8(&REGISTERS::bc, &REGISTERS::a);
    break;
  case 0x03:
    printf("%s", disasm[0x03]);
    clock_m += clock_m_cycles[0x03];
    INC_r16(&REGISTERS::bc);
    break;
  case 0x04:
    printf("%s", disasm[0x04]);
    clock_m += clock_m_cycles[0x04];
    INC_r8(&REGISTERS::b);
    break;
  case 0x05:
    printf("%s", disasm[0x05]);
    clock_m += clock_m_cycles[0x05];
    DEC_r8(&REGISTERS::b);
    break;
  case 0x06: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x06], val);
    clock_m += clock_m_cycles[0x06];
    LD_r8_n8(&REGISTERS::b, val);
  } break;
  case 0x07:
    break;
  case 0x08: {
    uint16_t val = read_word(reg.pc++);
    printf(disasm[0x08], val);
    clock_m += clock_m_cycles[0x08];
    LD_n16_SP(val);
  } break;
  case 0x09:
    printf("%s", disasm[0x09]);
    clock_m += clock_m_cycles[0x09];
    ADD_HL_r16(&REGISTERS::bc);
    break;
  case 0x0A:
    printf("%s", disasm[0x0a]);
    clock_m += clock_m_cycles[0x0a];
    LD_r8_r16(&REGISTERS::a, &REGISTERS::bc);
    break;
  case 0x0B:
    printf("%s", disasm[0x0b]);
    clock_m += clock_m_cycles[0x0b];
    DEC_r16(&REGISTERS::bc);
    break;
  case 0x0C:
    printf("%s", disasm[0x0c]);
    clock_m += clock_m_cycles[0x0c];
    INC_r8(&REGISTERS::c);
    break;
  case 0x0D:
    printf("%s", disasm[0x0d]);
    clock_m += clock_m_cycles[0x0d];
    DEC_r8(&REGISTERS::c);
    break;
  case 0x0E:
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x0e], val);
    clock_m += clock_m_cycles[0x0e];
    LD_r8_n8(&REGISTERS::c, val);
    break;
  case 0x0F:
    break;
  case 0x10:
    break;
  case 0x11: {
    uint16_t val = read_word(reg.pc++);
    printf(disasm[0x11], val);
    clock_m += clock_m_cycles[0x11];
    LD_r16_n16(&REGISTERS::de, val);
  } break;
  case 0x12:
    printf("%s", disasm[0x12]);
    clock_m += clock_m_cycles[0x12];
    LD_r16_r8(&REGISTERS::de, &REGISTERS::a);
    break;
  case 0x13:
    printf("%s", disasm[0x13]);
    clock_m += clock_m_cycles[0x13];
    INC_r16(&REGISTERS::de);
    break;
  case 0x14:
    printf("%s", disasm[0x14]);
    clock_m += clock_m_cycles[0x14];
    INC_r8(&REGISTERS::d);
    break;
  case 0x15:
    printf("%s", disasm[0x15]);
    clock_m += clock_m_cycles[0x15];
    DEC_r8(&REGISTERS::d);
    break;
  case 0x16: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x16], val);
    clock_m += clock_m_cycles[0x16];
    LD_r8_n8(&REGISTERS::d, val);
  } break;
  case 0x17:
    break;
  case 0x18: {
    int8_t val = read_byte(reg.pc++);
    printf(disasm[0x18], val);
    clock_m += clock_m_cycles[0x18];
    JR_n16(val);
  } break;
  case 0x19:
    printf("%s", disasm[0x19]);
    clock_m += clock_m_cycles[0x19];
    ADD_HL_r16(&REGISTERS::de);
    break;
  case 0x1A:
    printf("%s", disasm[0x1a]);
    clock_m += clock_m_cycles[0x1a];
    LD_r8_r16(&REGISTERS::a, &REGISTERS::de);
    break;
  case 0x1B:
    printf("%s", disasm[0x1b]);
    clock_m += clock_m_cycles[0x1b];
    DEC_r16(&REGISTERS::de);
    break;
  case 0x1C:
    printf("%s", disasm[0x1c]);
    clock_m += clock_m_cycles[0x1c];
    INC_r8(&REGISTERS::e);
    break;
  case 0x1D:
    printf("%s", disasm[0x1d]);
    clock_m += clock_m_cycles[0x1d];
    DEC_r8(&REGISTERS::e);
    break;
  case 0x1E: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x1e], val);
    clock_m += clock_m_cycles[0x1e];
    LD_r8_n8(&REGISTERS::e, val);
  } break;
  case 0x1F:
    break;
  case 0x20: {
    int8_t val = read_byte(reg.pc++);
    printf(disasm[0x20], val);
    clock_m += clock_m_cycles[0x20];
    JR_CC_n16(flag_value(N) & flag_value(Z), val);
  } break;
  case 0x21: {
    uint16_t val = read_word(reg.pc++);
    printf(disasm[0x21], val);
    clock_m += clock_m_cycles[0x21];
    LD_r16_n16(&REGISTERS::hl, val);
  } break;
  case 0x22:
    printf("%s", disasm[0x22]);
    clock_m += clock_m_cycles[0x22];
    LD_r16_r8(&REGISTERS::hl, &REGISTERS::a);
    break;
  case 0x23:
    printf("%s", disasm[0x23]);
    clock_m += clock_m_cycles[0x23];
    INC_r16(&REGISTERS::hl);
    break;
  case 0x24:
    printf("%s", disasm[0x24]);
    clock_m += clock_m_cycles[0x24];
    INC_r8(&REGISTERS::h);
    break;
  case 0x25:
    printf("%s", disasm[0x25]);
    clock_m += clock_m_cycles[0x25];
    DEC_r8(&REGISTERS::h);
    break;
  case 0x26: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x26], val);
    clock_m += clock_m_cycles[0x26];
    LD_r8_n8(&REGISTERS::h, val);
  } break;
  case 0x27: {
    printf("%s", disasm[0x27]);
    clock_m += clock_m_cycles[0x27];
    DAA();
  } break;
  case 0x28: {
    int8_t val = read_byte(reg.pc++);
    printf(disasm[0x28], val);
    clock_m += clock_m_cycles[0x28];
    JR_CC_n16(flag_value(Z), val);
  } break;
  case 0x29:
    printf("%s", disasm[0x29]);
    clock_m += clock_m_cycles[0x29];
    ADD_HL_r16(&REGISTERS::hl);
    break;
  case 0x2A:
    printf("%s", disasm[0x2a]);
    clock_m += clock_m_cycles[0x2a];
    LD_r8_r16(&REGISTERS::a, &REGISTERS::hl);
    break;
  case 0x2B:
    printf("%s", disasm[0x2b]);
    clock_m += clock_m_cycles[0x2b];
    DEC_r16(&REGISTERS::hl);
    break;
  case 0x2C:
    printf("%s", disasm[0x2c]);
    clock_m += clock_m_cycles[0x2c];
    INC_r8(&REGISTERS::l);
    break;
  case 0x2D:
    printf("%s", disasm[0x2d]);
    clock_m += clock_m_cycles[0x2d];
    DEC_r8(&REGISTERS::l);
    break;
  case 0x2E: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x2e], val);
    clock_m += clock_m_cycles[0x2e];
    LD_r8_n8(&REGISTERS::l, val);
  } break;
  case 0x2F: {
    printf("%s", disasm[0x2f]);
    clock_m += clock_m_cycles[0x2f];
    CPL();
  } break;
  case 0x30: {
    int8_t val = read_byte(reg.pc++);
    printf(disasm[0x30], val);
    clock_m += clock_m_cycles[0x30];
    JR_CC_n16(flag_value(N) & flag_value(C), val);
  } break;
  case 0x31: {
    uint16_t val = read_word(reg.pc++);
    printf(disasm[0x31], val);
    clock_m += clock_m_cycles[0x31];
    LD_SP_n16(val);
  } break;
  case 0x32:
    printf("%s", disasm[0x32]);
    clock_m += clock_m_cycles[0x32];
    LD_HLD_A();
    break;
  case 0x33:
    printf("%s", disasm[0x33]);
    clock_m += clock_m_cycles[0x33];
    INC_SP();
    break;
  case 0x34:
    printf("%s", disasm[0x34]);
    clock_m += clock_m_cycles[0x34];
    INC_HL();
    break;
  case 0x35:
    printf("%s", disasm[0x35]);
    clock_m += clock_m_cycles[0x35];
    DEC_HL();
    break;
  case 0x36: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x36], val);
    clock_m += clock_m_cycles[0x36];
    LD_HL_n8(val);
  } break;
  case 0x37: {
  } break;
  case 0x38: {
    int8_t val = read_byte(reg.pc++);
    printf(disasm[0x38], val);
    clock_m += clock_m_cycles[0x38];
    JR_CC_n16(flag_value(C), val);
  } break;
  case 0x39:
    printf("%s", disasm[0x39]);
    clock_m += clock_m_cycles[0x39];
    ADD_HL_SP();
    break;
  case 0x3A:
    printf("%s", disasm[0x3a]);
    clock_m += clock_m_cycles[0x3a];
    LD_A_HLD();
    break;
  case 0x3B:
    printf("%s", disasm[0x3b]);
    clock_m += clock_m_cycles[0x3b];
    DEC_SP();
    break;
  case 0x3C:
    printf("%s", disasm[0x3c]);
    clock_m += clock_m_cycles[0x3c];
    INC_r8(&REGISTERS::a);
    break;
  case 0x3D:
    printf("%s", disasm[0x3d]);
    clock_m += clock_m_cycles[0x3d];
    DEC_r8(&REGISTERS::a);
    break;
  case 0x3E: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x3e], val);
    clock_m += clock_m_cycles[0x3e];
    LD_r8_n8(&REGISTERS::a, val);
  } break;
  case 0x3F: {
    printf("%s", disasm[0x3f]);
    clock_m += clock_m_cycles[0x3f];
    CCF();
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
  case 0x76:
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
  case 0xA0:
    break;
  case 0xA1:
    break;
  case 0xA2:
    break;
  case 0xA3:
    break;
  case 0xA4:
    break;
  case 0xA5:
    break;
  case 0xA6:
    break;
  case 0xA7:
    break;
  case 0xA8:
    break;
  case 0xA9:
    break;
  case 0xAA:
    break;
  case 0xAB:
    break;
  case 0xAC:
    break;
  case 0xAD:
    break;
  case 0xAE:
    break;
  case 0xAF:
    break;
  case 0xB0:
    break;
  case 0xB1:
    break;
  case 0xB2:
    break;
  case 0xB3:
    break;
  case 0xB4:
    break;
  case 0xB5:
    break;
  case 0xB6:
    break;
  case 0xB7:
    break;
  case 0xB8:
    break;
  case 0xB9:
    break;
  case 0xBA:
    break;
  case 0xBB:
    break;
  case 0xBC:
    break;
  case 0xBD:
    break;
  case 0xBE:
    break;
  case 0xBF:
    break;
  case 0xC0:
    break;
  case 0xCB:
    break;
  case 0xFF:
    break;
  default:
    break;
  }
}
