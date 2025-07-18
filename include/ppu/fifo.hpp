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

  MMU *mmu;
  int ticks{0};
  TILE::LAYERS layer{TILE::BACKGROUND};
  uint8_t tile_line{0};
  uint8_t also_tile_line{0};
  uint8_t tile_index{0};
  uint8_t tile_data_low{0};
  uint8_t tile_data_high{0};
  uint8_t tile_id{0};

  FIFO(const FIFO &) = delete;
  const FIFO &operator=(const FIFO &) = delete;

public:
  std::queue<Pixel> fifo;

  FIFO(MMU *_mmu);

  void start_fifo(TILE::LAYERS _layer, uint8_t _tile_line,
                  uint8_t _also_tile_line);

  void fifo_step();
};

#endif
