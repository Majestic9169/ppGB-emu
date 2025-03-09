#include "../include/memory.h"
#include <array>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>

Memory::Memory(std::ifstream &ROM) { load_memory(ROM, 0x4000); }

void Memory::print_mem(int n) {
  for (int i = 0; i < n; i++) {
    printf("%5d %2x\n", i, MEMORY[i]);
  }
}

void Memory::load_memory(std::ifstream &ROM, int MEM_END) {
  std::cout << "[*] Loading ROM into memory from 0x0000 = to " << std::hex
            << std::uppercase << MEM_END << std::endl;
  if (ROM.is_open()) {
    ROM.read((char *)&MEMORY, MEM_END);
    std::cout << "[+] Loaded ROM into memory" << std::endl;
  }
}
