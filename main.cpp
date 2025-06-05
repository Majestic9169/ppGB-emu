/*
 * main.cpp
 * - init, load, and run gameboy (accept cli arguments)
 * - include debugging flags (print asm, rom info)
 * - include flags to build for testing (headless + call test class)
 */

#include "include/cli_opts.hpp"
#include "include/gb.hpp"

int main(int argc, char **argv) {
  Opts cli_options(argc, argv);
  cli_options.print_debug();

  Gameboy gb{&cli_options};

  return 0;
}
