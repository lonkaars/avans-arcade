#include <math.h>

#include "demo.h"
#include "ppu/ppu.h"
#include "input.h"
#include "ppu/internals.h"

#define HH_DEMO_BALL_COUNT 5
hh_s_ppu_loc_fam_entry g_hh_demo_balls[HH_DEMO_BALL_COUNT];

void hh_demo_setup() {
	// load sprites
	hh_ppu_update_sprite(0, HH_DBG_SPRITE_BALL);
	hh_ppu_update_sprite(1, HH_DBG_SPRITE_CHECKERBOARD);

	// background pattern
	hh_ppu_update_color(0, 0, (hh_ppu_rgb_color_t) {0x1, 0x1, 0x3});
	hh_ppu_update_color(0, 1, (hh_ppu_rgb_color_t) {0x4, 0x4, 0x6});
	for (unsigned i = 0; i < HH_PPU_BG_CANVAS_TILES_H * HH_PPU_BG_CANVAS_TILES_V; i++) {
		hh_ppu_update_background(i, (hh_s_ppu_loc_bam_entry) {
			.horizontal_flip = false,
			.vertical_flip = false,
			.palette_index = 0,
			.tilemap_index = 1,
		});
	}

	// cool colors
	hh_ppu_update_color(1, 1, (hh_ppu_rgb_color_t) {0xf, 0x0, 0xf});
	hh_ppu_update_color(2, 1, (hh_ppu_rgb_color_t) {0xf, 0xf, 0xf});
	hh_ppu_update_color(3, 1, (hh_ppu_rgb_color_t) {0xf, 0x0, 0x0});
	hh_ppu_update_color(4, 1, (hh_ppu_rgb_color_t) {0x0, 0xf, 0xf});
	hh_ppu_update_color(5, 1, (hh_ppu_rgb_color_t) {0x0, 0x0, 0xf});

	// balls
	for (unsigned i = 0; i < HH_DEMO_BALL_COUNT; i++) {
		g_hh_demo_balls[i].horizontal_flip = false;
		g_hh_demo_balls[i].vertical_flip = false;
		g_hh_demo_balls[i].palette_index = i+1;
		g_hh_demo_balls[i].tilemap_index = 0;
	}
}

void hh_demo_loop(unsigned long frame) {
	// set background pattern position
	hh_ppu_update_background_pos((frame / 5) % HH_PPU_SPRITE_WIDTH, (frame / 20) % HH_PPU_SPRITE_HEIGHT);

	for (unsigned i = 0; i < HH_DEMO_BALL_COUNT; i++) {
		g_hh_demo_balls[i].position_x = HH_PPU_SCREEN_WIDTH/2  - HH_PPU_SPRITE_WIDTH/2  + (int)(60 * (double)sin((1*(double)frame / 10) + (double)i * 12));
		g_hh_demo_balls[i].position_y = HH_PPU_SCREEN_HEIGHT/2 - HH_PPU_SPRITE_HEIGHT/2 + (int)(30 * (double)sin((2*(double)frame / 10) + (double)i * 12));
		hh_ppu_update_foreground(i+16, g_hh_demo_balls[i]);
	}

	return;
	/* 
	hh_ppu_update_foreground(32, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = g_hh_controller_p1.dpad_up * 2,
		.tilemap_index = 0,
		.position_x = 0,
		.position_y = 0,
	});
	hh_ppu_update_foreground(33, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = g_hh_controller_p1.dpad_down * 2,
		.tilemap_index = 0,
		.position_x = 16,
		.position_y = 0,
	});
	hh_ppu_update_foreground(34, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = g_hh_controller_p1.dpad_left * 2,
		.tilemap_index = 0,
		.position_x = 32,
		.position_y = 0,
	});
	hh_ppu_update_foreground(35, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = g_hh_controller_p1.dpad_right * 2,
		.tilemap_index = 0,
		.position_x = 48,
		.position_y = 0,
	});
	hh_ppu_update_foreground(36, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = g_hh_controller_p1.button_primary * 2,
		.tilemap_index = 0,
		.position_x = 64,
		.position_y = 0,
	});
	hh_ppu_update_foreground(37, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = g_hh_controller_p1.button_secondary * 2,
		.tilemap_index = 0,
		.position_x = 80,
		.position_y = 0,
	});
	hh_ppu_update_foreground(40, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = g_hh_controller_p2.dpad_up * 2,
		.tilemap_index = 0,
		.position_x = 0,
		.position_y = 16,
	});
	hh_ppu_update_foreground(41, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = g_hh_controller_p2.dpad_down * 2,
		.tilemap_index = 0,
		.position_x = 16,
		.position_y = 16,
	});
	hh_ppu_update_foreground(42, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = g_hh_controller_p2.dpad_left * 2,
		.tilemap_index = 0,
		.position_x = 32,
		.position_y = 16,
	});
	hh_ppu_update_foreground(43, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = g_hh_controller_p2.dpad_right * 2,
		.tilemap_index = 0,
		.position_x = 48,
		.position_y = 16,
	});
	hh_ppu_update_foreground(44, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = g_hh_controller_p2.button_primary * 2,
		.tilemap_index = 0,
		.position_x = 64,
		.position_y = 16,
	});
	hh_ppu_update_foreground(45, (hh_s_ppu_loc_fam_entry) {
		.horizontal_flip = false,
		.vertical_flip = false,
		.palette_index = g_hh_controller_p2.button_secondary * 2,
		.tilemap_index = 0,
		.position_x = 80,
		.position_y = 16,
	});
	*/
}
