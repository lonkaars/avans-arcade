#include <SDL2/SDL.h>
#include <stdbool.h>

#include "ppu/internals.h"
#include "ppusim/mem.h"
#include "ppusim/sim.h"
#include "ppu/consts.h"

static void hh_ppusim_draw(SDL_Renderer* r, unsigned px, unsigned py, unsigned cr, unsigned cg, unsigned cb) {
	SDL_SetRenderDrawColor(r, cr, cg, cb, 255);
	SDL_RenderFillRect(r, &(SDL_Rect) {
		.x = px * HH_PPUSIM_UPSCALE_FACTOR,
		.y = py * HH_PPUSIM_UPSCALE_FACTOR,
		.w = HH_PPUSIM_UPSCALE_FACTOR,
		.h = HH_PPUSIM_UPSCALE_FACTOR
	});
}

/* transform xy if tile is flipped */
static uint16_t hh_ppusim_apply_transform(unsigned x, unsigned y, bool fliph, bool flipv) {
	unsigned tx = fliph ? HH_PPU_SPRITE_WIDTH - x - 1 : x;
	unsigned ty = flipv ? HH_PPU_SPRITE_HEIGHT - y - 1 : y;
	return ty * HH_PPU_SPRITE_WIDTH + tx;
}

/* @brief get current bg pixel cidx */
static uint8_t hh_ppusim_bg_pixel(unsigned x, unsigned y) {
	hh_ppu_data_t* aux = &g_hh_ppusim_vram[HH_PPU_VRAM_AUX_OFFSET];
	unsigned bg_shift_y = HH_RESIZE(aux[0], 7, 0);
	unsigned bg_shift_x = HH_RESIZE(aux[0], 15, 8) | HH_RESIZE(aux[1], 0, 0) << 8;
	unsigned abs_x = bg_shift_x + x;
	unsigned abs_y = bg_shift_y + y;
	unsigned grid_x = abs_x / HH_PPU_SPRITE_WIDTH;
	unsigned grid_y = abs_y / HH_PPU_SPRITE_HEIGHT;
	unsigned loc_x = abs_x - grid_x * HH_PPU_SPRITE_WIDTH;
	unsigned loc_y = abs_y - grid_y * HH_PPU_SPRITE_HEIGHT;
	unsigned bam_offset = grid_y * HH_PPU_BG_CANVAS_TILES_H + grid_x;
	hh_ppu_data_t bam = g_hh_ppusim_vram[HH_PPU_VRAM_BAM_OFFSET + bam_offset];
	uint8_t cidx = 0;
	uint16_t tile_pixel_idx = hh_ppusim_apply_transform(loc_x, loc_y, HH_RESIZE(bam, 14, 14), HH_RESIZE(bam, 13, 13));
	uint16_t tile_idx = HH_RESIZE(bam, 9, 0);
	hh_ppu_addr_t ttm_addr = tile_idx + tile_pixel_idx / 5;
	uint8_t word_bit_addr = (tile_pixel_idx % 5) * 3;
	hh_ppu_data_t tmm = g_hh_ppusim_vram[HH_PPU_VRAM_TMM_OFFSET + ttm_addr];
	cidx |= HH_RESIZE(bam, 12, 10) << 3;
	cidx |= HH_RESIZE(tmm, word_bit_addr+2, word_bit_addr) << 0;
	return cidx;
}

void hh_ppusim_pixel(SDL_Renderer* r, unsigned x, unsigned y) {
	// loop through foreground sprites to get highest priority cidx
	uint8_t bg_cidx = hh_ppusim_bg_pixel(x, y);
	// check if palette color cidx of foreground is 0
	// lookup color from PAL
	// hh_ppusim_draw
	uint8_t cidx = bg_cidx;
	hh_ppu_data_t pal_rgb = g_hh_ppusim_vram[HH_PPU_VRAM_PAL_OFFSET + cidx];
	hh_ppu_rgb_color_t rgb = {
		HH_RESIZE(pal_rgb, 11, 8),
		HH_RESIZE(pal_rgb, 7, 4),
		HH_RESIZE(pal_rgb, 3, 0)
	};
	hh_ppusim_draw(r, x, y, rgb[0] << 4, rgb[1] << 4, rgb[2] << 4);
}

