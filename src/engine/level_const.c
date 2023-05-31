#include "engine/level_const.h"
#include "metadata_lvl1.h"
#include "metadata_lvl2.h"
#include "metadata_lvl3.h"
#include "metadata_lvl4.h"

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
	FILE *fp = fopen("../test/bin/lvl1.bin", "rb");
	if (fp == NULL) {
		printf("level 1.bin not found!\n");		return levels;
	} 
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp) / sizeof(int);
	fseek(fp, (0 * sizeof(int)) + sizeof(int), SEEK_SET);
	int* hh_game_level1 = malloc(size * sizeof(int));
	fread(hh_game_level1, sizeof(int), size, fp);
	fclose(fp);
	levels.level[0].place = hh_game_level1;
 
   // level 2
	levels.level[1].size.x=40;
	levels.level[1].size.y=100;
	levels.level[1].hh_enemies = hh_mtd_lvl2_enemy;
	levels.level[1].hh_lvl_tot_enemies = lvl2_enemy_size;// enemy_size;
	levels.level[1].player_start_pos = hh_lvl2_player_start_pos;
	levels.level[1].start_enemy_loc = lvl2_start_pos_enemy;
		for(int i=0; i < lvl2_enemy_size; i++){
		levels.level[1].hh_enemies[i].start = lvl2_start_pos_enemy[i];
	}	
	levels.level[1].Boss = (hh_entity){
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
	levels.level[1].Boss.pos = lvl2_boss_spawn_point;
	levels.level[1].Boss.start = lvl2_boss_spawn_point;
	FILE *lvl2 = fopen("../test/bin/lvl2.bin", "rb");
	if (lvl2 == NULL) {
		printf("lvl2.bin not found!\n");
		return levels;
	}
	fseek(lvl2, 0, SEEK_END);
	size = ftell(lvl2) / sizeof(int);
	fseek(lvl2, (0 * sizeof(int)) + sizeof(int), SEEK_SET);
	int* hh_game_level2 = malloc(size * sizeof(int));
	fread(hh_game_level2, sizeof(int), size, lvl2);
	fclose(lvl2);

	levels.level[1].place = hh_game_level2;

   // level 3
	levels.level[2].size.x=150;
	levels.level[2].size.y=28;
	levels.level[2].hh_enemies = hh_mtd_lvl3_enemy;
	levels.level[2].hh_lvl_tot_enemies = lvl3_enemy_size;// enemy_size;
	levels.level[2].player_start_pos = hh_lvl3_player_start_pos;
	levels.level[2].start_enemy_loc = lvl3_start_pos_enemy;
		for(int i=0; i < lvl3_enemy_size; i++){
		levels.level[2].hh_enemies[i].start = lvl3_start_pos_enemy[i];
	}	
	levels.level[2].Boss = (hh_entity){
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

	levels.level[2].Boss.pos = lvl3_boss_spawn_point;
	levels.level[2].Boss.start = lvl3_boss_spawn_point;
	FILE *lvl3 = fopen("../test/bin/lvl3.bin", "rb");
	if (lvl3 == NULL) {
		printf("lvl3.bin not found!\n");
		return levels;
	}
	fseek(lvl3, 0, SEEK_END);
	size = ftell(lvl3) / sizeof(int);
	fseek(lvl3, (0 * sizeof(int)) + sizeof(int), SEEK_SET);
	int* hh_game_level3 = malloc(size * sizeof(int));
	fread(hh_game_level3, sizeof(int), size, lvl3);
	fclose(lvl3);

	levels.level[2].place = hh_game_level3;

   // level 4
	levels.level[3].size.x=40;
	levels.level[3].size.y=150;
	levels.level[3].hh_enemies = hh_mtd_lvl4_enemy;
	levels.level[3].hh_lvl_tot_enemies = lvl4_enemy_size;// enemy_size;
	levels.level[3].player_start_pos = hh_lvl4_player_start_pos;
	levels.level[3].start_enemy_loc = lvl4_start_pos_enemy;
		for(int i=0; i < lvl4_enemy_size; i++){
		levels.level[3].hh_enemies[i].start = lvl4_start_pos_enemy[i];
	}	
	levels.level[3].Boss = (hh_entity){
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
	levels.level[3].Boss.pos = lvl4_boss_spawn_point;
	levels.level[3].Boss.start = lvl4_boss_spawn_point;
	FILE *lvl4 = fopen("../test/bin/lvl4.bin", "rb");
	if (lvl4 == NULL) {
		printf("lvl4.bin not found!\n");
		return levels;
	}
	fseek(lvl4, 0, SEEK_END);
	size = ftell(lvl4) / sizeof(int);
	fseek(lvl4, (0 * sizeof(int)) + sizeof(int), SEEK_SET);
	int* hh_game_level4 = malloc(size * sizeof(int));
	fread(hh_game_level4, sizeof(int), size, lvl4);
	fclose(lvl4);

	levels.level[3].place = hh_game_level4;

	levels.shop.size.x=40;
	levels.shop.size.y=15;
	FILE *shop = fopen("static/shop_new.bin", "rb");
	if (shop == NULL) {
		printf("shop.bin not found!\n");
		return levels;
	}
	fseek(shop, 0, SEEK_END);
	size = ftell(shop) / sizeof(int);
	fseek(shop, (0 * sizeof(int)) + sizeof(int), SEEK_SET);
	int* hh_game_shop = malloc(size * sizeof(int));
	fread(hh_game_shop, sizeof(int), size, shop);
	fclose(shop);

	levels.shop.place = hh_game_shop;


	return levels;
}

