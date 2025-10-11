#include <cstdint>
#include <vector>

// OAM stored from 0xfe00-0xfe9f
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
  bool WindowTileMap() const;
  bool isWindowEnable() const;
  uint16_t TileMap() const;
  bool BGTileMap() const;
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
class STAT_REG {
  uint8_t &ff41_ref;
  bool getBit(int bit_no) const;

public:
  STAT_REG(uint8_t &ff41);

  bool getStatline() const;
  void ResetLYEqualLYC();
  void SetLYEqualLYC();
  void SetPPUMode(uint8_t mode);
  uint8_t GetPPUMode() const;
};

// 0xFF0F - IF: Interrupt Flag
// 4 - is joypad being requested
// 3 - is serial being requested
// 2 - is timer being requested
// 1 - is lcd being requested
// 0 - is vblank being requested
class IF_REG {
  uint8_t &ff0f_ref;
  bool getBit(int bit_no) const;
  void setBit(int bit_no);
  void resetBit(int bit_no);

public:
  IF_REG(uint8_t &ff0f);

  bool CheckJoypad() const;
  bool CheckLCD() const;
  bool CheckSerial() const;
  bool CheckTimer() const;
  bool CheckVBLANK() const;
  void ReqJoypad();
  void ReqLCD();
  void ReqSerial();
  void ReqTimer();
  void ReqVBLANK();
  void ResetJoypad();
  void ResetLCD();
  void ResetSerial();
  void ResetTimer();
  void ResetVBLANK();
};

// 0xFFFF - IE: Interrupt Enable
// 4 - is joypad enabled
// 3 - is serial enabled
// 2 - is timer enabled
// 1 - is lcd enabled
// 0 - is vblank enabled
class IE_REG {
  uint8_t &ffff_ref;
  bool getBit(int bit_no) const;

public:
  IE_REG(uint8_t &ffff);

  bool ReqJoypad() const;
  bool ReqLCD() const;
  bool ReqSerial() const;
  bool ReqTimer() const;
  bool ReqVBLANK() const;
};
