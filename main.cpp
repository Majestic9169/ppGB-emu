#include "include/cpu.h"
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>

#ifndef DISABLE_DISPLAY
#include "include/display.h"
#endif

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "USAGE: " << argv[0] << " <path to rom>" << std::endl;
    return 1;
  }
  
  std::ifstream ROM;
  ROM.open(argv[1], std::ios::binary | std::ios::in);
  CPU gb(ROM);

#ifndef DISABLE_DISPLAY
  Display display(gb.framebuffer);
  
  while (display.is_active()) {
#else
  while (true) {  // For testing without display
#endif

    bool interrupted = gb.interrupt_check();
    if (!interrupted) {
      gb.INSTRUCTION_DECODER();
    }

    if (gb.clock_m > 0) {
      while (gb.clock_m--) {
        long long int dummy = 0xffffff;
        while (dummy--)
          ;
      }
    }

    gb.PPU_STEP();
    
#ifndef DISABLE_DISPLAY
    display.poll_event();
    display.render_frame();
#endif
  }

  ROM.close();
  return 0;
}
