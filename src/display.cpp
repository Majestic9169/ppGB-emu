#include "../include/display.h"
#include <cstdlib>

Display::Display(const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
                 const char *WINDOW_NAME)
    : window(nullptr), renderer(nullptr), texture(nullptr), active(false) {

  if (SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL3 Initiliasation Successful");
  } else {
    SDL_Log("%s\n", SDL_GetError());
    exit(1);
  }

  window = SDL_CreateWindow(WINDOW_NAME, 600, 300, 0);
  if (!window) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    exit(2);
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    exit(3);
  }

  texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
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
  for (int i = 0; i < 64 * 32; i++) {
    SDL_FRect pixel;
    pixel.w = 10;
    pixel.h = 10;
    pixel.x = (i % 64) * pixel.w; // Corrected positioning
    pixel.y = (i / 64.0) * pixel.h;

    if (!SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255)) {
      SDL_Log("Error setting render color: %s", SDL_GetError());
    }

    if (!SDL_RenderFillRect(renderer, &pixel)) {
      SDL_Log("RenderFillRect Error: %s", SDL_GetError());
    }
  }

  // Present the updated frame
  if (!SDL_RenderPresent(renderer)) {
    SDL_Log("Error presenting renderer: %s", SDL_GetError());
    return false;
  }

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
