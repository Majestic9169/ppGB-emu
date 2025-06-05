#ifndef MMU_H
#define MMU_H

#include "cli_opts.hpp"
#include <fstream>
#include <iostream>
#include <vector>

/*
 * MMU
 * - Class to manage all ROM accesses
 * - should be shared across the other classes
 * - learncpp said std::vector for non-constexpr uses and i have to load from a
 * file so...
 */
// TODO: define copy constructors. This is necessary because we have a pointer
class MMU {
private:
  std::vector<std::byte> ROM;
  Opts *cli_opts;

public:
  MMU(Opts *opts_) : ROM{}, cli_opts{opts_} {
    std::ifstream rom_file{cli_opts->rom_name(), std::ios::binary};
    if (!rom_file.good()) {
      std::cerr << RED << "[!] Error loading ROM_FILE\n" << COLOR_RESET;
      exit(2);
    }
    ROM.reserve(0xffff);
    rom_file.read(reinterpret_cast<char *>(ROM.data()), 0xffff);
    if (cli_opts->debug_enabled()) {
      std::cout << GRN << "[+] ROM successfully loaded\n" << COLOR_RESET;
    }
  }
};

#endif
