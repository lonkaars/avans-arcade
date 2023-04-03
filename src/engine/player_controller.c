#include "ppu/types.h"
#include "engine/camera.h"
#include "engine/draw_screen.h"
#include "engine/sprite_controller.h"
#include "engine/player_controller.h"
#include "input.h"

#include "engine/animator.h"
#include "engine/bullet.h"
void hh_player_actions() {
	static hh_entity player={
		.hp = 4,
		.speed = 6,
		.is_grounded = false,
		.is_hit = false,
		.radius = 16,
		.pos = (vec2){128+16,32},
		.vel = (vec2){0,0},
		.size = (vec2){32,32},
		.render = {
			.frame0 = 80,
			.palette = 3,
			.ppu_foreground_index = 0,
			.fam = (hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = 3,
				.tilemap_index = 80,
			}
		}
	}, player_new = {0};

    static hh_entity bullet={
//        .hp = 4,
        .speed = 6,
        .is_grounded = true,
        .is_hit = false,
        .radius = 8,
        .pos = (vec2){-16,-16},
        .vel = (vec2){0,0},
        .render = {
            .frame0 = 84,
            .palette = 3,
            .fam = (hh_s_ppu_loc_fam_entry){
                .horizontal_flip = false,
                .vertical_flip = false,
                .palette_index = 7,
                .tilemap_index = 84,
            }
        }
    };

	static hh_entity enemy={
		.hp = 4,
		.speed = 6,
		.is_grounded = false,
		.is_hit = false,
		.radius = 8,
		.pos = (vec2){128,48},
		.vel = (vec2){0,0},
		.size = (vec2){16,16},
		.render = {
			.frame0 = 20,
			.palette = 7,
			.ppu_foreground_index = 16,
			.fam = (hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = 7,
				.tilemap_index = 1,
			}
		}
	};
	player_new = player;
	// hh_input_read();
	static uint8_t hit = 0;
	int8_t hit_timer = 0;
	int8_t direction_x = (-1 * g_hh_controller_p1.dpad_left) + (1 * g_hh_controller_p1.dpad_right);
	int8_t direction_y = (-1 * g_hh_controller_p1.dpad_up) + (1 * g_hh_controller_p1.dpad_down);

	if(player.is_hit == true){
		hit_timer = 9;
		player.is_hit = false;	
	} 
	if(hit_timer > -10){
			hit_timer--;
	}

	if(hit_timer <= 0){
		if(direction_x != 0){
			if(player.vel.x > -1 * player.speed && player.vel.x < player.speed) {
				player.vel.x = player.vel.x + direction_x;
			} else {
				if (player.vel.x > 0) {
					player.vel.x--;
				} else if(player.vel.x < 0) {
					player.vel.x++; 
				}
			}
		} else {
			if (player.vel.x > 0) {
				player.vel.x--;
			} else if(player.vel.x < 0) {
			player.vel.x++; 
			}
		}
		
		/* // movement Y (w-s) disable gravity to use this
		if(direction_y != 0){
			if(player.vel.y > -4 && player.vel.y < 4 ) {
				player.vel.y =  player.vel.y + direction_y;
			}
		} else {
			if (player.vel.y > 0) {
				player.vel.y--;
			} else if(player.vel.y < 0) {
				player.vel.y++; 
			}
		}		
		
		*/
	} else {
		if (player.vel.x > 0) {
			player.vel.x--;
		} else if(player.vel.x < 0) {
			player.vel.x++; 
		}
			player.vel.y++;
	}
	

	if (g_hh_controller_p1.button_primary && player.is_grounded == true) {//JUMP
	 	player.vel.y = -10;
		player.is_grounded = false;
	} else if (player.vel.y < 6){
		 player.vel.y += 1; //gravity
	}
	
/*
	player.vel = (vec2){.x = (-1 * g_hh_controller_p1.dpad_left) + (1 * g_hh_controller_p1.dpad_right),
	.y = (-1 * g_hh_controller_p1.dpad_up) + (1 * g_hh_controller_p1.dpad_down) };

		player_new.vel = (vec2){
		.x = player.vel.x,
		.y = player.vel.y,
	};
*/

		player_new.vel = (vec2){
		.x = player.vel.x,
		.y = player.vel.y,
	};

	player_new = hh_enemy_collision(player, enemy);

	player_new.pos = (vec2){
		.x = player.pos.x + player_new.vel.x,
		.y = player.pos.y + player_new.vel.y,
	};


	player =  hh_background_collision( player, player_new);
	
	vec_cor cam_pos;//value in tiles
	cam_pos = hh_draw_screen(player.pos);
	hh_shoot_bullet(player.pos, cam_pos ,&bullet);
	uint16_t idx = 16;
	// hh_clear_sprite();
	hh_update_sprite(&idx, &enemy, cam_pos);
	hh_update_sprite(&idx, &player, cam_pos);

	idx =16;

	// TODO: make this a function call
	// update player sprite on ppu
	// player.render.fam.position_x = (player.pos.x-cam_pos.x);
	// player.render.fam.position_y = (player.pos.y-cam_pos.y);

	// enemy.render.fam.position_x = (enemy.pos.x-cam_pos.x);
	// enemy.render.fam.position_y = (enemy.pos.y-cam_pos.y);


	// TODO: make this loop a function call
	// for (int i = 0; i < 4; i++)
	// {
	// 	hh_s_ppu_loc_fam_entry temp = player.render.fam;
	// 	temp.position_x = player.render.fam.position_x+(!(player.vel.x>0)?-1:1)*(i%2?8:-8);
	// 	temp.position_y = player.render.fam.position_y+(i>1?0:-16);
	// 	temp.tilemap_index = player.render.fam.tilemap_index + i;
	// 	temp.horizontal_flip = !(player.vel.x>0);
	// 	hh_ppu_update_foreground(i,temp);

	// 	// hh_s_ppu_loc_fam_entry temp = {
	// 	// 	.position_x = player.render.fam.position_x+(!(player.vel.x>0)?-1:1)*(i%2?8:-8)
	// 	// };

	// }

	// hh_ppu_update_foreground(4, enemy.render.fam);

}



