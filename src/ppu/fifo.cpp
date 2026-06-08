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
FIFO::FIFO(MMU *_mmu, uint8_t &lx) : mmu{_mmu}, curr_lx{lx}, fifo{} {}

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

  // check for sprites on current scanline (sprite store populated by PPU)
  if (mmu->lcdc.areObjEnabled()) {
    for (const auto &s : sprite_store) {
      if (s.GetXPostition() == curr_lx + 8) {
        // HACK: replace instead of merging
        while (!fifo.empty())
          fifo.pop();

        sprite = s;
        // NOTE: the reason we do this bs is gb sprites of size 16 are locked to
        // having their top tile be the one with the 1 bit and the bottom one be
        // the onw with the 0 bit
        // anyway this helps pass the half mouth dmg-acid2 test
        uint8_t sprite_tile_id =
            (mmu->lcdc.ObjSize() == 16)
                ? ((sprite.tileIndex & 0xFE) | (tile_row_index() >= 8 ? 1 : 0))
                : sprite.tileIndex;
        uint8_t sprite_row_id = tile_row_index() % 8;
        uint8_t lo = mmu->GetTileFromIndex(sprite_tile_id, layer())
                         .GetRawTile()[2 * sprite_row_id];
        uint8_t hi = mmu->GetTileFromIndex(sprite_tile_id, layer())
                         .GetRawTile()[2 * sprite_row_id + 1];

        for (int bit = 7; bit >= 0; bit--) {
          int id = sprite.xFlip ? 7 - bit : bit;
          uint8_t color = ((hi >> id) & 0x01) << 1 | ((lo >> id) & 0x01);
          Pixel px;
          px.layer = layer();
          // px.color = sprite.palette ? mmu->OBP1.GetColor(color)
          //                           : mmu->OBP0.GetColor(color);
          // HACK: object palettes work but not for the waves :(
          px.color = mmu->BG_Palette.GetColor(color);
          fifo.push(px);
        }
        sprite.is_rendering = false;
        break;
      }
    }
  }

  switch (fifo_state) {
  case READ_TILE_ID: {
    sprite.is_rendering = false;
    TileAddr tile_addr{
        TileAddr(map_col_index(), map_row_index(), mmu->lcdc.BGTileMap())};
    if (renderingWindow()) {
      tile_addr.map = mmu->lcdc.WindowTileMap();
    }
    tile_id = mmu->read_byte(tile_addr);
    fifo_state = READ_TILE_DATA0;
  } break;
  case READ_TILE_DATA0:
    // if object update layer
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
      fifo.push(px);
    }
    lx += 8;
    fifo_state = READ_TILE_ID;
    break;
  }
}
