#include <cstdint>
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"
#include "../include/cli_opts.hpp"
#include "../include/opcodes.hpp"

int argc = 2;
char *argv[2] = {strdup("./ppGB"), strdup("../roms/tetris.gb")};
Opts cli_opts{argc, argv};
MMU mmu{&cli_opts};
Registers reg{};

Opcodes op{&mmu, &reg};

TEST_CASE("write_word functioning", "[mmu, 16]") {
  mmu.write_word(0x1010, 0xbeef);
  REQUIRE(mmu.read_word(0x1010) == 0xbeef);
  REQUIRE(mmu.read_byte(0x1010) == 0xef);
  REQUIRE(mmu.read_byte(0x1011) == 0xbe);
}

TEST_CASE("lcdc structure and update check", "[mmu, reg, lcdc]") {
  mmu.write_byte(0xff40, 0xee);
  REQUIRE(mmu.lcdc.isLCDenabled() == true);
  REQUIRE(mmu.lcdc.WindowTileMap() == 0x9c00);
  REQUIRE(mmu.lcdc.isWindowEnable() == true);
  REQUIRE(mmu.lcdc.TileMap() == 0x8800);
  REQUIRE(mmu.lcdc.BGTileMap() == 0x9c00);
  REQUIRE(mmu.lcdc.ObjSize() == 2);
  REQUIRE(mmu.lcdc.areObjEnabled() == true);
  REQUIRE(mmu.lcdc.BGWindowEnable() == false);
  mmu.write_byte(0xff40, 0x11);
  REQUIRE(mmu.lcdc.isLCDenabled() == false);
  REQUIRE(mmu.lcdc.WindowTileMap() == 0x9800);
  REQUIRE(mmu.lcdc.isWindowEnable() == false);
  REQUIRE(mmu.lcdc.TileMap() == 0x8000);
  REQUIRE(mmu.lcdc.BGTileMap() == 0x9800);
  REQUIRE(mmu.lcdc.ObjSize() == 1);
  REQUIRE(mmu.lcdc.areObjEnabled() == false);
  REQUIRE(mmu.lcdc.BGWindowEnable() == true);
}

TEST_CASE("get tile testing", "[mmu, tiles]") {
  mmu.write_byte(0x8000, 0x3c);
  mmu.write_byte(0x8001, 0x7e);
  mmu.write_byte(0x8002, 0x42);
  mmu.write_byte(0x8003, 0x42);
  mmu.write_byte(0x8004, 0x42);
  mmu.write_byte(0x8005, 0x42);
  mmu.write_byte(0x8006, 0x42);
  mmu.write_byte(0x8007, 0x42);
  mmu.write_byte(0x8008, 0x7e);
  mmu.write_byte(0x8009, 0x5e);
  mmu.write_byte(0x800a, 0x7e);
  mmu.write_byte(0x800b, 0x0a);
  mmu.write_byte(0x800c, 0x7c);
  mmu.write_byte(0x800d, 0x56);
  mmu.write_byte(0x800e, 0x38);
  mmu.write_byte(0x800f, 0x7c);

  std::vector<uint8_t> raw_data = {0x3c, 0x7e, 0x42, 0x42, 0x42, 0x42,
                                   0x42, 0x42, 0x7e, 0x5e, 0x7e, 0x0a,
                                   0x7c, 0x56, 0x38, 0x7c};

  REQUIRE(mmu.GetTileFromIndex(0, OBJECT).GetRawTile() == raw_data);

  mmu.write_byte(0x8000, 0x00);
  mmu.write_byte(0x8001, 0x00);
  mmu.write_byte(0x8002, 0x00);
  mmu.write_byte(0x8003, 0x00);
  mmu.write_byte(0x8004, 0x00);
  mmu.write_byte(0x8005, 0x00);
  mmu.write_byte(0x8006, 0x00);
  mmu.write_byte(0x8007, 0x00);
  mmu.write_byte(0x8008, 0x00);
  mmu.write_byte(0x8009, 0x00);
  mmu.write_byte(0x800a, 0x00);
  mmu.write_byte(0x800b, 0x00);
  mmu.write_byte(0x800c, 0x00);
  mmu.write_byte(0x800d, 0x00);
  mmu.write_byte(0x800e, 0x00);
  mmu.write_byte(0x800f, 0x00);

  mmu.write_byte(0xff40, 0x00);

  mmu.write_byte(0x9000, 0x3c);
  mmu.write_byte(0x9001, 0x7e);
  mmu.write_byte(0x9002, 0x42);
  mmu.write_byte(0x9003, 0x42);
  mmu.write_byte(0x9004, 0x42);
  mmu.write_byte(0x9005, 0x42);
  mmu.write_byte(0x9006, 0x42);
  mmu.write_byte(0x9007, 0x42);
  mmu.write_byte(0x9008, 0x7e);
  mmu.write_byte(0x9009, 0x5e);
  mmu.write_byte(0x900a, 0x7e);
  mmu.write_byte(0x900b, 0x0a);
  mmu.write_byte(0x900c, 0x7c);
  mmu.write_byte(0x900d, 0x56);
  mmu.write_byte(0x900e, 0x38);
  mmu.write_byte(0x900f, 0x7c);

  REQUIRE(mmu.GetTileFromIndex(0, WINDOW).GetRawTile() == raw_data);
}

