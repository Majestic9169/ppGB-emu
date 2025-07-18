/*
 * tile.hpp
 * - api for working with tiles
 */

#ifndef TILE_H
#define TILE_H
#include <cstdint>
#include <vector>

enum LAYERS {
  BACKGROUND, // 32x32 tile map, scrollable
  WINDOW,     // also tilemap, for status bars etc
  OBJECT      // sprites and stuff, this will be separately covered
};

class TILE {
  // 16 bytes
private:
  std::vector<uint8_t> raw_data;
  std::vector<uint16_t> spliced_data;

public:
  TILE(std::vector<uint8_t>::iterator start, std::vector<uint8_t>::iterator end)
      : raw_data{}, spliced_data{} {
    for (auto it = start; it != end; it++) {
      raw_data.push_back(*it);
    }
    // 2 raw bytes are being interleaved together to get the final tile
    // check the ppu tests for a better idea
    // TODO: something cool?
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

  TILE &operator=(const TILE &a) {
    raw_data = a.raw_data;
    spliced_data = a.spliced_data;
    return *this;
  }

  TILE(const TILE &a) : raw_data{}, spliced_data{} {
    raw_data = a.raw_data;
    spliced_data = a.spliced_data;
  }

  std::vector<uint16_t> GetRenderedTile() const { return spliced_data; }
  std::vector<uint8_t> GetRawTile() const { return raw_data; }
};

#endif
