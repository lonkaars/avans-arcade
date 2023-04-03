#include "gameplay.h"

// player struct


void hh_gameplay(hh_g_all_levels game, hh_e_game_state* hh_game_state){
	static hh_e_gameplay gameplay = hh_e_setup_screen;
	static hh_entity player1={
		.hp = 4,
		.speed = 6,
		.is_grounded = false,
		.is_hit = false,
		.radius = 8,
		.pos = (vec2){32,32},
		.size = (vec2){32,32},
		.vel = (vec2){0,0},
		.render = {
			.frame0 = 80,
			.palette = 3,
			.fam = (hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = 3,
				.tilemap_index = 80,
			}
		}
	};
	// enemy gets replaced here is just for reference
	static hh_entity enemy={
		.hp = 4,
		.speed = 6,
		.is_grounded = false, 
		.is_hit = false,
		.radius = 8,
		.pos = (vec2){128,48},
		.vel = (vec2){0,0},
		// .vec = (vec2){0,0},
		.render = {
			.frame0 = 20,
			.palette = 7,
			.fam = (hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = 7,
				.tilemap_index = 1,
			}
		}
	};

	switch (gameplay)
	{
	case hh_e_setup_screen:
		hh_setup_screen(game.level[game.current_level]);
		gameplay = hh_e_play_level;
		break;
	case hh_e_play_level:
		// TODO: here come all the different functions for the gameplay
		vec_cor cam_pos;//value in tiles
		cam_pos = hh_draw_screen(player1.pos);
		hh_player_actions(&player1,cam_pos);

		// enemy's



		if(game.level[game.current_level].hh_level_completed){
			gameplay = hh_e_level_complete;
		}
		break;
	case hh_e_level_complete:
		if(game.current_level < 3){
			game.current_level++;
			gameplay = hh_e_setup_screen;
		}
		else {
         gameplay = hh_e_game_over;
      }
		break;
	case hh_e_game_over:
		// todo make reset levels
		hh_reset_levels();
		gameplay = hh_e_setup_screen;
		*hh_game_state = 	hh_e_state_game_over;
		break;
	default:
		break;
	}
	
}
void hh_reset_levels(){}
