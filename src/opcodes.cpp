#include "../include/cpu.h"
#include <cstdint>
#include <cstdio>
#include <ctime>

const char *disasm[256] = {
    "[00] NOP\n",                     // 0x00
    "[01] LD BC, 0x%04X\n",           // 0x01
    "[02] LD (BC), A\n",              // 0x02
    "[03] INC BC\n",                  // 0x03
    "[04] INC B\n",                   // 0x04
    "[05] DEC B\n",                   // 0x05
    "[06] LD B, 0x%02X\n",            // 0x06
    "[07] RLCA\n",                    // 0x07
    "[08] LD (0x%04X), SP\n",         // 0x08
    "[09] ADD HL, BC\n",              // 0x09
    "[0a] LD A, (BC)\n",              // 0x0a
    "[0b] DEC BC\n",                  // 0x0b
    "[0c] INC C\n",                   // 0x0c
    "[0d] DEC C\n",                   // 0x0d
    "[0e] LD C, 0x%02X\n",            // 0x0e
    "[0f] RRCA\n",                    // 0x0f
    "[10] STOP\n",                    // 0x10
    "[11] LD DE, 0x%04X\n",           // 0x11
    "[12] LD (DE), A\n",              // 0x12
    "[13] INC DE\n",                  // 0x13
    "[14] INC D\n",                   // 0x14
    "[15] DEC D\n",                   // 0x15
    "[16] LD D, 0x%02X\n",            // 0x16
    "[17] RLA\n",                     // 0x17
    "[18] JR 0x%02X\n",               // 0x18
    "[19] ADD HL, DE\n",              // 0x19
    "[1a] LD A, (DE)\n",              // 0x1a
    "[1b] DEC DE\n",                  // 0x1b
    "[1c] INC E\n",                   // 0x1c
    "[1d] DEC E\n",                   // 0x1d
    "[1e] LD E, 0x%02X\n",            // 0x1e
    "[1f] RRA\n",                     // 0x1f
    "[20] JR NZ, 0x%02X\n",           // 0x20
    "[21] LD HL, 0x%04X\n",           // 0x21
    "[22] LDI (HL), A\n",             // 0x22
    "[23] INC HL\n",                  // 0x23
    "[24] INC H\n",                   // 0x24
    "[25] DEC H\n",                   // 0x25
    "[26] LD H, 0x%02X\n",            // 0x26
    "[27] DAA\n",                     // 0x27
    "[28] JR Z, 0x%02X\n",            // 0x28
    "[29] ADD HL, HL\n",              // 0x29
    "[2a] LDI A, (HL)\n",             // 0x2a
    "[2b] DEC HL\n",                  // 0x2b
    "[2c] INC L\n",                   // 0x2c
    "[2d] DEC L\n",                   // 0x2d
    "[2e] LD L, 0x%02X\n",            // 0x2e
    "[2f] CPL\n",                     // 0x2f
    "[30] JR NC, 0x%02X\n",           // 0x30
    "[31] LD SP, 0x%04X\n",           // 0x31
    "[32] LDD (HL), A\n",             // 0x32
    "[33] INC SP\n",                  // 0x33
    "[34] INC (HL)\n",                // 0x34
    "[35] DEC (HL)\n",                // 0x35
    "[36] LD (HL), 0x%02X\n",         // 0x36
    "[37] SCF\n",                     // 0x37
    "[38] JR C, 0x%02X\n",            // 0x38
    "[39] ADD HL, SP\n",              // 0x39
    "[3a] LDD A, (HL)\n",             // 0x3a
    "[3b] DEC SP\n",                  // 0x3b
    "[3c] INC A\n",                   // 0x3c
    "[3d] DEC A\n",                   // 0x3d
    "[3e] LD A, 0x%02X\n",            // 0x3e
    "[3f] CCF\n",                     // 0x3f
    "[40] LD B, B\n",                 // 0x40
    "[41] LD B, C\n",                 // 0x41
    "[42] LD B, D\n",                 // 0x42
    "[43] LD B, E\n",                 // 0x43
    "[44] LD B, H\n",                 // 0x44
    "[45] LD B, L\n",                 // 0x45
    "[46] LD B, (HL)\n",              // 0x46
    "[47] LD B, A\n",                 // 0x47
    "[48] LD C, B\n",                 // 0x48
    "[49] LD C, C\n",                 // 0x49
    "[4a] LD C, D\n",                 // 0x4a
    "[4b] LD C, E\n",                 // 0x4b
    "[4c] LD C, H\n",                 // 0x4c
    "[4d] LD C, L\n",                 // 0x4d
    "[4e] LD C, (HL)\n",              // 0x4e
    "[4f] LD C, A\n",                 // 0x4f
    "[50] LD D, B\n",                 // 0x50
    "[51] LD D, C\n",                 // 0x51
    "[52] LD D, D\n",                 // 0x52
    "[53] LD D, E\n",                 // 0x53
    "[54] LD D, H\n",                 // 0x54
    "[55] LD D, L\n",                 // 0x55
    "[56] LD D, (HL)\n",              // 0x56
    "[57] LD D, A\n",                 // 0x57
    "[58] LD E, B\n",                 // 0x58
    "[59] LD E, C\n",                 // 0x59
    "[5a] LD E, D\n",                 // 0x5a
    "[5b] LD E, E\n",                 // 0x5b
    "[5c] LD E, H\n",                 // 0x5c
    "[5d] LD E, L\n",                 // 0x5d
    "[5e] LD E, (HL)\n",              // 0x5e
    "[5f] LD E, A\n",                 // 0x5f
    "[60] LD H, B\n",                 // 0x60
    "[61] LD H, C\n",                 // 0x61
    "[62] LD H, D\n",                 // 0x62
    "[63] LD H, E\n",                 // 0x63
    "[64] LD H, H\n",                 // 0x64
    "[65] LD H, L\n",                 // 0x65
    "[66] LD H, (HL)\n",              // 0x66
    "[67] LD H, A\n",                 // 0x67
    "[68] LD L, B\n",                 // 0x68
    "[69] LD L, C\n",                 // 0x69
    "[6a] LD L, D\n",                 // 0x6a
    "[6b] LD L, E\n",                 // 0x6b
    "[6c] LD L, H\n",                 // 0x6c
    "[6d] LD L, L\n",                 // 0x6d
    "[6e] LD L, (HL)\n",              // 0x6e
    "[6f] LD L, A\n",                 // 0x6f
    "[70] LD (HL), B\n",              // 0x70
    "[71] LD (HL), C\n",              // 0x71
    "[72] LD (HL), D\n",              // 0x72
    "[73] LD (HL), E\n",              // 0x73
    "[74] LD (HL), H\n",              // 0x74
    "[75] LD (HL), L\n",              // 0x75
    "[76] HALT\n",                    // 0x76
    "[77] LD (HL), A\n",              // 0x77
    "[78] LD A, B\n",                 // 0x78
    "[79] LD A, C\n",                 // 0x79
    "[7a] LD A, D\n",                 // 0x7a
    "[7b] LD A, E\n",                 // 0x7b
    "[7c] LD A, H\n",                 // 0x7c
    "[7d] LD A, L\n",                 // 0x7d
    "[7e] LD A, (HL)\n",              // 0x7e
    "[7f] LD A, A\n",                 // 0x7f
    "[80] ADD A, B\n",                // 0x80
    "[81] ADD A, C\n",                // 0x81
    "[82] ADD A, D\n",                // 0x82
    "[83] ADD A, E\n",                // 0x83
    "[84] ADD A, H\n",                // 0x84
    "[85] ADD A, L\n",                // 0x85
    "[86] ADD A, (HL)\n",             // 0x86
    "[87] ADD A\n",                   // 0x87
    "[88] ADC B\n",                   // 0x88
    "[89] ADC C\n",                   // 0x89
    "[8a] ADC D\n",                   // 0x8a
    "[8b] ADC E\n",                   // 0x8b
    "[8c] ADC H\n",                   // 0x8c
    "[8d] ADC L\n",                   // 0x8d
    "[8e] ADC (HL)\n",                // 0x8e
    "[8f] ADC A\n",                   // 0x8f
    "[90] SUB B\n",                   // 0x90
    "[91] SUB C\n",                   // 0x91
    "[92] SUB D\n",                   // 0x92
    "[93] SUB E\n",                   // 0x93
    "[94] SUB H\n",                   // 0x94
    "[95] SUB L\n",                   // 0x95
    "[96] SUB (HL)\n",                // 0x96
    "[97] SUB A\n",                   // 0x97
    "[98] SBC B\n",                   // 0x98
    "[99] SBC C\n",                   // 0x99
    "[9a] SBC D\n",                   // 0x9a
    "[9b] SBC E\n",                   // 0x9b
    "[9c] SBC H\n",                   // 0x9c
    "[9d] SBC L\n",                   // 0x9d
    "[9e] SBC (HL)\n",                // 0x9e
    "[9f] SBC A\n",                   // 0x9f
    "[a0] AND B\n",                   // 0xa0
    "[a1] AND C\n",                   // 0xa1
    "[a2] AND D\n",                   // 0xa2
    "[a3] AND E\n",                   // 0xa3
    "[a4] AND H\n",                   // 0xa4
    "[a5] AND L\n",                   // 0xa5
    "[a6] AND (HL)\n",                // 0xa6
    "[a7] AND A\n",                   // 0xa7
    "[a8] XOR B\n",                   // 0xa8
    "[a9] XOR C\n",                   // 0xa9
    "[aa] XOR D\n",                   // 0xaa
    "[ab] XOR E\n",                   // 0xab
    "[ac] XOR H\n",                   // 0xac
    "[ad] XOR L\n",                   // 0xad
    "[ae] XOR (HL)\n",                // 0xae
    "[af] XOR A\n",                   // 0xaf
    "[b0] OR B\n",                    // 0xb0
    "[b1] OR C\n",                    // 0xb1
    "[b2] OR D\n",                    // 0xb2
    "[b3] OR E\n",                    // 0xb3
    "[b4] OR H\n",                    // 0xb4
    "[b5] OR L\n",                    // 0xb5
    "[b6] OR (HL)\n",                 // 0xb6
    "[b7] OR A\n",                    // 0xb7
    "[b8] CP B\n",                    // 0xb8
    "[b9] CP C\n",                    // 0xb9
    "[ba] CP D\n",                    // 0xba
    "[bb] CP E\n",                    // 0xbb
    "[bc] CP H\n",                    // 0xbc
    "[bd] CP L\n",                    // 0xbd
    "[be] CP (HL)\n",                 // 0xbe
    "[bf] CP A\n",                    // 0xbf
    "[c0] RET NZ\n",                  // 0xc0
    "[c1] POP BC\n",                  // 0xc1
    "[c2] JP NZ, 0x%04X\n",           // 0xc2
    "[c3] JP 0x%04X\n",               // 0xc3
    "[c4] CALL NZ, 0x%04X\n",         // 0xc4
    "[c5] PUSH BC\n",                 // 0xc5
    "[c6] ADD A, 0x%02X\n",           // 0xc6
    "[c7] RST 0x00\n",                // 0xc7
    "[c8] RET Z\n",                   // 0xc8
    "[c9] RET\n",                     // 0xc9
    "[ca] JP Z, 0x%04X\n",            // 0xca
    "[cb] CB %02X\n",                 // 0xcb
    "[cc] CALL Z, 0x%04X\n",          // 0xcc
    "[cd] CALL 0x%04X\n",             // 0xcd
    "[ce] ADC 0x%02X\n",              // 0xce
    "[cf] RST 0x08\n",                // 0xcf
    "[d0] RET NC\n",                  // 0xd0
    "[d1] POP DE\n",                  // 0xd1
    "[d2] JP NC, 0x%04X\n",           // 0xd2
    "[d3] UNKNOWN\n",                 // 0xd3
    "[d4] CALL NC, 0x%04X\n",         // 0xd4
    "[d5] PUSH DE\n",                 // 0xd5
    "[d6] SUB 0x%02X\n",              // 0xd6
    "[d7] RST 0x10\n",                // 0xd7
    "[d8] RET C\n",                   // 0xd8
    "[d9] RETI\n",                    // 0xd9
    "[da] JP C, 0x%04X\n",            // 0xda
    "[db] UNKNOWN\n",                 // 0xdb
    "[dc] CALL C, 0x%04X\n",          // 0xdc
    "[dd] UNKNOWN\n",                 // 0xdd
    "[de] SBC 0x%02X\n",              // 0xde
    "[df] RST 0x18\n",                // 0xdf
    "[e0] LD (0xFF00 + 0x%02X), A\n", // 0xe0
    "[e1] POP HL\n",                  // 0xe1
    "[e2] LD (0xFF00 + C), A\n",      // 0xe2
    "[e3] UNKNOWN\n",                 // 0xe3
    "[e4] UNKNOWN\n",                 // 0xe4
    "[e5] PUSH HL\n",                 // 0xe5
    "[e6] AND 0x%02X\n",              // 0xe6
    "[e7] RST 0x20\n",                // 0xe7
    "[e8] ADD SP,0x%02X\n",           // 0xe8
    "[e9] JP HL\n",                   // 0xe9
    "[ea] LD (0x%04X), A\n",          // 0xea
    "[eb] UNKNOWN\n",                 // 0xeb
    "[ec] UNKNOWN\n",                 // 0xec
    "[ed] UNKNOWN\n",                 // 0xed
    "[ee] XOR 0x%02X\n",              // 0xee
    "[ef] RST 0x28\n",                // 0xef
    "[f0] LD A, (0xFF00 + 0x%02X)\n", // 0xf0
    "[f1] POP AF\n",                  // 0xf1
    "[f2] LD A, (0xFF00 + C)\n",      // 0xf2
    "[f3] DI\n",                      // 0xf3
    "[f4] UNKNOWN\n",                 // 0xf4
    "[f5] PUSH AF\n",                 // 0xf5
    "[f6] OR 0x%02X\n",               // 0xf6
    "[f7] RST 0x30\n",                // 0xf7
    "[f8] LD HL, SP+0x%02X\n",        // 0xf8
    "[f9] LD SP, HL\n",               // 0xf9
    "[fa] LD A, (0x%04X)\n",          // 0xfa
    "[fb] EI\n",                      // 0xfb
    "[fc] UNKNOWN\n",                 // 0xfc
    "[fd] UNKNOWN\n",                 // 0xfd
    "[fe] CP 0x%02X\n",               // 0xfe
    "[ff] RST 0x38\n",                // 0xff
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
  switch (read_byte(reg.pc++)) {
  case 0x00:
    printf("%s", disasm[0x00]);
    clock_m += clock_m_cycles[0x00];
    NOP();
    break;
  case 0x01: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
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
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  case 0x08: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
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
  case 0x0E: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x0e], val);
    clock_m += clock_m_cycles[0x0e];
    LD_r8_n8(&REGISTERS::c, val);
  } break;
  case 0x0F:
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  case 0x10:
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  case 0x11: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
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
    reg.pc++;
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
    reg.pc++;
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
    printf("%s", disasm[0x40]);
    clock_m += clock_m_cycles[0x40];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::b);
    break;
  case 0x41:
    printf("%s", disasm[0x41]);
    clock_m += clock_m_cycles[0x41];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::c);
    break;
  case 0x42:
    printf("%s", disasm[0x42]);
    clock_m += clock_m_cycles[0x42];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::d);
    break;
  case 0x43:
    printf("%s", disasm[0x43]);
    clock_m += clock_m_cycles[0x43];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::e);
    break;
  case 0x44:
    printf("%s", disasm[0x44]);
    clock_m += clock_m_cycles[0x44];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::h);
    break;
  case 0x45:
    printf("%s", disasm[0x45]);
    clock_m += clock_m_cycles[0x45];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::l);
    break;
  case 0x46:
    printf("%s", disasm[0x46]);
    clock_m += clock_m_cycles[0x46];
    LD_r8_HL(&REGISTERS::b);
    break;
  case 0x47:
    printf("%s", disasm[0x47]);
    clock_m += clock_m_cycles[0x47];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::a);
    break;
  case 0x48:
    printf("%s", disasm[0x48]);
    clock_m += clock_m_cycles[0x48];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::b);
    break;
  case 0x49:
    printf("%s", disasm[0x49]);
    clock_m += clock_m_cycles[0x49];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::c);
    break;
  case 0x4A:
    printf("%s", disasm[0x4a]);
    clock_m += clock_m_cycles[0x4a];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::d);
    break;
  case 0x4B:
    printf("%s", disasm[0x4b]);
    clock_m += clock_m_cycles[0x4b];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::e);
    break;
  case 0x4C:
    printf("%s", disasm[0x4c]);
    clock_m += clock_m_cycles[0x4c];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::h);
    break;
  case 0x4D:
    printf("%s", disasm[0x4d]);
    clock_m += clock_m_cycles[0x4d];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::l);
    break;
  case 0x4E:
    printf("%s", disasm[0x4e]);
    clock_m += clock_m_cycles[0x4e];
    LD_r8_HL(&REGISTERS::c);
    break;
  case 0x4F:
    printf("%s", disasm[0x4f]);
    clock_m += clock_m_cycles[0x4f];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::a);
    break;
  case 0x50:
    printf("%s", disasm[0x50]);
    clock_m += clock_m_cycles[0x50];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::b);
    break;
  case 0x51:
    printf("%s", disasm[0x51]);
    clock_m += clock_m_cycles[0x51];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::c);
    break;
  case 0x52:
    printf("%s", disasm[0x52]);
    clock_m += clock_m_cycles[0x52];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::d);
    break;
  case 0x53:
    printf("%s", disasm[0x53]);
    clock_m += clock_m_cycles[0x53];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::e);
    break;
  case 0x54:
    printf("%s", disasm[0x54]);
    clock_m += clock_m_cycles[0x54];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::h);
    break;
  case 0x55:
    printf("%s", disasm[0x55]);
    clock_m += clock_m_cycles[0x55];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::l);
    break;
  case 0x56:
    printf("%s", disasm[0x56]);
    clock_m += clock_m_cycles[0x56];
    LD_r8_HL(&REGISTERS::d);
    break;
  case 0x57:
    printf("%s", disasm[0x57]);
    clock_m += clock_m_cycles[0x57];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::a);
    break;
  case 0x58:
    printf("%s", disasm[0x58]);
    clock_m += clock_m_cycles[0x58];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::b);
    break;
  case 0x59:
    printf("%s", disasm[0x59]);
    clock_m += clock_m_cycles[0x59];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::c);
    break;
  case 0x5A:
    printf("%s", disasm[0x5a]);
    clock_m += clock_m_cycles[0x5a];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::d);
    break;
  case 0x5B:
    printf("%s", disasm[0x5b]);
    clock_m += clock_m_cycles[0x5b];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::e);
    break;
  case 0x5C:
    printf("%s", disasm[0x5c]);
    clock_m += clock_m_cycles[0x5c];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::h);
    break;
  case 0x5D:
    printf("%s", disasm[0x5d]);
    clock_m += clock_m_cycles[0x5d];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::l);
    break;
  case 0x5E:
    printf("%s", disasm[0x5e]);
    clock_m += clock_m_cycles[0x5e];
    LD_r8_HL(&REGISTERS::e);
    break;
  case 0x5F:
    printf("%s", disasm[0x5f]);
    clock_m += clock_m_cycles[0x5f];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::a);
    break;
  case 0x60:
    printf("%s", disasm[0x60]);
    clock_m += clock_m_cycles[0x60];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::b);
    break;
  case 0x61:
    printf("%s", disasm[0x61]);
    clock_m += clock_m_cycles[0x61];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::c);
    break;
  case 0x62:
    printf("%s", disasm[0x62]);
    clock_m += clock_m_cycles[0x62];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::d);
    break;
  case 0x63:
    printf("%s", disasm[0x63]);
    clock_m += clock_m_cycles[0x63];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::e);
    break;
  case 0x64:
    printf("%s", disasm[0x64]);
    clock_m += clock_m_cycles[0x64];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::h);
    break;
  case 0x65:
    printf("%s", disasm[0x65]);
    clock_m += clock_m_cycles[0x65];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::l);
    break;
  case 0x66:
    printf("%s", disasm[0x66]);
    clock_m += clock_m_cycles[0x66];
    LD_r8_HL(&REGISTERS::h);
    break;
  case 0x67:
    printf("%s", disasm[0x67]);
    clock_m += clock_m_cycles[0x67];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::a);
    break;
  case 0x68:
    printf("%s", disasm[0x68]);
    clock_m += clock_m_cycles[0x68];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::b);
    break;
  case 0x69:
    printf("%s", disasm[0x69]);
    clock_m += clock_m_cycles[0x69];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::c);
    break;
  case 0x6A:
    printf("%s", disasm[0x6a]);
    clock_m += clock_m_cycles[0x6a];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::d);
    break;
  case 0x6B:
    printf("%s", disasm[0x6b]);
    clock_m += clock_m_cycles[0x6b];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::e);
    break;
  case 0x6C:
    printf("%s", disasm[0x6c]);
    clock_m += clock_m_cycles[0x6c];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::h);
    break;
  case 0x6D:
    printf("%s", disasm[0x6d]);
    clock_m += clock_m_cycles[0x6d];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::l);
    break;
  case 0x6E:
    printf("%s", disasm[0x6e]);
    clock_m += clock_m_cycles[0x6e];
    LD_r8_HL(&REGISTERS::l);
    break;
  case 0x6F:
    printf("%s", disasm[0x6f]);
    clock_m += clock_m_cycles[0x6f];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::a);
    break;
  case 0x70:
    printf("%s", disasm[0x70]);
    clock_m += clock_m_cycles[0x70];
    LD_HL_r8(&REGISTERS::b);
    break;
  case 0x71:
    printf("%s", disasm[0x71]);
    clock_m += clock_m_cycles[0x71];
    LD_HL_r8(&REGISTERS::c);
    break;
  case 0x72:
    printf("%s", disasm[0x72]);
    clock_m += clock_m_cycles[0x72];
    LD_HL_r8(&REGISTERS::d);
    break;
  case 0x73:
    printf("%s", disasm[0x73]);
    clock_m += clock_m_cycles[0x73];
    LD_HL_r8(&REGISTERS::e);
    break;
  case 0x74:
    printf("%s", disasm[0x74]);
    clock_m += clock_m_cycles[0x74];
    LD_HL_r8(&REGISTERS::h);
    break;
  case 0x75:
    printf("%s", disasm[0x75]);
    clock_m += clock_m_cycles[0x75];
    LD_HL_r8(&REGISTERS::l);
    break;
  case 0x76:
    break;
  case 0x77:
    printf("%s", disasm[0x77]);
    clock_m += clock_m_cycles[0x77];
    LD_HL_r8(&REGISTERS::a);
    break;
  case 0x78:
    printf("%s", disasm[0x78]);
    clock_m += clock_m_cycles[0x78];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::b);
    break;
  case 0x79:
    printf("%s", disasm[0x79]);
    clock_m += clock_m_cycles[0x79];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::c);
    break;
  case 0x7A:
    printf("%s", disasm[0x7a]);
    clock_m += clock_m_cycles[0x7a];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::d);
    break;
  case 0x7B:
    printf("%s", disasm[0x7b]);
    clock_m += clock_m_cycles[0x7b];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::e);
    break;
  case 0x7C:
    printf("%s", disasm[0x7c]);
    clock_m += clock_m_cycles[0x7c];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::h);
    break;
  case 0x7D:
    printf("%s", disasm[0x7d]);
    clock_m += clock_m_cycles[0x7d];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::l);
    break;
  case 0x7E:
    printf("%s", disasm[0x7e]);
    clock_m += clock_m_cycles[0x7e];
    LD_r8_HL(&REGISTERS::a);
    break;
  case 0x7F:
    printf("%s", disasm[0x7f]);
    clock_m += clock_m_cycles[0x7f];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::a);
    break;
  case 0x80:
    printf("%s", disasm[0x80]);
    clock_m += clock_m_cycles[0x80];
    ADD_A_r8(&REGISTERS::b);
    break;
  case 0x81:
    printf("%s", disasm[0x81]);
    clock_m += clock_m_cycles[0x81];
    ADD_A_r8(&REGISTERS::c);
    break;
  case 0x82:
    printf("%s", disasm[0x82]);
    clock_m += clock_m_cycles[0x82];
    ADD_A_r8(&REGISTERS::d);
    break;
  case 0x83:
    printf("%s", disasm[0x83]);
    clock_m += clock_m_cycles[0x83];
    ADD_A_r8(&REGISTERS::e);
    break;
  case 0x84:
    printf("%s", disasm[0x84]);
    clock_m += clock_m_cycles[0x84];
    ADD_A_r8(&REGISTERS::h);
    break;
  case 0x85:
    printf("%s", disasm[0x85]);
    clock_m += clock_m_cycles[0x85];
    ADD_A_r8(&REGISTERS::l);
    break;
  case 0x86:
    printf("%s", disasm[0x86]);
    clock_m += clock_m_cycles[0x86];
    ADD_A_HL();
    break;
  case 0x87:
    printf("%s", disasm[0x87]);
    clock_m += clock_m_cycles[0x87];
    ADD_A_r8(&REGISTERS::a);
    break;
  case 0x88:
    printf("%s", disasm[0x88]);
    clock_m += clock_m_cycles[0x88];
    ADC_A_r8(&REGISTERS::b);
    break;
  case 0x89:
    printf("%s", disasm[0x89]);
    clock_m += clock_m_cycles[0x89];
    ADC_A_r8(&REGISTERS::c);
    break;
  case 0x8A:
    printf("%s", disasm[0x8a]);
    clock_m += clock_m_cycles[0x8a];
    ADC_A_r8(&REGISTERS::d);
    break;
  case 0x8B:
    printf("%s", disasm[0x8b]);
    clock_m += clock_m_cycles[0x8b];
    ADC_A_r8(&REGISTERS::e);
    break;
  case 0x8C:
    printf("%s", disasm[0x8c]);
    clock_m += clock_m_cycles[0x8c];
    ADC_A_r8(&REGISTERS::h);
    break;
  case 0x8D:
    printf("%s", disasm[0x8d]);
    clock_m += clock_m_cycles[0x8d];
    ADC_A_r8(&REGISTERS::l);
    break;
  case 0x8E:
    printf("%s", disasm[0x8e]);
    clock_m += clock_m_cycles[0x8e];
    ADC_A_HL();
    break;
  case 0x8F:
    printf("%s", disasm[0x8f]);
    clock_m += clock_m_cycles[0x8f];
    ADC_A_r8(&REGISTERS::a);
    break;
  case 0x90:
    printf("%s", disasm[0x90]);
    clock_m += clock_m_cycles[0x90];
    SUB_A_r8(&REGISTERS::b);
    break;
  case 0x91:
    printf("%s", disasm[0x91]);
    clock_m += clock_m_cycles[0x91];
    SUB_A_r8(&REGISTERS::c);
    break;
  case 0x92:
    printf("%s", disasm[0x92]);
    clock_m += clock_m_cycles[0x92];
    SUB_A_r8(&REGISTERS::d);
    break;
  case 0x93:
    printf("%s", disasm[0x93]);
    clock_m += clock_m_cycles[0x93];
    SUB_A_r8(&REGISTERS::e);
    break;
  case 0x94:
    printf("%s", disasm[0x94]);
    clock_m += clock_m_cycles[0x94];
    SUB_A_r8(&REGISTERS::h);
    break;
  case 0x95:
    printf("%s", disasm[0x95]);
    clock_m += clock_m_cycles[0x95];
    SUB_A_r8(&REGISTERS::l);
    break;
  case 0x96:
    printf("%s", disasm[0x96]);
    clock_m += clock_m_cycles[0x96];
    SUB_A_HL();
    break;
  case 0x97:
    printf("%s", disasm[0x97]);
    clock_m += clock_m_cycles[0x97];
    SUB_A_r8(&REGISTERS::a);
    break;
  case 0x98:
    printf("%s", disasm[0x98]);
    clock_m += clock_m_cycles[0x98];
    SBC_A_r8(&REGISTERS::b);
    break;
  case 0x99:
    printf("%s", disasm[0x99]);
    clock_m += clock_m_cycles[0x99];
    SBC_A_r8(&REGISTERS::c);
    break;
  case 0x9A:
    printf("%s", disasm[0x9a]);
    clock_m += clock_m_cycles[0x9a];
    SBC_A_r8(&REGISTERS::d);
    break;
  case 0x9B:
    printf("%s", disasm[0x9b]);
    clock_m += clock_m_cycles[0x9b];
    SBC_A_r8(&REGISTERS::e);
    break;
  case 0x9C:
    printf("%s", disasm[0x9c]);
    clock_m += clock_m_cycles[0x9c];
    SBC_A_r8(&REGISTERS::h);
    break;
  case 0x9D:
    printf("%s", disasm[0x9d]);
    clock_m += clock_m_cycles[0x9d];
    SBC_A_r8(&REGISTERS::l);
    break;
  case 0x9E:
    printf("%s", disasm[0x9e]);
    clock_m += clock_m_cycles[0x9e];
    SBC_A_HL();
    break;
  case 0x9F:
    printf("%s", disasm[0x9f]);
    clock_m += clock_m_cycles[0x9f];
    SBC_A_r8(&REGISTERS::a);
    break;
  case 0xa0:
    printf("%s", disasm[0xa0]);
    clock_m += clock_m_cycles[0xa0];
    AND_A_r8(&REGISTERS::b);
    break;
  case 0xa1:
    printf("%s", disasm[0xa1]);
    clock_m += clock_m_cycles[0xa1];
    AND_A_r8(&REGISTERS::c);
    break;
  case 0xa2:
    printf("%s", disasm[0xa2]);
    clock_m += clock_m_cycles[0xa2];
    AND_A_r8(&REGISTERS::d);
    break;
  case 0xa3:
    printf("%s", disasm[0xa3]);
    clock_m += clock_m_cycles[0xa3];
    AND_A_r8(&REGISTERS::e);
    break;
  case 0xa4:
    printf("%s", disasm[0xa4]);
    clock_m += clock_m_cycles[0xa4];
    AND_A_r8(&REGISTERS::h);
    break;
  case 0xa5:
    printf("%s", disasm[0xa5]);
    clock_m += clock_m_cycles[0xa5];
    AND_A_r8(&REGISTERS::l);
    break;
  case 0xa6:
    printf("%s", disasm[0xa6]);
    clock_m += clock_m_cycles[0xa6];
    AND_A_HL();
    break;
  case 0xa7:
    printf("%s", disasm[0xa7]);
    clock_m += clock_m_cycles[0xa7];
    AND_A_r8(&REGISTERS::a);
    break;
  case 0xa8:
    printf("%s", disasm[0xa8]);
    clock_m += clock_m_cycles[0xa8];
    XOR_A_r8(&REGISTERS::b);
    break;
  case 0xa9:
    printf("%s", disasm[0xa9]);
    clock_m += clock_m_cycles[0xa9];
    XOR_A_r8(&REGISTERS::c);
    break;
  case 0xaa:
    printf("%s", disasm[0xaa]);
    clock_m += clock_m_cycles[0xaa];
    XOR_A_r8(&REGISTERS::d);
    break;
  case 0xab:
    printf("%s", disasm[0xab]);
    clock_m += clock_m_cycles[0xab];
    XOR_A_r8(&REGISTERS::e);
    break;
  case 0xac:
    printf("%s", disasm[0xac]);
    clock_m += clock_m_cycles[0xac];
    XOR_A_r8(&REGISTERS::h);
    break;
  case 0xad:
    printf("%s", disasm[0xad]);
    clock_m += clock_m_cycles[0xad];
    XOR_A_r8(&REGISTERS::l);
    break;
  case 0xae:
    printf("%s", disasm[0xae]);
    clock_m += clock_m_cycles[0xae];
    XOR_A_HL();
    break;
  case 0xaf:
    printf("%s", disasm[0xaf]);
    clock_m += clock_m_cycles[0xaf];
    XOR_A_r8(&REGISTERS::a);
    break;
  case 0xb0:
    printf("%s", disasm[0xb0]);
    clock_m += clock_m_cycles[0xb0];
    OR_A_r8(&REGISTERS::b);
    break;
  case 0xb1:
    printf("%s", disasm[0xb1]);
    clock_m += clock_m_cycles[0xb1];
    OR_A_r8(&REGISTERS::c);
    break;
  case 0xb2:
    printf("%s", disasm[0xb2]);
    clock_m += clock_m_cycles[0xb2];
    OR_A_r8(&REGISTERS::d);
    break;
  case 0xb3:
    printf("%s", disasm[0xb3]);
    clock_m += clock_m_cycles[0xb3];
    OR_A_r8(&REGISTERS::e);
    break;
  case 0xb4:
    printf("%s", disasm[0xb4]);
    clock_m += clock_m_cycles[0xb4];
    OR_A_r8(&REGISTERS::h);
    break;
  case 0xb5:
    printf("%s", disasm[0xb5]);
    clock_m += clock_m_cycles[0xb5];
    OR_A_r8(&REGISTERS::l);
    break;
  case 0xb6:
    printf("%s", disasm[0xb6]);
    clock_m += clock_m_cycles[0xb6];
    OR_A_HL();
    break;
  case 0xb7:
    printf("%s", disasm[0xb7]);
    clock_m += clock_m_cycles[0xb7];
    OR_A_r8(&REGISTERS::a);
    break;
  case 0xb8:
    printf("%s", disasm[0xb8]);
    clock_m += clock_m_cycles[0xb8];
    CP_A_r8(&REGISTERS::b);
    break;
  case 0xb9:
    printf("%s", disasm[0xb9]);
    clock_m += clock_m_cycles[0xb9];
    CP_A_r8(&REGISTERS::c);
    break;
  case 0xbA:
    printf("%s", disasm[0xba]);
    clock_m += clock_m_cycles[0xba];
    CP_A_r8(&REGISTERS::d);
    break;
  case 0xbB:
    printf("%s", disasm[0xbb]);
    clock_m += clock_m_cycles[0xbb];
    CP_A_r8(&REGISTERS::e);
    break;
  case 0xbC:
    printf("%s", disasm[0xbc]);
    clock_m += clock_m_cycles[0xbc];
    CP_A_r8(&REGISTERS::h);
    break;
  case 0xbD:
    printf("%s", disasm[0xbd]);
    clock_m += clock_m_cycles[0xbd];
    CP_A_r8(&REGISTERS::l);
    break;
  case 0xbE:
    printf("%s", disasm[0xbe]);
    clock_m += clock_m_cycles[0xbe];
    CP_A_HL();
    break;
  case 0xbF:
    printf("%s", disasm[0xbf]);
    clock_m += clock_m_cycles[0xbf];
    CP_A_r8(&REGISTERS::a);
    break;
  case 0xc0:
    printf("%s", disasm[0xc0]);
    clock_m += clock_m_cycles[0xc0];
    RET_CC(flag_value(N) & flag_value(Z));
    break;
  case 0xc1:
    printf("%s", disasm[0xc1]);
    clock_m += clock_m_cycles[0xc1];
    POP_r16(&REGISTERS::bc);
    break;
  case 0xc2: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0xc2], val);
    clock_m += clock_m_cycles[0xc2];
    JP_CC_n16(flag_value(N) & flag_value(Z), val);
  } break;
  case 0xc3: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0xc3], val);
    clock_m += clock_m_cycles[0xc3];
    JP_n16(val);
  } break;
  case 0xc4: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0xc4], val);
    clock_m += clock_m_cycles[0xc4];
    CALL_CC_n16(flag_value(N) & flag_value(Z), val);
  } break;
  case 0xc5:
    printf("%s", disasm[0xc5]);
    clock_m += clock_m_cycles[0xc5];
    PUSH_r16(&REGISTERS::bc);
    break;
  case 0xc6: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0xc6], val);
    clock_m += clock_m_cycles[0xc6];
    ADD_A_n8(val);
  } break;
  case 0xc7:
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  case 0xc8:
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  case 0xc9:
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  case 0xcA:
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  case 0xcb:
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  case 0xcc:
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  case 0xcd:
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  case 0xce: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0xce], val);
    clock_m += clock_m_cycles[0xce];
    ADC_A_n8(val);
  } break;
  case 0xcF:
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  case 0xe0: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0xe0], val);
    clock_m += clock_m_cycles[0xe0];
    LDH_n16_A(val);
  } break;
  case 0xFF:
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  default:
    printf("ERROR: DEFAULT 0x%02X\n", reg.pc);
    printf("ERROR 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  }
}
