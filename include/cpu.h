#include "memory.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>

class CPU {
private:
  Memory ROM;

  int clock_m;

  struct reg {
    union {
      struct {
        std::uint8_t a;
        std::uint8_t f;
      };
      std::uint16_t af;
    };
    union {
      struct {
        std::uint8_t b;
        std::uint8_t c;
      };
      std::uint16_t bc;
    };
    union {
      struct {
        std::uint8_t d;
        std::uint8_t e;
      };
      std::uint16_t de;
    };
    union {
      struct {
        std::uint8_t h;
        std::uint8_t l;
      };
      std::uint16_t hl;
    };
    std::uint16_t sp;
    std::uint16_t pc;
  } reg;

public:
  CPU(std::ifstream &ROM_);
  void print_reg();

  /* ====================================
   *          CONTROL/MISC
   * ====================================
   */
  void NOP();  // 0x00
  void STOP(); // 0x10
  void HALT(); // 0x76

  /* ====================================
   *          CONTROL/BRANCH
   * ====================================
   */
  void JR();   // 0x18, 0x20, 0x28, 0x30 0x38
  void RET();  // 0xc0, 0xd0, 0xc8, 0xd8, 0xc9, 0xd9
  void JP();   // 0xc2, 0xd2, 0xc3, 0xe9, 0xca, 0xda
  void CALL(); // 0xc4, 0xd4, 0xcc, 0xdc, 0xcd
  void RST();  // 0xe7, 0xd7, 0xc7, 0xd7, 0xcf, 0xdf, 0xef, 0xff

  /* ====================================
   *       8-bit LOAD/STORE/MOVE
   * ====================================
   */
  void LD(); // too many

  /* ====================================
   *       16-bit LOAD/STORE/MOVE
   * ====================================
   */
  void LD_16(); // quite a lot
  void POP();

  /* ====================================
   *     8-bit ARITHMETIC LOGIC UNIT
   * ====================================
   */
  void INC();
  void DEC();
  void DAA();
  void CPL();
  void CCF();
  void ADD();
  void ADC();
  void SUB();
  void SBC();
  void AND();
  void XOR();
  void OR();
  void CP();

  /* ====================================
   *     16-bit ARITHMETIC LOGIC UNIT
   * ====================================
   */
  void INC_16();
  void ADD_16();
  void DEC_16();
  void LD_HL_SP();

  /* ====================================
   *     8-bit ROTATE/SHIFT bit
   * ====================================
   */
  void RLCA();
  void RLA();
  void RRCA();
  void RRA();
  void RLC();
  void RRC();
  void RL();
  void RR();
  void SLA();
  void SRA();
  void SWAP();
  void SRL();
  void BIT();
  void RES();
  void SET();
};
