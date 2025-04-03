#include "../include/cpu.h"
#include <cstdint>
#include <cstdio>
#include <ctime>

const char *disasm[256] = {
    "0x%04x: [00] NOP\n",                      // 0x00
    "0x%04x: [01] LD BC, 0x%04X\n",            // 0x01
    "0x%04x: [02] LD (BC), A\n",               // 0x02
    "0x%04x: [03] INC BC\n",                   // 0x03
    "0x%04x: [04] INC B\n",                    // 0x04
    "0x%04x: [05] DEC B\n",                    // 0x05
    "0x%04x: [06] LD B, 0x%02X\n",             // 0x06
    "0x%04x: [07] RLCA\n",                     // 0x07
    "0x%04x: [08] LD (0x%04X), SP\n",          // 0x08
    "0x%04x: [09] ADD HL, BC\n",               // 0x09
    "0x%04x: [0a] LD A, (BC)\n",               // 0x0a
    "0x%04x: [0b] DEC BC\n",                   // 0x0b
    "0x%04x: [0c] INC C\n",                    // 0x0c
    "0x%04x: [0d] DEC C\n",                    // 0x0d
    "0x%04x: [0e] LD C, 0x%02X\n",             // 0x0e
    "0x%04x: [0f] RRCA\n",                     // 0x0f
    "0x%04x: [10] STOP\n",                     // 0x10
    "0x%04x: [11] LD DE, 0x%04X\n",            // 0x11
    "0x%04x: [12] LD (DE), A\n",               // 0x12
    "0x%04x: [13] INC DE\n",                   // 0x13
    "0x%04x: [14] INC D\n",                    // 0x14
    "0x%04x: [15] DEC D\n",                    // 0x15
    "0x%04x: [16] LD D, 0x%02X\n",             // 0x16
    "0x%04x: [17] RLA\n",                      // 0x17
    "0x%04x: [18] JR 0x%02X\n",                // 0x18
    "0x%04x: [19] ADD HL, DE\n",               // 0x19
    "0x%04x: [1a] LD A, (DE)\n",               // 0x1a
    "0x%04x: [1b] DEC DE\n",                   // 0x1b
    "0x%04x: [1c] INC E\n",                    // 0x1c
    "0x%04x: [1d] DEC E\n",                    // 0x1d
    "0x%04x: [1e] LD E, 0x%02X\n",             // 0x1e
    "0x%04x: [1f] RRA\n",                      // 0x1f
    "0x%04x: [20] JR NZ, 0x%02X\n",            // 0x20
    "0x%04x: [21] LD HL, 0x%04X\n",            // 0x21
    "0x%04x: [22] LDI (HL), A\n",              // 0x22
    "0x%04x: [23] INC HL\n",                   // 0x23
    "0x%04x: [24] INC H\n",                    // 0x24
    "0x%04x: [25] DEC H\n",                    // 0x25
    "0x%04x: [26] LD H, 0x%02X\n",             // 0x26
    "0x%04x: [27] DAA\n",                      // 0x27
    "0x%04x: [28] JR Z, 0x%02X\n",             // 0x28
    "0x%04x: [29] ADD HL, HL\n",               // 0x29
    "0x%04x: [2a] LDI A, (HL)\n",              // 0x2a
    "0x%04x: [2b] DEC HL\n",                   // 0x2b
    "0x%04x: [2c] INC L\n",                    // 0x2c
    "0x%04x: [2d] DEC L\n",                    // 0x2d
    "0x%04x: [2e] LD L, 0x%02X\n",             // 0x2e
    "0x%04x: [2f] CPL\n",                      // 0x2f
    "0x%04x: [30] JR NC, 0x%02X\n",            // 0x30
    "0x%04x: [31] LD SP, 0x%04X\n",            // 0x31
    "0x%04x: [32] LDD (HL), A\n",              // 0x32
    "0x%04x: [33] INC SP\n",                   // 0x33
    "0x%04x: [34] INC (HL)\n",                 // 0x34
    "0x%04x: [35] DEC (HL)\n",                 // 0x35
    "0x%04x: [36] LD (HL), 0x%02X\n",          // 0x36
    "0x%04x: [37] SCF\n",                      // 0x37
    "0x%04x: [38] JR C, 0x%02X\n",             // 0x38
    "0x%04x: [39] ADD HL, SP\n",               // 0x39
    "0x%04x: [3a] LDD A, (HL)\n",              // 0x3a
    "0x%04x: [3b] DEC SP\n",                   // 0x3b
    "0x%04x: [3c] INC A\n",                    // 0x3c
    "0x%04x: [3d] DEC A\n",                    // 0x3d
    "0x%04x: [3e] LD A, 0x%02X\n",             // 0x3e
    "0x%04x: [3f] CCF\n",                      // 0x3f
    "0x%04x: [40] LD B, B\n",                  // 0x40
    "0x%04x: [41] LD B, C\n",                  // 0x41
    "0x%04x: [42] LD B, D\n",                  // 0x42
    "0x%04x: [43] LD B, E\n",                  // 0x43
    "0x%04x: [44] LD B, H\n",                  // 0x44
    "0x%04x: [45] LD B, L\n",                  // 0x45
    "0x%04x: [46] LD B, (HL)\n",               // 0x46
    "0x%04x: [47] LD B, A\n",                  // 0x47
    "0x%04x: [48] LD C, B\n",                  // 0x48
    "0x%04x: [49] LD C, C\n",                  // 0x49
    "0x%04x: [4a] LD C, D\n",                  // 0x4a
    "0x%04x: [4b] LD C, E\n",                  // 0x4b
    "0x%04x: [4c] LD C, H\n",                  // 0x4c
    "0x%04x: [4d] LD C, L\n",                  // 0x4d
    "0x%04x: [4e] LD C, (HL)\n",               // 0x4e
    "0x%04x: [4f] LD C, A\n",                  // 0x4f
    "0x%04x: [50] LD D, B\n",                  // 0x50
    "0x%04x: [51] LD D, C\n",                  // 0x51
    "0x%04x: [52] LD D, D\n",                  // 0x52
    "0x%04x: [53] LD D, E\n",                  // 0x53
    "0x%04x: [54] LD D, H\n",                  // 0x54
    "0x%04x: [55] LD D, L\n",                  // 0x55
    "0x%04x: [56] LD D, (HL)\n",               // 0x56
    "0x%04x: [57] LD D, A\n",                  // 0x57
    "0x%04x: [58] LD E, B\n",                  // 0x58
    "0x%04x: [59] LD E, C\n",                  // 0x59
    "0x%04x: [5a] LD E, D\n",                  // 0x5a
    "0x%04x: [5b] LD E, E\n",                  // 0x5b
    "0x%04x: [5c] LD E, H\n",                  // 0x5c
    "0x%04x: [5d] LD E, L\n",                  // 0x5d
    "0x%04x: [5e] LD E, (HL)\n",               // 0x5e
    "0x%04x: [5f] LD E, A\n",                  // 0x5f
    "0x%04x: [60] LD H, B\n",                  // 0x60
    "0x%04x: [61] LD H, C\n",                  // 0x61
    "0x%04x: [62] LD H, D\n",                  // 0x62
    "0x%04x: [63] LD H, E\n",                  // 0x63
    "0x%04x: [64] LD H, H\n",                  // 0x64
    "0x%04x: [65] LD H, L\n",                  // 0x65
    "0x%04x: [66] LD H, (HL)\n",               // 0x66
    "0x%04x: [67] LD H, A\n",                  // 0x67
    "0x%04x: [68] LD L, B\n",                  // 0x68
    "0x%04x: [69] LD L, C\n",                  // 0x69
    "0x%04x: [6a] LD L, D\n",                  // 0x6a
    "0x%04x: [6b] LD L, E\n",                  // 0x6b
    "0x%04x: [6c] LD L, H\n",                  // 0x6c
    "0x%04x: [6d] LD L, L\n",                  // 0x6d
    "0x%04x: [6e] LD L, (HL)\n",               // 0x6e
    "0x%04x: [6f] LD L, A\n",                  // 0x6f
    "0x%04x: [70] LD (HL), B\n",               // 0x70
    "0x%04x: [71] LD (HL), C\n",               // 0x71
    "0x%04x: [72] LD (HL), D\n",               // 0x72
    "0x%04x: [73] LD (HL), E\n",               // 0x73
    "0x%04x: [74] LD (HL), H\n",               // 0x74
    "0x%04x: [75] LD (HL), L\n",               // 0x75
    "0x%04x: [76] HALT\n",                     // 0x76
    "0x%04x: [77] LD (HL), A\n",               // 0x77
    "0x%04x: [78] LD A, B\n",                  // 0x78
    "0x%04x: [79] LD A, C\n",                  // 0x79
    "0x%04x: [7a] LD A, D\n",                  // 0x7a
    "0x%04x: [7b] LD A, E\n",                  // 0x7b
    "0x%04x: [7c] LD A, H\n",                  // 0x7c
    "0x%04x: [7d] LD A, L\n",                  // 0x7d
    "0x%04x: [7e] LD A, (HL)\n",               // 0x7e
    "0x%04x: [7f] LD A, A\n",                  // 0x7f
    "0x%04x: [80] ADD A, B\n",                 // 0x80
    "0x%04x: [81] ADD A, C\n",                 // 0x81
    "0x%04x: [82] ADD A, D\n",                 // 0x82
    "0x%04x: [83] ADD A, E\n",                 // 0x83
    "0x%04x: [84] ADD A, H\n",                 // 0x84
    "0x%04x: [85] ADD A, L\n",                 // 0x85
    "0x%04x: [86] ADD A, (HL)\n",              // 0x86
    "0x%04x: [87] ADD A\n",                    // 0x87
    "0x%04x: [88] ADC B\n",                    // 0x88
    "0x%04x: [89] ADC C\n",                    // 0x89
    "0x%04x: [8a] ADC D\n",                    // 0x8a
    "0x%04x: [8b] ADC E\n",                    // 0x8b
    "0x%04x: [8c] ADC H\n",                    // 0x8c
    "0x%04x: [8d] ADC L\n",                    // 0x8d
    "0x%04x: [8e] ADC (HL)\n",                 // 0x8e
    "0x%04x: [8f] ADC A\n",                    // 0x8f
    "0x%04x: [90] SUB B\n",                    // 0x90
    "0x%04x: [91] SUB C\n",                    // 0x91
    "0x%04x: [92] SUB D\n",                    // 0x92
    "0x%04x: [93] SUB E\n",                    // 0x93
    "0x%04x: [94] SUB H\n",                    // 0x94
    "0x%04x: [95] SUB L\n",                    // 0x95
    "0x%04x: [96] SUB (HL)\n",                 // 0x96
    "0x%04x: [97] SUB A\n",                    // 0x97
    "0x%04x: [98] SBC B\n",                    // 0x98
    "0x%04x: [99] SBC C\n",                    // 0x99
    "0x%04x: [9a] SBC D\n",                    // 0x9a
    "0x%04x: [9b] SBC E\n",                    // 0x9b
    "0x%04x: [9c] SBC H\n",                    // 0x9c
    "0x%04x: [9d] SBC L\n",                    // 0x9d
    "0x%04x: [9e] SBC (HL)\n",                 // 0x9e
    "0x%04x: [9f] SBC A\n",                    // 0x9f
    "0x%04x: [a0] AND B\n",                    // 0xa0
    "0x%04x: [a1] AND C\n",                    // 0xa1
    "0x%04x: [a2] AND D\n",                    // 0xa2
    "0x%04x: [a3] AND E\n",                    // 0xa3
    "0x%04x: [a4] AND H\n",                    // 0xa4
    "0x%04x: [a5] AND L\n",                    // 0xa5
    "0x%04x: [a6] AND (HL)\n",                 // 0xa6
    "0x%04x: [a7] AND A\n",                    // 0xa7
    "0x%04x: [a8] XOR B\n",                    // 0xa8
    "0x%04x: [a9] XOR C\n",                    // 0xa9
    "0x%04x: [aa] XOR D\n",                    // 0xaa
    "0x%04x: [ab] XOR E\n",                    // 0xab
    "0x%04x: [ac] XOR H\n",                    // 0xac
    "0x%04x: [ad] XOR L\n",                    // 0xad
    "0x%04x: [ae] XOR (HL)\n",                 // 0xae
    "0x%04x: [af] XOR A\n",                    // 0xaf
    "0x%04x: [b0] OR B\n",                     // 0xb0
    "0x%04x: [b1] OR C\n",                     // 0xb1
    "0x%04x: [b2] OR D\n",                     // 0xb2
    "0x%04x: [b3] OR E\n",                     // 0xb3
    "0x%04x: [b4] OR H\n",                     // 0xb4
    "0x%04x: [b5] OR L\n",                     // 0xb5
    "0x%04x: [b6] OR (HL)\n",                  // 0xb6
    "0x%04x: [b7] OR A\n",                     // 0xb7
    "0x%04x: [b8] CP B\n",                     // 0xb8
    "0x%04x: [b9] CP C\n",                     // 0xb9
    "0x%04x: [ba] CP D\n",                     // 0xba
    "0x%04x: [bb] CP E\n",                     // 0xbb
    "0x%04x: [bc] CP H\n",                     // 0xbc
    "0x%04x: [bd] CP L\n",                     // 0xbd
    "0x%04x: [be] CP (HL)\n",                  // 0xbe
    "0x%04x: [bf] CP A\n",                     // 0xbf
    "0x%04x: [c0] RET NZ\n",                   // 0xc0
    "0x%04x: [c1] POP BC\n",                   // 0xc1
    "0x%04x: [c2] JP NZ, 0x%04X\n",            // 0xc2
    "0x%04x: [c3] JP 0x%04X\n",                // 0xc3
    "0x%04x: [c4] CALL NZ, 0x%04X\n",          // 0xc4
    "0x%04x: [c5] PUSH BC\n",                  // 0xc5
    "0x%04x: [c6] ADD A, 0x%02X\n",            // 0xc6
    "0x%04x: [c7] RST 0x00\n",                 // 0xc7
    "0x%04x: [c8] RET Z\n",                    // 0xc8
    "0x%04x: [c9] RET\n",                      // 0xc9
    "0x%04x: [ca] JP Z, 0x%04X\n",             // 0xca
    "0x%04x: [cb] CB %02X\n",                  // 0xcb
    "0x%04x: [cc] CALL Z, 0x%04X\n",           // 0xcc
    "0x%04x: [cd] CALL 0x%04X\n",              // 0xcd
    "0x%04x: [ce] ADC 0x%02X\n",               // 0xce
    "0x%04x: [cf] RST 0x08\n",                 // 0xcf
    "0x%04x: [d0] RET NC\n",                   // 0xd0
    "0x%04x: [d1] POP DE\n",                   // 0xd1
    "0x%04x: [d2] JP NC, 0x%04X\n",            // 0xd2
    "0x%04x: [d3] UNKNOWN\n",                  // 0xd3
    "0x%04x: [d4] CALL NC, 0x%04X\n",          // 0xd4
    "0x%04x: [d5] PUSH DE\n",                  // 0xd5
    "0x%04x: [d6] SUB 0x%02X\n",               // 0xd6
    "0x%04x: [d7] RST 0x10\n",                 // 0xd7
    "0x%04x: [d8] RET C\n",                    // 0xd8
    "0x%04x: [d9] RETI\n",                     // 0xd9
    "0x%04x: [da] JP C, 0x%04X\n",             // 0xda
    "0x%04x: [db] UNKNOWN\n",                  // 0xdb
    "0x%04x: [dc] CALL C, 0x%04X\n",           // 0xdc
    "0x%04x: [dd] UNKNOWN\n",                  // 0xdd
    "0x%04x: [de] SBC 0x%02X\n",               // 0xde
    "0x%04x: [df] RST 0x18\n",                 // 0xdf
    "0x%04x: [e0] LDH (0xFF00 + 0x%02X), A\n", // 0xe0
    "0x%04x: [e1] POP HL\n",                   // 0xe1
    "0x%04x: [e2] LDH (0xFF00 + C), A\n",      // 0xe2
    "0x%04x: [e3] UNKNOWN\n",                  // 0xe3
    "0x%04x: [e4] UNKNOWN\n",                  // 0xe4
    "0x%04x: [e5] PUSH HL\n",                  // 0xe5
    "0x%04x: [e6] AND 0x%02X\n",               // 0xe6
    "0x%04x: [e7] RST 0x20\n",                 // 0xe7
    "0x%04x: [e8] ADD SP,0x%02X\n",            // 0xe8
    "0x%04x: [e9] JP HL\n",                    // 0xe9
    "0x%04x: [ea] LD (0x%04X), A\n",           // 0xea
    "0x%04x: [eb] UNKNOWN\n",                  // 0xeb
    "0x%04x: [ec] UNKNOWN\n",                  // 0xec
    "0x%04x: [ed] UNKNOWN\n",                  // 0xed
    "0x%04x: [ee] XOR 0x%02X\n",               // 0xee
    "0x%04x: [ef] RST 0x28\n",                 // 0xef
    "0x%04x: [f0] LDH A, (0xFF00 + 0x%02X)\n", // 0xf0
    "0x%04x: [f1] POP AF\n",                   // 0xf1
    "0x%04x: [f2] LDH A, (0xFF00 + C)\n",      // 0xf2
    "0x%04x: [f3] DI\n",                       // 0xf3
    "0x%04x: [f4] UNKNOWN\n",                  // 0xf4
    "0x%04x: [f5] PUSH AF\n",                  // 0xf5
    "0x%04x: [f6] OR 0x%02X\n",                // 0xf6
    "0x%04x: [f7] RST 0x30\n",                 // 0xf7
    "0x%04x: [f8] LD HL, SP+0x%02X\n",         // 0xf8
    "0x%04x: [f9] LD SP, HL\n",                // 0xf9
    "0x%04x: [fa] LD A, (0x%04X)\n",           // 0xfa
    "0x%04x: [fb] EI\n",                       // 0xfb
    "0x%04x: [fc] UNKNOWN\n",                  // 0xfc
    "0x%04x: [fd] UNKNOWN\n",                  // 0xfd
    "0x%04x: [fe] CP 0x%02X\n",                // 0xfe
    "0x%04x: [ff] RST 0x38\n",                 // 0xff
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

  switch (read_byte(reg.pc++)) {
  case 0x00:
    printf(disasm[0x00], curr_pc);
    clock_m += clock_m_cycles[0x00];
    NOP();
    break;
  case 0x01: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0x01], curr_pc, val);
    clock_m += clock_m_cycles[0x01];
    LD_r16_n16(&REGISTERS::bc, val);
  } break;
  case 0x02:
    printf(disasm[0x02], curr_pc);
    clock_m += clock_m_cycles[0x02];
    LD_r16_r8(&REGISTERS::bc, &REGISTERS::a);
    break;
  case 0x03:
    printf(disasm[0x03], curr_pc);
    clock_m += clock_m_cycles[0x03];
    INC_r16(&REGISTERS::bc);
    break;
  case 0x04:
    printf(disasm[0x04], curr_pc);
    clock_m += clock_m_cycles[0x04];
    INC_r8(&REGISTERS::b);
    break;
  case 0x05:
    printf(disasm[0x05], curr_pc);
    clock_m += clock_m_cycles[0x05];
    DEC_r8(&REGISTERS::b);
    break;
  case 0x06: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x06], curr_pc, val);
    clock_m += clock_m_cycles[0x06];
    LD_r8_n8(&REGISTERS::b, val);
  } break;
  case 0x07:
    printf(disasm[0x07], curr_pc);
    clock_m += clock_m_cycles[0x07];
    RLCA();
    break;
  case 0x08: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0x08], curr_pc, val);
    clock_m += clock_m_cycles[0x08];
    LD_n16_SP(val);
  } break;
  case 0x09:
    printf(disasm[0x09], curr_pc);
    clock_m += clock_m_cycles[0x09];
    ADD_HL_r16(&REGISTERS::bc);
    break;
  case 0x0A:
    printf(disasm[0x0a], curr_pc);
    clock_m += clock_m_cycles[0x0a];
    LD_r8_r16(&REGISTERS::a, &REGISTERS::bc);
    break;
  case 0x0B:
    printf(disasm[0x0b], curr_pc);
    clock_m += clock_m_cycles[0x0b];
    DEC_r16(&REGISTERS::bc);
    break;
  case 0x0C:
    printf(disasm[0x0c], curr_pc);
    clock_m += clock_m_cycles[0x0c];
    INC_r8(&REGISTERS::c);
    break;
  case 0x0D:
    printf(disasm[0x0d], curr_pc);
    clock_m += clock_m_cycles[0x0d];
    DEC_r8(&REGISTERS::c);
    break;
  case 0x0E: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x0e], curr_pc, val);
    clock_m += clock_m_cycles[0x0e];
    LD_r8_n8(&REGISTERS::c, val);
  } break;
  case 0x0F:
    printf(disasm[0x0F], curr_pc);
    clock_m += clock_m_cycles[0x0F];
    RRCA();
    break;
  case 0x10:
    printf(disasm[0x10], curr_pc);
    clock_m += clock_m_cycles[0x10];
    // printf("ERROR 0x10 0x%04X\n", read_byte(reg.pc));
    exit(1);
    break;
  case 0x11: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0x11], curr_pc, val);
    clock_m += clock_m_cycles[0x11];
    LD_r16_n16(&REGISTERS::de, val);
  } break;
  case 0x12:
    printf(disasm[0x12], curr_pc);
    clock_m += clock_m_cycles[0x12];
    LD_r16_r8(&REGISTERS::de, &REGISTERS::a);
    break;
  case 0x13:
    printf(disasm[0x13], curr_pc);
    clock_m += clock_m_cycles[0x13];
    INC_r16(&REGISTERS::de);
    break;
  case 0x14:
    printf(disasm[0x14], curr_pc);
    clock_m += clock_m_cycles[0x14];
    INC_r8(&REGISTERS::d);
    break;
  case 0x15:
    printf(disasm[0x15], curr_pc);
    clock_m += clock_m_cycles[0x15];
    DEC_r8(&REGISTERS::d);
    break;
  case 0x16: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x16], curr_pc, val);
    clock_m += clock_m_cycles[0x16];
    LD_r8_n8(&REGISTERS::d, val);
  } break;
  case 0x17:
    break;
  case 0x18: {
    int val = read_byte(reg.pc++);
    printf(disasm[0x18], curr_pc, val);
    clock_m += clock_m_cycles[0x18];
    JR_n16(val);
  } break;
  case 0x19:
    printf(disasm[0x19], curr_pc);
    clock_m += clock_m_cycles[0x19];
    ADD_HL_r16(&REGISTERS::de);
    break;
  case 0x1A:
    printf(disasm[0x1a], curr_pc);
    clock_m += clock_m_cycles[0x1a];
    LD_r8_r16(&REGISTERS::a, &REGISTERS::de);
    break;
  case 0x1B:
    printf(disasm[0x1b], curr_pc);
    clock_m += clock_m_cycles[0x1b];
    DEC_r16(&REGISTERS::de);
    break;
  case 0x1C:
    printf(disasm[0x1c], curr_pc);
    clock_m += clock_m_cycles[0x1c];
    INC_r8(&REGISTERS::e);
    break;
  case 0x1D:
    printf(disasm[0x1d], curr_pc);
    clock_m += clock_m_cycles[0x1d];
    DEC_r8(&REGISTERS::e);
    break;
  case 0x1E: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x1e], curr_pc, val);
    clock_m += clock_m_cycles[0x1e];
    LD_r8_n8(&REGISTERS::e, val);
  } break;
  case 0x1F:
    break;
  case 0x20: {
    int val = read_byte(reg.pc++);
    printf(disasm[0x20], curr_pc, val);
    clock_m += clock_m_cycles[0x20];
    JR_CC_n16(flag_value(N) & flag_value(Z), val);
  } break;
  case 0x21: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0x21], curr_pc, val);
    clock_m += clock_m_cycles[0x21];
    LD_r16_n16(&REGISTERS::hl, val);
  } break;
  case 0x22:
    printf(disasm[0x22], curr_pc);
    clock_m += clock_m_cycles[0x22];
    LD_r16_r8(&REGISTERS::hl, &REGISTERS::a);
    break;
  case 0x23:
    printf(disasm[0x23], curr_pc);
    clock_m += clock_m_cycles[0x23];
    INC_r16(&REGISTERS::hl);
    break;
  case 0x24:
    printf(disasm[0x24], curr_pc);
    clock_m += clock_m_cycles[0x24];
    INC_r8(&REGISTERS::h);
    break;
  case 0x25:
    printf(disasm[0x25], curr_pc);
    clock_m += clock_m_cycles[0x25];
    DEC_r8(&REGISTERS::h);
    break;
  case 0x26: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x26], curr_pc, val);
    clock_m += clock_m_cycles[0x26];
    LD_r8_n8(&REGISTERS::h, val);
  } break;
  case 0x27: {
    printf(disasm[0x27], curr_pc);
    clock_m += clock_m_cycles[0x27];
    DAA();
  } break;
  case 0x28: {
    int val = read_byte(reg.pc++);
    printf(disasm[0x28], curr_pc, val);
    clock_m += clock_m_cycles[0x28];
    JR_CC_n16(flag_value(Z), val);
  } break;
  case 0x29:
    printf(disasm[0x29], curr_pc);
    clock_m += clock_m_cycles[0x29];
    ADD_HL_r16(&REGISTERS::hl);
    break;
  case 0x2A:
    printf(disasm[0x2a], curr_pc);
    clock_m += clock_m_cycles[0x2a];
    LD_r8_r16(&REGISTERS::a, &REGISTERS::hl);
    break;
  case 0x2B:
    printf(disasm[0x2b], curr_pc);
    clock_m += clock_m_cycles[0x2b];
    DEC_r16(&REGISTERS::hl);
    break;
  case 0x2C:
    printf(disasm[0x2c], curr_pc);
    clock_m += clock_m_cycles[0x2c];
    INC_r8(&REGISTERS::l);
    break;
  case 0x2D:
    printf(disasm[0x2d], curr_pc);
    clock_m += clock_m_cycles[0x2d];
    DEC_r8(&REGISTERS::l);
    break;
  case 0x2E: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x2e], curr_pc, val);
    clock_m += clock_m_cycles[0x2e];
    LD_r8_n8(&REGISTERS::l, val);
  } break;
  case 0x2F: {
    printf(disasm[0x2f], curr_pc);
    clock_m += clock_m_cycles[0x2f];
    CPL();
  } break;
  case 0x30: {
    int val = read_byte(reg.pc++);
    printf(disasm[0x30], curr_pc, val);
    clock_m += clock_m_cycles[0x30];
    JR_CC_n16(flag_value(N) & flag_value(C), val);
  } break;
  case 0x31: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0x31], curr_pc, val);
    clock_m += clock_m_cycles[0x31];
    LD_SP_n16(val);
  } break;
  case 0x32:
    printf(disasm[0x32], curr_pc);
    clock_m += clock_m_cycles[0x32];
    LD_HLD_A();
    break;
  case 0x33:
    printf(disasm[0x33], curr_pc);
    clock_m += clock_m_cycles[0x33];
    INC_SP();
    break;
  case 0x34:
    printf(disasm[0x34], curr_pc);
    clock_m += clock_m_cycles[0x34];
    INC_HL();
    break;
  case 0x35:
    printf(disasm[0x35], curr_pc);
    clock_m += clock_m_cycles[0x35];
    DEC_HL();
    break;
  case 0x36: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x36], curr_pc, val);
    clock_m += clock_m_cycles[0x36];
    LD_HL_n8(val);
  } break;
  case 0x37: {
  } break;
  case 0x38: {
    int val = read_byte(reg.pc++);
    printf(disasm[0x38], curr_pc, val);
    clock_m += clock_m_cycles[0x38];
    JR_CC_n16(flag_value(C), val);
  } break;
  case 0x39:
    printf(disasm[0x39], curr_pc);
    clock_m += clock_m_cycles[0x39];
    ADD_HL_SP();
    break;
  case 0x3A:
    printf(disasm[0x3a], curr_pc);
    clock_m += clock_m_cycles[0x3a];
    LD_A_HLD();
    break;
  case 0x3B:
    printf(disasm[0x3b], curr_pc);
    clock_m += clock_m_cycles[0x3b];
    DEC_SP();
    break;
  case 0x3C:
    printf(disasm[0x3c], curr_pc);
    clock_m += clock_m_cycles[0x3c];
    INC_r8(&REGISTERS::a);
    break;
  case 0x3D:
    printf(disasm[0x3d], curr_pc);
    clock_m += clock_m_cycles[0x3d];
    DEC_r8(&REGISTERS::a);
    break;
  case 0x3E: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0x3e], curr_pc, val);
    clock_m += clock_m_cycles[0x3e];
    LD_r8_n8(&REGISTERS::a, val);
  } break;
  case 0x3F: {
    printf(disasm[0x3f], curr_pc);
    clock_m += clock_m_cycles[0x3f];
    CCF();
  } break;
  case 0x40:
    printf(disasm[0x40], curr_pc);
    clock_m += clock_m_cycles[0x40];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::b);
    break;
  case 0x41:
    printf(disasm[0x41], curr_pc);
    clock_m += clock_m_cycles[0x41];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::c);
    break;
  case 0x42:
    printf(disasm[0x42], curr_pc);
    clock_m += clock_m_cycles[0x42];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::d);
    break;
  case 0x43:
    printf(disasm[0x43], curr_pc);
    clock_m += clock_m_cycles[0x43];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::e);
    break;
  case 0x44:
    printf(disasm[0x44], curr_pc);
    clock_m += clock_m_cycles[0x44];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::h);
    break;
  case 0x45:
    printf(disasm[0x45], curr_pc);
    clock_m += clock_m_cycles[0x45];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::l);
    break;
  case 0x46:
    printf(disasm[0x46], curr_pc);
    clock_m += clock_m_cycles[0x46];
    LD_r8_HL(&REGISTERS::b);
    break;
  case 0x47:
    printf(disasm[0x47], curr_pc);
    clock_m += clock_m_cycles[0x47];
    LD_r8_r8(&REGISTERS::b, &REGISTERS::a);
    break;
  case 0x48:
    printf(disasm[0x48], curr_pc);
    clock_m += clock_m_cycles[0x48];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::b);
    break;
  case 0x49:
    printf(disasm[0x49], curr_pc);
    clock_m += clock_m_cycles[0x49];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::c);
    break;
  case 0x4A:
    printf(disasm[0x4a], curr_pc);
    clock_m += clock_m_cycles[0x4a];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::d);
    break;
  case 0x4B:
    printf(disasm[0x4b], curr_pc);
    clock_m += clock_m_cycles[0x4b];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::e);
    break;
  case 0x4C:
    printf(disasm[0x4c], curr_pc);
    clock_m += clock_m_cycles[0x4c];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::h);
    break;
  case 0x4D:
    printf(disasm[0x4d], curr_pc);
    clock_m += clock_m_cycles[0x4d];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::l);
    break;
  case 0x4E:
    printf(disasm[0x4e], curr_pc);
    clock_m += clock_m_cycles[0x4e];
    LD_r8_HL(&REGISTERS::c);
    break;
  case 0x4F:
    printf(disasm[0x4f], curr_pc);
    clock_m += clock_m_cycles[0x4f];
    LD_r8_r8(&REGISTERS::c, &REGISTERS::a);
    break;
  case 0x50:
    printf(disasm[0x50], curr_pc);
    clock_m += clock_m_cycles[0x50];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::b);
    break;
  case 0x51:
    printf(disasm[0x51], curr_pc);
    clock_m += clock_m_cycles[0x51];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::c);
    break;
  case 0x52:
    printf(disasm[0x52], curr_pc);
    clock_m += clock_m_cycles[0x52];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::d);
    break;
  case 0x53:
    printf(disasm[0x53], curr_pc);
    clock_m += clock_m_cycles[0x53];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::e);
    break;
  case 0x54:
    printf(disasm[0x54], curr_pc);
    clock_m += clock_m_cycles[0x54];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::h);
    break;
  case 0x55:
    printf(disasm[0x55], curr_pc);
    clock_m += clock_m_cycles[0x55];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::l);
    break;
  case 0x56:
    printf(disasm[0x56], curr_pc);
    clock_m += clock_m_cycles[0x56];
    LD_r8_HL(&REGISTERS::d);
    break;
  case 0x57:
    printf(disasm[0x57], curr_pc);
    clock_m += clock_m_cycles[0x57];
    LD_r8_r8(&REGISTERS::d, &REGISTERS::a);
    break;
  case 0x58:
    printf(disasm[0x58], curr_pc);
    clock_m += clock_m_cycles[0x58];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::b);
    break;
  case 0x59:
    printf(disasm[0x59], curr_pc);
    clock_m += clock_m_cycles[0x59];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::c);
    break;
  case 0x5A:
    printf(disasm[0x5a], curr_pc);
    clock_m += clock_m_cycles[0x5a];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::d);
    break;
  case 0x5B:
    printf(disasm[0x5b], curr_pc);
    clock_m += clock_m_cycles[0x5b];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::e);
    break;
  case 0x5C:
    printf(disasm[0x5c], curr_pc);
    clock_m += clock_m_cycles[0x5c];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::h);
    break;
  case 0x5D:
    printf(disasm[0x5d], curr_pc);
    clock_m += clock_m_cycles[0x5d];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::l);
    break;
  case 0x5E:
    printf(disasm[0x5e], curr_pc);
    clock_m += clock_m_cycles[0x5e];
    LD_r8_HL(&REGISTERS::e);
    break;
  case 0x5F:
    printf(disasm[0x5f], curr_pc);
    clock_m += clock_m_cycles[0x5f];
    LD_r8_r8(&REGISTERS::e, &REGISTERS::a);
    break;
  case 0x60:
    printf(disasm[0x60], curr_pc);
    clock_m += clock_m_cycles[0x60];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::b);
    break;
  case 0x61:
    printf(disasm[0x61], curr_pc);
    clock_m += clock_m_cycles[0x61];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::c);
    break;
  case 0x62:
    printf(disasm[0x62], curr_pc);
    clock_m += clock_m_cycles[0x62];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::d);
    break;
  case 0x63:
    printf(disasm[0x63], curr_pc);
    clock_m += clock_m_cycles[0x63];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::e);
    break;
  case 0x64:
    printf(disasm[0x64], curr_pc);
    clock_m += clock_m_cycles[0x64];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::h);
    break;
  case 0x65:
    printf(disasm[0x65], curr_pc);
    clock_m += clock_m_cycles[0x65];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::l);
    break;
  case 0x66:
    printf(disasm[0x66], curr_pc);
    clock_m += clock_m_cycles[0x66];
    LD_r8_HL(&REGISTERS::h);
    break;
  case 0x67:
    printf(disasm[0x67], curr_pc);
    clock_m += clock_m_cycles[0x67];
    LD_r8_r8(&REGISTERS::h, &REGISTERS::a);
    break;
  case 0x68:
    printf(disasm[0x68], curr_pc);
    clock_m += clock_m_cycles[0x68];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::b);
    break;
  case 0x69:
    printf(disasm[0x69], curr_pc);
    clock_m += clock_m_cycles[0x69];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::c);
    break;
  case 0x6A:
    printf(disasm[0x6a], curr_pc);
    clock_m += clock_m_cycles[0x6a];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::d);
    break;
  case 0x6B:
    printf(disasm[0x6b], curr_pc);
    clock_m += clock_m_cycles[0x6b];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::e);
    break;
  case 0x6C:
    printf(disasm[0x6c], curr_pc);
    clock_m += clock_m_cycles[0x6c];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::h);
    break;
  case 0x6D:
    printf(disasm[0x6d], curr_pc);
    clock_m += clock_m_cycles[0x6d];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::l);
    break;
  case 0x6E:
    printf(disasm[0x6e], curr_pc);
    clock_m += clock_m_cycles[0x6e];
    LD_r8_HL(&REGISTERS::l);
    break;
  case 0x6F:
    printf(disasm[0x6f], curr_pc);
    clock_m += clock_m_cycles[0x6f];
    LD_r8_r8(&REGISTERS::l, &REGISTERS::a);
    break;
  case 0x70:
    printf(disasm[0x70], curr_pc);
    clock_m += clock_m_cycles[0x70];
    LD_HL_r8(&REGISTERS::b);
    break;
  case 0x71:
    printf(disasm[0x71], curr_pc);
    clock_m += clock_m_cycles[0x71];
    LD_HL_r8(&REGISTERS::c);
    break;
  case 0x72:
    printf(disasm[0x72], curr_pc);
    clock_m += clock_m_cycles[0x72];
    LD_HL_r8(&REGISTERS::d);
    break;
  case 0x73:
    printf(disasm[0x73], curr_pc);
    clock_m += clock_m_cycles[0x73];
    LD_HL_r8(&REGISTERS::e);
    break;
  case 0x74:
    printf(disasm[0x74], curr_pc);
    clock_m += clock_m_cycles[0x74];
    LD_HL_r8(&REGISTERS::h);
    break;
  case 0x75:
    printf(disasm[0x75], curr_pc);
    clock_m += clock_m_cycles[0x75];
    LD_HL_r8(&REGISTERS::l);
    break;
  case 0x76:
    break;
  case 0x77:
    printf(disasm[0x77], curr_pc);
    clock_m += clock_m_cycles[0x77];
    LD_HL_r8(&REGISTERS::a);
    break;
  case 0x78:
    printf(disasm[0x78], curr_pc);
    clock_m += clock_m_cycles[0x78];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::b);
    break;
  case 0x79:
    printf(disasm[0x79], curr_pc);
    clock_m += clock_m_cycles[0x79];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::c);
    break;
  case 0x7A:
    printf(disasm[0x7a], curr_pc);
    clock_m += clock_m_cycles[0x7a];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::d);
    break;
  case 0x7B:
    printf(disasm[0x7b], curr_pc);
    clock_m += clock_m_cycles[0x7b];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::e);
    break;
  case 0x7C:
    printf(disasm[0x7c], curr_pc);
    clock_m += clock_m_cycles[0x7c];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::h);
    break;
  case 0x7D:
    printf(disasm[0x7d], curr_pc);
    clock_m += clock_m_cycles[0x7d];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::l);
    break;
  case 0x7E:
    printf(disasm[0x7e], curr_pc);
    clock_m += clock_m_cycles[0x7e];
    LD_r8_HL(&REGISTERS::a);
    break;
  case 0x7F:
    printf(disasm[0x7f], curr_pc);
    clock_m += clock_m_cycles[0x7f];
    LD_r8_r8(&REGISTERS::a, &REGISTERS::a);
    break;
  case 0x80:
    printf(disasm[0x80], curr_pc);
    clock_m += clock_m_cycles[0x80];
    ADD_A_r8(&REGISTERS::b);
    break;
  case 0x81:
    printf(disasm[0x81], curr_pc);
    clock_m += clock_m_cycles[0x81];
    ADD_A_r8(&REGISTERS::c);
    break;
  case 0x82:
    printf(disasm[0x82], curr_pc);
    clock_m += clock_m_cycles[0x82];
    ADD_A_r8(&REGISTERS::d);
    break;
  case 0x83:
    printf(disasm[0x83], curr_pc);
    clock_m += clock_m_cycles[0x83];
    ADD_A_r8(&REGISTERS::e);
    break;
  case 0x84:
    printf(disasm[0x84], curr_pc);
    clock_m += clock_m_cycles[0x84];
    ADD_A_r8(&REGISTERS::h);
    break;
  case 0x85:
    printf(disasm[0x85], curr_pc);
    clock_m += clock_m_cycles[0x85];
    ADD_A_r8(&REGISTERS::l);
    break;
  case 0x86:
    printf(disasm[0x86], curr_pc);
    clock_m += clock_m_cycles[0x86];
    ADD_A_HL();
    break;
  case 0x87:
    printf(disasm[0x87], curr_pc);
    clock_m += clock_m_cycles[0x87];
    ADD_A_r8(&REGISTERS::a);
    break;
  case 0x88:
    printf(disasm[0x88], curr_pc);
    clock_m += clock_m_cycles[0x88];
    ADC_A_r8(&REGISTERS::b);
    break;
  case 0x89:
    printf(disasm[0x89], curr_pc);
    clock_m += clock_m_cycles[0x89];
    ADC_A_r8(&REGISTERS::c);
    break;
  case 0x8A:
    printf(disasm[0x8a], curr_pc);
    clock_m += clock_m_cycles[0x8a];
    ADC_A_r8(&REGISTERS::d);
    break;
  case 0x8B:
    printf(disasm[0x8b], curr_pc);
    clock_m += clock_m_cycles[0x8b];
    ADC_A_r8(&REGISTERS::e);
    break;
  case 0x8C:
    printf(disasm[0x8c], curr_pc);
    clock_m += clock_m_cycles[0x8c];
    ADC_A_r8(&REGISTERS::h);
    break;
  case 0x8D:
    printf(disasm[0x8d], curr_pc);
    clock_m += clock_m_cycles[0x8d];
    ADC_A_r8(&REGISTERS::l);
    break;
  case 0x8E:
    printf(disasm[0x8e], curr_pc);
    clock_m += clock_m_cycles[0x8e];
    ADC_A_HL();
    break;
  case 0x8F:
    printf(disasm[0x8f], curr_pc);
    clock_m += clock_m_cycles[0x8f];
    ADC_A_r8(&REGISTERS::a);
    break;
  case 0x90:
    printf(disasm[0x90], curr_pc);
    clock_m += clock_m_cycles[0x90];
    SUB_A_r8(&REGISTERS::b);
    break;
  case 0x91:
    printf(disasm[0x91], curr_pc);
    clock_m += clock_m_cycles[0x91];
    SUB_A_r8(&REGISTERS::c);
    break;
  case 0x92:
    printf(disasm[0x92], curr_pc);
    clock_m += clock_m_cycles[0x92];
    SUB_A_r8(&REGISTERS::d);
    break;
  case 0x93:
    printf(disasm[0x93], curr_pc);
    clock_m += clock_m_cycles[0x93];
    SUB_A_r8(&REGISTERS::e);
    break;
  case 0x94:
    printf(disasm[0x94], curr_pc);
    clock_m += clock_m_cycles[0x94];
    SUB_A_r8(&REGISTERS::h);
    break;
  case 0x95:
    printf(disasm[0x95], curr_pc);
    clock_m += clock_m_cycles[0x95];
    SUB_A_r8(&REGISTERS::l);
    break;
  case 0x96:
    printf(disasm[0x96], curr_pc);
    clock_m += clock_m_cycles[0x96];
    SUB_A_HL();
    break;
  case 0x97:
    printf(disasm[0x97], curr_pc);
    clock_m += clock_m_cycles[0x97];
    SUB_A_r8(&REGISTERS::a);
    break;
  case 0x98:
    printf(disasm[0x98], curr_pc);
    clock_m += clock_m_cycles[0x98];
    SBC_A_r8(&REGISTERS::b);
    break;
  case 0x99:
    printf(disasm[0x99], curr_pc);
    clock_m += clock_m_cycles[0x99];
    SBC_A_r8(&REGISTERS::c);
    break;
  case 0x9A:
    printf(disasm[0x9a], curr_pc);
    clock_m += clock_m_cycles[0x9a];
    SBC_A_r8(&REGISTERS::d);
    break;
  case 0x9B:
    printf(disasm[0x9b], curr_pc);
    clock_m += clock_m_cycles[0x9b];
    SBC_A_r8(&REGISTERS::e);
    break;
  case 0x9C:
    printf(disasm[0x9c], curr_pc);
    clock_m += clock_m_cycles[0x9c];
    SBC_A_r8(&REGISTERS::h);
    break;
  case 0x9D:
    printf(disasm[0x9d], curr_pc);
    clock_m += clock_m_cycles[0x9d];
    SBC_A_r8(&REGISTERS::l);
    break;
  case 0x9E:
    printf(disasm[0x9e], curr_pc);
    clock_m += clock_m_cycles[0x9e];
    SBC_A_HL();
    break;
  case 0x9F:
    printf(disasm[0x9f], curr_pc);
    clock_m += clock_m_cycles[0x9f];
    SBC_A_r8(&REGISTERS::a);
    break;
  case 0xa0:
    printf(disasm[0xa0], curr_pc);
    clock_m += clock_m_cycles[0xa0];
    AND_A_r8(&REGISTERS::b);
    break;
  case 0xa1:
    printf(disasm[0xa1], curr_pc);
    clock_m += clock_m_cycles[0xa1];
    AND_A_r8(&REGISTERS::c);
    break;
  case 0xa2:
    printf(disasm[0xa2], curr_pc);
    clock_m += clock_m_cycles[0xa2];
    AND_A_r8(&REGISTERS::d);
    break;
  case 0xa3:
    printf(disasm[0xa3], curr_pc);
    clock_m += clock_m_cycles[0xa3];
    AND_A_r8(&REGISTERS::e);
    break;
  case 0xa4:
    printf(disasm[0xa4], curr_pc);
    clock_m += clock_m_cycles[0xa4];
    AND_A_r8(&REGISTERS::h);
    break;
  case 0xa5:
    printf(disasm[0xa5], curr_pc);
    clock_m += clock_m_cycles[0xa5];
    AND_A_r8(&REGISTERS::l);
    break;
  case 0xa6:
    printf(disasm[0xa6], curr_pc);
    clock_m += clock_m_cycles[0xa6];
    AND_A_HL();
    break;
  case 0xa7:
    printf(disasm[0xa7], curr_pc);
    clock_m += clock_m_cycles[0xa7];
    AND_A_r8(&REGISTERS::a);
    break;
  case 0xa8:
    printf(disasm[0xa8], curr_pc);
    clock_m += clock_m_cycles[0xa8];
    XOR_A_r8(&REGISTERS::b);
    break;
  case 0xa9:
    printf(disasm[0xa9], curr_pc);
    clock_m += clock_m_cycles[0xa9];
    XOR_A_r8(&REGISTERS::c);
    break;
  case 0xaa:
    printf(disasm[0xaa], curr_pc);
    clock_m += clock_m_cycles[0xaa];
    XOR_A_r8(&REGISTERS::d);
    break;
  case 0xab:
    printf(disasm[0xab], curr_pc);
    clock_m += clock_m_cycles[0xab];
    XOR_A_r8(&REGISTERS::e);
    break;
  case 0xac:
    printf(disasm[0xac], curr_pc);
    clock_m += clock_m_cycles[0xac];
    XOR_A_r8(&REGISTERS::h);
    break;
  case 0xad:
    printf(disasm[0xad], curr_pc);
    clock_m += clock_m_cycles[0xad];
    XOR_A_r8(&REGISTERS::l);
    break;
  case 0xae:
    printf(disasm[0xae], curr_pc);
    clock_m += clock_m_cycles[0xae];
    XOR_A_HL();
    break;
  case 0xaf:
    printf(disasm[0xaf], curr_pc);
    clock_m += clock_m_cycles[0xaf];
    XOR_A_r8(&REGISTERS::a);
    break;
  case 0xb0:
    printf(disasm[0xb0], curr_pc);
    clock_m += clock_m_cycles[0xb0];
    OR_A_r8(&REGISTERS::b);
    break;
  case 0xb1:
    printf(disasm[0xb1], curr_pc);
    clock_m += clock_m_cycles[0xb1];
    OR_A_r8(&REGISTERS::c);
    break;
  case 0xb2:
    printf(disasm[0xb2], curr_pc);
    clock_m += clock_m_cycles[0xb2];
    OR_A_r8(&REGISTERS::d);
    break;
  case 0xb3:
    printf(disasm[0xb3], curr_pc);
    clock_m += clock_m_cycles[0xb3];
    OR_A_r8(&REGISTERS::e);
    break;
  case 0xb4:
    printf(disasm[0xb4], curr_pc);
    clock_m += clock_m_cycles[0xb4];
    OR_A_r8(&REGISTERS::h);
    break;
  case 0xb5:
    printf(disasm[0xb5], curr_pc);
    clock_m += clock_m_cycles[0xb5];
    OR_A_r8(&REGISTERS::l);
    break;
  case 0xb6:
    printf(disasm[0xb6], curr_pc);
    clock_m += clock_m_cycles[0xb6];
    OR_A_HL();
    break;
  case 0xb7:
    printf(disasm[0xb7], curr_pc);
    clock_m += clock_m_cycles[0xb7];
    OR_A_r8(&REGISTERS::a);
    break;
  case 0xb8:
    printf(disasm[0xb8], curr_pc);
    clock_m += clock_m_cycles[0xb8];
    CP_A_r8(&REGISTERS::b);
    break;
  case 0xb9:
    printf(disasm[0xb9], curr_pc);
    clock_m += clock_m_cycles[0xb9];
    CP_A_r8(&REGISTERS::c);
    break;
  case 0xbA:
    printf(disasm[0xba], curr_pc);
    clock_m += clock_m_cycles[0xba];
    CP_A_r8(&REGISTERS::d);
    break;
  case 0xbB:
    printf(disasm[0xbb], curr_pc);
    clock_m += clock_m_cycles[0xbb];
    CP_A_r8(&REGISTERS::e);
    break;
  case 0xbC:
    printf(disasm[0xbc], curr_pc);
    clock_m += clock_m_cycles[0xbc];
    CP_A_r8(&REGISTERS::h);
    break;
  case 0xbD:
    printf(disasm[0xbd], curr_pc);
    clock_m += clock_m_cycles[0xbd];
    CP_A_r8(&REGISTERS::l);
    break;
  case 0xbE:
    printf(disasm[0xbe], curr_pc);
    clock_m += clock_m_cycles[0xbe];
    CP_A_HL();
    break;
  case 0xbF:
    printf(disasm[0xbf], curr_pc);
    clock_m += clock_m_cycles[0xbf];
    CP_A_r8(&REGISTERS::a);
    break;
  case 0xc0:
    printf(disasm[0xc0], curr_pc);
    clock_m += clock_m_cycles[0xc0];
    RET_CC(flag_value(N) & flag_value(Z));
    break;
  case 0xc1:
    printf(disasm[0xc1], curr_pc);
    clock_m += clock_m_cycles[0xc1];
    POP_r16(&REGISTERS::bc);
    break;
  case 0xc2: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0xc2], curr_pc, val);
    clock_m += clock_m_cycles[0xc2];
    JP_CC_n16(flag_value(N) & flag_value(Z), val);
  } break;
  case 0xc3: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0xc3], curr_pc, val);
    clock_m += clock_m_cycles[0xc3];
    JP_n16(val);
  } break;
  case 0xc4: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0xc4], curr_pc, val);
    clock_m += clock_m_cycles[0xc4];
    CALL_CC_n16(flag_value(N) & flag_value(Z), val);
  } break;
  case 0xc5:
    printf(disasm[0xc5], curr_pc);
    clock_m += clock_m_cycles[0xc5];
    PUSH_r16(&REGISTERS::bc);
    break;
  case 0xc6: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0xc6], curr_pc, val);
    clock_m += clock_m_cycles[0xc6];
    ADD_A_n8(val);
  } break;
  case 0xc7:
    printf(disasm[0xc7], curr_pc);
    clock_m += clock_m_cycles[0xc7];
    CALL_n16(0x00);
    break;
  case 0xc8:
    printf(disasm[0xc8], curr_pc);
    clock_m += clock_m_cycles[0xc8];
    RET_CC(flag_value(Z));
    break;
  case 0xc9:
    printf(disasm[0xc8], curr_pc);
    clock_m += clock_m_cycles[0xc8];
    RET();
    break;
  case 0xca: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0xca], curr_pc, val);
    clock_m += clock_m_cycles[0xca];
    JP_CC_n16(flag_value(Z), val);
  } break;
  case 0xcb: {
    uint8_t val = read_byte(reg.pc);
    printf(disasm[0xcb], curr_pc, val);
    clock_m += clock_m_cycles[0xcb];
    CB_INSTRUCTION_DECODER();
  } break;
  case 0xcc:
    printf("ERROR 0xCC 0x%04X\n", read_byte(curr_pc));
    exit(1);
    break;
  case 0xcd: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0xcd], curr_pc, val);
    clock_m += clock_m_cycles[0xcd];
    CALL_n16(val);
  } break;
  case 0xce: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0xce], curr_pc, val);
    clock_m += clock_m_cycles[0xce];
    ADC_A_n8(val);
  } break;
  case 0xd0: {
    printf(disasm[0xd0], curr_pc);
    clock_m += clock_m_cycles[0xd0];
    RET_CC(flag_value(N) & flag_value(C));
  } break;
  case 0xd1: {
    printf(disasm[0xd1], curr_pc);
    clock_m += clock_m_cycles[0xd1];
    POP_r16(&REGISTERS::de);
  } break;
  case 0xD2: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0xD2], curr_pc, val);
    clock_m += clock_m_cycles[0xD2];
    JP_CC_n16(flag_value(C) && flag_value(N), val);
  } break;
  case 0xd5: {
    printf(disasm[0xd5], curr_pc);
    clock_m += clock_m_cycles[0xd5];
    PUSH_r16(&REGISTERS::de);
  } break;
  case 0xd6: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0xd6], curr_pc, val);
    clock_m += clock_m_cycles[0xd6];
    SUB_A_n8(val);
  } break;
  case 0xd9: {
    printf(disasm[0xd9], curr_pc);
    clock_m += clock_m_cycles[0xd9];
    RETI();
  } break;
  case 0xDA: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0xDA], curr_pc, val);
    clock_m += clock_m_cycles[0xDA];
    JP_CC_n16(flag_value(C), val);
  } break;
  case 0xE0: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0xe0], curr_pc, val);
    clock_m += clock_m_cycles[0xe0];
    LDH_n16_A(val);
  } break;
  case 0xe1: {
    printf(disasm[0xe1], curr_pc);
    clock_m += clock_m_cycles[0xe1];
    POP_r16(&REGISTERS::hl);
  } break;
  case 0xE2:
    printf(disasm[0xE2], curr_pc);
    clock_m += clock_m_cycles[0xE2];
    LDH_C_A();
    break;
  case 0xe5: {
    printf(disasm[0xe5], curr_pc);
    clock_m += clock_m_cycles[0xe5];
    PUSH_r16(&REGISTERS::hl);
  } break;
  case 0xE6: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0xE6], curr_pc, val);
    clock_m += clock_m_cycles[0xE6];
    AND_A_n8(val);
  } break;
  case 0xE8: {
    int val = read_byte(reg.pc++);
    printf(disasm[0xE8], curr_pc, val);
    clock_m += clock_m_cycles[0xE8];
    ADD_SP_e8(val);
  } break;
  case 0xE9:
    printf(disasm[0xE9], curr_pc);
    clock_m += clock_m_cycles[0xE9];
    break;
  case 0xEA: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0xea], curr_pc, val);
    clock_m += clock_m_cycles[0xea];
    LD_n16_A(val);
  } break;
  case 0xEC: {
    printf(disasm[0xEC], curr_pc);
    clock_m += clock_m_cycles[0xEC];
  } break;
  case 0xEE: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0xEE], curr_pc, val);
    clock_m += clock_m_cycles[0xEE];
    XOR_A_n8(val);
  } break;
  case 0xEF: {
    printf(disasm[0xEF], curr_pc);
    clock_m += clock_m_cycles[0xEF];
    RST_vec(0x28);
  } break;
  case 0xf0: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0xf0], curr_pc, val);
    clock_m += clock_m_cycles[0xf0];
    LDH_A_n16(val);
  } break;
  case 0xF1: {
    printf(disasm[0xF1], curr_pc);
    clock_m += clock_m_cycles[0xF1];
    POP_AF();
  } break;
  case 0xf3: {
    printf(disasm[0xf3], curr_pc);
    clock_m += clock_m_cycles[0xf3];
    DI();
  } break;
  case 0xF5: {
    printf(disasm[0xF5], curr_pc);
    clock_m += clock_m_cycles[0xF5];
    PUSH_AF();
  } break;
  case 0xF8: {
    int val = read_byte(reg.pc++);
    printf(disasm[0xF8], curr_pc, val);
    clock_m += clock_m_cycles[0xF8];
    LD_HL_SP_e8(val);
  } break;
  case 0xF9: {
    printf(disasm[0xF9], curr_pc);
    clock_m += clock_m_cycles[0xF9];
    LD_SP_HL();
  } break;
  case 0xFA: {
    uint16_t val = read_word(reg.pc++);
    reg.pc++;
    printf(disasm[0xfa], curr_pc, val);
    clock_m += clock_m_cycles[0xfa];
  } break;
  case 0xfb: {
    printf(disasm[0xfb], curr_pc);
    clock_m += clock_m_cycles[0xfb];
    EI();
  } break;
  case 0xFC: {
    printf(disasm[0xFC], curr_pc);
    clock_m += clock_m_cycles[0xFC];
  } break;
  case 0xFE: {
    uint8_t val = read_byte(reg.pc++);
    printf(disasm[0xFE], curr_pc, val);
    clock_m += clock_m_cycles[0xFE];
    CP_A_n8(val);
  } break;
  case 0xFF: {
    printf(disasm[0xFF], curr_pc);
    clock_m += clock_m_cycles[0xFF];
    RST_vec(0x38);
  } break;
  default:
    printf("ERROR: DEFAULT 0x%02X\n", curr_pc);
    printf("ERROR 0x%04X\n", read_byte(curr_pc));
    exit(1);
    break;
  }
}
