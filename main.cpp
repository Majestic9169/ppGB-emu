#include "include/cpu.h"
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#define DEBUG

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "USAGE: " << argv[0] << " <path to rom>" << std::endl;
  }
  std::ifstream ROM;
  ROM.open(argv[1], std::ios::binary | std::ios::in);
  CPU gb(ROM);

  while (1) {
    gb.INSTRUCTION_DECODER();
    // gb.print_reg();
    if (gb.clock_m > 0)
      while (gb.clock_m--) {
        long long int dummy = 0xffffff;
        while (dummy--)
          ;
      }
  }

  ROM.close();
}
