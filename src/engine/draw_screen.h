#pragma once


// every function call for drawing the screen goes here.
#include "engine/maths.h"
#include "ppu/ppu.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#define HH_map_size_X 80
#define HH_map_size_Y 60

/** @brief return a single tile from world binary */
uint8_t hh_world_to_tile(vec2 pos);
/** @brief shift to level if viewport changed position */
void hh_draw_screen(vec2 viewport);
/** @brief send data to BAM memory from binary level */
void hh_setup_screen();
