#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"
#include "../include/cpu.h"
#include "../include/display.h"
#include <fstream>

TEST_CASE("BOOT REGISTER TEST") {
  std::ifstream ROM;
  ROM.open("../roms/Tetris.gb", std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(gb.framebuffer);

  REQUIRE(gb.read_byte(0xFF50) == 0xFF);

  ROM.close();
}
