#include "../include/display.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <algorithm>
#include <cstdlib>

Display::Display(Color *framebuf, const int WINDOW_WIDTH,
                 const int WINDOW_HEIGHT, const char *WINDOW_NAME)
    : framebuffer(framebuf), window(nullptr), renderer(nullptr),
      texture(nullptr), active(false) {

  viewport_pixels.fill(0xFF);

  if (SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL3 Initiliasation Successful");
  } else {
    SDL_Log("%s\n", SDL_GetError());
    exit(1);
  }

  window = SDL_CreateWindow(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

  SDL_RenderTexture(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
  return true;
};

void Display::poll_event() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      active = false;
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
    std::cout << "after copy " << i << std::endl;
  }
  if (SDL_UpdateTexture(texture, NULL, viewport_pixels.data(), WIDTH * 4)) {
    std::cout << "SDL Error" << SDL_GetError() << std::endl;
  }
}
