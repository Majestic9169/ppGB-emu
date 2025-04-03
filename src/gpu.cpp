#include "../include/cpu.h"
#include "../include/display.h"
#include <cstdint>

void CPU::PPU_STEP() {
  // If LCD is disabled, stay in HBLANK mode
  if (!flag_value(LCD_ENABLE, LCD_CONTROL)) {
    mode = HBLANK;
    write_byte(0xFF41, (read_byte(0xFF41) & 0xFC) | (mode & 3));
    return;
  }

  uint8_t scanline = read_byte(0xFF44);
  uint8_t lyc = read_byte(0xFF45);
  uint8_t stat = read_byte(0xFF41);

  // Check for LY=LYC coincidence
  bool coincidence = (lyc == scanline);
  flag_value(COINCIDENCE_FLAG, coincidence, LCD_STATUS);

  // Update mode based on PPU clock
  switch (mode) {
  case OAM: {
    if (ppu_clock >= OAM_CYCLES) {
      ppu_clock = 0;
      mode = DRAW;
      write_byte(0xFF41, (stat & 0xFC) | (mode & 3));
    }
    break;
  }
  case DRAW: {
    if (ppu_clock >= DRAW_CYCLES) {
      ppu_clock = 0;
      mode = HBLANK;
      write_byte(0xFF41, (stat & 0xFC) | (mode & 3));

      // Render the current scanline
      bool pixel_row[GB_WIDTH] = {false};
      scanline_background(pixel_row);
      scanline_window();
      scanline_sprites(pixel_row);

      // Check for HBLANK interrupt
      if (flag_value(HBLANK_INTERRUPT, LCD_STATUS)) {
        interrupt_flags |= (1 << INT_LCD);
      }
    }
    break;
  }
  case HBLANK: {
    if (ppu_clock >= HBLANK_CYCLES) {
      ppu_clock = 0;
      scanline++;

      if (scanline == VBLANK_START_LINE) {
        mode = VBLANK;
        write_byte(0xFF41, (stat & 0xFC) | (mode & 3));
        can_render = true;

        // Trigger VBLANK interrupt
        interrupt_flags |= (1 << INT_VBLANK);
        
        // Check for VBLANK interrupt
        if (flag_value(VBLANK_INTERRUPT, LCD_STATUS)) {
          interrupt_flags |= (1 << INT_LCD);
        }
      } else {
        mode = OAM;
        write_byte(0xFF41, (stat & 0xFC) | (mode & 3));
        
        // Check for OAM interrupt
        if (flag_value(OAM_INTERRUPT, LCD_STATUS)) {
          interrupt_flags |= (1 << INT_LCD);
        }
      }
      write_byte(0xFF44, scanline);
    }
    break;
  }
  case VBLANK: {
    if (ppu_clock >= CYCLES_PER_SCANLINE) {
      ppu_clock = 0;
      scanline++;

      if (scanline > VBLANK_END_LINE) {
        scanline = 0;
        mode = OAM;
        write_byte(0xFF41, (stat & 0xFC) | (mode & 3));
        
        // Check for OAM interrupt
        if (flag_value(OAM_INTERRUPT, LCD_STATUS)) {
          interrupt_flags |= (1 << INT_LCD);
        }
      }
      write_byte(0xFF44, scanline);
    }
    break;
  }
  }

  // Check for LY=LYC interrupt
  if (coincidence && flag_value(COINCIDENCE_INTERRUPT, LCD_STATUS)) {
    interrupt_flags |= (1 << INT_LCD);
  }

  ppu_clock++;
}

