#pragma once

#include "engine/maths.h"

typedef uint8_t hh_idx_t;

typedef enum {
	fire, ice, poison
}hh_e_damage_t;


typedef struct {
	int8_t hp;
	int8_t dmg;
	hh_e_damage_t dmg_type;
	int8_t speed_x, speed_y;

} hh_s_atributes;


typedef struct {
	hh_s_ppu_loc_fam_entry fam; //screen
	hh_idx_t frame0;
	hh_idx_t palette;
	
}hh_s_rendering;

typedef struct {
	vec2 pos, vel, size;
	bool is_grounded;
	bool is_hit;
	uint8_t radius;
	int8_t hp;
	int8_t speed;
	hh_s_rendering render;

}hh_entity;

typedef struct {
	hh_entity p;
	hh_s_atributes atr;
}hh_s_player;
