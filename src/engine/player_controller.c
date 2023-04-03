#include "ppu/types.h"
#include "engine/camera.h"
#include "engine/draw_screen.h"
#include "engine/sprite_controller.h"
#include "engine/player_controller.h"
#include "input.h"

#include "engine/animator.h"
#include "engine/bullet.h"
void hh_player_actions(hh_entity* player, vec_cor cam_pos){
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
	static hh_entity player_new = {0};
	player_new = *player;

	int8_t hit_timer = 0;
	int8_t direction_x = (-1 * g_hh_controller_p1.dpad_left) + (1 * g_hh_controller_p1.dpad_right);
	int8_t direction_y = (-1 * g_hh_controller_p1.dpad_up) + (1 * g_hh_controller_p1.dpad_down);

	hh_hit_entity(player,hit_timer,direction_x);
	hh_gravity_entity(player);
	if(g_hh_controller_p1.button_primary){
		hh_jump_entity(player);
	}


		player_new.vel = (vec2){
		.x = player->vel.x,
		.y = player->vel.y,
	};
	
	player_new.pos = (vec2){
		.x = player->pos.x + player_new.vel.x,
		.y = player->pos.y + player_new.vel.y,
	};


	*player =  hh_background_collision ( *player, player_new);
	
	hh_shoot_bullet(player->pos, cam_pos ,&bullet);
	uint16_t idx = 16;
	idx = hh_update_sprite(idx, player, cam_pos);
	// idx = hh_update_sprite(idx, &enemy, cam_pos);

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



