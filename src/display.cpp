#include "../include/display.h"
#include "../include/cpu.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <memory.h>

Display::Display(std::array<Color, 160 * 144> &framebuf, CPU *cpu_ptr,
                 const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
                 const char *WINDOW_NAME)
    : framebuffer(framebuf), window(nullptr), renderer(nullptr),
      texture(nullptr), active(false), cpu(cpu_ptr) {

  viewport_pixels.fill(0xFF);

  if (SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL3 Initiliasation Successful");
  } else {
    SDL_Log("%s\n", SDL_GetError());
    exit(1);
  }

  window = SDL_CreateWindow(WINDOW_NAME, WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2,
                            SDL_WINDOW_VULKAN);
  if (!window) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    exit(2);
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    exit(3);
  }
  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_STRETCH);

  texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
  if (!texture) {
    SDL_Log("Failed to create texture: %s", SDL_GetError());
    exit(4);
  }

  active = true;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}

bool Display::is_active() { return active; }

bool Display::render_frame() {
  if (!SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)) {
    SDL_Log("Error setting render color: %s", SDL_GetError());
  }
  SDL_RenderClear(renderer);
  SDL_SetRenderTarget(renderer, texture);

  draw();

  SDL_RenderTexture(renderer, texture, NULL, &viewport_rect);
  SDL_RenderPresent(renderer);
  return true;
};

void Display::poll_event() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_KEY_DOWN:
      switch (event.key.key) {
      case SDLK_RIGHT:
        cpu->key_down(JOYPAD_RIGHT);
        break;
      case SDLK_LEFT:
        cpu->key_down(JOYPAD_LEFT);
        break;
      case SDLK_UP:
        cpu->key_down(JOYPAD_UP);
        break;
      case SDLK_DOWN:
        cpu->key_down(JOYPAD_DOWN);
        break;
      case SDLK_Z:
        cpu->key_down(JOYPAD_A);
        break;
      case SDLK_X:
        cpu->key_down(JOYPAD_B);
        break;
      case SDLK_SPACE:
        cpu->key_down(JOYPAD_START);
        break;
      case SDLK_RETURN:
        cpu->key_down(JOYPAD_SELECT);
        break;
      }
      break;

    case SDL_EVENT_KEY_UP:
      switch (event.key.key) {
      case SDLK_RIGHT:
        cpu->key_up(JOYPAD_RIGHT);
        break;
      case SDLK_LEFT:
        cpu->key_up(JOYPAD_LEFT);
        break;
      case SDLK_UP:
        cpu->key_up(JOYPAD_UP);
        break;
      case SDLK_DOWN:
        cpu->key_up(JOYPAD_DOWN);
        break;
      case SDLK_Z:
        cpu->key_up(JOYPAD_A);
        break;
      case SDLK_X:
        cpu->key_up(JOYPAD_B);
        break;
      case SDLK_SPACE:
        cpu->key_up(JOYPAD_START);
        break;
      case SDLK_RETURN:
        cpu->key_up(JOYPAD_SELECT);
        break;
      }
      break;

    case SDL_EVENT_QUIT:
      active = false;
      break;
    }
  }
}

Display::~Display() {
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
};

void Display::draw() {
  for (int i = 0; i < 144 * 160; i++) {
    Color color = framebuffer[i];
    std::copy(color.colors, color.colors + 4, viewport_pixels.begin() + i * 4);
  }
  if (!SDL_UpdateTexture(texture, NULL, viewport_pixels.data(), 160 * 4)) {
    std::cout << "SDL Error" << SDL_GetError() << std::endl;
  }
}
