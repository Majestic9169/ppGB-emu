/*
 * palette.hpp
 * - define a color (2 bits)
 * - and a palette (collection of 4 colors)
 */

#ifndef PALETTE_H
#define PALETTE_H

#include <cstdint>

class Palette {
public:
  enum Colors { WHITE, LIGHT_GRAY, DARK_GRAY, BLACK };

  Palette(uint8_t &ref);
  Colors GetColor0() const;
  Colors GetColor1() const;
  Colors GetColor2() const;
  Colors GetColor3() const;

private:
  uint8_t &palette_ref;
  Colors color_extractor(uint8_t color_index) const;
};

#endif
