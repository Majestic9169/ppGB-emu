/*
 * cli_opts.hpp
 * - take cli arguments
 * - parse them to check for test mode and debug mode
 */

#ifndef CLI_OPTS_H
#define CLI_OPTS_H

#include "ANSI-color-codes.hpp"
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

  // delete copy and assignment constructor
  Opts(const Opts &) = delete;
  Opts operator=(const Opts &) = delete;

public:
  // create vector of arguments
  Opts(int argc, char **argv);

  // valid arguments format
  bool validate_args();

  // check if test mode enabled
  bool test_enabled();

  // check if debug mode enabled
  bool debug_enabled();

  // return ROM_FILE
  std::string rom_name();

  void print_debug();
};

#endif
