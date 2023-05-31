#include "ppu/ppu.h"
#include "ppu/types.h"
#include "ppu/consts.h"

#include "static/tilemap.h"


#include "engine/draw_screen.h"
#include "engine/entity.h"

void hh_init_title_screen(){

	// hh_clear_screen();

	//send data
	uint8_t idx = 0;
	const uint8_t tilemap_offset = HH_TM_TITLE_SCREEN_ICON_OFFSET;
	int tiles_h = HH_PPU_BG_CANVAS_TILES_H;
	int vp_h = HH_PPU_SCREEN_WIDTH/HH_PPU_SPRITE_WIDTH; //screen_h in tiles
	int vert_offset = tiles_h*3;

	const uint8_t arr[4][4] = {
		{0,1,1,0},
		{2,3,3,2},
		{4,0,0,4},
		{5,6,6,5},
	};
	int val, counter =0;
	hh_ppu_update_color(5, 0, (hh_ppu_rgb_color_t) {0x1, 0x1, 0x1});
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			val = arr[i][j];

			hh_s_ppu_loc_bam_entry temp = {
			.vertical_flip=false, .horizontal_flip = ((j % 4 < 2) ? false : true),
			.palette_index = (counter == 9 || counter == 10? 5:3), .tilemap_index = (val > 0 ? (tilemap_offset + val) : 0)
			};

			int vert_pos = tiles_h*i;
			int x_pos = j;
			idx = vert_offset + vert_pos + x_pos + vp_h/2-2;
			
			hh_ppu_update_background(idx,temp);
			counter++;
		}

	}
	

	const uint8_t letters_offset = HH_TM_TITLE_SCREEN_LETTERES_LARGE_OFFSET;
	const int _size_hooded = 7, _size_v = 2;

	// char* hh = "hooded";
	int hooded_lookup[7][2]={
		{0,1},{0,2},//H
		{3,4},{3,4},//oo
		{5,6},{13,9},//de
		{5,6}//d
	};

	counter = 8;
	for (int i = 0; i < _size_hooded; i++) {
		for (int vert = 1; vert <= _size_v; vert++) {
			//TODO: move 'H' a few pixels to the right for a more cohesive font spacing
			hh_ppu_update_foreground(counter++, (hh_s_ppu_loc_fam_entry) {
				.vertical_flip = false, .horizontal_flip = false,
				.palette_index = 6, .tilemap_index = letters_offset + hooded_lookup[i][vert-1],
				.position_x = (16*i + 64+48), .position_y = (16*(vert > 1 ? 0:1)*-1 + 64+8+16 +(i==2 || i==3 ? 6:0))
			});	
		}
	}


	hh_ppu_update_color(5, 1, (hh_ppu_rgb_color_t) {0xa, 0x3, 0x3});
	hh_ppu_update_color(5, 2, (hh_ppu_rgb_color_t) {0xc, 0x5, 0x3});

	const int _size_havoc = 6;
	int lookup_havoc[6][2]={
		{0,1},{0,2},//H
		{13,10},{7,8},//av
		{13,11},{13,12}//oc
	};

	counter = 8 + (_size_hooded * _size_v);
	for (int i = 0; i < _size_havoc; i++) {
		for (int vert = 1; vert <= _size_v; vert++) {
			//TODO: move 'H' a few pixels to the right for a more cohesive font spacing
			hh_ppu_update_foreground(counter++, (hh_s_ppu_loc_fam_entry) {
				.vertical_flip = false, .horizontal_flip = (i > 4 && vert==0 ? 1:0),
				.palette_index = 5, .tilemap_index = letters_offset + lookup_havoc[i][vert-1],
				.position_x = (16*i +64+32+8), .position_y = (16*(vert > 1 ? 0:1)*-1 + 64+8+48)
			});	
		}
	}
}
