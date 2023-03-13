#pragma once

#include <stdbool.h>

/** @brief game controller state */
typedef struct {
	bool dpad_up;
	bool dpad_down;
	bool dpad_left;
	bool dpad_right;
	bool button_primary;
	bool button_secondary;
} hh_s_gamepad;

/** @brief player 1's controller */
extern hh_s_gamepad g_hh_controller_p1;
/** @brief player 2's controller */
extern hh_s_gamepad g_hh_controller_p2;

/** @brief update g_hh_controller_p1 and 2 by reading buttons */
void hh_input_read();
