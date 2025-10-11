/*
 * fifo.hpp
 * - code for the pixel fetcher of the ppu
 * - very complex and weird, operates at half the speed of the ppu
 */

#include "../../include/ppu/fifo.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <sys/types.h>

// this is so fucking weird
// i hate it
FIFO::FIFO(MMU *_mmu) : mmu{_mmu}, fifo{} {}

void FIFO::start_fifo() {
  while (!fifo.empty()) {
    fifo.pop();
  }
  lx = 0;
  drop_pixels = mmu->scx() % 8;
  fifo_state = READ_TILE_ID;
}

void FIFO::fifo_step() {
  ticks++;

  if (ticks < 2) {
    return;
  }

  ticks = 0;

  switch (fifo_state) {
  case READ_TILE_ID: {
    TileAddr tile_addr{
        TileAddr(map_col_index(), map_row_index(), mmu->lcdc.BGTileMap())};
    if (renderingWindow()) {
      tile_addr.map = mmu->lcdc.WindowTileMap();
      tile_id = mmu->read_byte(tile_addr);
    } else {
      tile_id = mmu->read_byte(tile_addr);
    }
    fifo_state = READ_TILE_DATA0;
  } break;
  case READ_TILE_DATA0:
    tile_data_low = mmu->GetTileFromIndex(tile_id, layer())
                        .GetRawTile()[tile_row_index() * 2];
    fifo_state = READ_TILE_DATA1;
    break;
  case READ_TILE_DATA1:
    tile_data_high = mmu->GetTileFromIndex(tile_id, layer())
                         .GetRawTile()[tile_row_index() * 2 + 1];
    fifo_state = PUSH;
    break;
  case PUSH:
    for (int bit = 7; bit >= 0; bit--) {
      // interleave 2 bytes
      uint8_t color = ((tile_data_high >> bit) & 0x01) << 1 |
                      ((tile_data_low >> bit) & 0x01);
      Pixel px;
      px.color = mmu->BG_Palette.GetColor(color);
      px.layer = layer();
      // dont push scx%8 if left edge
      if (!renderingWindow()) {
        if (map_col_index() == mmu->scx() / 8 && drop_pixels > 0) {
          drop_pixels--;
          continue;
        }
      }
      fifo.push(px);
    }
    lx += 8;
    fifo_state = READ_TILE_ID;
    break;
  }
}
