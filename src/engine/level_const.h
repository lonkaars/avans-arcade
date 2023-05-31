
#ifndef level_const_H
#define level_const_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "engine/maths.h"
#include "engine/entity.h"

typedef enum {
	hh_e_state_starting_screen,
	hh_e_state_shop,
	hh_e_state_gameplay,
	hh_e_state_game_over,
	hh_e_state_high_score
} hh_e_game_state;
//entity array met enemeies
typedef struct  {
	vec2 size;
	int* place;
	
	int hh_lvl_tot_enemies;
	vec2* start_enemy_loc;
	hh_entity* hh_enemies;

	vec2 player_start_pos;
	hh_entity Boss;
}hh_level_entity;

typedef struct  {
	hh_level_entity level[4];
	hh_level_entity shop;
	int current_level;


}hh_g_all_levels;

/** @brief init all the levels*/
hh_g_all_levels hh_init_game_levels();

#endif
