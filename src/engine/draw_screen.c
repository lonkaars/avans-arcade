#include "engine/draw_screen.h"
#include "engine/sprite_controller.h"

static struct draw_screen
{
	void* levels[12]
};


uint8_t hh_world_to_tile(vec2 pos){
	//TODO: remove magic file name here
    FILE* level = fopen("static/level1_test.bin", "rb"); /* open binary file */
    if (!level) { /* check if file opened successfully */
        fprintf(stderr, "Error: Failed to open file.\n");
        return 0;
    }
	int index = ((pos.y/16)*40 + pos.x/16);//TODO: remove magic number(s)
	fseek(level, (index * sizeof(int)) + sizeof(int), SEEK_SET);
	int tile;// = (int*)malloc(sizeof(int));
   fread(&tile, sizeof(int), 1, level); // read 1 tile from binary

	fclose(level);
	// int val = tile;
	// free(tile);
	return tile;
}


// remeber old value to know which part to update.
vec2 previousViewport = { .x = 0, .y = 0 };
void hh_draw_screen(vec_cor viewport){
	if (viewport.x == previousViewport.x && viewport.y == previousViewport.y) return;
	
	hh_ppu_update_aux((hh_s_ppu_loc_aux){
		.bg_shift_x = viewport.x,
		.bg_shift_y = viewport.y,
		.fg_fetch	= 0,
		.sysreset	= 0,
	});

	// update previous viewport values
	previousViewport = viewport;
}

void hh_setup_screen(){
	//(HH_map_size_X*HH_map_size_Y)
	int size = 2400; // max X = 40 en max Y = 80
	//TODO: remove magic file name here
	FILE* level = fopen("static/level1_test.bin", "rb"); /* open binary file */
	if (!level) { /* check if file opened successfully */
		fprintf(stderr, "Error: Failed to open level file.\n");
		return;
	}
	fseek(level, (0* sizeof(int)) + sizeof(int), SEEK_SET);
	int* tile = (int*)malloc(size*sizeof(int));
   fread(tile, sizeof(int), size, level); // read 1 tile from binary

	fclose(level);

	for(int BAM_index = 0; BAM_index < size; BAM_index++){
		hh_ppu_update_background(BAM_index, (hh_s_ppu_loc_bam_entry){
			.horizontal_flip = false,
			.vertical_flip   = false,
			.palette_index   = hh_get_palette(tile[BAM_index]),
			.tilemap_index   = tile[BAM_index],
		});
	}
	free(tile);
}
void hh_clear_screen(){
	// (HH_PPU_SCREEN_HEIGHT*HH_PPU_SCREEN_WIDTH)/(HH_PPU_SPRITE_HEIGHT*HH_PPU_SPRITE_WIDTH)
	for (int i = 0; i < HH_PPU_BG_CANVAS_TILES_H*HH_PPU_BG_CANVAS_TILES_V; i++) {
		hh_s_ppu_loc_bam_entry temp = {
			.vertical_flip=false,.horizontal_flip = false,
			.palette_index = 3,.tilemap_index = 0
		};
		hh_ppu_update_background(i,temp);
		hh_ppu_update_color(3,0,(hh_ppu_rgb_color_t){0x0,0x0,0x0});
	}
	hh_ppu_update_aux((hh_s_ppu_loc_aux){
		.bg_shift_x = 0,
		.bg_shift_y = 0,
		.fg_fetch	= 0,
		.sysreset	= 0,
	});
}

void hh_clear_sprite(){
	for (int i = 0; i < HH_PPU_FG_SPRITE_COUNT; i++) {
		hh_ppu_update_foreground(i,(hh_s_ppu_loc_fam_entry){
			.position_x = -16,
			.position_y = -16,
		});
	}
}
