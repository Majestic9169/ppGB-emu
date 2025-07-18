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
#include <sys/types.h>

// this is so fucking weird
// i hate it
FIFO::FIFO(MMU *_mmu) : mmu{_mmu}, fifo{} {}

void FIFO::start_fifo(TILE::LAYERS _layer, uint8_t _tile_line,
                      uint8_t _also_tile_line) {
  while (!fifo.empty()) {
    fifo.pop();
  }
  tile_index = 0;
  also_tile_line = _also_tile_line;
  layer = _layer;
  tile_line = _tile_line;
  fifo_state = READ_TILE_ID;
}

void FIFO::fifo_step() {
  ticks++;
  if (ticks < 2) {
    return;
  }
  ticks = 0;

  if (mmu->lcdc.isWindowEnable() && mmu->ly() >= mmu->wy() &&
      mmu->ly() < (mmu->wy() + 32)) {
    layer = TILE::WINDOW;
  } else {
    layer = TILE::BACKGROUND;
  }

  switch (fifo_state) {
  case READ_TILE_ID:
    if (layer == TILE::BACKGROUND) {
      tile_id = mmu->read_byte(mmu->lcdc.BGTileMap() + also_tile_line * 32 +
                               tile_index);
    } else if (layer == TILE::WINDOW) {
      tile_id = mmu->read_byte(mmu->lcdc.WindowTileMap() + tile_index);
    } else {
      // object tile map remains constant
      // TODO: implement object priorities
      tile_id = mmu->read_byte(0x8000 + tile_index);
    }
    fifo_state = READ_TILE_DATA0;
    break;
  case READ_TILE_DATA0:
    tile_data_low = mmu->GetTileFromIndex(tile_id, TILE::BACKGROUND)
                        .GetRawTile()[tile_line * 2];
    fifo_state = READ_TILE_DATA1;
    break;
  case READ_TILE_DATA1:
    tile_data_high = mmu->GetTileFromIndex(tile_id, TILE::BACKGROUND)
                         .GetRawTile()[tile_line * 2 + 1];
    fifo_state = PUSH;
    break;
  case PUSH:
    for (int bit = 7; bit >= 0; bit--) {
      // interleave 2 bytes
      uint8_t color = ((tile_data_high >> bit) & 0x01) << 1 |
                      ((tile_data_low >> bit) & 0x01);
      Pixel px;
      px.color = static_cast<Palette::Colors>(color);
      px.layer = layer;
      fifo.push(px);
    }
    tile_index++;
    fifo_state = READ_TILE_ID;
    break;
  }
}
