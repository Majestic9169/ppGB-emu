#ifndef INCLUDE_CPU
#define INCLUDE_CPU

#include "memory.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/types.h>

enum JOYPAD_BUTTON {
  JOYPAD_A = (1 << 0),
  JOYPAD_B = (1 << 1),
  JOYPAD_SELECT = (1 << 2),
  JOYPAD_START = (1 << 3),
  JOYPAD_RIGHT = (1 << 4),
  JOYPAD_LEFT = (1 << 5),
  JOYPAD_UP = (1 << 6),
  JOYPAD_DOWN = (1 << 7),
};
enum INTERRUPTS { INT_VBLANK, INT_LCD, INT_TIMER, INT_SERIAL, INT_JOYPAD };
enum FLAGS { C = 4, H = 5, N = 6, Z = 7 };
enum LCDC_FLAGS {
  BG_ENABLE,
  SPR_ENABLE,
  SPR_SIZE,
  BG_MAP_SELECT,
  TILE_SELECT,
  WIN_ENABLE,
  WIN_MAP_SELECT,
  LCD_ENABLE
};
enum STAT_FLAGS {
  PPU_MODE = 0,
  COINCIDENCE_FLAG = 2,
  HBLANK_INTERRUPT,
  VBLANK_INTERRUPT,
  OAM_INTERRUPT,
  COINCIDENCE_INTERRUPT
};
enum FLAG_MODE { REG_F, LCD_CONTROL, LCD_STATUS };
enum LCD_MODE { HBLANK, VBLANK, OAM, DRAW };

class CPU {
private:
  Memory ROM;

public:
  struct REGISTERS {
    union {
      struct {
        std::uint8_t f;
        std::uint8_t a;
      };
      std::uint16_t af;
    };
    union {
      struct {
        std::uint8_t c;
        std::uint8_t b;
      };
      std::uint16_t bc;
    };
    union {
      struct {
        std::uint8_t e;
        std::uint8_t d;
      };
      std::uint16_t de;
    };
    union {
      struct {
        std::uint8_t l;
        std::uint8_t h;
      };
      std::uint16_t hl;
    };
    std::uint16_t sp;
    std::uint16_t pc;
  } reg;

  int clock_m;
  bool halted = false;
  bool IME;

  LCD_MODE mode;
  bool can_render;

  // PPU timing variables
  int ppu_clock;
  uint8_t interrupt_flags;

  // PPU timing constants
  static constexpr int CYCLES_PER_SCANLINE = 456;
  static constexpr int CYCLES_PER_FRAME = CYCLES_PER_SCANLINE * 154;
  static constexpr int OAM_CYCLES = 80;
  static constexpr int DRAW_CYCLES = 172;
  static constexpr int HBLANK_CYCLES = 87;
  static constexpr int VBLANK_START_LINE = 144;
  static constexpr int VBLANK_END_LINE = 153;

  using R8_PTR = uint8_t REGISTERS::*;
  using R16_PTR = uint16_t REGISTERS::*;

  CPU(std::ifstream &ROM_);

  void print_reg();

  // MMU Functions
  uint8_t read_byte(uint16_t addr);
  uint16_t read_word(uint16_t addr);
  void write_byte(uint16_t addr, uint8_t val);
  void write_word(uint16_t addr, uint16_t val);

  // CPU reg functionsl
  uint16_t read_word_from_pc();
  uint8_t read_byte_from_pc();
  uint8_t read_reg(R8_PTR r);
  void write_reg(R8_PTR r, uint8_t val);
  uint16_t read_reg(R16_PTR r);
  void write_reg(R16_PTR r, uint16_t val);

  bool flag_value(uint8_t f, FLAG_MODE = REG_F);
  void flag_value(uint8_t f, bool set, FLAG_MODE = REG_F);
  void reset_flags();
  void set_z_flag(uint test);
  void set_h_flag(uint test);
  void set_c_flag(uint test);

  // PPU Functions
  void scanline_background(bool *pixel_row);
  void scanline_window();
  void scanline_sprites(bool *pixel_row);
  void scanlines();
  void PPU_STEP();
  std::array<Color, 160 * 144> framebuffer;

  // opcodes
  void INSTRUCTION_DECODER();
  void CB_INSTRUCTION_DECODER();

  // Interrupt handling
  bool interrupt_check();
  void interrupt_handle(INTERRUPTS interrupt, uint8_t jump_addr);
  void key_up(JOYPAD_BUTTON butt);
  void key_down(JOYPAD_BUTTON butt);
  uint8_t read_joypad();
  void write_joypad(uint8_t value);

  /* ====================================
   *          CONTROL/MISC
   * ====================================
   */
  void NOP();
  void STOP(); // 0x10
  void HALT(); // 0x76

  /* ====================================
   *          CONTROL/BRANCH
   * ====================================
   */
  void JR_n16(int8_t e8);
  void JR_CC_n16(bool condition, int8_t e8);

