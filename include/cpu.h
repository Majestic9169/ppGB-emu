#include "memory.h"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>

class CPU {
private:
  Memory ROM;

  int clock_m;

  struct REGISTERS {
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
  using R_PTR = uint8_t REGISTERS::*;

  CPU(std::ifstream &ROM_);
  void print_reg();
  uint8_t read_byte(uint16_t addr);
  uint16_t read_word(uint16_t addr);
  void write_byte(uint16_t addr, uint8_t val);
  void write_word(uint16_t addr, uint16_t val);

  uint8_t read_reg(R_PTR r);
  void write_reg(R_PTR r, uint8_t val);

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
  void LD_r16_r8(uint16_t addr, R_PTR r); // too many
  void LD_r8_r16(R_PTR r, uint16_t addr); // too many
  void LD_r8_r8(R_PTR r1, R_PTR r2);
  void LD_r8_n8(R_PTR r1, uint8_t n8);
  void LD_HL_r8(R_PTR r);
  void LD_HL_n8(uint8_t n);
  void LD_r8_HL(R_PTR r);
  // start here
  // void LD_r16_A(R_PTR r);
  // void LD_n16_A(uint16_t n16);
  // void LD_A_r16(R_PTR r);
  // void LD_A_n16(uint16_t n16);
  void LD_HLI_A();
  void LD_HLD_A();
  void LD_A_HLD();
  void LD_A_HLI();
  void LDH_n16_A(uint16_t n);
  void LDH_C_A();
  void LDH_A_n16(uint16_t n16);
  void LDH_A_C();

  /* ====================================
   *       16-bit LOAD/STORE/MOVE
   * ====================================
   */
  void LD_r16_n16(CPU::R_PTR r1, CPU::R_PTR r2, uint16_t n16); // quite a lot
  void LD_SP_n16(uint16_t n16);
  void LD_n16_SP(uint16_t n16);
  void LD_HL_SP_e8(int8_t e8); // implement flags
  void LD_SP_HL();
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
