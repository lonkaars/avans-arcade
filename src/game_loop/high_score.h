#pragma once

#include "input.h"
#include "engine/draw_screen.h"
#include "engine/maths.h"
#include "ppu/ppu.h"

#include "game_loop/ui.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	hh_e_score_show,
	hh_e_score_main,
	hh_e_score_end,
} hh_e_score_states;

typedef struct {
	hh_s_gamepad pressed, rising, falling;
} hh_s_controller_exp;

void hh_high_score(hh_e_game_state* hh_game_state);

bool hh_controller_exp_pressed(hh_s_controller_exp con1);

void hh_update_controller_exp(hh_s_controller_exp* in);

void hh_kb_display(uint8_t sel);
