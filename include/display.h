#ifndef INCLUDE_DISPLAY
#define INCLUDE_DISPLAY

#include "cpu.h"
#include "memory.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <array>
#include <cstdint>
#include <string>

constexpr int WIDTH{160};
constexpr int HEIGHT{144};

class Display {
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  bool active;
  CPU *cpu;

  std::array<uint8_t, 160 * 144 * 4> viewport_pixels;
  SDL_FRect viewport_rect = {0, 0, WIDTH, HEIGHT};
  std::array<Color, 160 * 144> framebuffer;

public:
  Display(std::array<Color, 160 * 144> &framebuf, CPU *cpu_ptr,
          const int WINDOW_WIDTH = WIDTH, const int WINDOW_HEIGHT = HEIGHT,
          const char *WINDOW_NAME = "ppGB Emulator");
  bool render_frame();
  bool is_active();
  void poll_event();
  void draw();
  ~Display();
};

#endif
