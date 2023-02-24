#include <stdlib.h>
#include <math.h>

#include "ppu/ppu.h"
#include "ppu/consts.h"

hh_s_ppu_loc_sprite* hh_debug_circle_sprite() {
	hh_s_ppu_loc_sprite* s = malloc(sizeof(hh_s_ppu_loc_sprite));

	for (int x = 0; x < HH_PPU_SPRITE_WIDTH; x++)
		for (int y = 0; y < HH_PPU_SPRITE_HEIGHT; y++)
			*s[y * HH_PPU_SPRITE_WIDTH + x] = (pow(x - 8, 2) + pow(y - 8, 2) < 67) ? 1 : 0;

	return s;
}

int main() {
	hh_ppu_init();

	// hh_ppu_update_aux((hh_s_ppu_loc_aux) {
	// 	.bg_shift_x = 0,
	// 	.bg_shift_y = 0,
	// 	.fg_fetch = 0,
	// 	.sysreset = 0,
	// });

	hh_s_ppu_loc_sprite* sprite = hh_debug_circle_sprite();
	// hh_ppu_update_sprite(0, *sprite);
	free(sprite);

	while (1);

	hh_ppu_deinit();
}

void hh_ppu_vblank_interrupt() {
	static unsigned long frame = 0;
	frame++;
}
