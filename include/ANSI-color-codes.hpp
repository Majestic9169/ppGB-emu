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
const std::string BLK = "\033[0;30m";
const std::string RED = "\033[0;31m";
const std::string GRN = "\033[0;32m";
const std::string YEL = "\033[0;33m";
const std::string BLU = "\033[0;34m";
const std::string MAG = "\033[0;35m";
const std::string CYN = "\033[0;36m";
const std::string WHT = "\033[0;37m";

// Regular bold text
const std::string BBLK = "\033[1;30m";
const std::string BRED = "\033[1;31m";
const std::string BGRN = "\033[1;32m";
const std::string BYEL = "\033[1;33m";
const std::string BBLU = "\033[1;34m";
const std::string BMAG = "\033[1;35m";
const std::string BCYN = "\033[1;36m";
const std::string BWHT = "\033[1;37m";

// Regular underline text
const std::string UBLK = "\033[4;30m";
const std::string URED = "\033[4;31m";
const std::string UGRN = "\033[4;32m";
const std::string UYEL = "\033[4;33m";
const std::string UBLU = "\033[4;34m";
const std::string UMAG = "\033[4;35m";
const std::string UCYN = "\033[4;36m";
const std::string UWHT = "\033[4;37m";

// Regular background
const std::string BLKB = "\033[40m";
const std::string REDB = "\033[41m";
const std::string GRNB = "\033[42m";
const std::string YELB = "\033[43m";
const std::string BLUB = "\033[44m";
const std::string MAGB = "\033[45m";
const std::string CYNB = "\033[46m";
const std::string WHTB = "\033[47m";

// High intensty background
const std::string BLKHB = "\033[0;100m";
const std::string REDHB = "\033[0;101m";
const std::string GRNHB = "\033[0;102m";
const std::string YELHB = "\033[0;103m";
const std::string BLUHB = "\033[0;104m";
const std::string MAGHB = "\033[0;105m";
const std::string CYNHB = "\033[0;106m";
const std::string WHTHB = "\033[0;107m";

// High intensty text
const std::string HBLK = "\033[0;90m";
const std::string HRED = "\033[0;91m";
const std::string HGRN = "\033[0;92m";
const std::string HYEL = "\033[0;93m";
const std::string HBLU = "\033[0;94m";
const std::string HMAG = "\033[0;95m";
const std::string HCYN = "\033[0;96m";
const std::string HWHT = "\033[0;97m";

// Bold high intensity text
const std::string BHBLK = "\033[1;90m";
const std::string BHRED = "\033[1;91m";
const std::string BHGRN = "\033[1;92m";
const std::string BHYEL = "\033[1;93m";
const std::string BHBLU = "\033[1;94m";
const std::string BHMAG = "\033[1;95m";
const std::string BHCYN = "\033[1;96m";
const std::string BHWHT = "\033[1;97m";

// Reset
const std::string reset = "\033[0m";
const std::string CRESET = "\033[0m";
const std::string COLOR_RESET = "\033[0m";

#endif
