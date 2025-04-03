#include "../include/display.h"
#include "../include/cpu.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory.h>

Display::Display(std::array<Color, GB_WIDTH * GB_HEIGHT> &framebuf,
                 CPU *cpu_ptr, const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
                 const char *WINDOW_NAME)
    : framebuffer(framebuf), window(nullptr), renderer(nullptr),
      texture(nullptr), active(false), cpu(cpu_ptr), window_width(WINDOW_WIDTH),
      window_height(WINDOW_HEIGHT), window_title(WINDOW_NAME) {
  // Initialize SDL2
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("SDL2 Initialization Failed: %s\n", SDL_GetError());
    exit(1);
  }

  // Create window with proper flags
  window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (!window) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    exit(2);
  }

  // Create renderer with vsync enabled
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    exit(3);
  }

  // Create texture for the GameBoy display
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, GB_WIDTH, GB_HEIGHT);

  if (!texture) {
    SDL_Log("Failed to create texture: %s", SDL_GetError());
    exit(4);
  }

  // Set up initial render state
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  active = true;
}

bool Display::is_active() const { return active; }

bool Display::render_frame() {
  if (!active)
    return false;

  // Clear the renderer
  // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  // SDL_RenderClear(renderer);

  // Update the texture with new frame data
  draw();

  // Render the texture
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  return true;
}

void Display::handle_key_event(const SDL_Event &event, bool is_key_down) {
  switch (event.key.keysym.sym) {
  case SDLK_RIGHT:
    is_key_down ? cpu->key_down(JOYPAD_RIGHT) : cpu->key_up(JOYPAD_RIGHT);
    break;
  case SDLK_LEFT:
    is_key_down ? cpu->key_down(JOYPAD_LEFT) : cpu->key_up(JOYPAD_LEFT);
    break;
  case SDLK_UP:
    is_key_down ? cpu->key_down(JOYPAD_UP) : cpu->key_up(JOYPAD_UP);
    break;
  case SDLK_DOWN:
    is_key_down ? cpu->key_down(JOYPAD_DOWN) : cpu->key_up(JOYPAD_DOWN);
    break;
  case SDLK_z:
    is_key_down ? cpu->key_down(JOYPAD_A) : cpu->key_up(JOYPAD_A);
    break;
  case SDLK_x:
    is_key_down ? cpu->key_down(JOYPAD_B) : cpu->key_up(JOYPAD_B);
    break;
  case SDLK_SPACE:
    is_key_down ? cpu->key_down(JOYPAD_START) : cpu->key_up(JOYPAD_START);
    break;
  case SDLK_RETURN:
    is_key_down ? cpu->key_down(JOYPAD_SELECT) : cpu->key_up(JOYPAD_SELECT);
    break;
  }
}

void Display::poll_event() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_KEYDOWN:
      handle_key_event(event, true);
      break;

    case SDL_KEYUP:
      handle_key_event(event, false);
      break;

    case SDL_QUIT:
      active = false;
      break;

    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
        active = false;
      }
      break;
    }
  }
}

Display::~Display() {
  if (texture)
    SDL_DestroyTexture(texture);
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  SDL_Quit();
}

void Display::draw() {
  // Convert framebuffer to viewport pixels
  for (int i = 0; i < GB_WIDTH * GB_HEIGHT; i++) {
    Color color = framebuffer[i];
    std::copy(color.colors, color.colors + 4, viewport_pixels.begin() + i * 4);
  }

  // Update texture with new frame data
  if (SDL_UpdateTexture(texture, NULL, viewport_pixels.data(), GB_WIDTH * 4) ==
      -1) {
    SDL_Log("Failed to update texture: %s", SDL_GetError());
  }
}
