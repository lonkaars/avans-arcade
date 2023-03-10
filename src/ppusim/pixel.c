#include <SDL2/SDL.h>
#include <stdbool.h>

#include "ppu/consts.h"
#include "ppu/internals.h"
#include "ppusim/mem.h"
#include "ppusim/work.h"

/* transform xy if tile is flipped */
static uint16_t hh_ppusim_apply_transform(unsigned x, unsigned y, bool fliph, bool flipv) {
	unsigned tx = fliph ? HH_PPU_SPRITE_WIDTH - x - 1 : x;
	unsigned ty = flipv ? HH_PPU_SPRITE_HEIGHT - y - 1 : y;
	return ty * HH_PPU_SPRITE_WIDTH + tx;
}

/* @brief get current bg pixel cidx */
static uint8_t hh_ppusim_bg_pixel(unsigned x, unsigned y) {
	hh_ppu_data_t *aux		= &g_hh_ppusim_vram[HH_PPU_VRAM_AUX_OFFSET];
	unsigned bg_shift_y		= HH_RESIZE(aux[0], 7, 0);
	unsigned bg_shift_x		= HH_RESIZE(aux[0], 15, 8) | HH_RESIZE(aux[1], 0, 0) << 8;
	unsigned abs_x			= bg_shift_x + x;
	unsigned abs_y			= bg_shift_y + y;
	unsigned grid_x			= abs_x / HH_PPU_SPRITE_WIDTH;
	unsigned grid_y			= abs_y / HH_PPU_SPRITE_HEIGHT;
	unsigned loc_x			= abs_x - grid_x * HH_PPU_SPRITE_WIDTH;
	unsigned loc_y			= abs_y - grid_y * HH_PPU_SPRITE_HEIGHT;
	unsigned bam_offset		= grid_y * HH_PPU_BG_CANVAS_TILES_H + grid_x;
	hh_ppu_data_t bam		= g_hh_ppusim_vram[HH_PPU_VRAM_BAM_OFFSET + bam_offset];
	uint8_t cidx			= 0;
	uint16_t tile_pixel_idx = hh_ppusim_apply_transform(loc_x, loc_y, HH_RESIZE(bam, 14, 14), HH_RESIZE(bam, 13, 13));
	uint16_t tile_idx		= HH_RESIZE(bam, 9, 0);
	hh_ppu_addr_t ttm_addr	= tile_idx * HH_PPU_VRAM_TMM_SPRITE_SIZE + tile_pixel_idx / 5;
	uint8_t word_bit_addr	= (tile_pixel_idx % 5) * 3;
	hh_ppu_data_t tmm		= g_hh_ppusim_vram[HH_PPU_VRAM_TMM_OFFSET + ttm_addr];
	cidx |= HH_RESIZE(bam, 12, 10) << 3;
	cidx |= HH_RESIZE(tmm, word_bit_addr + 2, word_bit_addr) << 0;
	return cidx;
}

/* @brief get current fg pixel cidx */
static uint8_t hh_ppusim_fg_pixel(unsigned x, unsigned y) {
	x += 16;
	y += 16;
	uint8_t cidx = 0;
	for (unsigned i = 0; i < HH_PPU_FG_SPRITE_COUNT; i++) {
		unsigned fam_offset = i * HH_PPU_VRAM_FAM_ENTRY_SIZE;
		hh_ppu_data_t *fam	= &g_hh_ppusim_vram[HH_PPU_VRAM_FAM_OFFSET + fam_offset];
		unsigned sprite_y	= HH_RESIZE(fam[0], 15, 13) | HH_RESIZE(fam[1], 4, 0) << 3;
		unsigned sprite_x	= HH_RESIZE(fam[1], 13, 5);
		if (x < sprite_x) continue;
		if (x >= sprite_x + HH_PPU_SPRITE_WIDTH) continue;
		if (y < sprite_y) continue;
		if (y >= sprite_y + HH_PPU_SPRITE_HEIGHT) continue;
		unsigned loc_x			= x - sprite_x;
		unsigned loc_y			= y - sprite_y;
		uint16_t tile_pixel_idx = hh_ppusim_apply_transform(loc_x, loc_y, HH_RESIZE(fam[1], 15, 15), HH_RESIZE(fam[1], 14, 14));
		uint16_t tile_idx		= HH_RESIZE(fam[0], 9, 0);
		hh_ppu_addr_t ttm_addr	= tile_idx * HH_PPU_VRAM_TMM_SPRITE_SIZE + tile_pixel_idx / 5;
		uint8_t word_bit_addr	= (tile_pixel_idx % 5) * 3;
		hh_ppu_data_t tmm		= g_hh_ppusim_vram[HH_PPU_VRAM_TMM_OFFSET + ttm_addr];
		unsigned cidx_col		= HH_RESIZE(tmm, word_bit_addr + 2, word_bit_addr);
		if (cidx_col == 0) continue;
		unsigned cidx_pal = HH_RESIZE(fam[0], 12, 10);
		cidx			  = (cidx_col << 0) | (cidx_pal << 3);
		break;
	}
	return cidx;
}

void hh_ppusim_pixel(uint8_t *s, unsigned x, unsigned y) {
	uint8_t bg_cidx		   = hh_ppusim_bg_pixel(x, y);
	uint8_t fg_cidx		   = hh_ppusim_fg_pixel(x, y);
	uint8_t cidx		   = (fg_cidx & HH_MASK(3)) == 0 ? bg_cidx : fg_cidx;
	hh_ppu_data_t pal_rgb  = g_hh_ppusim_vram[HH_PPU_VRAM_PAL_OFFSET + cidx];
	hh_ppu_rgb_color_t rgb = {HH_RESIZE(pal_rgb, 11, 8), HH_RESIZE(pal_rgb, 7, 4), HH_RESIZE(pal_rgb, 3, 0)};

	s[0] = rgb[0] << 4;
	s[1] = rgb[1] << 4;
	s[2] = rgb[2] << 4;
}
