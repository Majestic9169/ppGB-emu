/*
 * tile.hpp
 * - api for working with tiles
 */

#ifndef TILE_H
#define TILE_H
#include <cstdint>
#include <span>
#include <vector>

class TILE {
  // 16 bytes
private:
  std::span<uint8_t, 16> raw_data;

public:
  enum LAYERS {
    BACKGROUND, // 32x32 tile map, scrollable
    WINDOW,     // also tilemap, for status bars etc
    OBJECT      // sprites and stuff, this will be separately covered
  };

  TILE(std::vector<uint8_t>::iterator start);

  std::span<uint8_t, 16> GetRawTile() const;
};

#endif
