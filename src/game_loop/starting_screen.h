#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	hh_e_state_show,
	hh_e_state_input,
	hh_e_state_end
} hh_e_screen_states;


bool hh_show_starting_screen();

