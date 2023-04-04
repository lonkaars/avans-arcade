#pragma once
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
	int hh_total_enemies;
	int* place;
	bool hh_level_completed;
}hh_level_entity;

typedef struct  {
	hh_level_entity level[2];
	int current_level;


}hh_g_all_levels;

/** @brief init all the levels*/
hh_g_all_levels hh_init_game_levels();
