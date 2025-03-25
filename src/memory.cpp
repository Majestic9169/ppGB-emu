#include "../include/memory.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>

Memory::Memory(std::ifstream &ROM) {
  load_memory(ROM, 0xFD00);
  MEMORY[0xFF0F] = 0xe1;
  MEMORY[0xFF41] = 0x80;
  MEMORY[0xFF40] = 0x91;
  MEMORY[0xFF50] = 0xFF;
}

void Memory::print_mem(int n) {
  for (int i = 0; i < n; i++) {
    printf("%5d %2x\n", i, MEMORY[i]);
  }
}

void Memory::load_memory(std::ifstream &ROM, int MEM_END) {
  std::cout << std::hex << std::uppercase << std::setfill('0');
  std::cout << "[*] Loading ROM into memory from 0x0000 to 0x" << std::setw(4)
            << MEM_END << std::endl;
  if (ROM.is_open()) {
    ROM.read((char *)&MEMORY, MEM_END);
    // for (int i = 0x8000; i < 0xFF00; i++) {
    //   MEMORY[i] = 0;
    // }
    std::cout << "[+] Loaded ROM into memory" << std::endl;
  }
  std::cout << std::dec;
}

uint8_t Memory::read_byte(uint16_t addr) { return MEMORY[addr]; }
void Memory::write_byte(uint16_t addr, uint8_t val) { MEMORY[addr] = val; }
