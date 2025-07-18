/*
 * palette.hpp
 * - define a color (2 bits)
 * - and a palette (collection of 4 colors)
 */

#include "../../include/mmu/palette.hpp"
#include <cstdint>

Palette::Palette(uint8_t &ref) : palette_ref{ref} {}

Palette::Colors Palette::color_extractor(uint8_t color_index) const {
  return static_cast<Colors>((palette_ref & (0x03 << (2 * color_index))) >>
                             (2 * color_index));
}

Palette::Colors Palette::GetColor0() const { return color_extractor(0); }
Palette::Colors Palette::GetColor1() const { return color_extractor(1); }
Palette::Colors Palette::GetColor2() const { return color_extractor(2); }
Palette::Colors Palette::GetColor3() const { return color_extractor(3); }
