#pragma once

#include "input.h"
#include "engine/draw_screen.h"
#include "engine/level_const.h"


#include <stdint.h>
#include <stdbool.h>

typedef enum {
	hh_e_game_over_show,
	hh_e_game_over_input,
	hh_e_game_over_end,
} hh_e_game_over;


void hh_game_over(hh_e_game_state*);
