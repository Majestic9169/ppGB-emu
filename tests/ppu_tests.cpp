#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"
#include "../include/cpu.h"
#include "../include/display.h"
#include <fstream>

TEST_CASE("GPU LCD Control Register and Flags", "[gpu]") {
  std::ifstream ROM;
  ROM.open("../roms/tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);
  Display display(gb.framebuffer, &gb);

  SECTION("LCD Enable/Disable") {
    // Test LCD Enable flag
    gb.write_byte(0xFF40, 0x80);
    REQUIRE(gb.flag_value(LCD_ENABLE, LCD_CONTROL) == 1);

    gb.write_byte(0xFF40, 0x00);
    REQUIRE(gb.flag_value(LCD_ENABLE, LCD_CONTROL) == 0);
  }

  SECTION("LCD Status Register Flags") {
    // Test LYC=LY Coincidence Flag
    gb.write_byte(0xFF44, 0x50);
    gb.write_byte(0xFF45, 0x50);
    gb.PPU_STEP();
    REQUIRE(gb.flag_value(COINCIDENCE_FLAG, LCD_STATUS) == 1);

    // Test Mode Flags
    gb.mode = HBLANK;
    gb.PPU_STEP();
    REQUIRE((gb.read_byte(0xFF41) & 0x03) == OAM); // Should transition to OAM

    gb.mode = VBLANK;
    gb.write_byte(0xFF44, 153); // Last scanline
    gb.PPU_STEP();
    REQUIRE(gb.read_byte(0xFF44) == 0); // Should reset to scanline 0
    REQUIRE((gb.read_byte(0xFF41) & 0x03) == OAM);
  }

  SECTION("Background and Window Control") {
    // Test Background Map Select
    gb.write_byte(0xFF40, 0x08); // Set bit 3 for alternate background map
    REQUIRE(gb.flag_value(BG_MAP_SELECT, LCD_CONTROL) == 1);

    // Test Window Enable
    gb.write_byte(0xFF40, 0x20); // Set bit 5 to enable window
    REQUIRE(gb.flag_value(WIN_ENABLE, LCD_CONTROL) == 1);

    // Test Sprite Size
    gb.write_byte(0xFF40, 0x04); // Set bit 2 for 8x16 sprites
    REQUIRE(gb.flag_value(SPR_SIZE, LCD_CONTROL) == 1);
  }

  SECTION("Scroll Registers") {
    // Test Scroll X/Y
    gb.write_byte(0xFF42, 0x45); // Set Scroll Y
    gb.write_byte(0xFF43, 0x23); // Set Scroll X
    REQUIRE(gb.read_byte(0xFF42) == 0x45);
    REQUIRE(gb.read_byte(0xFF43) == 0x23);
  }

  SECTION("Window Position") {
    // Test Window X/Y position
    gb.write_byte(0xFF4A, 0x55); // Set Window Y
    gb.write_byte(0xFF4B, 0x33); // Set Window X
    REQUIRE(gb.read_byte(0xFF4A) == 0x55);
    REQUIRE(gb.read_byte(0xFF4B) == 0x33);
  }

  SECTION("PPU Mode Transitions") {
    // Test complete PPU cycle
    gb.write_byte(0xFF40, 0x80); // Enable LCD
    gb.mode = HBLANK;

    // HBLANK -> OAM
    gb.PPU_STEP();
    REQUIRE((gb.read_byte(0xFF41) & 0x03) == OAM);

    // OAM -> DRAW
    gb.PPU_STEP();
    REQUIRE((gb.read_byte(0xFF41) & 0x03) == DRAW);

    // DRAW -> HBLANK
    gb.PPU_STEP();
    REQUIRE((gb.read_byte(0xFF41) & 0x03) == HBLANK);

    // Test VBLANK entry
    gb.write_byte(0xFF44, 143); // Set to last visible scanline
    gb.mode = HBLANK;
    gb.PPU_STEP();
    REQUIRE(gb.mode == VBLANK);
    REQUIRE(gb.can_render == true);
  }

  SECTION("Sprite Attributes") {
    // Setup a test sprite
    gb.write_byte(0xFE00, 0x10); // Y position
    gb.write_byte(0xFE01, 0x20); // X position
    gb.write_byte(0xFE02, 0x05); // Tile number
    gb.write_byte(0xFE03, 0x80); // Attributes (priority enabled)

    bool pixel_row[160] = {0};
    gb.scanline_sprites(pixel_row);

    // Test sprite visibility
    gb.write_byte(0xFF44, 0x10); // Set current scanline to match sprite Y
    REQUIRE(gb.read_byte(0xFE00) == 0x10); // Y position
    REQUIRE(gb.read_byte(0xFE01) == 0x20); // X position
  }

  ROM.close();
}
