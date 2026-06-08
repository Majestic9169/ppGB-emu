/*
 * MMU
 * - Class to manage all ROM accesses
 * - should be shared across the other classes
 * - learncpp said std::vector for non-constexpr uses and i have to load from a
 * file so...
 */

#include "../../include/mmu/mmu.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <sys/types.h>

void MMU::header_information() {
  std::string title{std::string{ROM.begin() + 0x134, ROM.begin() + 0x143}};
  bool gbc_support = ROM[0x143] == 0x80 || ROM[0x143] == 0xc0 ? true : false;
  int rom_size = 0x8000 * (1 << ROM[0x148]);
  int rom_banks = rom_size / 0x4000;
  std::cout << YEL << "[~] ROM Header Information\n" << COLOR_RESET;
  std::cout << "Title       " << title << std::endl;
  std::cout << "GBC support " << std::boolalpha << gbc_support
            << std::noboolalpha << std::endl;
  std::cout << "ROM size    " << rom_size << std::endl;
  std::cout << "No. Banks   " << rom_banks << std::endl;
}

MMU::MMU(Opts *opts_) : ROM(ROM_SIZE), cli_opts{opts_}, OAM{} {
  std::ifstream rom_file{cli_opts->rom_name(), std::ios::binary};

  if (!rom_file.good()) {
    std::cerr << RED << "[!] Error loading ROM_FILE\n" << COLOR_RESET;
    exit(2);
  }

  rom_file.read(reinterpret_cast<char *>(ROM.data()), ROM_SIZE);
  if (cli_opts->debug_enabled()) {
    std::cout << GRN << "[+] ROM successfully loaded\n" << COLOR_RESET;
    header_information();
  }

  // std::ifstream boot_rom_file{"./roms/dmg_boot.bin", std::ios::binary};
  // if (!boot_rom_file.good()) {
  //   std::cerr << RED << "[!] Error loading BOOT_ROM_FILE\n" << COLOR_RESET;
  //   exit(2);
  // }
  // boot_rom_file.read(reinterpret_cast<char *>(ROM.data()), 0x100);
  // if (cli_opts->debug_enabled()) {
  //   std::cout << GRN << "[+] BOOT_ROM successfully loaded\n" << COLOR_RESET;
  //   header_information();
  // }

  // init OAM
  OAM.reserve(40);
  for (size_t i = 0; i < 40; i++) {
    OAM.emplace_back(ROM.begin() + 0xfe00 + (i * 4));
  }

  // init hardware registers
  ROM[0xFF0F] = 0xe1; // IF
  ROM[0xFF40] = 0x91; // LCDC
  ROM[0xFF41] = 0x85; // STAT
  ROM[0xFF47] = 0xfc; // BGP
  ROM[0xFF48] = 0xff; // OBP0
  ROM[0xFF49] = 0xff; // OBP1
  ROM[0xFF50] = 0x01; // disable boot rom
  ROM[0xFFFF] = 0x00; // IE
}

// TODO: clean up with util function
void MMU::hexdump() const {
  FILE *vram = fopen("vram.dump", "w");
  for (size_t i = 0x8000; i < 0xA000; i += 16) {
    // offset
    fprintf(vram, "%08zx  ", i);
    // hex bytes
    for (size_t j = 0; j < 16; ++j) {
      if (i + j < ROM.size())
        fprintf(vram, "%02X ", ROM[i + j]);
      else
        fprintf(vram, "   ");
    }
    fprintf(vram, " ");
    // ASCII representation
    for (size_t j = 0; j < 16; ++j) {
      if (i + j < ROM.size()) {
        char c = ROM[i + j];
        fprintf(vram, "%c", (c >= 32 && c < 127) ? c : '.');
      }
    }
    fprintf(vram, "\n");
  }
  fprintf(vram, "=========== OAM - RAM =============\n");
  for (size_t i = 0xfe00; i < 0xfea0; i += 16) {
    // offset
    fprintf(vram, "%08zx  ", i);
    // hex bytes
    for (size_t j = 0; j < 16; ++j) {
      if (i + j < ROM.size())
        fprintf(vram, "%02X ", ROM[i + j]);
      else
        fprintf(vram, "   ");
    }
    fprintf(vram, " ");
    // ASCII representation
    for (size_t j = 0; j < 16; ++j) {
      if (i + j < ROM.size()) {
        char c = ROM[i + j];
        fprintf(vram, "%c", (c >= 32 && c < 127) ? c : '.');
      }
    }
    fprintf(vram, "\n");
  }
  fprintf(vram, "=========== OAM - OAM =============\n");
  for (size_t i = 0; i < OAM.size(); i += 16) {
    // offset
    fprintf(vram, "%08zx  ", i);
    // hex bytes
    for (size_t j = 0; j < 16; ++j) {
      if (i + j < OAM.size())
        fprintf(vram, "%02X ", OAM[i + j].GetTileIndex());
      else
        fprintf(vram, "   ");
    }
    fprintf(vram, " ");
    // ASCII representation
    for (size_t j = 0; j < 16; ++j) {
      if (i + j < OAM.size()) {
        char c = OAM[i + j].GetTileIndex();
        fprintf(vram, "%c", (c >= 32 && c < 127) ? c : '.');
      }
    }
    fprintf(vram, "\n");
  }
  fclose(vram);
}

uint8_t &MMU::read_byte(uint16_t addr) { return ROM[addr]; }
uint16_t MMU::read_word(uint16_t addr) {
  return ROM[addr] | ROM[addr + 1] << 8;
}
void MMU::write_byte(uint16_t addr, uint8_t val) {
  if (cli_opts->debug_enabled() && addr == 0xff40) {
    printf("LCDC write: %02x ly: %d\n", val, ly());
  }
  ROM[addr] = val;
}
void MMU::write_word(uint16_t addr, uint16_t val) {
  write_byte(addr, static_cast<uint8_t>(val));
  write_byte(addr + 1, static_cast<uint8_t>(val >> 8));
}

// PPU Access Shit

// background scroll
uint8_t &MMU::scy() { return ROM[0xff42]; }
uint8_t &MMU::scx() { return ROM[0xff43]; }

// ly and lyc
uint8_t &MMU::ly() { return ROM[0xff44]; }
uint8_t &MMU::lyc() { return ROM[0xff45]; }

// window position
uint8_t &MMU::wy() { return ROM[0xff4a]; }
uint8_t &MMU::wx() { return ROM[0xff4b]; }

// get tiles from either of the 2 tile maps
TILE MMU::GetTileFromIndex(uint16_t index, TILE::LAYERS layer) {
  uint16_t base_addr{0x8000};
  uint16_t offset{0};

  if (layer == TILE::OBJECT || lcdc.TileMap() == 0x8000) {
    offset = index * 16;
  } else {
    base_addr = 0x9000;
    int8_t signed_index = static_cast<int8_t>(index);
    offset = signed_index * 16;
  }

  if (cli_opts->debug_enabled()) {
    printf("GetTile: index=%02x layer=%d addr=%04x lcdc=%02x\n", index, layer,
           base_addr + offset, read_byte(0xff40));
  }

  const auto start{ROM.begin() + base_addr + offset};
  TILE tile{start};
  return tile;
}
