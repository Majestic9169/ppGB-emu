#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>

const int MEMORY_SIZE = 0xFFFF;

class GameBoy {
private:
  std::array<std::uint8_t, MEMORY_SIZE> MEMORY;

  struct reg {
    union {
      struct {
        std::uint8_t a;
        std::uint8_t f;
      };
      std::uint16_t af;
    };
    union {
      struct {
        std::uint8_t b;
        std::uint8_t c;
      };
      std::uint16_t bc;
    };
    union {
      struct {
        std::uint8_t d;
        std::uint8_t e;
      };
      std::uint16_t de;
    };
    union {
      struct {
        std::uint8_t h;
        std::uint8_t l;
      };
      std::uint16_t hl;
    };
    std::uint16_t sp;
    std::uint16_t pc;
  } reg;

public:
  GameBoy(std::ifstream &ROM) {
    reg.a = 0x01;
    reg.f = 0xb0;
    reg.b = 0x00;
    reg.c = 0x13;
    reg.d = 0x00;
    reg.e = 0xd8;
    reg.h = 0x01;
    reg.l = 0x4d;
    reg.sp = 0xfffe;
    reg.pc = 0x100;
    std::cout << "[*] Loading ROM into memory from = to 0x3FFF" << std::endl;
    if (ROM.is_open()) {
      ROM.read((char *)&MEMORY, 0x4000);
      std::cout << "[+] Loaded ROM into memory" << std::endl;
    }
  }
  void print_mem(int n) {
    for (int i = 0; i < n; i++) {
      printf("%5d %2x\n", i, MEMORY[i]);
    }
  }
  void print_reg() {
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
};

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "USAGE: " << argv[0] << " <path to rom>" << std::endl;
  }
  std::ifstream ROM;
  ROM.open(argv[1], std::ios::binary | std::ios::in);
  GameBoy gb(ROM);
  gb.print_reg();
  gb.print_mem(0x100);
  ROM.close();
}
