/*
 * ANSI-color-codes.cpp
 * - for some spice in the output
 */

#ifndef ANSI_COLOR_CODES
#define ANSI_COLOR_CODES
#include <string>

/*
 * fun fact: "\e" is the GNU ansi escape code
 *           "\033" is the POSIX standard
 */

// Regular text
std::string BLK = "\033[0;30m";
std::string RED = "\033[0;31m";
std::string GRN = "\033[0;32m";
std::string YEL = "\033[0;33m";
std::string BLU = "\033[0;34m";
std::string MAG = "\033[0;35m";
std::string CYN = "\033[0;36m";
std::string WHT = "\033[0;37m";

// Regular bold text
std::string BBLK = "\033[1;30m";
std::string BRED = "\033[1;31m";
std::string BGRN = "\033[1;32m";
std::string BYEL = "\033[1;33m";
std::string BBLU = "\033[1;34m";
std::string BMAG = "\033[1;35m";
std::string BCYN = "\033[1;36m";
std::string BWHT = "\033[1;37m";

// Regular underline text
std::string UBLK = "\033[4;30m";
std::string URED = "\033[4;31m";
std::string UGRN = "\033[4;32m";
std::string UYEL = "\033[4;33m";
std::string UBLU = "\033[4;34m";
std::string UMAG = "\033[4;35m";
std::string UCYN = "\033[4;36m";
std::string UWHT = "\033[4;37m";

// Regular background
std::string BLKB = "\033[40m";
std::string REDB = "\033[41m";
std::string GRNB = "\033[42m";
std::string YELB = "\033[43m";
std::string BLUB = "\033[44m";
std::string MAGB = "\033[45m";
std::string CYNB = "\033[46m";
std::string WHTB = "\033[47m";

// High intensty background
std::string BLKHB = "\033[0;100m";
std::string REDHB = "\033[0;101m";
std::string GRNHB = "\033[0;102m";
std::string YELHB = "\033[0;103m";
std::string BLUHB = "\033[0;104m";
std::string MAGHB = "\033[0;105m";
std::string CYNHB = "\033[0;106m";
std::string WHTHB = "\033[0;107m";

// High intensty text
std::string HBLK = "\033[0;90m";
std::string HRED = "\033[0;91m";
std::string HGRN = "\033[0;92m";
std::string HYEL = "\033[0;93m";
std::string HBLU = "\033[0;94m";
std::string HMAG = "\033[0;95m";
std::string HCYN = "\033[0;96m";
std::string HWHT = "\033[0;97m";

// Bold high intensity text
std::string BHBLK = "\033[1;90m";
std::string BHRED = "\033[1;91m";
std::string BHGRN = "\033[1;92m";
std::string BHYEL = "\033[1;93m";
std::string BHBLU = "\033[1;94m";
std::string BHMAG = "\033[1;95m";
std::string BHCYN = "\033[1;96m";
std::string BHWHT = "\033[1;97m";

// Reset
std::string reset = "\033[0m";
std::string CRESET = "\033[0m";
std::string COLOR_RESET = "\033[0m";

#endif
