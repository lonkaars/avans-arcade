#pragma once
#include "engine/draw_screen.h"
#include "engine/player_controller.h"
#include "engine/sprite_controller.h"
#include "game_loop/starting_screen.h"
#include "engine/level_const.h"
#include "engine/animator.h"

typedef enum {
	hh_e_setup_screen,
	hh_e_play_level,
	hh_e_level_complete,
	hh_e_game_over,
}hh_e_gameplay;

void hh_reset_levels();
void hh_gameplay(hh_g_all_levels, hh_e_game_state*);

