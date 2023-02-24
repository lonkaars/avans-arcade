#include <stdlib.h>
#include <math.h>
#ifdef HH_TARGET_DESKTOP
#include <stdio.h>
#endif

#include "main.h"
#include "ppu/ppu.h"
#include "ppu/consts.h"

bool g_hh_run = true;

int main() {
	hh_setup();
	hh_loop();
	hh_exit();
	return 0;
}

void hh_ppu_vblank_interrupt() {
	static unsigned long frame = 0;
	frame++;

#ifdef HH_TARGET_DESKTOP
	printf("frame %lu\n", frame);
#endif
}

void hh_setup() {
	hh_ppu_init();

	hh_ppu_update_aux((hh_s_ppu_loc_aux) {
		.bg_shift_x = 24,
		.bg_shift_y = 0,
		.fg_fetch = 1,
		.sysreset = 1,
	});

	hh_s_ppu_loc_sprite sprite = {0};

	for (int x = 0; x < HH_PPU_SPRITE_WIDTH; x++)
		for (int y = 0; y < HH_PPU_SPRITE_HEIGHT; y++)
			sprite[y * HH_PPU_SPRITE_WIDTH + x] = (pow(x - 8, 2) + pow(y - 8, 2) < 67) ? 1 : 0;

	hh_ppu_update_sprite(0, sprite);

	hh_ppu_update_foreground(0, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = 1,
		.tilemap_index = 0,
		.position_x = 30,
		.position_y = 40
	});

	hh_ppu_update_color(1, 1, (hh_ppu_rgb_color_t) {15, 0, 15});
}

void hh_exit() {
	g_hh_run = false;

	hh_ppu_deinit();
}
