/*
 * ANSI-color-codes.cpp
 * - for some spice in the output
 */

#ifndef ANSI_COLOR_CODES
#define ANSI_COLOR_CODES

#include <string_view>

/*
 * fun fact: "\e" is the GNU ansi escape code
 *           "\033" is the POSIX standard
 */

// Regular text
constexpr std::string_view BLK = "\033[0;30m";
constexpr std::string_view RED = "\033[0;31m";
constexpr std::string_view GRN = "\033[0;32m";
constexpr std::string_view YEL = "\033[0;33m";
constexpr std::string_view BLU = "\033[0;34m";
constexpr std::string_view MAG = "\033[0;35m";
constexpr std::string_view CYN = "\033[0;36m";
constexpr std::string_view WHT = "\033[0;37m";

// Regular bold text
constexpr std::string_view BBLK = "\033[1;30m";
constexpr std::string_view BRED = "\033[1;31m";
constexpr std::string_view BGRN = "\033[1;32m";
constexpr std::string_view BYEL = "\033[1;33m";
constexpr std::string_view BBLU = "\033[1;34m";
constexpr std::string_view BMAG = "\033[1;35m";
constexpr std::string_view BCYN = "\033[1;36m";
constexpr std::string_view BWHT = "\033[1;37m";

// Regular underline text
constexpr std::string_view UBLK = "\033[4;30m";
constexpr std::string_view URED = "\033[4;31m";
constexpr std::string_view UGRN = "\033[4;32m";
constexpr std::string_view UYEL = "\033[4;33m";
constexpr std::string_view UBLU = "\033[4;34m";
constexpr std::string_view UMAG = "\033[4;35m";
constexpr std::string_view UCYN = "\033[4;36m";
constexpr std::string_view UWHT = "\033[4;37m";

// Regular background
constexpr std::string_view BLKB = "\033[40m";
constexpr std::string_view REDB = "\033[41m";
constexpr std::string_view GRNB = "\033[42m";
constexpr std::string_view YELB = "\033[43m";
constexpr std::string_view BLUB = "\033[44m";
constexpr std::string_view MAGB = "\033[45m";
constexpr std::string_view CYNB = "\033[46m";
constexpr std::string_view WHTB = "\033[47m";

// High intensty background
constexpr std::string_view BLKHB = "\033[0;100m";
constexpr std::string_view REDHB = "\033[0;101m";
constexpr std::string_view GRNHB = "\033[0;102m";
constexpr std::string_view YELHB = "\033[0;103m";
constexpr std::string_view BLUHB = "\033[0;104m";
constexpr std::string_view MAGHB = "\033[0;105m";
constexpr std::string_view CYNHB = "\033[0;106m";
constexpr std::string_view WHTHB = "\033[0;107m";

// High intensty text
constexpr std::string_view HBLK = "\033[0;90m";
constexpr std::string_view HRED = "\033[0;91m";
constexpr std::string_view HGRN = "\033[0;92m";
constexpr std::string_view HYEL = "\033[0;93m";
constexpr std::string_view HBLU = "\033[0;94m";
constexpr std::string_view HMAG = "\033[0;95m";
constexpr std::string_view HCYN = "\033[0;96m";
constexpr std::string_view HWHT = "\033[0;97m";

// Bold high intensity text
constexpr std::string_view BHBLK = "\033[1;90m";
constexpr std::string_view BHRED = "\033[1;91m";
constexpr std::string_view BHGRN = "\033[1;92m";
constexpr std::string_view BHYEL = "\033[1;93m";
constexpr std::string_view BHBLU = "\033[1;94m";
constexpr std::string_view BHMAG = "\033[1;95m";
constexpr std::string_view BHCYN = "\033[1;96m";
constexpr std::string_view BHWHT = "\033[1;97m";

// Reset
constexpr std::string_view reset = "\033[0m";
constexpr std::string_view CRESET = "\033[0m";
constexpr std::string_view COLOR_RESET = "\033[0m";

#endif
