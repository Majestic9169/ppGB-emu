#include <cstdint>
#define CATCH_CONFIG_MAIN

#include "../include/catch.hpp"
#include "../include/cli_opts.hpp"
#include "../include/ppu.hpp"
#include "../include/reg.hpp"

int argc = 2;
char *argv[2] = {strdup("./ppGB"), strdup("../roms/tetris.gb")};
Opts cli_opts{argc, argv};
MMU mmu{&cli_opts};
Registers reg{};

PPU ppu{&cli_opts, &mmu};

TEST_CASE("tile splicing check", "[ppu, tile]") {
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

  TILE tile = mmu.GetTileFromIndex(0);

  std::vector<uint16_t> rendered_tile{0x2ff8, 0x300c, 0x300c, 0x300c,
                                      0x37fc, 0x15dc, 0x3778, 0x2fe0};
  REQUIRE(tile.GetRenderedTile() == rendered_tile);
}
