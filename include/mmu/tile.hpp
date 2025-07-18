/*
 * tile.hpp
 * - api for working with tiles
 */

#ifndef TILE_H
#define TILE_H
#include <cstdint>
#include <vector>

class TILE {
  // 16 bytes
private:
  std::vector<uint8_t> raw_data;
  std::vector<uint16_t> spliced_data;

public:
  enum LAYERS {
    BACKGROUND, // 32x32 tile map, scrollable
    WINDOW,     // also tilemap, for status bars etc
    OBJECT      // sprites and stuff, this will be separately covered
  };

  TILE(std::vector<uint8_t>::iterator start,
       std::vector<uint8_t>::iterator end);

  TILE &operator=(const TILE &a);

  TILE(const TILE &a);

  std::vector<uint16_t> GetRenderedTile() const;
  std::vector<uint8_t> GetRawTile() const;
};

#endif
