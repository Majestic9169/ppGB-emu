#ifndef GB_H
#define GB_H

#include "cli_opts.hpp"
#include "mmu.hpp"

/*
 * Gameboy
 * - contain all components and manage their construction
 * - provide an interface for user to run game
 */
class Gameboy {
private:
  MMU mmu;

public:
  Gameboy(Opts *opts_) : mmu(opts_) {};
};

#endif
