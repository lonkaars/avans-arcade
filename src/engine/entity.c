#include <stdbool.h>

#include "engine/entity.h"
#include "engine/maths.h"

/*
    PLAYER: (pos on X)
    ,___,
    |   |
    | X |
    |___|

*/

bool hh_collision(vec_cor pos1, vec2 pos2){
    if (pos2.x == CLAMP(pos2.x, pos1.x, pos1.x+16)){// hit x
        return true;
    }

    if (pos2.y == CLAMP(pos2.y, pos1.y, pos1.y+16)){// hit y
        return true;
    }
    return false;
}

void hh_solve_collision(vec2 pos_environment, hh_entity* entity){
	if (!hh_collision(pos_environment,entity->pos))
		return;

	printf("BONK!/n");
	// if (entity->vel.y > 0){
	// 	entity->pos.y = MAX(entity->pos.y,pos_environment.y);
	// 	entity->vel.y = 0;
	// } else {
	// 	entity->pos.y = MIN(entity->pos.y,pos_environment.y);
	// 	entity->vel.y = 0;
	// }
	// if (entity->vel.x <= 0){
	// 	entity->pos.x = MIN(entity->pos.x,pos_environment.x-16);
	// 	entity->vel.x = 0;
	// } else {
	// 	entity->pos.x = MAX(entity->pos.x,pos_environment.x+16);
	// 	entity->vel.x = 0;
	// } 
}

hh_entity hh_background_collision (hh_entity temp_old_entity,hh_entity temp_new_entity){

		temp_new_entity.is_grounded = false;
 
// solves x collision
	if (temp_old_entity.vel.x <= 0) {
		if (hh_colidable(hh_world_to_tile((vec2){.x=temp_new_entity.pos.x + 0, .y=temp_old_entity.pos.y + 0})) || 
			hh_colidable(hh_world_to_tile((vec2){.x=temp_new_entity.pos.x + 0, .y=temp_old_entity.pos.y + (temp_old_entity.size.y-1)}))) {
			temp_new_entity.pos.x = (temp_new_entity.pos.x & ~15) + 16,
			temp_new_entity.vel.x = 0;
		}
	} else {
		if (hh_colidable(hh_world_to_tile((vec2){.x=temp_new_entity.pos.x + temp_old_entity.size.x, .y=temp_old_entity.pos.y + 0})) || 
			hh_colidable(hh_world_to_tile((vec2){.x=temp_new_entity.pos.x + temp_old_entity.size.x, .y=temp_old_entity.pos.y + (temp_old_entity.size.y-1)}))) {
			temp_new_entity.pos.x = temp_new_entity.pos.x & ~15, // <-- magic comma, NOT TOUCHY
			temp_new_entity.vel.x = 0;
		}
	}

	//solves y collision
	if (temp_old_entity.vel.y <= 0) {
		if (hh_colidable(hh_world_to_tile((vec2){.x=temp_new_entity.pos.x + 0, .y=temp_new_entity.pos.y + 0})) || 
			hh_colidable(hh_world_to_tile((vec2){.x=temp_new_entity.pos.x + (temp_new_entity.size.x-1), .y=temp_new_entity.pos.y + 0}))) {
			temp_new_entity.pos.y = (temp_new_entity.pos.y & ~15) + 16,
			temp_new_entity.vel.y = 0;
		}
	} else {
		if (hh_colidable(hh_world_to_tile((vec2){.x=temp_new_entity.pos.x + 0, .y=temp_new_entity.pos.y + (temp_new_entity.size.y-1)})) || 
			hh_colidable(hh_world_to_tile((vec2){.x=temp_new_entity.pos.x + (temp_new_entity.size.x-1), .y=temp_new_entity.pos.y + (temp_new_entity.size.y-1)}))) {
			temp_new_entity.pos.y = temp_new_entity.pos.y & ~15,
			temp_new_entity.vel.y = 0;
			temp_new_entity.is_grounded = true;
		}
	}
	temp_old_entity = temp_new_entity;
	// temp_old_entity.is_grounded = temp_new_entity.is_grounded;
	// temp_old_entity.pos = temp_new_entity.pos; 
	// temp_old_entity.vel = temp_new_entity.vel;
	return temp_old_entity;
}

bool hh_background_collision_bulllet (hh_entity temp_old_entity){

	//	temp_new_entity.is_grounded = false;
 
// solves x collision
	if (temp_old_entity.vel.x <= 0) {
		if (hh_colidable(hh_world_to_tile((vec2){.x=temp_old_entity.pos.x + 0, .y=temp_old_entity.pos.y + 0})) || 
			hh_colidable(hh_world_to_tile((vec2){.x=temp_old_entity.pos.x + 0, .y=temp_old_entity.pos.y + (temp_old_entity.size.y-1)}))) {
				return true;
		}
	} else {
		if (hh_colidable(hh_world_to_tile((vec2){.x=temp_old_entity.pos.x + temp_old_entity.size.x, .y=temp_old_entity.pos.y + 0})) || 
			hh_colidable(hh_world_to_tile((vec2){.x=temp_old_entity.pos.x + temp_old_entity.size.x, .y=temp_old_entity.pos.y + (temp_old_entity.size.y-1)}))) {
				return true;
		}
	}

	//solves y collision
	if (temp_old_entity.vel.y <= 0) {
		if (hh_colidable(hh_world_to_tile((vec2){.x=temp_old_entity.pos.x + 0, .y=temp_old_entity.pos.y + 0})) || 
			hh_colidable(hh_world_to_tile((vec2){.x=temp_old_entity.pos.x + (temp_old_entity.size.x-1), .y=temp_old_entity.pos.y + 0}))) {
			return true;
		}
	} else {
		if (hh_colidable(hh_world_to_tile((vec2){.x=temp_old_entity.pos.x + 0, .y=temp_old_entity.pos.y + (temp_old_entity.size.y-1)})) || 
			hh_colidable(hh_world_to_tile((vec2){.x=temp_old_entity.pos.x + (temp_old_entity.size.x-1), .y=temp_old_entity.pos.y + (temp_old_entity.size.y-1)}))) {
				return true;
		}
	}
	// temp_old_entity = temp_new_entity;
	// temp_old_entity.is_grounded = temp_new_entity.is_grounded;
	// temp_old_entity.pos = temp_new_entity.pos; 
	// temp_old_entity.vel = temp_new_entity.vel;
	return false;
}

