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

  struct registers {
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
  };

public:
  GameBoy(std::ifstream &ROM) {
    std::cout << "[*] Loading ROM into memory from = to 0x3FFF" << std::endl;
    if (ROM.is_open()) {
      ROM.read((char *)&MEMORY, 0x4000);
      std::cout << "[+] Loaded ROM into memory" << std::endl;
    }
  }
  void print_mem() {
    for (int i = 0; i < 0x4000; i++) {
      printf("%5d %2x\n", i, MEMORY[i]);
    }
  }
};

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "USAGE: " << argv[0] << " <path to rom>" << std::endl;
  }
  std::ifstream ROM;
  ROM.open(argv[1], std::ios::binary | std::ios::in);
  GameBoy gb(ROM);
  // gb.print_mem();
  ROM.close();
}