  void RET();
  void RET_CC(bool condition);
  void RETI();

  void JP_n16(uint16_t n16);
  void JP_CC_n16(bool condition, uint16_t n16);
  void JP_HL();

  void CALL_n16(uint16_t n16);
  void CALL_CC_n16(bool condition, uint16_t n16);

  void RST_vec(uint8_t);

  /* ====================================
   *       8-bit LOAD/STORE/MOVE
   * ====================================
   */
  void LD_r16_r8(R16_PTR r1, R8_PTR r2);
  void LD_r8_r16(R8_PTR r1, R16_PTR r2);
  void LD_r8_r8(R8_PTR r1, R8_PTR r2);
  void LD_r8_n8(R8_PTR r1, uint8_t n8);
  void LD_HL_r8(R8_PTR r);
  void LD_HL_n8(uint8_t n);
  void LD_r8_HL(R8_PTR r);
  // void LD_r16_A(R8_PTR r);
  void LD_n16_A(uint16_t n16);
  // void LD_A_r16(R8_PTR r);
  void LD_A_n16(uint16_t n16);
  void LD_HLI_A();
  void LD_HLD_A();
  void LD_A_HLD();
  void LD_A_HLI();

  void LDH_n16_A(uint8_t n8);
  void LDH_C_A();
  void LDH_A_n16(uint8_t n8);
  void LDH_A_C();

  /* ====================================
   *       16-bit LOAD/STORE/MOVE
   * ====================================
   */
  void LD_r16_n16(R16_PTR r, uint16_t n16);
  void LD_SP_n16(uint16_t n16);
  void LD_n16_SP(uint16_t n16);
  void LD_HL_SP_e8(int8_t e8); // implement flags
  void LD_SP_HL();

  void POP_r16(R16_PTR r);
  void POP_AF();

  void PUSH_r16(R16_PTR r);
  void PUSH_AF();

  /* ====================================
   *     8-bit ARITHMETIC LOGIC UNIT
   * ====================================
   */
  void ADC_A_r8(R8_PTR r);
  void ADC_A_HL();
  void ADC_A_n8(uint8_t n8);

  void ADD_A_r8(R8_PTR r);
  void ADD_A_HL();
  void ADD_A_n8(uint8_t n8);
  void ADD_SP_e8(int8_t e8);

  void AND_A_r8(R8_PTR r);
  void AND_A_HL();
  void AND_A_n8(uint8_t n8);

  void CP_A_r8(R8_PTR r);
  void CP_A_HL();
  void CP_A_n8(uint8_t n8);

  void CPL();

  void DAA();

  void DEC_r8(R8_PTR r);
  void DEC_HL();

  void INC_r8(R8_PTR r);
  void INC_HL();

  void CCF();

  void SUB_A_r8(R8_PTR r);
  void SUB_A_HL();
  void SUB_A_n8(uint8_t n8);

  void SBC_A_r8(R8_PTR r);
  void SBC_A_HL();
  void SBC_A_n8(uint8_t n8);

  void XOR_A_r8(R8_PTR r);
  void XOR_A_HL();
  void XOR_A_n8(uint8_t n8);

  void OR_A_r8(R8_PTR r);
  void OR_A_HL();
  void OR_A_n8(uint8_t n8);

  /* ====================================
   *     16-bit ARITHMETIC LOGIC UNIT
   * ====================================
   */
  void INC_r16(R16_PTR r);
  void INC_SP();

  void ADD_HL_r16(R16_PTR r);
  void ADD_HL_SP();

  void DEC_r16(R16_PTR r);
  void DEC_SP();

  void LD_HL_SP();

  /* ====================================
   *     8-bit ROTATE/SHIFT bit
   * ====================================
   */
  void BIT_u3_r8(uint8_t u3, R8_PTR r);
  void BIT_u3_HL(uint8_t u3);

  void RES_u3_r8(uint8_t u3, R8_PTR r);
  void RES_u3_HL(uint8_t u3);

  void RL_r8(R8_PTR r);
  void RL_HL();

  void RLA();

  void RLC_r8(R8_PTR r);
  void RLC_HL();

  void RLCA();

  void RR_r8(R8_PTR r);
  void RR_HL();

  void RRA();

  void RRC_r8(R8_PTR r);
  void RRC_HL();

  void RRCA();

  void SET_u3_r8(uint8_t u3, R8_PTR r);
  void SET_u3_HL(uint8_t u3);

  void SLA_r8(R8_PTR r);
  void SLA_HL();

  void SRA_r8(R8_PTR r);
  void SRA_HL();

  void SRL_r8(R8_PTR r);
  void SRL_HL();

  void SWAP_r8(R8_PTR r);
  void SWAP_HL();

  /* ====================================
   *          MISCELLANEOUS
   * ====================================
   */
  void DI();
  void EI();
};

#endif
