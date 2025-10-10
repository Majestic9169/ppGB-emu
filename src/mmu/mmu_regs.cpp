#include "../../include/mmu/mmu_regs.hpp"
#include <cstdint>
#include <vector>

// OBJECT
Object::Object(std::vector<uint8_t>::iterator start)
    : y_pos{*start}, x_pos{*(start + 1)}, tile_index{*(start + 2)},
      attributes{*(start + 3)} {};

bool Object::getBit(int bit_no) const { return attributes & (1 << bit_no); }

uint8_t Object::GetYPostition() const { return y_pos; }
uint8_t Object::GetXPostition() const { return x_pos; }
uint8_t Object::GetTileIndex() const { return tile_index; }

bool Object::GetPriority() const { return getBit(7); }
bool Object::GetYFlip() const { return getBit(6); }
bool Object::GetXFlip() const { return getBit(5); }
bool Object::GetPallete() const { return getBit(4); }

// LCDC
LCDC_REG::LCDC_REG(uint8_t &ff40) : ff40_ref{ff40} {}

bool LCDC_REG::getBit(int bit_no) const { return ff40_ref & (1 << bit_no); }

bool LCDC_REG::isLCDenabled() const { return getBit(7); }
bool LCDC_REG::WindowTileMap() const { return getBit(6); }
bool LCDC_REG::isWindowEnable() const { return getBit(5); }
uint16_t LCDC_REG::TileMap() const { return getBit(4) ? 0x8000 : 0x8800; }
bool LCDC_REG::BGTileMap() const { return getBit(3); }
uint8_t LCDC_REG::ObjSize() const { return getBit(2) ? 2 : 1; }
bool LCDC_REG::areObjEnabled() const { return getBit(1); }
bool LCDC_REG::BGWindowEnable() const { return getBit(0); }

// STAT
STAT_REG::STAT_REG(uint8_t &ff41) : ff41_ref{ff41} {}

bool STAT_REG::getBit(int bit_no) const { return ff41_ref & (1 << bit_no); }
void STAT_REG::ResetLYEqualLYC() { ff41_ref &= 0xfb; }
void STAT_REG::SetLYEqualLYC() { ff41_ref |= 0x04; }
void STAT_REG::SetPPUMode(uint8_t mode) {
  switch (mode) {
  case 0:
    ff41_ref &= 0xfc;
    break;
  case 1:
    ff41_ref = (ff41_ref & 0xfc) | 0x01;
    break;
  case 2:
    ff41_ref = (ff41_ref & 0xfc) | 0x02;
    break;
  case 3:
    ff41_ref |= 0x03;
    break;
  default:
    break;
  }
}

// IF
IF_REG::IF_REG(uint8_t &ff0f) : ff0f_ref{ff0f} {}

void IF_REG::setBit(int bit_no) { ff0f_ref |= (1 << bit_no); }
bool IF_REG::getBit(int bit_no) const { return ff0f_ref & (1 << bit_no); }
void IF_REG::resetBit(int bit_no) {
  uint8_t mask = ~(1 << bit_no);
  ff0f_ref = ff0f_ref & mask;
}

bool IF_REG::CheckJoypad() const { return getBit(4); }
bool IF_REG::CheckSerial() const { return getBit(3); }
bool IF_REG::CheckTimer() const { return getBit(2); }
bool IF_REG::CheckLCD() const { return getBit(1); }
bool IF_REG::CheckVBLANK() const { return getBit(0); }

void IF_REG::ReqJoypad() { setBit(4); }
void IF_REG::ReqSerial() { setBit(3); }
void IF_REG::ReqTimer() { setBit(2); }
void IF_REG::ReqLCD() { setBit(1); }
void IF_REG::ReqVBLANK() { setBit(0); }

void IF_REG::ResetJoypad() { resetBit(4); }
void IF_REG::ResetSerial() { resetBit(3); }
void IF_REG::ResetTimer() { resetBit(2); }
void IF_REG::ResetLCD() { resetBit(1); }
void IF_REG::ResetVBLANK() { resetBit(0); }

// IE
IE_REG::IE_REG(uint8_t &ffff) : ffff_ref{ffff} {}

bool IE_REG::getBit(int bit_no) const { return ffff_ref & (1 << bit_no); }

bool IE_REG::ReqJoypad() const { return getBit(4); }
bool IE_REG::ReqSerial() const { return getBit(3); }
bool IE_REG::ReqTimer() const { return getBit(2); }
bool IE_REG::ReqLCD() const { return getBit(1); }
bool IE_REG::ReqVBLANK() const { return getBit(0); }
