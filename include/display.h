#ifndef INCLUDE_DISPLAY
#define INCLUDE_DISPLAY

#include "cpu.h"
#include "memory.h"
#include <SDL2/SDL.h>
#include <array>
#include <cstdint>
#include <string>

// GameBoy display dimensions
constexpr int GB_WIDTH = 160;
constexpr int GB_HEIGHT = 144;
constexpr int SCALE_FACTOR = 2;  // Scale factor for window size

class Display {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    bool active;
    CPU* cpu;

    // Framebuffer for the GameBoy display
    std::array<uint8_t, GB_WIDTH * GB_HEIGHT * 4> viewport_pixels;
    std::array<Color, GB_WIDTH * GB_HEIGHT> framebuffer;

    // Window dimensions
    const int window_width;
    const int window_height;
    const std::string window_title;

    void draw();
    void handle_key_event(const SDL_Event& event, bool is_key_down);

public:
    Display(std::array<Color, GB_WIDTH * GB_HEIGHT>& framebuf, CPU* cpu_ptr,
            const int WINDOW_WIDTH = GB_WIDTH * SCALE_FACTOR,
            const int WINDOW_HEIGHT = GB_HEIGHT * SCALE_FACTOR,
            const char* WINDOW_NAME = "ppGB Emulator");
    ~Display();

    bool render_frame();
    bool is_active() const;
    void poll_event();
};

#endif
