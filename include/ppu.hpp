/*
 * ppu.hpp
 * - code for all direct graphics handling using SDL
 * - is the bridge between the gb vram and the sdl surface
 */

#ifndef PPU_H
#define PPU_H
#include "mmu.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

constexpr int WIDTH{160};
constexpr int HEIGHT{144};

class PPU {
private:
  Opts *cli;
  MMU *mmu;
  SDL_Window *SDLWindow;
  SDL_Event *E;

  PPU(const PPU &) = delete;
  PPU &operator=(const PPU &) = delete;

public:
  // TODO: add checks to make sure SDL is initted
  // TODO: add proper clang-format file
  PPU(Opts *cli_, MMU *mmu_)
      : cli{cli_}, mmu{mmu_},
        SDLWindow{SDL_CreateWindow("ppGB Window", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0)},
        E{} {};
};

#endif
