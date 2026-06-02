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
