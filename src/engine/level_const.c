#include "engine/level_const.h"
#include "metadata_lvl1.h"

#include "static/level/shop.level.h"
#include "static/level/lvl1.level.h"

hh_g_all_levels hh_init_game_levels(){
	hh_g_all_levels levels;
	levels.current_level=0;

	// level 1
	levels.level[0].size.x=100;
	levels.level[0].size.y=28;
	levels.level[0].hh_enemies = hh_mtd_lvl1_enemy;
	levels.level[0].hh_lvl_tot_enemies = lvl1_enemy_size;// enemy_size;
	levels.level[0].player_start_pos = hh_lvl1_player_start_pos;
	for(int i=0; i < lvl1_enemy_size; i++){
		levels.level[0].hh_enemies[i].start = lvl1_start_pos_enemy[i];
	}
		
	levels.level[0].Boss = (hh_entity){
		.object_type = charging_boss,
		.hp = 10,
		.speed = 2,
		.is_grounded = false, 
		.is_hit = false,
		.radius = 16,
		.size = (vec2){48,32},
		.vel = (vec2){0,0},
		// .vec = (vec2){0,0},
		.render = {
			.frame0 = HH_TM_BOSS1_OFFSET,
			.palette = 6,
			.fam = (hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = 6,
				.tilemap_index = HH_TM_BOSS1_OFFSET,
			}
		}
	};
	levels.level[0].Boss.pos = lvl1_boss_spawn_point;
	levels.level[0].Boss.start = lvl1_boss_spawn_point;
	int* hh_game_level1 = (int*) level_lvl1_bin;
	levels.level[0].place = hh_game_level1;
 
	/*
	levels.shop.size.x=40;
	levels.shop.size.y=15;
	int* hh_game_shop = (int*) level_shop_bin;
	levels.shop.place = hh_game_shop;
	*/

	return levels;
}

