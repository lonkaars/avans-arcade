#include "gameplay.h"
#include "engine/entity.h"
#include "static/tilemap.h"
#include "game_loop/ui.h"
#include "global.h"

// player struct


void hh_gameplay(hh_g_all_levels* game, hh_e_game_state* hh_game_state, hh_e_upgrades upg, int** points){
	static hh_e_gameplay gameplay = hh_e_setup_screen;
	static hh_entity* bullets;
	static hh_entity player1={
		.is_grounded = false,
		.is_hit = false,
		.radius = 12,
		.pos = (vec2){32,200},
		.size = (vec2){16,32},
		.vel = (vec2){0,0},
		.render = {
			.frame0 = HH_TM_GOZER_OFFSET,
			.palette = 3,
			.fam = (hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = 3,
				.tilemap_index = HH_TM_GOZER_OFFSET,
			}
		}
	};
	switch (gameplay)
	{
	case hh_e_setup_screen:
		hh_upgrade_handler(&player1,upg);
		bullets = hh_init_bullets(g_hh_player_ammo_count);
		hh_setup_screen(game->level[game->current_level]);
		gameplay = hh_e_play_level;
		player1.pos = game->level[game->current_level].player_start_pos;
		break;
	case hh_e_play_level:
		int total_enemies = game->level[game->current_level].hh_lvl_tot_enemies;

		vec_cor cam_pos;
		cam_pos = hh_draw_screen(player1.pos);
		hh_player_actions(&player1);
		hh_multiple_bullets(player1,bullets);
		hh_multiple_enemies( game->level[game->current_level].hh_enemies, &player1, cam_pos, total_enemies);

		// enemy collisions check
		hh_check_all_collisions(&player1, game->level[game->current_level].hh_enemies,total_enemies,bullets,g_hh_player_ammo_count,cam_pos);
		hh_solve_hitted_enemies( game->level[game->current_level].hh_enemies,total_enemies);

		hh_boss_battle(&game->level[game->current_level].Boss,&player1,bullets,g_hh_player_ammo_count, cam_pos	);

	
	   hh_render_all_entities(&player1,bullets, game->level[game->current_level].hh_enemies, &game->level[game->current_level].Boss, g_hh_player_ammo_count, total_enemies, cam_pos);
		if(player1.hp == 0){ 
			gameplay = hh_e_game_over;
			**points = hh_point_counter(*game);
		}
		else if(game->level[game->current_level].Boss.hp < 1){
			gameplay = hh_e_level_complete;
			**points = hh_point_counter(*game);
		}
		
		break;
	case hh_e_level_complete:
		game->current_level++;
		if(game->current_level == sizeof(game->level)/sizeof(game->level[0])){
			gameplay = hh_e_game_over;
		}
		else{
			*hh_game_state = 	hh_e_state_shop;
		}
		gameplay = hh_e_setup_screen;
		
		break;
	case hh_e_game_over:
		// todo make reset levels
		hh_reset_levels(&player1 , game);
		gameplay = hh_e_setup_screen;
		*hh_game_state = 	hh_e_state_game_over;
		break;
	default:
		break;
	}
	
}
void hh_reset_levels(hh_entity* player, hh_g_all_levels* game){
	*player = (hh_entity){
		.hp = g_hh_player_starting_hp,
		.speed = g_hh_player_starting_speed,
		.pos = game->level[0].player_start_pos,

		.is_grounded = false,
		.is_hit = false,
		.radius = 12,
		.size = (vec2){16,32},
		.vel = (vec2){0,0},
		.render = {
			.frame0 = HH_TM_GOZER_OFFSET,
			.palette = 3,
			.fam = (hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = 3,
				.tilemap_index = HH_TM_GOZER_OFFSET,
			}
		}
	
	};
	g_hh_player_hp = g_hh_player_starting_hp;
	g_hh_player_max_hp = g_hh_player_starting_max_hp;
	g_hh_player_jump = g_hh_player_starting_jump_height;
	g_hh_player_dmg = g_hh_player_starting_dmg;
	g_hh_player_ammo_count = g_hh_player_starting_ammo_count;
	g_hh_player_movement_speed = g_hh_player_starting_speed;
	
	for(int i = 0; i <= game->current_level; i++){
		vec2 start = game->level[i].Boss.start;
		game->level[i].Boss = (hh_entity){
		.pos = start,
		.start = start,
		.object_type = charging_boss,
		.hp = g_hh_boss_starting_hp,
		.speed = g_hh_boss_starting_speed,
		.is_grounded = false, 
		.is_hit = false,
		.radius = 16,
		.size = (vec2){48,32},
		.vel = (vec2){0,0},
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
		for(int cnt = 0; cnt < game->level[i].hh_lvl_tot_enemies; cnt++){
			vec2 start = game->level[i].hh_enemies[cnt].start;
			if(game->level[i].hh_enemies[cnt].object_type == slime || game->level[i].hh_enemies[cnt].object_type == jumping_slime){
				game->level[i].hh_enemies[cnt] =	(hh_entity){
					.pos = start,
					.start = start,
					.object_type = (game->level[i].hh_enemies[cnt].object_type == slime) ? slime : jumping_slime,
					.hp = g_hh_slime_hp,
					.speed = g_hh_slime_speed,
					.is_grounded = false, 
					.is_hit = false,
					.radius = g_hh_slime_radius,
					.size = (vec2){16,16},
					.vel = (vec2){0,0},
					.render = {
						.frame0 = HH_TM_SLIME_OFFSET,
						.palette = 2,
						.fam = (hh_s_ppu_loc_fam_entry){
							.horizontal_flip = false,
							.vertical_flip = false,
							.palette_index = 2,
							.tilemap_index = HH_TM_SLIME_OFFSET,
						}
					}
				};
			}
			else{
				game->level[i].hh_enemies[cnt] =	(hh_entity){
					.pos = start,
					.start = start,
					.object_type = (game->level[i].hh_enemies[cnt].object_type == slime) ? slime : jumping_slime,
					.hp = g_hh_terror_owl_hp,
					.speed = g_hh_terror_owl_speed,
					.is_grounded = false, 
					.is_hit = false,
					.radius = g_hh_terror_owl_radius,
					.size = (vec2){16,16},
					.vel = (vec2){0,0},
					.render = {
						.frame0 = HH_TM_BIRD_OFFSET,
						.palette = 4,
						.fam = (hh_s_ppu_loc_fam_entry){
							.horizontal_flip = false,
							.vertical_flip = false,
							.palette_index = 4,
							.tilemap_index = HH_TM_BIRD_OFFSET,
						}
					}
				};
			}
			
		}
	}
	game->current_level=0;
	
}


void hh_render_all_entities(hh_entity* player, hh_entity* bullets, hh_entity* enemies, hh_entity* boss, int bullet_size, int enemy_size, vec_cor cam_pos){

	uint16_t index = 0;
	hh_update_sprite(&index , player, cam_pos);
	
	for (int i = 0; i < bullet_size; i++) {
		hh_update_sprite(&index,&bullets[i],cam_pos);
 	}
	for (int i = 0; i < enemy_size; i++) {
		hh_update_sprite(&index,&enemies[i],cam_pos);
	}
	hh_update_sprite(&index,boss,cam_pos);
	hh_ui_show_hp(&index, player, g_hh_player_max_hp, cam_pos);
}

void hh_upgrade_handler(hh_entity* player, hh_e_upgrades upg){
	switch (upg)
	{
	case hh_e_upg_heal:
		g_hh_player_hp += (g_hh_player_max_hp - g_hh_player_hp);
		break;
	case hh_e_upg_max_health:
		g_hh_player_max_hp++;
		break;
	case hh_e_upg_jump:
		g_hh_player_jump++;
		break;
	case hh_e_upg_damage:
		g_hh_player_dmg++;
		break;
	case hh_e_upg_max_ammo:
		g_hh_player_ammo_count++;
		break;
	case hh_e_upg_speed:
		g_hh_player_movement_speed++;
		break;
	case hh_e_NULL:
	default:
		break;
	}
	player->hp=g_hh_player_hp;
	player->speed=g_hh_player_movement_speed;
}

int hh_point_counter(hh_g_all_levels game){
	int points = 0;
	for(int lvl = 0; lvl <= game.current_level; lvl++){
	// 	points = (game.level[lvl].Boss.hp == 0) ? points += g_hh_boss_points : 0;
		if(game.level[lvl].Boss.hp == 0){
			points += g_hh_boss_points;
			
		}
		for(int i =0; i < game.level[lvl].hh_lvl_tot_enemies;i++){
			// points = (game.level[lvl].hh_enemies[i].hp == 0) ? points += 5 : points;
			if(game.level[lvl].hh_enemies[i].hp == 0){
				points += g_hh_small_monster_points;
				
			}
			//printf("%d\n",game.level[lvl].hh_enemies[i].hp);
		}
	}
	
	return points;
}
