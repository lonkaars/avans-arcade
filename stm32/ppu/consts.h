#pragma once

#define HH_PPU_PALETTE_COUNT 8
#define HH_PPU_PALETTE_COLOR_COUNT 8
#define HH_PPU_RAM_BUS_ADDR_WIDTH 16
#define HH_PPU_RAM_BUS_DATA_WIDTH 16
#define HH_PPU_FG_SPRITE_COUNT 128
#define HH_PPU_SPRITE_WIDTH 16
#define HH_PPU_SPRITE_HEIGHT 16
#define HH_PPU_SCREEN_WIDTH 320
#define HH_PPU_SCREEN_HEIGHT 240
#define HH_PPU_BG_CANVAS_TILES_H 40
#define HH_PPU_BG_CANVAS_TILES_V 30

#include "types.h"

#define HH_PPU_VRAM_TMM_OFFSET ((hh_ppu_addr_t) 0x0000)
#define HH_PPU_VRAM_TMM_SIZE ((hh_ppu_addr_t) 0xd000)
#define HH_PPU_VRAM_BAM_OFFSET ((hh_ppu_addr_t) 0xd000)
#define HH_PPU_VRAM_BAM_SIZE ((hh_ppu_addr_t) 0x04b0)
#define HH_PPU_VRAM_FAM_OFFSET ((hh_ppu_addr_t) 0xd800)
#define HH_PPU_VRAM_FAM_SIZE ((hh_ppu_addr_t) 0x0100)
#define HH_PPU_VRAM_PAL_OFFSET ((hh_ppu_addr_t) 0xdc00)
#define HH_PPU_VRAM_PAL_SIZE ((hh_ppu_addr_t) 0x0040)
#define HH_PPU_VRAM_AUX_OFFSET ((hh_ppu_addr_t) 0xde00)
#define HH_PPU_VRAM_AUX_SIZE ((hh_ppu_addr_t) 0x0002)
