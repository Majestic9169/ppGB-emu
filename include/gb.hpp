#ifndef GB_H
#define GB_H

#include "cli_opts.hpp"
#include "cpu.hpp"
#include "mmu.hpp"

/*
 * Gameboy
 * - contain all components and manage their construction
 * - provide an interface for user to run game
 */
class Gameboy {
private:
  MMU mmu;
  CPU cpu;
  // delete copy and assignment constructor
  Gameboy(const Gameboy &) = delete;
  Gameboy operator=(const Gameboy &) = delete;

public:
  Gameboy(Opts *opts_) : mmu(opts_), cpu{opts_, &mmu} {};
  void run() {
    while (1) {
      cpu.cpu_step();
    }
  }
};

#endif
