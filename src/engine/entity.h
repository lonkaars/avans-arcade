#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "ppu/types.h"

#include "engine/maths.h"

typedef uint8_t hh_idx_t;

typedef enum {
	fire, ice, poison
}hh_e_damage_t;

typedef struct {
	hh_s_ppu_loc_fam_entry fam; //screen
	hh_idx_t frame0;
	hh_idx_t palette;
	
}hh_s_rendering;

typedef struct {
	int8_t hp;
	int8_t dmg;
	hh_e_damage_t dmg_type;
	int8_t speed_x, speed_y;

} hh_s_atributes;


typedef struct {
    vec2 pos, vel, vec;
    bool is_grounded;
	 bool is_hit;
	 uint8_t radius;
    int8_t hp;
    int8_t speed;
	 hh_s_rendering render;
    //armor/block?
}hh_entity;

typedef struct {
	hh_entity p;
	hh_s_atributes atr;
}hh_s_player;


/// @brief detect for collision enity and eviroment
/// @param pos1 position of environment tile to be checked
/// @param pos2 position entity
/// @return true if collision between enity and environment
bool hh_collision(vec2 pos1, vec2 pos2);

/// @brief solve collisions
/// @param environment position
/// @param entity position
/// @return solved new entity position
void hh_solve_collision(vec2 pos_environment, hh_entity* entity);

/// @brief solve collision of entity with background tiles
/// @param temp_old_entity old data of entity 
/// @param temp_new_entity new data of entity where it wants to go to
/// @return updated new entity where it actually can go to
hh_entity hh_background_collision (hh_entity temp_old_entity, hh_entity temp_new_entity);

/// @brief solve collision of player with enemy 
/// @param temp_player data of player 
/// @param temp_enemy data of enemy
/// @return updated player with new stats if hitted with enemy
hh_entity hh_enemy_collision(hh_entity temp_player, hh_entity temp_enemy);

/// @brief calculate if circles (entity) hit each other 
/// @param object_1 position of first object (entity)
/// @param object_2 position of second object (entity)
/// @param radius_1 radius of first object (entity)
/// @param radius_2 radius of second object (entity)
/// @return true if objects collids
bool hh_distance_circles (vec2 object_1, vec2 object_2, int radius_1, int radius_2);


