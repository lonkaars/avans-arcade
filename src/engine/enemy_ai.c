#include "engine/enemy_ai.h"

int8_t hh_get_direction(int32_t player_x, int32_t enemy_x){
	int movement_direction = player_x - enemy_x; // = player.pos.x - player.prev_pos.x;
	if (movement_direction > 0) {
		return 1; // move in positive direction
	} else if (movement_direction < 0) {
		return -1; // move in negative direction
	} else {
		return 0; // don't move
	}
}
void hh_update_enemy_movement(hh_entity* enemy){
	hh_entity enemy_new = {0};
	enemy_new = *enemy;
	enemy_new.vel = (vec2){
		.x = enemy->vel.x,
		.y = enemy->vel.y,
	};
	
	enemy_new.pos = (vec2){
		.x = enemy->pos.x + enemy_new.vel.x,
		.y = enemy->pos.y + enemy_new.vel.y,
	};


	*enemy =  hh_background_collision ( *enemy, enemy_new);
}
void hh_slime_ai(hh_entity* enemy, hh_entity player){
	int8_t direction = hh_get_direction(player.pos.x, enemy->pos.x);
	if((player.pos.x - enemy->pos.x) >= -hunt_player && (player.pos.x - enemy->pos.x) <= hunt_player){
   	hh_movement_entity(enemy, direction);
	}
	hh_gravity_entity(enemy);
	hh_update_enemy_movement(enemy);

}

void hh_jump_slime_ai(hh_entity* enemy, hh_entity player){
	int8_t direction = hh_get_direction(player.pos.x, enemy->pos.x);
	hh_gravity_entity(enemy);
	if((player.pos.x - enemy->pos.x) >= -hunt_player && (player.pos.x - enemy->pos.x) <= hunt_player){
   	hh_movement_entity(enemy, direction);
		// TODO: fix this if statement and make it cleaner. this makes the enemy jump when the player mets the condition
		if((player.pos.y - enemy->pos.y) < -16 && (player.pos.y - enemy->pos.y) >= -100 && (player.pos.x - enemy->pos.x) >= -10 && (player.pos.x - enemy->pos.x) <= 10){
			hh_jump_entity(enemy);
		}
	}

	hh_update_enemy_movement(enemy);

}
#define terror_owl_attack_timer 100
#define terror_owl_follow_player_delay 5 
#define terror_owl_attack_player_delay 12
#define terror_owl_dive_speed 3
#define delay_frames 2
#define terror_owl_minimal_distance_from_player 100
void hh_terror_owl_ai(hh_entity* enemy, hh_entity player){
	static int count=0;
	static int last_y_location=0;
    
	if (count < terror_owl_attack_timer) {
		// Move towards player position smoothly
		int delta_x = (-50 + player.pos.x - enemy->pos.x) / terror_owl_follow_player_delay;
		enemy->pos.x += delta_x;
		enemy->pos.y = player.pos.y - terror_owl_minimal_distance_from_player;

		count++;
		last_y_location=player.pos.y;
	} else {
		if(enemy->pos.y <= last_y_location){
			int delta_x = (player.pos.x - enemy->pos.x) / terror_owl_attack_player_delay;
			enemy->pos.x += delta_x;
			enemy->pos.y += terror_owl_dive_speed;
		}
		else{
			count = 0;
		}
	}

}
void hh_update_enemy(hh_entity* enemy ,  hh_entity player, vec_cor cam_pos){
	// static hh_e_entity_type type;
	switch (enemy->object_type)
	{
	case slime:
		hh_slime_ai(enemy,player);
		break;
	case jumping_slime:
		hh_jump_slime_ai(enemy,player);
		break;
	case terror_owl:
		hh_terror_owl_ai(enemy,player);
		break;
	default:
		break;
	}
}
