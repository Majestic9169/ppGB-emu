/*
 * ppu.hpp
 * - code for all direct graphics handling using SDL
 * - is the bridge between the gb vram and the sdl surface
 */

#ifndef PPU_H
#define PPU_H

#include "mmu.hpp"
#include "tile.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <queue>
#include <sys/types.h>

constexpr int WIDTH{160};
constexpr int HEIGHT{144};

struct Pixel {
  // TODO: shift Colors enum to a separate place
  enum Colors { WHITE, LIGHT_GRAY, DARK_GRAY, BLACK } color;
  LAYERS layer;
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
  // TODO: maybe ticks should be shared between fifo and ppu
  int ticks{0};
  LAYERS layer{BACKGROUND};
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

  FIFO(MMU *_mmu) : mmu{_mmu}, fifo{} {}

  void start_fifo(LAYERS _layer, uint8_t _tile_line, uint8_t _also_tile_line) {
    while (!fifo.empty()) {
      fifo.pop();
    }
    tile_index = 0;
    also_tile_line = _also_tile_line;
    layer = _layer;
    tile_line = _tile_line;
    fifo_state = READ_TILE_ID;
  }

  void fifo_step() {
    ticks++;
    if (ticks < 2) {
      return;
    }
    ticks = 0;

    if (mmu->lcdc.isWindowEnable() && mmu->ly() >= mmu->wy() &&
        mmu->ly() < (mmu->wy() + 32)) {
      layer = WINDOW;
    } else {
      layer = BACKGROUND;
    }

    switch (fifo_state) {
    case READ_TILE_ID:
      if (layer == BACKGROUND) {
        printf("queueing tile index 0x%04x\n",
               mmu->lcdc.BGTileMap() + tile_index);
        tile_id = mmu->read_byte(mmu->lcdc.BGTileMap() + also_tile_line * 32 +
                                 tile_index);
      } else if (layer == WINDOW) {
        tile_id = mmu->read_byte(mmu->lcdc.WindowTileMap() + tile_index);
      } else {
        // object tile map remains constant
        // TODO: implement object priorities
        tile_id = mmu->read_byte(0x8000 + tile_index);
      }
      fifo_state = READ_TILE_DATA0;
      break;
    case READ_TILE_DATA0:
      tile_data_low = mmu->GetTileFromIndex(tile_id, BACKGROUND)
                          .GetRawTile()[tile_line * 2];
      fifo_state = READ_TILE_DATA1;
      break;
    case READ_TILE_DATA1:
      tile_data_high = mmu->GetTileFromIndex(tile_id, BACKGROUND)
                           .GetRawTile()[tile_line * 2 + 1];
      fifo_state = PUSH;
      break;
    case PUSH:
      for (int bit = 7; bit >= 0; bit--) {
        // interleave 2 bytes
        uint8_t color = ((tile_data_high >> bit) & 0x01) << 1 |
                        ((tile_data_low >> bit) & 0x01);
        Pixel px;
        px.color = static_cast<Pixel::Colors>(color);
        px.layer = layer;
        fifo.push(px);
      }
      tile_index++;
      fifo_state = READ_TILE_ID;
      break;
    }
  }
};

class PPU {
private:
  enum PPU_STATE {
    MODE0_HBLANK,
    MODE1_VBLANK,
    MODE2_OAM_SCAN,
    MODE3_PIXEL_TRANSFER
  } ppu_state{MODE3_PIXEL_TRANSFER};

  Opts *cli;
  MMU *mmu;
  SDL_Window *SDLWindow;
  FIFO pixel_fifo;

  uint8_t &ly;
  uint8_t lx{0};
  int ticks{0};

  PPU(const PPU &) = delete;
  PPU &operator=(const PPU &) = delete;

  SDL_Surface *GetSurface() const { return SDL_GetWindowSurface(SDLWindow); }

  Uint32 MapColorToSDL(Pixel::Colors color, SDL_PixelFormat *format) {
    switch (color) {
    case Pixel::WHITE:
      return SDL_MapRGBA(format, 224, 248, 208, 255); // GB white
    case Pixel::LIGHT_GRAY:
      return SDL_MapRGBA(format, 136, 192, 112, 255);
    case Pixel::DARK_GRAY:
      return SDL_MapRGBA(format, 52, 104, 86, 255);
    case Pixel::BLACK:
      return SDL_MapRGBA(format, 8, 24, 32, 255);
    default:
      return SDL_MapRGBA(format, 255, 0, 255, 255); // Magenta for error
    }
  }

  void SetPixel(Uint32 color) {
    Uint8 *pixelPtr = (Uint8 *)GetSurface()->pixels + ly * GetSurface()->pitch +
                      lx * GetSurface()->format->BytesPerPixel;
    *(Uint32 *)pixelPtr = color;
  }

public:
  // TODO: add checks to make sure SDL is initted
  // TODO: add proper clang-format file
  PPU(Opts *cli_, MMU *mmu_)
      : cli{cli_}, mmu{mmu_},
        SDLWindow{SDL_CreateWindow("ppGB Window", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0)},
        pixel_fifo{mmu_}, ly{mmu->ly()} {};

  void ppu_step() {
    GetSurface();
    ticks++;
    switch (ppu_state) {
    case MODE2_OAM_SCAN:
      lx = 0;
      // 2 ticks per object, 40 objects
      if (ticks == 80) {
        lx = 0;
        pixel_fifo.start_fifo(BACKGROUND, ly % 8, ly / 8);
        ppu_state = MODE3_PIXEL_TRANSFER;
      }
      break;
    case MODE3_PIXEL_TRANSFER:
      pixel_fifo.fifo_step();
      // if (pixel_fifo.fifo.size() <= 8) {
      //   pixel_fifo.fifo_step();
      // }
      if (!pixel_fifo.fifo.empty()) {
        Pixel px = pixel_fifo.fifo.front();
        pixel_fifo.fifo.pop();
        SetPixel(MapColorToSDL(px.color, GetSurface()->format));
        lx++;
      }
      // HBLANK is entered after each row of pixels has been rendered
      if (lx == 160) {
        ppu_state = MODE0_HBLANK;
      }
      break;
    case MODE0_HBLANK:
      if (ticks == 456) {
        ticks = 0;
        ly++;
        // VBLANK is entered after an entire frame has been rendered
        if (ly == 144) {
          ppu_state = MODE1_VBLANK;
        } else {
          ppu_state = MODE2_OAM_SCAN;
        }
      }
      break;
    case MODE1_VBLANK:
      if (ticks == 456) {
        ticks = 0;
        ly++;
        // wait 10 more rows before moving to next frame
        if (ly == 153) {
          ly = 0;
          ppu_state = MODE2_OAM_SCAN;
        }
      }
      break;
    }
  }

  void Update() { SDL_UpdateWindowSurface(SDLWindow); }

  void print_debug() const {
    printf("ly: %d, lx: %d\n", ly, lx);
    printf("lcdc = %4x\n", mmu->read_byte(0xff40));
    printf("stat = %4x\n", mmu->read_byte(0xff41));
    if (ppu_state == MODE0_HBLANK) {
      printf("state: hblank\n");
    } else if (ppu_state == MODE1_VBLANK) {
      printf("state: vblank\n");
    } else if (ppu_state == MODE2_OAM_SCAN) {
      printf("state: oam scan\n");
    } else {
      printf("state: pixel transfer\n");
    }
  }
};

#endif
