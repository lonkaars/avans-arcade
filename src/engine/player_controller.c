#include "ppu/types.h"
#include "engine/camera.h"
#include "engine/draw_screen.h"
#include "engine/sprite_controller.h"
#include "engine/player_controller.h"
#include "input.h"
#include "global.h"
#include "static/tilemap.h"

#include "engine/animator.h"
#include "engine/bullet.h"
void hh_player_actions(hh_entity* player){
	player->is_hit=false;
	static hh_entity player_new = {0};
	player_new = *player;

	int8_t hit_timer = 0;
	int8_t direction_x = (-1 * g_hh_controller_p1.dpad_left) + (1 * g_hh_controller_p1.dpad_right);
	int8_t direction_y = (-1 * g_hh_controller_p1.dpad_up) + (1 * g_hh_controller_p1.dpad_down);

	hh_player_move_hit(player,hit_timer,direction_x);
	hh_gravity_entity(player);
	if(g_hh_controller_p1.button_primary){
		hh_jump_entity(player,g_hh_player_jump	);
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
	
	if (direction_x != 0) {
		player->render.fam.horizontal_flip = (direction_x > 0 ?0:1);
	}

}



