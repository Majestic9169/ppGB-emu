#include <cstdint>
#include <vector>

// OAM stored from 0xfe00-0xfe9f
// TODO: do something about the OAM DMA transfer
class Object {
private:
  // can't create structs because i need them to be references to the ROM
  // so i just store the bytes as references
  uint8_t &y_pos;
  uint8_t &x_pos;
  uint8_t &tile_index;
  uint8_t &attributes;
  bool getBit(int bit_no) const;

public:
  Object(std::vector<uint8_t>::iterator start);

  uint8_t GetYPostition() const;
  uint8_t GetXPostition() const;
  uint8_t GetTileIndex() const;

  bool GetPriority() const;
  bool GetYFlip() const;
  bool GetXFlip() const;
  bool GetPallete() const;
};

// 0xFF40
// 7 - lcd/ppu enable
// 6 - window tile map (0x9800-0x9bff | 0x9c00-0x9fff)
// 5 - window enable
// 4 - bg and window tile map (0x8800-0x97ff | 0x8000-0x8fff)
// 3 - bg tile map (0x9800-0x9bff | 0x9c00-0x9fff)
// 2 - obj size (8x8 | 8x16)
// 1 - obj enable
// 0 - bg/window enable/priority
class LCDC_REG {
private:
  uint8_t &ff40_ref;
  bool getBit(int bit_no) const;

public:
  LCDC_REG(uint8_t &ff40);

  bool isLCDenabled() const;
  uint16_t WindowTileMap() const;
  bool isWindowEnable() const;
  uint16_t TileMap() const;
  uint16_t BGTileMap() const;
  uint8_t ObjSize() const;
  bool areObjEnabled() const;
  bool BGWindowEnable() const;
};

// 0xFF41 - STAT: LCD status
// 6 - lyc int select
// 5 - mode 2 int select
// 4 - mode 1 int select
// 3 - mode 0 int select
// 2 - lyc == ly
// 1, 0 - ppu mode
// TODO: add a pointer to PPU or framebuffer or whatever to update ly, lyc etc
class STAT_REG {
  uint8_t &ff41_ref;
  bool getBit(int bit_no) const;

public:
  STAT_REG(uint8_t &ff41);

  // Not a fan of these names, lemme know any better conventions
  // Getters
  bool GetLYCIntSelect() const;
  bool GetMode2IntSelect() const;
  bool GetMode1IntSelect() const;
  bool GetMode0IntSelect() const;
  bool GetLYEqualLYC() const;

  // Setters
  void SetLYCIntSelect();
  void SetMode2IntSelect();
  void SetMode1IntSelect();
  void SetMode0IntSelect();
};
