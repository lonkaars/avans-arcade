#include "engine/animator.h"
#include "engine/entity.h"
#include "engine/maths.h"
#include "ppu/consts.h"
#include "ppu/ppu.h"


#define hh_white_palette 6

void hh_animate_hit(hh_s_rendering* in, bool hit) {
	if (hit) {
		in->fam.palette_index = hh_white_palette;
	} else {
		in->fam.palette_index = in->palette;
	}
}

void hh_animate(hh_s_rendering* in, uint16_t start, uint16_t end, uint8_t step) {
	if (in->fam.palette_index >= start && in->fam.palette_index < end) {
		in->fam.palette_index += step;
	} else {// rollover
		in->fam.palette_index = start;
	}
}

uint16_t hh_update_sprite(uint16_t idx, hh_entity* in, vec_cor cam) {
	hh_animate_hit(&in->render, in->is_hit);
	hh_s_ppu_loc_fam_entry temp = in->render.fam;
	for (int y = 0; y < CEILI(in->size.y,16); y++) {
		temp.position_y = CLAMP(in->pos.y - cam.y + y*HH_PPU_SPRITE_HEIGHT, -16, HH_PPU_SCREEN_HEIGHT);
		for (int x = 0; x < CEILI(in->size.x,16); x++) {
			temp.position_x = CLAMP(in->pos.x - cam.x + x*HH_PPU_SPRITE_WIDTH, -16, HH_PPU_SCREEN_WIDTH);
			hh_ppu_update_foreground(++idx, temp);	
			temp.tilemap_index++;
		}
	}
	return idx;
}


// void hh_update_sprite(hh_entity* in, vec_cor cam) {
// 	hh_animate_hit(&in->render, in->is_hit);
// 	// uint16_t idx = in->render.ppu_foreground_index;
// 	uint16_t idx = 0;
// 	hh_s_ppu_loc_fam_entry temp = in->render.fam;
// 	for (int y = 0; y < CEILI(in->size.y,16); y++) {
// 		temp.position_y = CLAMP(in->pos.y - cam.y + y*HH_PPU_SPRITE_HEIGHT, -16, HH_PPU_SCREEN_HEIGHT);
// 		for (int x = 0; x < CEILI(in->size.x,16); x++) {
// 			temp.position_x = CLAMP(in->pos.x - cam.x + x*HH_PPU_SPRITE_WIDTH, -16, HH_PPU_SCREEN_WIDTH);
// 			hh_ppu_update_foreground(idx++, temp);	
// 			temp.tilemap_index++;
// 		}
// 	}
// }
