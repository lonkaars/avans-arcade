#include "gameplay.h"
#include "engine/entity.h"


void hh_gameplay(hh_g_all_levels* game, hh_e_game_state* hh_game_state){
	static hh_e_gameplay gameplay = hh_e_setup_screen;
	static hh_entity* bullets;
	static hh_entity player1={
		.hp = 4,
		.speed = 6,
		.is_grounded = false,
		.is_hit = false,
		.radius = 16,
		.pos = (vec2){32,32},
		.size = (vec2){32,32},
		.vel = (vec2){0,0},
		.render = {
			.frame0 = 3,
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
		.object_type = jumping_slime,
		.hp = 4,
		.speed = 2,
		.is_grounded = false, 
		.is_hit = false,
		.radius = 8,
		.pos = (vec2){200,200},
		.size = (vec2){16,16},
		.vel = (vec2){0,0},
		// .vec = (vec2){0,0},
		.render = {
			.frame0 = 1,
			.palette = 7,
			.fam = (hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = 7,
				.tilemap_index = 1,
			}
		}
	};
	static int total_bullets = 5;
	switch (gameplay)
	{
	case hh_e_setup_screen:
		bullets = hh_init_bullets(total_bullets);
		hh_setup_screen(game->level[game->current_level]);
		gameplay = hh_e_play_level;
		break;
	case hh_e_play_level:

		vec_cor cam_pos;
		cam_pos = hh_draw_screen(player1.pos);
		hh_player_actions(&player1);
		hh_multiple_bullets(player1.pos,bullets);
		hh_multiple_enemies(&enemy, player1, cam_pos, 1);
		hh_check_all_collisions(&player1,&enemy,1,bullets,total_bullets,cam_pos);
		hh_solve_hitted_enemies(&enemy,1);
	  	hh_render_all_entities(&player1,bullets,&enemy,total_bullets,1, cam_pos);


		
		if(game->level[game->current_level].hh_level_completed){ 
			gameplay = hh_e_level_complete;
		}
		else if(player1.hp == 0){
			gameplay = hh_e_game_over;
		}
		break;
	case hh_e_level_complete:
		// TODO: from complete to shop or game_over
		if(game->current_level < 3){
			game->current_level+=1;
			gameplay = hh_e_setup_screen;
		}
		else {
			
         gameplay = hh_e_game_over;
      }
		break;
	case hh_e_game_over:
		// todo make reset levels
		hh_reset_levels(&player1);
		gameplay = hh_e_setup_screen;
		*hh_game_state = 	hh_e_state_game_over;
		break;
	default:
		break;
	}
	
}
void hh_reset_levels(hh_entity* player){
	*player = (hh_entity){
		.hp = 4,
		.speed = 6,
		.is_grounded = false,
		.is_hit = false,
		.radius = 16,
		.pos = (vec2){32,32},
		.size = (vec2){32,32},
		.vel = (vec2){0,0},
		.render = {
			.frame0 = 3,
			.palette = 3,
			.fam = (hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = 3,
				.tilemap_index = 80,
			}
		}
	};
}


void hh_render_all_entities(hh_entity* player, hh_entity* bullets, hh_entity* enemies, int bullet_size, int enemy_size, vec_cor cam_pos){

	int index = 0;
	hh_update_sprite(0 , player, cam_pos);
	
	for (int i = 0; i < bullet_size; i++) {
		hh_update_sprite(i+5,&bullets[i],cam_pos);
 	}
	for (int i = 0; i < enemy_size; i++) {
		hh_update_sprite(i+5+bullet_size,&enemies[i],cam_pos);
	}
}
