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

// Not a fan of these names, lemme know any better conventions
// Getters
void STAT_REG::ResetLYCIntSelect() { ff41_ref &= 0xbf; }
void STAT_REG::ResetMode2IntSelect() { ff41_ref &= 0xdf; }
void STAT_REG::ResetMode1IntSelect() { ff41_ref &= 0xef; }
void STAT_REG::ResetMode0IntSelect() { ff41_ref &= 0xf7; }
void STAT_REG::ResetLYEqualLYC() { ff41_ref &= 0xfb; }

// Setters
void STAT_REG::SetLYCIntSelect() { ff41_ref |= 0x40; }
void STAT_REG::SetMode2IntSelect() { ff41_ref |= 0x20; }
void STAT_REG::SetMode1IntSelect() { ff41_ref |= 0x10; }
void STAT_REG::SetMode0IntSelect() { ff41_ref |= 0x08; }
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
