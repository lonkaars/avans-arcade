#include "engine/animator.h"
#include "engine/entity.h"
#include "engine/maths.h"
#include "ppu/consts.h"
#include "ppu/ppu.h"


#define hh_white_palette 6
#define hh_animate_cycle 4

void hh_animate_hit(hh_s_rendering* in, bool hit) {
	if (hit) {
		in->fam.palette_index = hh_white_palette;
		in->cooldown = hh_animate_cycle;
	} else if(in->cooldown == 0) {
		in->fam.palette_index = in->palette;
	} else {
		if (in->cooldown > 0) {
			in->cooldown--;
		}
	}
}

void hh_animate(hh_s_rendering* in, uint16_t start, uint16_t end, uint8_t step) {
	if (in->fam.tilemap_index < start) {//check for sudden animation change
		in->fam.tilemap_index = start;
		in->cooldown = hh_animate_cycle;
	} else {
		if (in->cooldown-- == 0) {

			if (in->fam.tilemap_index < end) {
				in->fam.tilemap_index += step;
			} else {// rollover
				in->fam.tilemap_index = start;
			}
			in->cooldown = hh_animate_cycle;
		}

	}
	
}

//TODO: if entity not inside of screen, don't update idx (problems with old idx not being overwritten anymore)
void hh_update_sprite(uint16_t *idx, hh_entity* in, vec_cor cam) {
	hh_animate_hit(&in->render, in->is_hit);
	hh_s_ppu_loc_fam_entry temp = in->render.fam;
	for (int y = 0; y < CEILI(in->size.y,16); y++) {
		temp.position_y = CLAMP(in->pos.y - cam.y + y*HH_PPU_SPRITE_HEIGHT, -16, HH_PPU_SCREEN_HEIGHT);
		for (int x = 0; x < CEILI(in->size.x,16); x++) {
			temp.position_x = CLAMP(in->pos.x - cam.x + x*HH_PPU_SPRITE_WIDTH, -16, HH_PPU_SCREEN_WIDTH);
			// if (temp.position_x != -16 && temp.position_y != -16) {// removes redundant sprites
				hh_ppu_update_foreground(++*idx, temp);
				temp.tilemap_index++;
				// hh_animate(&in->render, start+counter,end+counter,4);
			// }
		}
	}
}

void hh_show_quad(uint16_t *idx, hh_s_rendering* in) {
	hh_s_ppu_loc_fam_entry temp = in->fam;
	temp.position_y = CLAMP(in->fam.position_y, -16, HH_PPU_SCREEN_HEIGHT);
	temp.position_x = CLAMP(in->fam.position_x, -16, HH_PPU_SCREEN_WIDTH);
	hh_ppu_update_foreground(++*idx, temp);
}
