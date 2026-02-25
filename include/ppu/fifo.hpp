/*
 * fifo.hpp
 * - code for the pixel fetcher of the ppu
 * - very complex and weird, operates at half the speed of the ppu
 */

#ifndef FIFO_H
#define FIFO_H

#include "../mmu/mmu.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <queue>
#include <sys/types.h>

struct Pixel {
  Palette::Colors color;
  TILE::LAYERS layer;
};

// this is so fucking weird
// i hate it
class FIFO {
private:
  enum FIFO_STATE {
    READ_TILE_ID,
    READ_TILE_DATA0,
    READ_TILE_DATA1,
    PUSH
  } fifo_state{READ_TILE_ID};

  struct TileAddr {
    uint8_t x : 5;
    uint8_t y : 5;
    uint8_t map : 1;
    uint8_t pre : 5;

    TileAddr(uint8_t x_, uint8_t y_, uint8_t map_)
        : x{x_}, y{y_}, map{map_}, pre{0b10011} {};

    operator uint16_t() const {
      return (pre << 11) | (map << 10) | (y << 5) | x;
    }
  };

  MMU *mmu;
  int ticks{0};
  uint8_t lx{0};

  bool renderingWindow() const {
    bool in_window = mmu->ly() >= mmu->wy();
    in_window = in_window && (mmu->wx() - 8 < lx);
    in_window = in_window && mmu->lcdc.isWindowEnable();
    return in_window;
  }

  // returns the row inside a tile to render
  uint8_t tile_row_index() const {
    if (renderingWindow()) {
      return (mmu->ly() - mmu->wy()) % 8;
    } else {
      return (mmu->ly() + mmu->scy()) % 8;
    }
  }

  // return row index of tile to select in map
  uint8_t map_row_index() const {
    if (renderingWindow()) {
      return (mmu->ly() - mmu->wy()) / 8;
    } else {
      return (uint8_t(mmu->ly() + mmu->scy()) / 8);
    }
  }

  // return col index of tile to select in map
  uint8_t map_col_index() const {
    if (renderingWindow()) {
      return (lx - mmu->wx() + 7) / 8;
    } else {
      return ((lx + mmu->scx()) / 8);
    }
  }

  TILE::LAYERS layer{TILE::BACKGROUND};
  uint8_t tile_data_low{0};
  uint8_t tile_data_high{0};
  uint8_t tile_id{0};
  uint8_t drop_pixels{0};

  FIFO(const FIFO &) = delete;
  const FIFO &operator=(const FIFO &) = delete;

  TILE::LAYERS choose_layer() const;

public:
  std::queue<Pixel> fifo;

  std::vector<Object> sprite_store{};

  FIFO(MMU *_mmu);

  void start_fifo();

  void fifo_step();
};

#endif
