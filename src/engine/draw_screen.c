#include "engine/draw_screen.h"

hh_level_entity level;
uint64_t offsetY=0;
uint64_t offsetX=0;
uint8_t hh_world_to_tile(vec2 pos){
	
	int index = (((pos.y/HH_PPU_SPRITE_HEIGHT)*level.size.x) + (pos.x/HH_PPU_SPRITE_WIDTH));//TODO: remove magic number(s)
	int tile= level.place[index];
	return tile;
}

void hh_update_screen(vec2 view){
	int current_tile_y = view.y / HH_PPU_SPRITE_HEIGHT;
	int current_tile_x = view.x / HH_PPU_SPRITE_WIDTH;
	int offset_py = view.y - (offsetY / 40 * HH_PPU_SPRITE_HEIGHT);
	int offset_px = view.x - offsetX * HH_PPU_SPRITE_WIDTH;
	static int prev_offset = 0;

	int size = MIN(HH_PPU_BG_CANVAS_TILES_H,level.size.x) * MIN(HH_PPU_BG_CANVAS_TILES_V,level.size.y);
	if( (offset_py == 0 || offset_py > 230) && level.size.y > level.size.x && prev_offset != offset_py){
		if(offset_py==0){
			offsetY = MAX(current_tile_y-14,0) * level.size.x;
		}
		else{
			offsetY = current_tile_y * level.size.x;
		}

		// Update the background screen
		for (int BAM_index = 0; BAM_index < size; BAM_index++) {
			hh_ppu_update_background(BAM_index, (hh_s_ppu_loc_bam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = hh_get_palette(level.place[offsetY + BAM_index]),
				.tilemap_index = level.place[offsetY + BAM_index],
			});
		}
		prev_offset = offset_py;
	}
	else if ((offset_px == 0 || offset_px > 310) && level.size.x > level.size.y && prev_offset != offset_px)
	{
		if(offset_px==0){
			offsetX = MAX((current_tile_x - 14),0);
		}
		else{
			offsetX = current_tile_x;
		}
		// Update the background screen
		for (int BAM_index = 0; BAM_index < size; BAM_index++) {
			int var = (BAM_index / HH_PPU_BG_CANVAS_TILES_H) * level.size.x + MIN((BAM_index % HH_PPU_BG_CANVAS_TILES_H + offsetX),level.size.x);
			hh_ppu_update_background(BAM_index, (hh_s_ppu_loc_bam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = hh_get_palette(level.place[var]),
				.tilemap_index = level.place[var],
			});
		}
	}

	prev_offset = offset_px;
	
}
void hh_setup_screen(hh_level_entity currentlevel){
	hh_clear_screen();
	hh_clear_sprite();
	level = currentlevel;
	offsetY=0;
	offsetX=0;
	int size = MIN(HH_PPU_BG_CANVAS_TILES_H, level.size.x) * MIN(HH_PPU_BG_CANVAS_TILES_V, level.size.y);
	for (int BAM_index = 0; BAM_index < size; BAM_index++) {
		int var = (BAM_index / HH_PPU_BG_CANVAS_TILES_H) * level.size.x + BAM_index % HH_PPU_BG_CANVAS_TILES_H;
		hh_ppu_update_background(BAM_index, (hh_s_ppu_loc_bam_entry){
			.horizontal_flip = false,
			.vertical_flip   = false,
			.palette_index   = hh_get_palette(currentlevel.place[var]),
			.tilemap_index   = currentlevel.place[var],
		});
	}
}
vec_cor hh_draw_screen(vec2 player) {
   static vec_cor previousViewport = {0, 0};
	int offset_py = offsetY / 40 * HH_PPU_SPRITE_HEIGHT;
	
	int offset_px = (offsetX * HH_PPU_SPRITE_WIDTH) ;
	vec_cor viewport = hh_update_camera(player,(vec2){.x = offset_px, .y = offset_py},(vec2){.x = HH_PPU_SCREEN_WIDTH + offset_px, .y = 480 + offset_py});//TODO: remove magic number(s)
	viewport.x = CLAMP(viewport.x, 0, level.size.x * HH_PPU_SPRITE_WIDTH - HH_PPU_SCREEN_WIDTH);
	viewport.y = CLAMP(viewport.y, 0, level.size.y * HH_PPU_SPRITE_HEIGHT - HH_PPU_SCREEN_HEIGHT);
	
	hh_update_screen((vec2){.y=viewport.y,.x=viewport.x});
	hh_ppu_update_aux((hh_s_ppu_loc_aux){
		.bg_shift_x = viewport.x - offset_px,
		.bg_shift_y = viewport.y - offset_py,
		.fg_fetch	= 0,
		.sysreset	= 0,
	});
	previousViewport = viewport;

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
			.position_x = -HH_PPU_SPRITE_WIDTH,
			.position_y = -HH_PPU_SPRITE_HEIGHT,
		});
	}
}
