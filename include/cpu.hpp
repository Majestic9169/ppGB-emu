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

#include "reg.hpp"

class CPU {
private:
public:
  Registers reg;
};

#endif
