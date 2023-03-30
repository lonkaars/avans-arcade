#include "engine/draw_screen.h"
//#include "engine/sprite_controller.h"

hh_level_entity level;
uint64_t offset=0;
uint8_t hh_world_to_tile(vec2 pos){
	
	int index = (((pos.y/16)*hh_max_x_size) + (pos.x/16));//TODO: remove magic number(s)
	int tile= level.place[index];
	return tile;
}

void hh_update_screen(vec2 view, vec2 player){
	int currentTileY = view.y / 16;
	int offset_px = view.y-(offset / 40 * 16);


	// || (offset_px == 0 && lastUpdated != 0 && player.y/16 == currentTileY-5)
	if( (offset_px > 230) ){
		int size = (hh_max_x_size) * hh_max_y_size;
		offset = currentTileY * level.x;

		// Update the background screen
		for (int BAM_index = 0; BAM_index < size; BAM_index++) {
			hh_ppu_update_background(BAM_index, (hh_s_ppu_loc_bam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = hh_get_palette(level.place[offset+BAM_index]),
				.tilemap_index = level.place[offset+BAM_index],
			});
		}
	}
}
void hh_setup_screen(hh_level_entity currentlevel){
	hh_clear_screen();
	hh_clear_sprite();
	level = currentlevel;
	int size = hh_max_x_size* hh_max_y_size;
	printf("hier\n");
	for(int BAM_index = 0; BAM_index < size; BAM_index++){
		hh_ppu_update_background(BAM_index, (hh_s_ppu_loc_bam_entry){
			.horizontal_flip = false,
			.vertical_flip   = false,
			.palette_index   = hh_get_palette(currentlevel.place[BAM_index]),
			.tilemap_index   = currentlevel.place[BAM_index],
		});
	}
}
vec_cor hh_draw_screen(vec2 player) {
   static vec_cor previousViewport = {0, 0};
	int offset_px = offset/40*16;
	//printf("%d\n",offset_px%2);
	vec_cor viewport = hh_update_camera(player,(vec2){.x=0,.y=offset_px},(vec2){.x=20*16,.y=30*16+offset_px/4});//TODO: remove magic number(s)

	hh_update_screen((vec2){.y=viewport.y,.x=0},player);
   if (viewport.x == previousViewport.x && viewport.y == previousViewport.y){}
	else{
		// update previous viewport values

		//printf("viewport y %d\n",viewport.y-(offset/40*16));
		//printf("offset %d\n",offset);
		hh_ppu_update_aux((hh_s_ppu_loc_aux){
			.bg_shift_x = viewport.x - 0,
			.bg_shift_y = viewport.y-(offset/40*16),
			.fg_fetch	= 0,
			.sysreset	= 0,
		});
		previousViewport = viewport;

	}

	return viewport;
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
