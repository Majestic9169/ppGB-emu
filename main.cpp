/*
 * main.cpp
 * - init, load, and run gameboy (accept cli arguments)
 * - include debugging flags (print asm, rom info)
 * - include flags to build for testing (headless + call test class)
 */

#include "include/cli_opts.hpp"
#include <iostream>

int main(int argc, char **argv) {
  Opts cli_options(argc, argv);
  std::cout << cli_options.rom_name() << std::endl;
  std::cout << cli_options.test_enabled() << std::endl;
  std::cout << cli_options.debug_enabled() << std::endl;
  return 0;
}
