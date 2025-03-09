#include "../include/cpu.h"
#include <cstring>
#include <fstream>
#include <iostream>

CPU::CPU(std::ifstream &ROM_)
    : reg{.a = 0x01,
          .f = 0xb0,
          .b = 0x00,
          .c = 0x13,
          .d = 0x00,
          .e = 0xd8,
          .h = 0x01,
          .l = 0x4d,
          .sp = 0xfffe,
          .pc = 0x100},
      ROM(ROM_) {}

void CPU::print_reg() {
  std::cout << "a = " << static_cast<int>(reg.a) << std::endl;
  std::cout << "f = " << static_cast<int>(reg.f) << std::endl;
  std::cout << "af = " << reg.af << std::endl;
  std::cout << "b = " << static_cast<int>(reg.b) << std::endl;
  std::cout << "c = " << static_cast<int>(reg.c) << std::endl;
  std::cout << "bc = " << reg.bc << std::endl;
  std::cout << "d = " << static_cast<int>(reg.d) << std::endl;
  std::cout << "e = " << static_cast<int>(reg.e) << std::endl;
  std::cout << "de = " << reg.de << std::endl;
  std::cout << "h = " << static_cast<int>(reg.h) << std::endl;
  std::cout << "l = " << static_cast<int>(reg.l) << std::endl;
  std::cout << "hl = " << reg.hl << std::endl;
  std::cout << "sp = " << reg.sp << std::endl;
  std::cout << "pc = " << reg.pc << std::endl;
}
