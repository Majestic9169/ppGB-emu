/*
 * MMU
 * - Class to manage all ROM accesses
 * - should be shared across the other classes
 * - learncpp said std::vector for non-constexpr uses and i have to load from a
 * file so...
 */

#include "../../include/mmu/mmu.hpp"
#include <cstdio>
#include <fstream>
#include <sys/types.h>

MMU::MMU(Opts *opts_) : ROM(ROM_SIZE), cli_opts{opts_}, OAM{} {
  std::ifstream rom_file{cli_opts->rom_name(), std::ios::binary};

  if (!rom_file.good()) {
    std::cerr << RED << "[!] Error loading ROM_FILE\n" << COLOR_RESET;
    exit(2);
  }

  rom_file.read(reinterpret_cast<char *>(ROM.data()), ROM_SIZE);
  if (cli_opts->debug_enabled()) {
    std::cout << GRN << "[+] ROM successfully loaded\n" << COLOR_RESET;
  }

  // init OAM
  OAM.reserve(40);
  for (size_t i = 0; i < 40; i++) {
    OAM.emplace_back(ROM.begin() + 0xfe00 + (i * 4));
  }

  // hexdump();
}

void MMU::hexdump() const {
  for (size_t i = 0; i < ROM.size(); i += 16) {
    // offset
    printf("%08zx  ", i);
    // hex bytes
    for (size_t j = 0; j < 16; ++j) {
      if (i + j < ROM.size())
        printf("%02X ", ROM[i + j]);
      else
        printf("   ");
    }
    printf(" ");
    // ASCII representation
    for (size_t j = 0; j < 16; ++j) {
      if (i + j < ROM.size()) {
        char c = ROM[i + j];
        printf("%c", (c >= 32 && c < 127) ? c : '.');
      }
    }
    printf("\n");
  }
}

uint8_t &MMU::read_byte(uint16_t addr) { return ROM[addr]; }
uint16_t MMU::read_word(uint16_t addr) {
  return ROM[addr] | ROM[addr + 1] << 8;
}
void MMU::write_byte(uint16_t addr, uint8_t val) { ROM[addr] = val; }
void MMU::write_word(uint16_t addr, uint16_t val) {
  write_byte(addr, static_cast<uint8_t>(val));
  write_byte(addr + 1, static_cast<uint8_t>(val >> 8));
}

// PPU Access Shit

// background scroll
uint8_t &MMU::scy() { return ROM[0xff42]; }
uint8_t &MMU::scx() { return ROM[0xff43]; }

// ly and lyc
uint8_t &MMU::ly() { return ROM[0xff44]; }
uint8_t &MMU::lyc() { return ROM[0xff45]; }

// window position
uint8_t &MMU::wy() { return ROM[0xff4a]; }
uint8_t &MMU::wx() { return ROM[0xff4b]; }

// get tiles from either of the 2 tile maps
TILE MMU::GetTileFromIndex(uint16_t index, TILE::LAYERS layer) {
  if (layer == TILE::OBJECT) {
    TILE tile{ROM.begin() + 0x8000 + index * 16,
              ROM.begin() + 0x8000 + index * 16 + 16};
    return tile;
  } else if (lcdc.TileMap() == 0x8000) {
    TILE tile{ROM.begin() + 0x8000 + index * 16,
              ROM.begin() + 0x8000 + index * 16 + 16};
    return tile;
  } else {
    if (index < 128) {
      TILE tile{ROM.begin() + 0x9000 + index * 16,
                ROM.begin() + 0x9000 + index * 16 + 16};
      return tile;
    } else {
      TILE tile{ROM.begin() + 0x8800 + (index - 128) * 16,
                ROM.begin() + 0x8800 + (index - 128) * 16 + 16};
      return tile;
    }
  }
}
