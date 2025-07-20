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

TILE::LAYERS FIFO::choose_layer() const {
  if (mmu->lcdc.isWindowEnable() && mmu->wy() <= mmu->ly()) {
    ;
  };
  return TILE::BACKGROUND;
}

void FIFO::start_fifo() {
  while (!fifo.empty()) {
    fifo.pop();
  }
  tile_line = (mmu->ly() + mmu->scy()) % 8;
  tile_column_index = mmu->scx() / 8;
  tile_row_index = (mmu->ly() + mmu->scy()) / 8;
  drop_pixels = mmu->scx() % 8;
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
      tile_id =
          mmu->read_byte(mmu->lcdc.BGTileMap() + (tile_row_index % 32) * 32 +
                         (tile_column_index % 32));
    } else if (layer == TILE::WINDOW) {
      tile_id =
          mmu->read_byte(mmu->lcdc.WindowTileMap() +
                         (tile_row_index % 32) * 32 + (tile_column_index % 32));
    } else {
      tile_id = mmu->read_byte(0x8000 + tile_column_index);
    }
    fifo_state = READ_TILE_DATA0;
    break;
  case READ_TILE_DATA0:
    tile_data_low =
        mmu->GetTileFromIndex(tile_id, layer).GetRawTile()[tile_line * 2];
    fifo_state = READ_TILE_DATA1;
    break;
  case READ_TILE_DATA1:
    tile_data_high =
        mmu->GetTileFromIndex(tile_id, layer).GetRawTile()[tile_line * 2 + 1];
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
      // dont push scx%8 if left edge
      if (tile_column_index == mmu->scx() / 8 && (bit - (drop_pixels)) > 0) {
        continue;
      }
      fifo.push(px);
    }
    tile_column_index++;
    fifo_state = READ_TILE_ID;
    break;
  }
}
