#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>

const int MEMORY_SIZE = 0xFFFF;

class Memory {
private:
  std::array<std::uint8_t, MEMORY_SIZE> MEMORY;

public:
  Memory(std::ifstream &ROM);
  void print_mem(int n);
  void load_memory(std::ifstream &ROM, int MEM_END);
  uint8_t read_byte(uint16_t addr);
  void write_byte(uint16_t addr, uint8_t val);
};
