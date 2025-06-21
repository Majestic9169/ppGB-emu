/*
 * MMU
 * - Class to manage all ROM accesses
 * - should be shared across the other classes
 * - learncpp said std::vector for non-constexpr uses and i have to load from a
 * file so...
 */

#ifndef MMU_H
#define MMU_H

#include "cli_opts.hpp"
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

constexpr std::size_t ROM_SIZE{0x10000};

class MMU {
private:
  // i considered std::byte but that's really restrictive and i usually need to
  // work with numbers
  std::vector<std::uint8_t> ROM;
  Opts *cli_opts;
  // delete copy constructor and assignment operator, don't wanna mess with
  // pointers. look at this btw lol
  // https://stackoverflow.com/questions/33776697/
  MMU(const MMU &) = delete;
  MMU &operator=(const MMU &) = delete;

  void header_information() {
    std::string title{std::string{ROM.begin() + 0x134, ROM.begin() + 0x143}};
    bool gbc_support = ROM[0x143] == 0x80 || ROM[0x143] == 0xc0 ? true : false;
    int rom_size = 0x8000 * (1 << ROM[0x148]);
    int rom_banks = rom_size / 0x4000;
    std::cout << YEL << "[~] ROM Header Information\n" << COLOR_RESET;
    std::cout << "Title       " << title << std::endl;
    std::cout << "GBC support " << std::boolalpha << gbc_support
              << std::noboolalpha << std::endl;
    std::cout << "ROM size    " << rom_size << std::endl;
    std::cout << "No. Banks   " << rom_banks << std::endl;
  }

public:
  MMU(Opts *opts_) : ROM(ROM_SIZE), cli_opts{opts_} {
    std::ifstream rom_file{cli_opts->rom_name(), std::ios::binary};
    if (!rom_file.good()) {
      std::cerr << RED << "[!] Error loading ROM_FILE\n" << COLOR_RESET;
      exit(2);
    }
    rom_file.read(reinterpret_cast<char *>(ROM.data()), ROM_SIZE);
    if (cli_opts->debug_enabled()) {
      std::cout << GRN << "[+] ROM successfully loaded\n" << COLOR_RESET;
      header_information();
    }
  }

  uint8_t &read_byte(uint16_t addr) { return ROM[addr]; }
  uint16_t read_word(uint16_t addr) { return ROM[addr] | ROM[addr + 1] << 8; }
  void write_byte(uint16_t addr, uint8_t val) { ROM[addr] = val; }
  void write_word(uint16_t addr, uint16_t val) {
    write_byte(addr, static_cast<uint8_t>(val));
    write_byte(addr + 1, static_cast<uint8_t>(val >> 8));
  }
};

#endif
