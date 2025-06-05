/*
 * cli_opts.hpp
 * - take cli arguments
 * - parse them to check for test mode and debug mode
 */

#ifndef CLI_OPTS_H
#define CLI_OPTS_H

#include "../src/ANSI-color-codes.cpp"
#include <algorithm>
#include <iostream>
#include <vector>

// cli arguments class
class Opts {
private:
  std::vector<std::string> args;
  bool is_test = false;
  bool debug_info = false;
  std::string ROM_FILE;
  std::string TEST_FLAG = "-t";
  std::string DEBUG_FLAG = "-d";
  std::string USAGE = "ppGB: ppGB [-t -d] ROM_FILE\n  " + TEST_FLAG +
                      "\ttesting mode\n  " + DEBUG_FLAG + "\tdebugging mode\n";

public:
  // create vector of arguments
  Opts(int argc, char **argv) : args{}, ROM_FILE{} {
    args.reserve(argc);
    for (int i = 0; i < argc; i++) {
      args.emplace_back(argv[i]);
    }
    if (validate_args()) {
      // search for "-t" or whatever in args
      auto test_flag = std::find(args.begin(), args.end(), TEST_FLAG);
      if (test_flag != args.end()) {
        is_test = true;
      }

      // search for "-d" or whatever in args
      auto debug_flag = std::find(args.begin(), args.end(), DEBUG_FLAG);
      if (debug_flag != args.end()) {
        debug_info = true;
      }

      ROM_FILE = rom_name();
    } else {
      std::cerr << RED + "[!] Too few arguments\n" + COLOR_RESET;
      std::cout << USAGE;
      exit(1);
    }
  }

  // valid arguments format
  bool validate_args() { return args.size() >= 2; }

  // check if test mode enabled
  bool test_enabled() { return is_test; }

  // check if debug mode enabled
  bool debug_enabled() { return debug_info; }

  // return ROM_FILE
  std::string rom_name() { return args.back(); }

  void print_debug() {
    if (debug_enabled()) {
      std::cout
          << YEL
          << "[~] Debug Mode is enabled. Debug information will be printed\n"
          << COLOR_RESET;
      std::cout << "ROM FILE   " << rom_name() << std::endl;
      std::cout << "TEST MODE  " << std::boolalpha << test_enabled()
                << std::endl;
      std::cout << "DEBUG MODE " << debug_enabled() << std::noboolalpha
                << std::endl;
    }
  }
};

#endif
