#include <stdint.h>

#include "engine/sprite_controller.h"
#include "ppu/types.h"
#include "ppu/consts.h"
#include "ppu/ppu.h"

uint8_t hh_get_palette(uint8_t tile_idx) {
	return hh_g_sprite_palette[tile_idx];
}

void hh_setup_palettes(){
	for (int idx = 0; idx < HH_PPU_PALETTE_COUNT; idx++) {
		for (int col = 0; col < HH_PPU_PALETTE_COLOR_COUNT; col++) {
			hh_ppu_update_color(idx,col,hh_g_palette[idx][col]);
		}
	}	
}

bool hh_colidable(uint8_t tile_idx){
	return (hh_get_palette(tile_idx) != 0);
}
