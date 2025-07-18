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
private:
  // delete copy and assignment constructors
  Registers(const Registers &) = delete;
  Registers operator=(const Registers &) = delete;

public:
// ISO C++ doesn't allow anonymous structs which is so stupid
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
  struct {
    union {
      struct {
        // TODO: convert f to std::bitset. This will be annoying and i'm not
        // even sure if i should
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
  uint16_t pc{0x100};
  bool ime;
  bool stopped = false;

  void set_z(uint8_t val) {
    if (val == 0) {
      f.z = 1;
    } else {
      f.z = 0;
    }
  }

  // WARN: ONLY FOR INCREMENT BY ONE
  void set_h(uint8_t val) {
    if ((val & 0x0f) == 0x0f) {
      f.h = 1;
    } else {
      f.h = 0;
    }
  }

  // overload for two number addition
  void set_h(uint8_t a, uint8_t b) {
    uint8_t res = (a & 0x0f) + (b & 0x0f);
    if ((res & 0xf0)) {
      f.h = 1;
    } else {
      f.h = 0;
    }
  }

  void set_c(uint8_t a, uint8_t b) {
    uint16_t res = a + b;
    if ((res & 0xff00)) {
      f.c = 1;
    } else {
      f.c = 0;
    }
  }
};

#endif
