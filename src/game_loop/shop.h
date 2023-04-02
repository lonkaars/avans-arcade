#pragma once

#include "input.h"
#include "engine/draw_screen.h"
#include "engine/level_const.h"


#include <stdint.h>
#include <stdbool.h>

typedef enum {
	hh_e_shop_show,
	hh_e_shop_input,
	hh_e_shop_end,
} hh_e_shop_states;


void hh_shop(hh_e_game_state*);
