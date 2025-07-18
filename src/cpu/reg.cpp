/*
 * Registers
 * - make use of struct/union for ease of access
 * - take care of endianness
 * - interface for working with flag register
 */

#include "../../include/cpu/reg.hpp"
#include <cstdint>

void Registers::set_z(uint8_t val) {
  if (val == 0) {
    f.z = 1;
  } else {
    f.z = 0;
  }
}

// WARN: ONLY FOR INCREMENT BY ONE
void Registers::set_h(uint8_t val) {
  if ((val & 0x0f) == 0x0f) {
    f.h = 1;
  } else {
    f.h = 0;
  }
}

// overload for two number addition
void Registers::set_h(uint8_t a, uint8_t b) {
  uint8_t res = (a & 0x0f) + (b & 0x0f);
  if ((res & 0xf0)) {
    f.h = 1;
  } else {
    f.h = 0;
  }
}

void Registers::set_c(uint8_t a, uint8_t b) {
  uint16_t res = a + b;
  if ((res & 0xff00)) {
    f.c = 1;
  } else {
    f.c = 0;
  }
}
