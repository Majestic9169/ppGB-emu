/*
 * tile.hpp
 * - api for working with tiles
 */

#include "../../include/mmu/tile.hpp"
#include <cstdint>
#include <vector>

TILE::TILE(std::vector<uint8_t>::iterator start,
           std::vector<uint8_t>::iterator end)
    : raw_data{}, spliced_data{} {
  for (auto it = start; it != end; it++) {
    raw_data.push_back(*it);
  }
  // 2 raw bytes are being interleaved together to get the final tile
  // check the ppu tests for a better idea
  // https://graphics.stanford.edu/~seander/bithacks.html
  spliced_data.reserve(8);
  for (int i = 0; i < 16; i += 2) {
    uint16_t res = 0;
    for (int j = 0; j < 8; j++) {
      uint8_t x_mask = (raw_data[i] & (1 << j));
      uint8_t y_mask = (raw_data[i + 1] & (1 << j));

      res |= (x_mask << j);
      res |= (y_mask << (j + 1));
    }
    spliced_data.emplace_back(res);
  }
}

TILE &TILE::operator=(const TILE &a) {
  raw_data = a.raw_data;
  spliced_data = a.spliced_data;
  return *this;
}

TILE::TILE(const TILE &a) : raw_data{}, spliced_data{} {
  raw_data = a.raw_data;
  spliced_data = a.spliced_data;
}

std::vector<uint16_t> TILE::GetRenderedTile() const { return spliced_data; }
std::vector<uint8_t> TILE::GetRawTile() const { return raw_data; }
