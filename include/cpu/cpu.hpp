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

#include "../cli_opts.hpp"
#include "../mmu/mmu.hpp"
#include "cb_opcodes.hpp"
#include "opcodes.hpp"
#include "reg.hpp"
#include <cstdint>

class CPU {
  // NOTE: this is necessary because c++ class members are initialised in the
  // order they are declared [-Wreorder]
private:
  Opts *cli_opts;
  MMU *mmu;
  // delete copy and assignment constructor
  CPU(const CPU &) = delete;
  CPU operator=(const CPU &) = delete;

public:
  Registers reg;

private:
  Opcodes op;
  CB_Opcodes cb;
  uint16_t curr_pc;
  uint16_t opcode;
  std::vector<uint8_t> op_ticks;
  std::vector<uint8_t> cb_ticks;

  void cb_cycle();

public:
  CPU(Opts *cli, MMU *_mmu);

  int check_interrupts(uint8_t old_if); // return T cycles

  // fetch-decode-execute cycle
  int cpu_step(); // return T cycles

  void print_reg();
};

#endif
