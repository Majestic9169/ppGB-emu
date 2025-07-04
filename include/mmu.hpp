/*
 * MMU
 * - Class to manage all ROM accesses
 * - should be shared across the other classes
 * - learncpp said std::vector for non-constexpr uses and i have to load from a
 * file so...
 */

#ifndef MMU_H
#define MMU_H

#include "cli_opts.hpp"
#include "tile.hpp"
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

constexpr std::size_t ROM_SIZE{0x10000};

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
  bool getBit(int bit_no) const { return ff40_ref & (1 << bit_no); }

public:
  LCDC_REG(uint8_t &ff40) : ff40_ref{ff40} {}

  bool isLCDenabled() const { return getBit(7); }
  uint16_t WindowTileMap() const { return getBit(6) ? 0x9c00 : 0x9800; }
  bool isWindowEnable() const { return getBit(5); }
  uint16_t TileMap() const { return getBit(4) ? 0x8000 : 0x8800; }
  uint16_t BGTileMap() const { return getBit(3) ? 0x9c00 : 0x9800; }
  uint8_t ObjSize() const { return getBit(2) ? 2 : 1; } // 2 tiles or 1 tile
  bool areObjEnabled() const { return getBit(1); }
  bool BGWindowEnable() const { return getBit(0); }
};

class MMU {
private:
  // i considered std::byte but that's really restrictive and i usually need to
  // work with numbers
  std::vector<std::uint8_t> ROM;
  Opts *cli_opts;
  // delete copy constructor and assignment operator, don't wanna mess with
  // pointers. look at this btw lol
  // https://stackoverflow.com/questions/33776t697/
  MMU(const MMU &) = delete;
  MMU &operator=(const MMU &) = delete;

  void header_information() {
    std::string title{std::string{ROM.begin() + 0x134, ROM.begin() + 0x143}};
    bool gbc_support = ROM[0x143] == 0x80 || ROM[0x143] == 0xc0 ? true : false;
    int rom_size = 0x8000 * (1 << ROM[0x148]);
    int rom_banks = rom_size / 0x4000;
    std::cout << YEL << "[~] ROM Header Information\n" << COLOR_RESET;
    std::cout << "Title       " << title << std::endl;
    std::cout << "GBC support " << std::boolalpha << gbc_support
              << std::noboolalpha << std::endl;
    std::cout << "ROM size    " << rom_size << std::endl;
    std::cout << "No. Banks   " << rom_banks << std::endl;
  }

public:
  MMU(Opts *opts_) : ROM(ROM_SIZE), cli_opts{opts_} {
    std::ifstream rom_file{cli_opts->rom_name(), std::ios::binary};
    if (!rom_file.good()) {
      std::cerr << RED << "[!] Error loading ROM_FILE\n" << COLOR_RESET;
      exit(2);
    }
    rom_file.read(reinterpret_cast<char *>(ROM.data()), ROM_SIZE);
    if (cli_opts->debug_enabled()) {
      std::cout << GRN << "[+] ROM successfully loaded\n" << COLOR_RESET;
      header_information();
    }
  }

  uint8_t &read_byte(uint16_t addr) { return ROM[addr]; }
  uint16_t read_word(uint16_t addr) { return ROM[addr] | ROM[addr + 1] << 8; }
  void write_byte(uint16_t addr, uint8_t val) { ROM[addr] = val; }
  void write_word(uint16_t addr, uint16_t val) {
    write_byte(addr, static_cast<uint8_t>(val));
    write_byte(addr + 1, static_cast<uint8_t>(val >> 8));
  }

  // PPU Access
  LCDC_REG lcdc{ROM[0xFF40]};

  TILE GetTileFromIndex(uint16_t index, LAYERS layer) {
    if (layer == OBJECT) {
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
};

#endif
