#include "ppu/types.h"
#include "engine/camera.h"
#include "engine/draw_screen.h"
#include "engine/sprite_controller.h"
#include "engine/player_controller.h"

#include "input.h"

#include "engine/bullet.h"
void hh_player_actions() {
	static Bullet bullet ={
		.isActive=false,
	};
	static hh_entity player={
		.hp = 4,
		.speed = 6,
		.is_grounded = false,
		.is_hit = false,
		.radius = 8,
		.pos = (vec2){32,32},
		.vel = (vec2){0,0},
		.vec = (vec2){0,0},
		.render = {
			.frame0 = 80,
			.palette = 3,
			.fam = (hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false,
				.vertical_flip = false,
				.palette_index = 2,
				.tilemap_index = 60,
			}
		}
	}, player_new = {0};


	static hh_entity enemy={
		.hp = 4,
		.speed = 6,
		.is_grounded = false,
		.is_hit = false,
		.radius = 8,
		.pos = (vec2){128,48},
		.vel = (vec2){0,0},
		.vec = (vec2){0,0},
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

	if(g_hh_controller_p1.button_secondary==true){
		shootBullet(player.pos,&bullet);
	}
	updateBullet(&bullet,5);


	
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
	
	
	// const int8_t maa = 3;
	// const int8_t mbb = -3;
	// if (g_hh_controller_p1.dpad_up)
	//
	// if (g_hh_controller_p1.dpad_down)
	//
	// if (g_hh_controller_p1.dpad_left) {
	// 	player.vel.x += mbb;
	// 	// g_hh_demo_balls[0].horizontal_flip = true;
	// }
	// if (g_hh_controller_p1.dpad_right) {
	// 	player.vel.x += maa;
	// 	// g_hh_demo_balls[0].horizontal_flip = true;
	// }
	// if (g_hh_controller_p1.button_primary /*&& player.is_grounded*/) //JUMP
	// 	player.vel.y += -6;
	// // // if (g_hh_controller_p1.button_secondary)

	// player.vel.y += 1; //gravity


	//END OF VECTOR CHANGES
	// player.vel.y = CLAMP(player.vel.y,-32,32);
	// player.vel.x = CLAMP(player.vel.x,-32,32);

	player_new.pos = (vec2){
		.x = player.pos.x + player_new.vel.x,
		.y = player.pos.y + player_new.vel.y,
	};



	// const uint8_t empty = 0;
	// hh_s_tiles tiles[9];
	// const vec2 tile_offset[9] = {
	// 	(vec2){-16,-16},(vec2){0,-16},(vec2){+16,-16},
	// 	(vec2){-16,0},  (vec2){0,0},  (vec2){+16,0},
	// 	(vec2){-16,+16},(vec2){0,+16},(vec2){+16,+16},
	// };
	// for (int i = 0; i < 9; i++) {
	// 	vec2 temp_pos = vec_add(player.pos, tile_offset[i]);
	// 	temp_pos =(vec2){
	// 		.x = temp_pos.x,
	// 		.y = temp_pos.y,
	// 	};
	// 	hh_s_tiles tile = {
	// 		.pos = temp_pos,
	// 		.idx = hh_world_to_tile(temp_pos)
	// 	};
	// 	if(hh_colidable(tile.idx)) {
	// 		tiles[i]=tile;
	// 		// printf(" collidable near!");
	// 	} else {
	// 		tiles[i].idx = 0;
	// 	}
	// }
	/*
	012
	345
	678
	*/
	// for (int i = 0; i < 9; i++)
	// {
	// 	if (tiles[i].idx != 0){
	// 		hh_solve_collision(tiles[i].pos, &player);
	// 	}
	// }

	player =  hh_background_collision ( player, player_new);
	
	//player = player_new;

	vec_cor cam_pos;//value in tiles
	// cam_pos = (vec2){0,0};
	cam_pos = hh_update_camera(player.pos,(vec2){0,0},(vec2){.x=20*16,.y=30*16});//TODO: remove magic number(s)
	// printf("%i, %i:%i, %i\n",player.pos.x,player.pos.y,cam_pos.x,cam_pos.y);
	hh_draw_screen(cam_pos);
	// update player sprite on ppu
	player.render.fam.position_x = (player.pos.x-cam_pos.x);
	player.render.fam.position_y = (player.pos.y-cam_pos.y);

	enemy.render.fam.position_x = (enemy.pos.x-cam_pos.x);
	enemy.render.fam.position_y = (enemy.pos.y-cam_pos.y);

	player.render.fam.tilemap_index = 2;//TODO: these two lines should be redundant
	player.render.fam.palette_index = 7;
	// hh_ppu_update_foreground(0, player.render.fam);

	for (int i = 0; i < 4; i++)
	{
		hh_s_ppu_loc_fam_entry temp = player.render.fam;
		temp.position_x = player.render.fam.position_x+(!(player.vel.x>0)?-1:1)*(i%2?8:-8);
		temp.position_y = player.render.fam.position_y+(i>1?0:-16);
		temp.tilemap_index = player.render.frame0 + i;
		temp.palette_index = player.render.palette;
		temp.horizontal_flip = !(player.vel.x>0);
		hh_ppu_update_foreground(i,temp);
	}
	


	hh_ppu_update_foreground(4, enemy.render.fam);

}



