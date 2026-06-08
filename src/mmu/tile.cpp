/*
 * tile.hpp
 * - api for working with tiles
 */

#include "../../include/mmu/tile.hpp"
#include <cstdint>
#include <vector>

TILE::TILE(std::vector<uint8_t>::iterator start) : raw_data{start, 16} {
  // NOTE: i'm leaving behind this link
  // https://graphics.stanford.edu/~seander/bithacks.html
}

std::span<uint8_t, 16> TILE::GetRawTile() const { return raw_data; }
std::vector<uint16_t> TILE::GetRenderedTile() const {
  std::vector<uint16_t> spliced_data(8);
  for (int i = 0; i < 16; i += 2) {
    uint16_t res{0};
    for (int j = 0; j < 8; j++) {
      uint8_t x_mask = (raw_data[i] & (1 << j));
      uint8_t y_mask = (raw_data[i + 1] & (1 << j));

      res |= (x_mask << j);
      res |= (y_mask << (j + 1));
    }
    spliced_data[i / 2] = res;
  }
  return spliced_data;
}
