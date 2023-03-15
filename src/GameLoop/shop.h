#pragma once

#include "input.h"
#include "engine/draw_screen.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	hh_e_STATE_SHOW,
	hh_e_STATE_Input,
	hh_e_STATE_END
} hh_e_ShopStates;

bool hh_show_Shop();
