/*
 * ppu.hpp
 * - code for all direct graphics handling using SDL
 * - is the bridge between the gb vram and the sdl surface
 */

#ifndef PPU_H
#define PPU_H

#include "../mmu/mmu.hpp"
#include "fifo.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <sys/types.h>

constexpr int WIDTH{160};
constexpr int HEIGHT{144};

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
  uint8_t scx{0}; // only read during oam scan
  int ticks{0};

  PPU(const PPU &) = delete;
  PPU &operator=(const PPU &) = delete;

  SDL_Surface *GetSurface() const;

  Uint32 MapColorToSDL(Palette::Colors color, SDL_PixelFormat *format);

  void SetPixel(Uint32 color);

public:
  // TODO: add checks to make sure SDL is initted
  // TODO: add proper clang-format file
  PPU(Opts *cli_, MMU *mmu_);

  void ppu_step();

  void Update();

  void print_debug() const;
};

#endif
