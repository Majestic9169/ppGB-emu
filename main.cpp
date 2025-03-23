#include "include/cpu.h"
#include "include/display.h"
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#define DEBUG

#define WINDOW_HEIGHT (64 * 10)
#define WINDOW_WIDTH (32 * 10)

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "USAGE: " << argv[0] << " <path to rom>" << std::endl;
  }
  std::ifstream ROM;
  ROM.open(argv[1], std::ios::binary | std::ios::in);
  CPU gb(ROM);

  Display display(WINDOW_WIDTH, WINDOW_HEIGHT, "ppGB emulator");

  while (display.is_active()) {
    gb.INSTRUCTION_DECODER();
    // gb.print_reg();
    if (gb.clock_m > 0) {
      while (gb.clock_m--) {
        long long int dummy = 0xffffff;
        while (dummy--)
          ;
      }
    }

    display.poll_event();
    display.render_frame();
  }

  ROM.close();
}
