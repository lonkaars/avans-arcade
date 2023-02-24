#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "ppu/consts.h"

typedef uint16_t hh_ppu_addr_t;
typedef uint16_t hh_ppu_data_t;

typedef uint8_t hh_ppu_rgb_color_t[3];

typedef hh_ppu_rgb_color_t hh_ppu_loc_palette_data_t[HH_PPU_PALETTE_COLOR_COUNT];
typedef hh_ppu_loc_palette_data_t hh_ppu_loc_palette_table_t[HH_PPU_PALETTE_COUNT];

typedef uint8_t hh_ppu_loc_sprite_data_t[HH_PPU_SPRITE_WIDTH * HH_PPU_SPRITE_HEIGHT];
typedef hh_ppu_loc_sprite_data_t hh_s_ppu_loc_sprite;

typedef struct {
	bool horizontal_flip;
	bool vertical_flip;
	uint8_t palette_index;
	uint8_t tilemap_index;
} hh_s_ppu_loc_bam_entry;

typedef struct {
	bool horizontal_flip;
	bool vertical_flip;
	int32_t position_x;
	int32_t position_y;
	uint8_t palette_index;
	uint8_t tilemap_index;
} hh_s_ppu_loc_fam_entry;

typedef struct {
	bool sysreset;
	bool fg_fetch;
	uint16_t bg_shift_x;
	uint16_t bg_shift_y;
} hh_s_ppu_loc_aux;

typedef uint16_t hh_ppu_native_color_t;