TEST_CASE("oam and objects test", "[oam, objects, ppu]") {
  std::vector<uint8_t> TEST_ROM{0x20, 0x21, 0x22, 0xf0};
  Object test_object(TEST_ROM.begin());

  REQUIRE(test_object.GetYPostition() == 0x20);
  REQUIRE(test_object.GetXPostition() == 0x21);
  REQUIRE(test_object.GetTileIndex() == 0x22);
  REQUIRE(test_object.GetPriority() == true);
  REQUIRE(test_object.GetYFlip() == true);
  REQUIRE(test_object.GetXFlip() == true);
  REQUIRE(test_object.GetPallete() == true);

  TEST_ROM = {0xde, 0xad, 0xbe, 0xef};

  REQUIRE(test_object.GetYPostition() == 0xde);
  REQUIRE(test_object.GetXPostition() == 0xad);
  REQUIRE(test_object.GetTileIndex() == 0xbe);
  REQUIRE(test_object.GetPriority() == true);
  REQUIRE(test_object.GetYFlip() == true);
  REQUIRE(test_object.GetXFlip() == true);
  REQUIRE(test_object.GetPallete() == false);

  mmu.write_byte(0xfe00, 0x69);
  mmu.write_byte(0xfe01, 0x6a);
  mmu.write_byte(0xfe02, 0x6b);
  mmu.write_byte(0xfe03, 0x6c);

  REQUIRE(mmu.OAM[0].GetYPostition() == 0x69);
  REQUIRE(mmu.OAM[0].GetXPostition() == 0x6a);
  REQUIRE(mmu.OAM[0].GetTileIndex() == 0x6b);
  REQUIRE(mmu.OAM[0].GetPriority() == false);
  REQUIRE(mmu.OAM[0].GetYFlip() == true);
  REQUIRE(mmu.OAM[0].GetXFlip() == true);
  REQUIRE(mmu.OAM[0].GetPallete() == false);

  mmu.write_byte(0xfe00, 0x6a);
  mmu.write_byte(0xfe01, 0x6b);
  mmu.write_byte(0xfe02, 0x6c);
  mmu.write_byte(0xfe03, 0x71);

  REQUIRE(mmu.OAM[0].GetYPostition() == 0x6a);
  REQUIRE(mmu.OAM[0].GetXPostition() == 0x6b);
  REQUIRE(mmu.OAM[0].GetTileIndex() == 0x6c);
  REQUIRE(mmu.OAM[0].GetPriority() == false);
  REQUIRE(mmu.OAM[0].GetYFlip() == true);
  REQUIRE(mmu.OAM[0].GetXFlip() == true);
  REQUIRE(mmu.OAM[0].GetPallete() == true);
}
