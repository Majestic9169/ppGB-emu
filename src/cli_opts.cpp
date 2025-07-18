/*
 * cli_opts.hpp
 * - take cli arguments
 * - parse them to check for test mode and debug mode
 */

#include "../include/cli_opts.hpp"

Opts::Opts(int argc, char **argv) : args{}, ROM_FILE{} {
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
bool Opts::validate_args() { return args.size() >= 2; }

// check if test mode enabled
bool Opts::test_enabled() { return is_test; }

// check if debug mode enabled
bool Opts::debug_enabled() { return debug_info; }

// return ROM_FILE
std::string Opts::rom_name() { return args.back(); }

void Opts::print_debug() {
  if (debug_enabled()) {
    std::cout
        << YEL
        << "[~] Debug Mode is enabled. Debug information will be printed\n"
        << COLOR_RESET;
    std::cout << "ROM FILE   " << rom_name() << std::endl;
    std::cout << "TEST MODE  " << std::boolalpha << test_enabled() << std::endl;
    std::cout << "DEBUG MODE " << debug_enabled() << std::noboolalpha
              << std::endl;
  }
}
