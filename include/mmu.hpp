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
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <vector>

constexpr std::size_t ROM_SIZE{0x10000};

class Palette {
private:
  enum Colors { WHITE, LIGHT_GRAY, DARK_GRAY, BLACK };
  uint8_t &palette_ref;
  Colors color_extractor(uint8_t color_index) const {
    return static_cast<Colors>((palette_ref & (0x03 << (2 * color_index))) >>
                               (2 * color_index));
  }

public:
  Palette(uint8_t &ref) : palette_ref{ref} {}
  Colors GetColor0() const { return color_extractor(0); }
  Colors GetColor1() const { return color_extractor(1); }
  Colors GetColor2() const { return color_extractor(2); }
  Colors GetColor3() const { return color_extractor(3); }
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
  bool getBit(int bit_no) const { return ff41_ref & (1 << bit_no); }

public:
  STAT_REG(uint8_t &ff41) : ff41_ref{ff41} {}

  // Not a fan of these names, lemme know any better conventions
  // Getters
  bool GetLYCIntSelect() const { return getBit(6); }
  bool GetMode2IntSelect() const { return getBit(5); }
  bool GetMode1IntSelect() const { return getBit(4); }
  bool GetMode0IntSelect() const { return getBit(3); }
  bool GetLYEqualLYC() const { return getBit(2); }

  // Setters
  void SetLYCIntSelect() { ff41_ref |= 0x40; }
  void SetMode2IntSelect() { ff41_ref |= 0x20; }
  void SetMode1IntSelect() { ff41_ref |= 0x10; }
  void SetMode0IntSelect() { ff41_ref |= 0x08; }
};

// objects are formed from the tiles maps, but they have extra data stored from
// 0xfe00-0xfe9f
// TODO: do something about the OAM DMA transfer
class Object {
private:
  // can't create structs because i need them to be references to the ROM
  // so i just store the bytes as references ig
  uint8_t &y_pos;
  uint8_t &x_pos;
  uint8_t &tile_index;
  uint8_t &attributes;
  bool getBit(int bit_no) const { return attributes & (1 << bit_no); }

public:
  Object(std::vector<uint8_t>::iterator start)
      : y_pos{*start}, x_pos{*(start + 1)}, tile_index{*(start + 2)},
        attributes{*(start + 3)} {};

  uint8_t GetYPostition() const { return y_pos; }
  uint8_t GetXPostition() const { return x_pos; }
  uint8_t GetTileIndex() const { return tile_index; }

  bool GetPriority() const { return getBit(7); }
  bool GetYFlip() const { return getBit(6); }
  bool GetXFlip() const { return getBit(5); }
  bool GetPallete() const { return getBit(4); }
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
  void hexdump() const { // Hexdump
    for (size_t i = 0; i < ROM.size(); i += 16) {
      // Print offset
      printf("%08zx  ", i);

      // Print hex bytes
      for (size_t j = 0; j < 16; ++j) {
        if (i + j < ROM.size())
          printf("%02X ", ROM[i + j]);
        else
          printf("   ");
      }

      printf(" ");

      // Print ASCII representation
      for (size_t j = 0; j < 16; ++j) {
        if (i + j < ROM.size()) {
          char c = ROM[i + j];
          printf("%c", (c >= 32 && c < 127) ? c : '.');
        }
      }
      printf("\n");
    }
  }

  MMU(Opts *opts_) : ROM(ROM_SIZE), cli_opts{opts_}, OAM{} {
    std::ifstream rom_file{cli_opts->rom_name(), std::ios::binary};
    if (!rom_file.good()) {
      std::cerr << RED << "[!] Error loading ROM_FILE\n" << COLOR_RESET;
      exit(2);
    }
    rom_file.read(reinterpret_cast<char *>(ROM.data()), ROM_SIZE);
    if (cli_opts->debug_enabled()) {
      std::cout << GRN << "[+] ROM successfully loaded\n" << COLOR_RESET;
    }
    // boot rom
    std::ifstream boot_rom_file{"./roms/sgb_bios.bin", std::ios::binary};
    if (!boot_rom_file.good()) {
      std::cerr << RED << "[!] Error loading BOOT_ROM_FILE\n" << COLOR_RESET;
      exit(2);
    }
    boot_rom_file.read(reinterpret_cast<char *>(ROM.data()), ROM_SIZE);
    if (cli_opts->debug_enabled()) {
      std::cout << GRN << "[+] BOOT_ROM successfully loaded\n" << COLOR_RESET;
      header_information();
    }

    // init OAM
    OAM.reserve(40);
    for (size_t i = 0; i < 40; i++) {
      OAM.emplace_back(ROM.begin() + 0xfe00 + (i * 4));
    }

    hexdump();
  }

  uint8_t &read_byte(uint16_t addr) { return ROM[addr]; }
  uint16_t read_word(uint16_t addr) { return ROM[addr] | ROM[addr + 1] << 8; }
  void write_byte(uint16_t addr, uint8_t val) { ROM[addr] = val; }
  void write_word(uint16_t addr, uint16_t val) {
    write_byte(addr, static_cast<uint8_t>(val));
    write_byte(addr + 1, static_cast<uint8_t>(val >> 8));
  }

  // PPU Access Shit

  // OAM
  std::vector<Object> OAM;

  // make lcdc usable
  LCDC_REG lcdc{ROM[0xFF40]};

  // stat reg
  // TODO: add ly and lyc reg somewhere
  STAT_REG stat{ROM[0xFF41]};

  // background scroll
  uint8_t &scy() { return ROM[0xff42]; }
  uint8_t &scx() { return ROM[0xff43]; }

  // ly and lyc
  uint8_t &ly() { return ROM[0xff44]; }
  uint8_t &lyc() { return ROM[0xff45]; }

  // palettes
  Palette BG_Palette{ROM[0xff47]};
  Palette OBP0{ROM[0xff48]};
  Palette OBP1{ROM[0xff49]};

  // window position
  uint8_t &wy() { return ROM[0xff4a]; }
  uint8_t &wx() { return ROM[0xff4b]; }

  // get tiles from either of the 2 tile maps
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