hh_entity hh_enemy_collision(hh_entity temp_player, hh_entity temp_enemy){
	if(temp_player.is_hit) return;
	bool collide = hh_distance_circles( temp_player.pos,  temp_enemy.pos, temp_player.radius, temp_enemy.radius);

	if (collide == true && temp_player.is_hit == false)
	{
		temp_player.is_hit = true;
		//angle = atan2( tempEntity.pos_y - tempPlayer.pos_y, tempEntity.pos_x - tempPlayer.pos_x);
		if(temp_player.pos.x <= temp_enemy.pos.x) //player left of enemy -- creates flinch movement L or R
		{
		//	printf("BONK-left!/n");
			temp_player.vel.y = -5;
			temp_player.vel.x = -8;
		}	else {
		//	printf("BONK-right!/n");
			temp_player.vel.y = -5;
			temp_player.vel.x = 8;
		}
		// ghost mode / invulnerable or other things on hit
		temp_player.hp--;
			
	} else {
		temp_player.is_hit = false;
		
	}


return temp_player;
}


bool hh_distance_circles (vec2 object_1, vec2 object_2, int radius_1, int radius_2){
	int a_squared = (object_1.x - object_2.x) * (object_1.x - object_2.x);
	int b_squared = (object_1.y - object_2.y) * (object_1.y - object_2.y);
	int c_squared = a_squared + b_squared; 
	int radius = ( radius_1 + radius_2) * ( radius_1 + radius_2 );

		if( c_squared <= radius ){
			return true;
		} else {
			return false;
		}
}

void hh_jump_entity(hh_entity* object_1, int jump_height){
	if (object_1->is_grounded == true) {//JUMP
	 	object_1->vel.y = -jump_height;
		object_1->is_grounded = false;
	}
}
void hh_gravity_entity(hh_entity* object_1){
	if (object_1->is_grounded == false) {
		if(object_1->vel.y < 15) {
			object_1->vel.y += 1; //gravity
		}
	}
}

void hh_movement_entity(hh_entity* object_1, int8_t* direction){
	if(direction != 0){
		if(object_1->vel.x > -1 * object_1->speed && object_1->vel.x < object_1->speed) {
			object_1->vel.x = object_1->vel.x + direction;
			object_1->render.fam.horizontal_flip=(object_1->vel.x>0?0:1);// flips direction of object_1
		} else {
			if (object_1->vel.x > 0) {
				object_1->vel.x--;
			} else if(object_1->vel.x < 0) {
				object_1->vel.x++; 
			}
		}
	} else {
		if (object_1->vel.x > 0) {
			object_1->vel.x--;
		} else if(object_1->vel.x < 0) {
		object_1->vel.x++; 
		}
	}
	
}
void hh_player_move_hit(hh_entity* object_1, int8_t* hit_timer, int8_t* direction){
	if(object_1->is_hit == true){
		hit_timer = 9;
		object_1->is_hit = false;
	} 
	if(hit_timer > -10){
			hit_timer--;
	}
	
	if(hit_timer <= 0){
		if(direction != 0){
			if(object_1->vel.x > -1 * object_1->speed && object_1->vel.x < object_1->speed) {
				object_1->vel.x = object_1->vel.x + direction;
			} else {
				if (object_1->vel.x > 0) {
					object_1->vel.x--;
				} else if(object_1->vel.x < 0) {
					object_1->vel.x++; 
				}
			}
		} else {
			if (object_1->vel.x > 0) {
				object_1->vel.x--;
			} else if(object_1->vel.x < 0) {
			object_1->vel.x++; 
			}
		}
		
	} else {
		if (object_1->vel.x > 0) {
			object_1->vel.x--;
		} else if(object_1->vel.x < 0) {
			object_1->vel.x++; 
		}
			object_1->vel.y++;
	}
	
}

void hh_check_all_collisions(hh_entity* player, hh_entity* enemies, int total_enemies, hh_entity* bullets, int total_bullets, vec_cor cam_pos){

	for(int i = 0; i < total_bullets; i++){
		if(!bullets[i].is_grounded){ 
			for (int enemy = 0; enemy < total_enemies; enemy++){
				
				if(hh_distance_circles(bullets[i].pos,enemies[enemy].pos,bullets[i].radius,enemies[enemy].radius)){
					enemies[enemy].is_hit=true;
					hh_bullet_death(&bullets[i]);
				}
			}	
		}
	}
	
}


