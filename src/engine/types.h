#pragma once

#include "engine/maths.h"

typedef uint8_t hh_ppu_fg_idx;
// typedef uint16_t hh_bg_idx;

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
	uint16_t frame0;
	uint16_t palette;
	uint16_t ppu_foreground_index;
	
}hh_s_rendering;

typedef struct {
	vec2 pos, vel, size;
	bool is_grounded;
	bool is_hit;
	uint8_t radius;
	int8_t hp;
	int8_t speed;
	int8_t ai_type;
	hh_s_rendering render;

}hh_entity;

typedef struct {
	hh_entity p;
	hh_s_atributes atr;
}hh_s_player;
