#pragma once

/** @brief amount of total palettes in PPU */
#define HH_PPU_PALETTE_COUNT 8
/** @brief amount of colors in palette */
#define HH_PPU_PALETTE_COLOR_COUNT 8
/** @brief address line width in bits */
#define HH_PPU_RAM_BUS_ADDR_WIDTH 16
/** @brief data line width in bits */
#define HH_PPU_RAM_BUS_DATA_WIDTH 16
/** @brief max amount of foreground sprites allowed */
#define HH_PPU_FG_SPRITE_COUNT 128
/** @brief sprite width in pixel */
#define HH_PPU_SPRITE_WIDTH 16
/** @brief sprite height in pixels */
#define HH_PPU_SPRITE_HEIGHT 16
/** @brief screen width in pixels */
#define HH_PPU_SCREEN_WIDTH 320
/** @brief screen height in pixels */
#define HH_PPU_SCREEN_HEIGHT 240
/** @brief amount of horizontal background tiles on background canvas */
#define HH_PPU_BG_CANVAS_TILES_H 40
/** @brief amount of vertical background tiles on background canvas */
#define HH_PPU_BG_CANVAS_TILES_V 30

#include "ppu/types.h"

/** @brief tilemap memory address offset */
#define HH_PPU_VRAM_TMM_OFFSET ((hh_ppu_addr_t) 0x0000)
/** @brief tilemap memory size in words */
#define HH_PPU_VRAM_TMM_SIZE ((hh_ppu_addr_t) 0xd000)
/** @brief sprite size in words */
#define HH_PPU_VRAM_TMM_SPRITE_SIZE 52
/** @brief pixels per word for sprite */
#define HH_PPU_VRAM_TMM_SPRITE_PPW 5
/** @brief background attribute memory address offset */
#define HH_PPU_VRAM_BAM_OFFSET ((hh_ppu_addr_t) 0xd000)
/** @brief background attribute memory size in words */
#define HH_PPU_VRAM_BAM_SIZE ((hh_ppu_addr_t) 0x04b0)
/** @brief background attribute memory entry size in words */
#define HH_PPU_VRAM_BAM_ENTRY_SIZE 1
/** @brief foreground attribute memory address offset */
#define HH_PPU_VRAM_FAM_OFFSET ((hh_ppu_addr_t) 0xd800)
/** @brief foreground attribute memory size in words */
#define HH_PPU_VRAM_FAM_SIZE ((hh_ppu_addr_t) 0x0100)
/** @brief foreground attribute memory entry size in words */
#define HH_PPU_VRAM_FAM_ENTRY_SIZE 2
/** @brief palette memory address offset */
#define HH_PPU_VRAM_PAL_OFFSET ((hh_ppu_addr_t) 0xdc00)
/** @brief palette memory size in words */
#define HH_PPU_VRAM_PAL_SIZE ((hh_ppu_addr_t) 0x0040)
/** @brief palette entry size in words */
#define HH_PPU_VRAM_PAL_ENTRY_SIZE 1
/** @brief auxiliary memory address offset */
#define HH_PPU_VRAM_AUX_OFFSET ((hh_ppu_addr_t) 0xde00)
/** @brief auxiliary memory size in words */
#define HH_PPU_VRAM_AUX_SIZE ((hh_ppu_addr_t) 0x0002)

/** @brief sprite size in bytes */
#define HH_PPU_BYTE_SPRITE_SIZE (HH_PPU_VRAM_TMM_SPRITE_SIZE * sizeof(hh_ppu_data_t))
/** @brief sprite size in 32-bit words (for stm) */
#define HH_PPU_NATIVE_SPRITE_SIZE (HH_PPU_BYTE_SPRITE_SIZE / sizeof(uint32_t))

