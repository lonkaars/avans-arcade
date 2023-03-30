#pragma once

#include "input.h"
#include "engine/draw_screen.h"
#include "engine/level_const.h"


#include <stdint.h>
#include <stdbool.h>

typedef enum {
	hh_e_game_over_SHOW,
	hh_e_game_over_Input,
	hh_e_game_over_END,
} hh_e_game_over;


void hh_game_over(hh_e_GameState*);
