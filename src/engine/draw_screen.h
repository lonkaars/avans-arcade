#pragma once


// every function call for drawing the screen goes here.
#include "engine/maths.h"

#include <stdint.h>
uint16_t hh_world_to_tile(vec2 pos);
void hh_draw_screen(vec2 viewport);