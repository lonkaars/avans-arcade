#pragma once

#include <stdint.h>
#include <stdbool.h>

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
