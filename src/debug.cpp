#include "../include/cpu.h"
#include <iomanip>

void CPU::print_reg() {
  std::cout << std::hex << std::uppercase << std::setfill('0');

  std::cout << "Registers:\n";
  std::cout << "A  = 0x" << std::setw(2) << static_cast<int>(reg.a)
            << std::endl;
  std::cout << "F  = 0x" << std::setw(2) << static_cast<int>(reg.f)
            << std::endl;
  std::cout << "AF = 0x" << std::setw(4) << reg.af << std::endl;
  std::cout << "B  = 0x" << std::setw(2) << static_cast<int>(reg.b)
            << std::endl;
  std::cout << "C  = 0x" << std::setw(2) << static_cast<int>(reg.c)
            << std::endl;
  std::cout << "BC = 0x" << std::setw(4) << reg.bc << std::endl;
  std::cout << "D  = 0x" << std::setw(2) << static_cast<int>(reg.d)
            << std::endl;
  std::cout << "E  = 0x" << std::setw(2) << static_cast<int>(reg.e)
            << std::endl;
  std::cout << "DE = 0x" << std::setw(4) << reg.de << std::endl;
  std::cout << "H  = 0x" << std::setw(2) << static_cast<int>(reg.h)
            << std::endl;
  std::cout << "L  = 0x" << std::setw(2) << static_cast<int>(reg.l)
            << std::endl;
  std::cout << "HL = 0x" << std::setw(4) << reg.hl << std::endl;
  std::cout << "SP = 0x" << std::setw(4) << reg.sp << std::endl;
  std::cout << "PC = 0x" << std::setw(4) << reg.pc << std::endl;

  std::cout << std::dec;
}
