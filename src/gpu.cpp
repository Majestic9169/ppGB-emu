#include "../include/cpu.h"
#include <cstdint>

void CPU::PPU_STEP() {
  if (!flag_value(LCD_ENABLE, LCD_CONTROL)) {
    mode = HBLANK;
    return;
  }

  switch (mode) {
  case HBLANK: {
    mode = OAM;
    uint8_t scanline = read_byte(0xFF44);
    write_byte(0xFF44, ++scanline);
    uint8_t lyc = read_byte(0xFF45);
    flag_value(COINCIDENCE_FLAG, (lyc == scanline), LCD_STATUS);
    if (lyc == scanline && flag_value(COINCIDENCE_INTERRUPT, LCD_STATUS)) {
    }

    if (scanline == 144) {
      mode = VBLANK;
      can_render = true;
      if (flag_value(VBLANK_INTERRUPT, LCD_STATUS)) {
      } else if (flag_value(OAM_INTERRUPT, LCD_STATUS)) {
      }
    }

    write_byte(0xFF41, (read_byte(0xFF41) & 0xFC) | (mode & 3));
  } break;
  case VBLANK: {
    uint8_t scanline = read_byte(0xFF44);
    write_byte(0xFF44, ++scanline);
    uint8_t lyc = read_byte(0xFF45);
    flag_value(COINCIDENCE_FLAG, (lyc == scanline), LCD_STATUS);
    if (lyc == scanline && flag_value(COINCIDENCE_INTERRUPT, LCD_STATUS)) {
    }

    if (scanline == 153) {
      write_byte(0xFF44, 0);
      mode = OAM;
      write_byte(0xFF41, (read_byte(0xFF41) & 0xFC) | (mode & 3));
      if (flag_value(OAM_INTERRUPT, LCD_STATUS)) {
      }
    }
  } break;
  case OAM: {
    mode = DRAW;
    write_byte(0xFF41, (read_byte(0xFF41) & 0xFC) | (mode & 3));
  } break;
  case DRAW: {
    mode = HBLANK;
    scanlines();
    write_byte(0xFF41, (read_byte(0xFF41) & 0xFC) | (mode & 3));
    if (flag_value(HBLANK_INTERRUPT, LCD_STATUS)) {
    }
  } break;
  }
}

void CPU::scanline_background(bool *pixel_row) {
  uint16_t addr = 0x9800;
  if (flag_value(BG_MAP_SELECT, LCD_CONTROL)) {
    addr += 0x400;
  }
  uint8_t scroll_x = read_byte(0xFF43);
  uint8_t scroll_y = read_byte(0xFF42);
  uint8_t scanline = read_byte(0xFF44);

  addr += (scroll_y + scanline) / (8 * 32);
  addr %= (32 * 32);

  uint16_t start_row = addr;
  uint16_t end_row = addr + 32;

  addr += (scroll_x >> 3);

  int x = scroll_x & 7;
  int y = (scanline + scroll_y) & 7;
  int pixel_offset = scanline * 160;
  int pixel = 0;

  for (int i = 0; i < 21; i++) {
    uint16_t tile_address = addr + 1;

    if (tile_address >= end_row) {
      tile_address = start_row + (tile_address % end_row);
    }

    int tile = read_byte(tile_address);

    if (flag_value(TILE_SELECT, LCD_CONTROL) && tile < 128) {
      tile += 256;
    }

    for (; x < 8; x++) {
      if (pixel >= 160) {
        break;
      }

      int color = ROM.tiles[tile].pixels[y][x];
      framebuffer[pixel_offset++] = ROM.colors[color == 0 ? 0 : 3];
      if (color > 0) {
        pixel_row[pixel] = true;
      }
      pixel++;
    }
    x = 0;
  }
}

void CPU::scanline_window() {
  if (!flag_value(WIN_ENABLE, LCD_CONTROL)) {
    return;
  }

  uint8_t scroll_x = read_byte(0xFF43);
  uint8_t scroll_y = read_byte(0xFF42);
  uint8_t scanline = read_byte(0xFF44);

  if (read_byte(0xFF4A) > scanline) {
    return;
  }

  uint16_t addr = 0x9800;
  if (flag_value(BG_MAP_SELECT, LCD_CONTROL)) {
    addr += 0x400;
  }

  addr += ((scanline - read_byte(0xFF4A)) / 8) * 32;

  int x = 0;
  int y = (scanline - read_byte(0xFF4A)) & 7;
  int pixel_offset = scanline * 160;
  pixel_offset += read_byte(0xFF4B) - 7;

  for (uint16_t tile_address = addr; tile_address < addr + 20; tile_address++) {
    int tile = read_byte(tile_address);

    if (!flag_value(BG_MAP_SELECT, LCD_CONTROL) && tile < 128) {
      tile += 256;
    }

    for (; x < 8; x++) {
      if (pixel_offset > sizeof(framebuffer)) {
        continue;
      }
      int color = ROM.tiles[tile].pixels[y][x];
      framebuffer[pixel_offset++] = ROM.colors[color == 0 ? 0 : 3];
    }
    x = 0;
  }
}

void CPU::scanline_sprites(bool *pixel_row) {
  int sprite_height = flag_value(SPR_SIZE, LCD_CONTROL) ? 16 : 8;

  bool visible_sprites[40];
  int sprite_row_count = 0;

  uint8_t scroll_x = read_byte(0xFF43);
  uint8_t scroll_y = read_byte(0xFF42);
  uint8_t scanline = read_byte(0xFF44);

  for (int i = 39; i >= 0; i--) {
    auto sprite = ROM.sprites[i];

    if ((sprite.y > scanline) || (sprite.y + sprite_height) <= scanline) {
      visible_sprites[i] = false;
      continue;
    }

    visible_sprites[i] = sprite_row_count++ <= 10;
  }

  for (int i = 39; i >= 0; i--) {
    if (!visible_sprites[i]) {
      continue;
    }

    auto sprite = ROM.sprites[i];
    if (((int)sprite.x < -7) || sprite.x >= 160) {
      continue;
    }

    int pixel_y = scanline - sprite.y;
    pixel_y = sprite.flags.y_flip
                  ? (7 + 8 * flag_value(SPR_SIZE, LCD_CONTROL)) - pixel_y
                  : pixel_y;
    for (int x = 0; x < 8; x++) {
      int tile_number =
          sprite.tile & (flag_value(SPR_SIZE, LCD_CONTROL) ? 0xFE : 0xFF);
      int color = 0;

      int x_temp = sprite.x + x;
      if (x_temp < 0 || x_temp >= 160) {
        continue;
      }

      int pixel_offset = scanline + 160 * x_temp;

      uint8_t pixel_x = sprite.flags.x_flip ? 7 - x : x;

      if (flag_value(SPR_SIZE, LCD_CONTROL) && (pixel_y >= 8)) {
        color = ROM.tiles[tile_number + 1].pixels[pixel_y - 8][pixel_x];
      } else {
        color = ROM.tiles[tile_number].pixels[pixel_y][pixel_x];
      }

      if (!color) {
        continue;
      }

      if (!pixel_row[x_temp] || !sprite.flags.render_priority) {
        framebuffer[pixel_offset] = ROM.colors[color];
      }
    }
  }
}

void CPU::scanlines() {
  bool pixel_row[160] = {0};
  scanline_background(pixel_row);
  scanline_window();
  scanline_sprites(pixel_row);
}
