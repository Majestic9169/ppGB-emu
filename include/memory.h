#ifndef INCLUDE_MEMORY
#define INCLUDE_MEMORY
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <sys/types.h>

const int MEMORY_SIZE = 0xFFFF;

struct Color {
  union {
    struct {
      uint8_t r, g, b, a;
    };
    uint8_t colors[4];
  };
};

class Memory {
private:
  std::array<std::uint8_t, MEMORY_SIZE> MEMORY;

public:
  Memory(std::ifstream &ROM);
  void print_mem(int n);
  void load_memory(std::ifstream &ROM, int MEM_END);
  uint8_t read_byte(uint16_t addr);
  void write_byte(uint16_t addr, uint8_t val);

  struct Sprite {
    uint8_t y;
    uint8_t x;
    uint8_t tile;
    struct {
      union {
        struct {
          uint8_t unused_1 : 1;
          uint8_t unused_2 : 1;
          uint8_t unused_3 : 1;
          uint8_t unused_4 : 1;
          uint8_t unused_5 : 1;
          uint8_t x_flip : 1;
          uint8_t y_flip : 1;
          uint8_t render_priority : 1;
        };
        uint8_t value;
      };
    } flags;
  } sprites[40] = {Sprite()};

  struct Tile {
    uint8_t pixels[8][8] = {0};
  } tiles[384];

  const Color colors[4] = {
      {255, 255, 255, 255},
      {192, 192, 192, 255},
      {96, 96, 96, 255},
      {0, 0, 0, 255},
  };
};

#endif // !DEBUG
