/*
 * ppu.hpp
 * - code for all direct graphics handling using SDL
 * - is the bridge between the gb vram and the sdl surface
 */

#include "../../include/ppu/ppu.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <sys/types.h>

SDL_Surface *PPU::GetSurface() const { return SDL_GetWindowSurface(SDLWindow); }

Uint32 PPU::MapColorToSDL(Palette::Colors color, SDL_PixelFormat *format) {
  switch (color) {
  case Palette::WHITE:
    return SDL_MapRGBA(format, 224, 248, 208, 255); // GB white
  case Palette::LIGHT_GRAY:
    return SDL_MapRGBA(format, 136, 192, 112, 255);
  case Palette::DARK_GRAY:
    return SDL_MapRGBA(format, 52, 104, 86, 255);
  case Palette::BLACK:
    return SDL_MapRGBA(format, 8, 24, 32, 255);
  default:
    return SDL_MapRGBA(format, 255, 0, 255, 255); // Magenta for error
  }
}

void PPU::SetPixel(Uint32 color) {
  Uint8 *pixelPtr = (Uint8 *)GetSurface()->pixels + ly * GetSurface()->pitch +
                    lx * GetSurface()->format->BytesPerPixel;
  *(Uint32 *)pixelPtr = color;
}

PPU::PPU(Opts *cli_, MMU *mmu_)
    : cli{cli_}, mmu{mmu_},
      SDLWindow{SDL_CreateWindow("ppGB Window", SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0)},
      pixel_fifo{mmu_}, ly{mmu->ly()} {};

void PPU::ppu_step() {
  GetSurface();
  ticks++;
  switch (ppu_state) {
  case MODE2_OAM_SCAN:
    lx = 0;
    // 2 ticks per object, 40 objects
    if (ticks == 80) {
      lx = 0;
      pixel_fifo.start_fifo();
      ppu_state = MODE3_PIXEL_TRANSFER;
    }
    break;
  case MODE3_PIXEL_TRANSFER:
    pixel_fifo.fifo_step();
    // don't pop if fifo has less than 8 pixels
    if (pixel_fifo.fifo.size() <= 8) {
      break;
    }
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

void PPU::Update() { SDL_UpdateWindowSurface(SDLWindow); }

// TODO: add debug levels
void PPU::print_debug() const {
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
