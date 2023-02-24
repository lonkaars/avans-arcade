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
	// printf("frame %lu\n", frame);
#endif

	unsigned shift = (double) sin((double) frame / 20) * 10 + 10;
	hh_ppu_update_aux((hh_s_ppu_loc_aux) {
		.bg_shift_x = shift,
		.bg_shift_y = 0,
		.fg_fetch = 0,
		.sysreset = 0,
	});
}

void hh_setup() {
	hh_ppu_init();

	hh_s_ppu_loc_sprite sprite = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 
		0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 
		0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0
	};

	for (int x = 0; x < HH_PPU_SPRITE_WIDTH; x++) {
		for (int y = 0; y < HH_PPU_SPRITE_HEIGHT; y++) {
			// sprite[y * HH_PPU_SPRITE_WIDTH + x] = (pow(x - 8, 2) + pow(y - 8, 2) < 64) ? 1 : 0;
			uint8_t p = sprite[y * HH_PPU_SPRITE_WIDTH + x];
			printf("%c%c%c", p ? 'X' : ' ', p ? 'X' : ' ', p ? 'X' : ' ');
		}
		printf("\n");
	}

	hh_ppu_update_sprite(0, sprite);

	// hh_ppu_update_foreground(0, (hh_s_ppu_loc_fam_entry) {
	// 	.horizontal_flip = false,
	// 	.vertical_flip = false,
	// 	.palette_index = 1,
	// 	.tilemap_index = 0,
	// 	.position_x = 30,
	// 	.position_y = 40,
	// });

	hh_ppu_update_background(3, (hh_s_ppu_loc_bam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = 1,
		.tilemap_index = 0,
	});
	hh_ppu_update_background(4, (hh_s_ppu_loc_bam_entry) {
		.horizontal_flip = true,
		.vertical_flip = false,
		.palette_index = 2,
		.tilemap_index = 0,
	});
	hh_ppu_update_background(5, (hh_s_ppu_loc_bam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = 3,
		.tilemap_index = 0,
	});
	hh_ppu_update_background(6, (hh_s_ppu_loc_bam_entry) {
		.horizontal_flip = false,
		.vertical_flip = true,
		.palette_index = 4,
		.tilemap_index = 0,
	});
	hh_ppu_update_background(7, (hh_s_ppu_loc_bam_entry) {
		.horizontal_flip = true,
		.vertical_flip = true,
		.palette_index = 5,
		.tilemap_index = 0,
	});

	hh_ppu_update_color(1, 1, (hh_ppu_rgb_color_t) {0xf, 0x0, 0xf});
	hh_ppu_update_color(2, 1, (hh_ppu_rgb_color_t) {0xf, 0xf, 0xf});
	hh_ppu_update_color(3, 1, (hh_ppu_rgb_color_t) {0xf, 0x0, 0x0});
	hh_ppu_update_color(4, 1, (hh_ppu_rgb_color_t) {0x0, 0xf, 0xf});
	hh_ppu_update_color(5, 1, (hh_ppu_rgb_color_t) {0x0, 0x0, 0xf});
}

void hh_exit() {
	g_hh_run = false;

	hh_ppu_deinit();
}
