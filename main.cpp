#include "include/cpu.h"
#include "include/display.h"
#include <chrono>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <thread>
#define DEBUG

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "USAGE: " << argv[0] << " <path to rom>" << std::endl;
    return 1;
  }

  std::ifstream ROM;
  ROM.open(argv[1], std::ios::binary | std::ios::in);
  if (!ROM.is_open()) {
    std::cout << "Error: Could not open ROM file" << std::endl;
    return 1;
  }

  CPU gb(ROM);
  Display display(gb.framebuffer, &gb);

  // Timing variables
  using clock = std::chrono::high_resolution_clock;
  const auto frame_duration = std::chrono::microseconds(16700); // ~60 FPS
  auto last_frame_time = clock::now();
  int frame_cycles = 0;

  while (display.is_active()) {
    auto current_time = clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        current_time - last_frame_time);

    if (elapsed >= frame_duration) {
      // Process CPU cycles until we reach the frame limit
      while (frame_cycles < CPU::CYCLES_PER_FRAME) {
        if (!gb.halted) {
          bool interrupted = gb.interrupt_check();
          if (!interrupted) {
            gb.INSTRUCTION_DECODER();
          }
        }
        gb.PPU_STEP();
        frame_cycles += gb.clock_m;
        gb.clock_m = 0;
      }

      // Reset frame cycle counter
      frame_cycles = 0;
      last_frame_time = current_time;

      // Render the frame
      display.render_frame();
    }

    // Handle events
    display.poll_event();

    // Sleep for a short time to prevent CPU hogging
    std::this_thread::sleep_for(std::chrono::microseconds(100));
  }

  ROM.close();
  return 0;
}