void CPU::scanline_background(bool *pixel_row) {
  if (!flag_value(BG_ENABLE, LCD_CONTROL)) {
    return;
  }

  uint8_t scroll_y = read_byte(0xFF42);
  uint8_t scroll_x = read_byte(0xFF43);
  uint8_t scanline = read_byte(0xFF44);

  // Calculate which tile row we're on
  uint16_t tile_row = ((scanline + scroll_y) / 8) % 32;
  uint16_t tile_col = (scroll_x / 8) % 32;

  // Calculate the base address for the tile map
  uint16_t tile_map_base =
      flag_value(BG_MAP_SELECT, LCD_CONTROL) ? 0x9C00 : 0x9800;
  uint16_t tile_data_base =
      flag_value(TILE_SELECT, LCD_CONTROL) ? 0x8000 : 0x8800;

  // Calculate the pixel row within the tile
  uint8_t pixel_row_in_tile = (scanline + scroll_y) % 8;

  // For each pixel in the scanline
  for (int x = 0; x < GB_WIDTH; x++) {
    // Calculate which tile we're on
    uint16_t current_tile_col = (tile_col + (x / 8)) % 32;

    // Get the tile number from the tile map
    uint16_t tile_addr = tile_map_base + (tile_row * 32) + current_tile_col;
    uint8_t tile_num = read_byte(tile_addr);

    // Adjust tile number based on tile data area
    if (tile_data_base == 0x8800) {
      tile_num = ((int8_t)tile_num) + 128;
    }

    // Calculate the address of the tile data
    uint16_t tile_data_addr =
        tile_data_base + (tile_num * 16) + (pixel_row_in_tile * 2);

    // Get the two bytes that define the colors for this row
    uint8_t low_byte = read_byte(tile_data_addr);
    uint8_t high_byte = read_byte(tile_data_addr + 1);

    // Calculate which bit pair we need
    int bit_position = 7 - ((x + scroll_x) % 8);

    // Get the color number (0-3)
    uint8_t color_num = ((high_byte >> bit_position) & 1) << 1;
    color_num |= (low_byte >> bit_position) & 1;

    // Get the actual color from the palette
    uint8_t palette = read_byte(0xFF47);
    uint8_t color = (palette >> (color_num * 2)) & 0x3;

    // Set the pixel in the framebuffer
    int pixel_offset = scanline * GB_WIDTH + x;
    framebuffer[pixel_offset] = ROM.colors[color];

    // Mark this pixel as drawn for sprite priority
    if (color != 0) {
      pixel_row[x] = true;
    }
  }
}

void CPU::scanline_window() {
  if (!flag_value(WIN_ENABLE, LCD_CONTROL)) {
    return;
  }

  uint8_t scanline = read_byte(0xFF44);
  uint8_t window_y = read_byte(0xFF4A);
  uint8_t window_x = read_byte(0xFF4B) - 7; // Window X is offset by 7

  // Check if we're within the window's Y range
  if (scanline < window_y) {
    return;
  }

  // Calculate which tile row we're on
  uint16_t tile_row = (scanline - window_y) / 8;
  uint16_t tile_col = 0;

  // Calculate the base address for the tile map
  uint16_t tile_map_base =
      flag_value(WIN_MAP_SELECT, LCD_CONTROL) ? 0x9C00 : 0x9800;
  uint16_t tile_data_base =
      flag_value(TILE_SELECT, LCD_CONTROL) ? 0x8000 : 0x8800;

  // Calculate the pixel row within the tile
  uint8_t pixel_row_in_tile = (scanline - window_y) % 8;

  // For each pixel in the scanline
  for (int x = 0; x < GB_WIDTH; x++) {
    // Skip pixels before the window's X position
    if (x < window_x) {
      continue;
    }

    // Calculate which tile we're on
    uint16_t current_tile_col = (x - window_x) / 8;

    // Get the tile number from the tile map
    uint16_t tile_addr = tile_map_base + (tile_row * 32) + current_tile_col;
    uint8_t tile_num = read_byte(tile_addr);

    // Adjust tile number based on tile data area
    if (tile_data_base == 0x8800) {
      tile_num = ((int8_t)tile_num) + 128;
    }

    // Calculate the address of the tile data
    uint16_t tile_data_addr =
        tile_data_base + (tile_num * 16) + (pixel_row_in_tile * 2);

    // Get the two bytes that define the colors for this row
    uint8_t low_byte = read_byte(tile_data_addr);
    uint8_t high_byte = read_byte(tile_data_addr + 1);

    // Calculate which bit pair we need
    int bit_position = 7 - ((x - window_x) % 8);

    // Get the color number (0-3)
    uint8_t color_num = ((high_byte >> bit_position) & 1) << 1;
    color_num |= (low_byte >> bit_position) & 1;

    // Get the actual color from the palette
    uint8_t palette = read_byte(0xFF47);
    uint8_t color = (palette >> (color_num * 2)) & 0x3;

    // Set the pixel in the framebuffer
    int pixel_offset = scanline * GB_WIDTH + x;
    framebuffer[pixel_offset] = ROM.colors[color];
  }
}

