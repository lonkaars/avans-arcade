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
	hh_gravity_entity(enemy);
	if(enemy->is_grounded){
		if((player.pos.x - enemy->pos.x) >= -hunt_player && (player.pos.x - enemy->pos.x) <= hunt_player){
			hh_movement_entity(enemy, direction);
		}
	}
	hh_update_enemy_movement(enemy);

	hh_animate(&enemy->render,
		HH_TM_SLIME_JUMPABLE_OFFSET ,
		HH_TM_SLIME_JUMPABLE_OFFSET + 3,1
	);

}

void hh_jump_slime_ai(hh_entity* enemy, hh_entity player){
	bool jumped = false;
	int8_t direction = hh_get_direction(player.pos.x, enemy->pos.x);
	hh_gravity_entity(enemy);
	if(enemy->is_grounded){
		if((player.pos.x - enemy->pos.x) >= -hunt_player && (player.pos.x - enemy->pos.x) <= hunt_player){
			hh_movement_entity(enemy, direction);
			// TODO: fix this if statement and make it cleaner. this makes the enemy jump when the player mets the condition
			if((player.pos.y - enemy->pos.y) < -16 && (player.pos.y - enemy->pos.y) >= -100 && (player.pos.x - enemy->pos.x) >= -10 && (player.pos.x - enemy->pos.x) <= 10){
				// jump height is 10. TODO make it a define/var
				hh_jump_entity(enemy, 10);
				jumped = true;
			}
		}
		hh_animate(&enemy->render,
			HH_TM_SLIME_JUMPABLE_OFFSET + (jumped?4:0),
			HH_TM_SLIME_JUMPABLE_OFFSET + (enemy->render.fam.tilemap_index>=HH_TM_SLIME_JUMPABLE_OFFSET+5 ? 7 : 3),1
		);
	}
	hh_update_enemy_movement(enemy);


}

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

void hh_charging_boss_ai(hh_entity* boss, hh_entity player){
	static int count=0;
	static int8_t direction;
	static int last_x_location=0;
	static int prev_location;
	hh_update_enemy_movement(boss);
	hh_gravity_entity(boss);

	if(boss->is_grounded){
		if (count < terror_owl_attack_timer) {
			// wait for the charge
			direction = hh_get_direction(player.pos.x, boss->pos.x);
			last_x_location = player.pos.x;
			count++;
		} else {
			boss->render.fam.horizontal_flip = (direction < 0) ? false:true;
			if(boss->pos.x != last_x_location){
				boss->vel.x +=  (direction * boss->speed);
				if(boss->pos.x == prev_location){
					count = 0;
				}
			}
			else{
				count = 0;
			}
			prev_location = boss->pos.x;
		}
	}
	
	
	// hh_update_enemy_movement(boss);
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
	case charging_boss:
		hh_charging_boss_ai(enemy,player);
		break;
	default:
		break;
	}
}
