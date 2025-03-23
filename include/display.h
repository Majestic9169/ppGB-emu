#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <string>

class Display {
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  bool active;

public:
  Display(const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
          const char *WINDOW_NAME);
  bool render_frame();
  bool is_active();
  void poll_event();
  ~Display();
};