void CPU::scanline_sprites(bool *pixel_row) {
  if (!flag_value(SPR_ENABLE, LCD_CONTROL)) {
    return;
  }

  uint8_t scanline = read_byte(0xFF44);
  bool large_sprites = flag_value(SPR_SIZE, LCD_CONTROL);
  int sprite_height = large_sprites ? 16 : 8;

  // Find sprites that intersect with the current scanline
  struct SpriteInfo {
    uint8_t y;
    uint8_t x;
    uint8_t tile;
    uint8_t attributes;
    int index;
  };
  SpriteInfo visible_sprites[10];
  int visible_count = 0;

  // Search OAM for sprites that intersect with the current scanline
  for (int i = 0; i < 40 && visible_count < 10; i++) {
    uint8_t y = read_byte(0xFE00 + i * 4) - 16;
    uint8_t x = read_byte(0xFE00 + i * 4 + 1) - 8;
    uint8_t tile = read_byte(0xFE00 + i * 4 + 2);
    uint8_t attributes = read_byte(0xFE00 + i * 4 + 3);

    if (scanline >= y && scanline < y + sprite_height) {
      visible_sprites[visible_count] = {y, x, tile, attributes, i};
      visible_count++;
    }
  }

  // Sort sprites by X coordinate (priority)
  for (int i = 0; i < visible_count - 1; i++) {
    for (int j = 0; j < visible_count - i - 1; j++) {
      if (visible_sprites[j].x > visible_sprites[j + 1].x) {
        std::swap(visible_sprites[j], visible_sprites[j + 1]);
      }
    }
  }

  // Render each visible sprite
  for (int i = 0; i < visible_count; i++) {
    SpriteInfo &sprite = visible_sprites[i];

    // Calculate the tile row within the sprite
    int tile_row = scanline - sprite.y;
    if (sprite.attributes & 0x40) { // Y-flip
      tile_row = sprite_height - 1 - tile_row;
    }

    // Get the tile number
    uint8_t tile_num = sprite.tile;
    if (large_sprites) {
      tile_num &= 0xFE; // Clear the lowest bit for large sprites
      if (tile_row >= 8) {
        tile_num |= 0x01; // Use the next tile for the bottom half
        tile_row -= 8;
      }
    }

    // Calculate the address of the tile data
    uint16_t tile_data_addr = 0x8000 + (tile_num * 16) + (tile_row * 2);

    // Get the two bytes that define the colors for this row
    uint8_t low_byte = read_byte(tile_data_addr);
    uint8_t high_byte = read_byte(tile_data_addr + 1);

    // Get the sprite palette
    uint8_t palette = read_byte(sprite.attributes & 0x10 ? 0xFF49 : 0xFF48);

    // For each pixel in the sprite row
    for (int x = 0; x < 8; x++) {
      int pixel_x = sprite.x + x;
      if (pixel_x < 0 || pixel_x >= GB_WIDTH) {
        continue;
      }

      // Skip if background pixel exists and sprite has priority
      if (pixel_row[pixel_x] && (sprite.attributes & 0x80)) {
        continue;
      }

      // Calculate which bit pair we need
      int bit_position = sprite.attributes & 0x20 ? x : 7 - x; // X-flip

      // Get the color number (0-3)
      uint8_t color_num = ((high_byte >> bit_position) & 1) << 1;
      color_num |= (low_byte >> bit_position) & 1;

      // Skip transparent pixels
      if (color_num == 0) {
        continue;
      }

      // Get the actual color from the palette
      uint8_t color = (palette >> (color_num * 2)) & 0x3;

      // Set the pixel in the framebuffer
      int pixel_offset = scanline * GB_WIDTH + pixel_x;
      framebuffer[pixel_offset] = ROM.colors[color];
    }
  }
}

void CPU::scanlines() {
  bool pixel_row[160] = {0};
  scanline_background(pixel_row);
  scanline_window();
  scanline_sprites(pixel_row);
}
