#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "ppu/consts.h"

/** @brief PPU VRAM address */
typedef uint16_t hh_ppu_addr_t;
/** @brief PPU VRAM data word */
typedef uint16_t hh_ppu_data_t;

/** @brief local RGB color */
typedef uint8_t hh_ppu_rgb_color_t[3];

/** @brief local palette */
typedef hh_ppu_rgb_color_t hh_ppu_loc_palette_data_t[HH_PPU_PALETTE_COLOR_COUNT];
/** @brief local palette table */
typedef hh_ppu_loc_palette_data_t hh_ppu_loc_palette_table_t[HH_PPU_PALETTE_COUNT];

/** @brief local sprite */
typedef uint8_t hh_ppu_loc_sprite_data_t[HH_PPU_SPRITE_WIDTH * HH_PPU_SPRITE_HEIGHT];
typedef hh_ppu_loc_sprite_data_t hh_s_ppu_loc_sprite;

/** @brief local background attribute memory entry */
typedef struct {
	bool horizontal_flip; /** @brief flip sprite horizontally */
	bool vertical_flip; /** @brief flip sprite vertically */
	uint8_t palette_index; /** @brief index of palette to use */
	uint8_t tilemap_index; /** @brief index of sprite to use */
} hh_s_ppu_loc_bam_entry;

/** @brief local foreground attribute memory entry */
typedef struct {
	bool horizontal_flip; /** @brief flip sprite horizontally */
	bool vertical_flip; /** @brief flip sprite vertically */
	int32_t position_x; /** @brief horizontal position from left edge of sprite to left edge of screen (-16 to 320), relative to viewport */
	int32_t position_y; /** @brief vertical position from top edge of sprite to top edge of screen (-16 to 240), relative to viewport */
	uint8_t palette_index; /** @brief index of palette to use */
	uint8_t tilemap_index; /** @brief index of sprite to use */
} hh_s_ppu_loc_fam_entry;

/** @brief local copy of auxiliary memory */
typedef struct {
	bool sysreset; /** @brief enable sysreset, self-resets */
	bool fg_fetch; /** @brief enable foreground fetch, self-resets */
	uint16_t bg_shift_x; /** @brief shift background canvas left (0 to 320) */
	uint16_t bg_shift_y; /** @brief shift background canvas down (0 to 240) */
} hh_s_ppu_loc_aux;

