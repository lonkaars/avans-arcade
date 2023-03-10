#include <SDL2/SDL.h>

#include "input.h"

hh_s_gamepad g_hh_controller_p1 = { 0 };
hh_s_gamepad g_hh_controller_p2 = { 0 };

void hh_input_read() {
	// SDL_PumpEvents();
	const Uint8* kb = SDL_GetKeyboardState(NULL);
	g_hh_controller_p1.dpad_up = kb[SDL_SCANCODE_W];
	g_hh_controller_p1.dpad_down = kb[SDL_SCANCODE_S];
	g_hh_controller_p1.dpad_left = kb[SDL_SCANCODE_A];
	g_hh_controller_p1.dpad_right = kb[SDL_SCANCODE_D];
}

