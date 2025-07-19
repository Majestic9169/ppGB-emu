/*
 * MMU
 * - Class to manage all ROM accesses
 * - should be shared across the other classes
 * - learncpp said std::vector for non-constexpr uses and i have to load from a
 * file so...
 */

#ifndef MMU_H
#define MMU_H

#include "../cli_opts.hpp"
#include "mmu_regs.hpp"
#include "palette.hpp"
#include "tile.hpp"
#include <cstdint>
#include <cstdio>
#include <sys/types.h>
#include <vector>

constexpr std::size_t ROM_SIZE{0x10000};

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

  void header_information();

public:
  void hexdump() const;

  MMU(Opts *opts_);

  uint8_t &read_byte(uint16_t addr);
  uint16_t read_word(uint16_t addr);
  void write_byte(uint16_t addr, uint8_t val);
  void write_word(uint16_t addr, uint16_t val);

  // PPU Access Shit

  // OAM
  std::vector<Object> OAM;

  // make lcdc usable
  LCDC_REG lcdc{ROM[0xFF40]};

  // stat reg
  STAT_REG stat{ROM[0xFF41]};

  // background scroll
  uint8_t &scy();
  uint8_t &scx();

  // ly and lyc
  uint8_t &ly();
  uint8_t &lyc();

  // palettes
  Palette BG_Palette{ROM[0xff47]};
  Palette OBP0{ROM[0xff48]};
  Palette OBP1{ROM[0xff49]};

  // window position
  uint8_t &wy();
  uint8_t &wx();

  // get tiles from either of the 2 tile maps
  TILE GetTileFromIndex(uint16_t index, TILE::LAYERS layer);
};

#endif
