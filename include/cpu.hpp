/*
 * CPU
 * - store registers and other state variables
 * - contain processing functions like for flag setting
 * - can access mmu
 * - store opcodes and instructions in separate class
 * - can access ppu
 */

#ifndef CPU_H
#define CPU_H

#include "mmu.hpp"
#include "reg.hpp"
// #include <cstdint>

class CPU {
private:
  MMU *mmu;

public:
  Registers reg;
  CPU(MMU *_mmu) : mmu(_mmu), reg{} {};

  // fetch-decode-execute cycle
  void cpu_step() {
    // uint8_t opcode = mmu->read_byte(reg.pc++);
    // TODO: create instruction/opcode class and fetch instruction/opcode
  }
};

#endif
