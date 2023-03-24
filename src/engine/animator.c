#include "engine/animator.h"
#include "engine/entity.h"
#include "engine/maths.h"


#define hh_white_palette 6

void hh_animate_hit(hh_s_rendering* in, bool hit) {
	if (hit) {
		in->fam.palette_index = hh_white_palette;
	} else {
		in->fam.palette_index = in->palette;
	}
}

void hh_animate(hh_s_rendering* in, hh_idx_t start, hh_idx_t end, uint8_t step) {
	if (in->fam.palette_index >= start && in->fam.palette_index < end) {
		in->fam.palette_index += step;
	} else {// rollover
		in->fam.palette_index = start;
	}
}

void hh_update_sprite(hh_entity* in) {
	hh_animate_hit(&in->render, in->is_hit);	
	
}
