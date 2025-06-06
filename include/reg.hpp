/*
 * Registers
 * - make use of struct/union for ease of access
 * - take care of endianness
 * - interface for working with flag register
 */

#ifndef REG_H
#define REG_H

#include <cstdint>

class Registers {
public:
// ISO C++ doesn't allow anonymous structs which is so stupid
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  struct {
    union {
      struct {
        struct {
          uint8_t ignore : 4;
          uint8_t c : 1;
          uint8_t h : 1;
          uint8_t n : 1;
          uint8_t z : 1;
        } f;
        uint8_t a;
      };
      uint16_t af;
    };
  };
  struct {
    union {
      struct {
        uint8_t c;
        uint8_t b;
      };
      uint16_t bc;
    };
  };
  struct {
    union {
      struct {
        uint8_t e;
        uint8_t d;
      };
      uint16_t de;
    };
  };
  struct {
    union {
      struct {
        uint8_t l;
        uint8_t h;
      };
      uint16_t hl;
    };
  };
#pragma GCC diagnostic pop

  uint16_t sp;
  uint16_t pc;

  void set_z(uint8_t val) {
    if (val == 0) {
      f.z = 1;
    } else {
      f.z = 0;
    }
  }

  void set_h(uint8_t val) {
    if ((val & 0x0f) == 0x0f) {
      f.h = 1;
    } else {
      f.h = 0;
    }
  }
};

#endif
