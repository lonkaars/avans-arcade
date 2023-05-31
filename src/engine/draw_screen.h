#pragma once


// every function call for drawing the screen goes here.
#include "engine/maths.h"
#include "ppu/ppu.h"
#include "engine/level_const.h"
#include "engine/sprite_controller.h"
// #include <stdio.h>
#include <stdint.h>
// #include <stdlib.h>
#include "engine/camera.h"

/** @brief return a single tile from world */
uint8_t hh_world_to_tile(vec2 pos);

/** @brief main function for the screen. shift through the map and update the camera based on the player */
vec_cor hh_draw_screen(vec2 player);

/** @brief sets the start of a level */
void hh_setup_screen(hh_level_entity currentlevel);
/** @brief updates screen if view is at the beginning or end */
void hh_update_screen(vec2 view);

/** @brief send black screen to background memory */
void hh_clear_screen();

/** @brief clears all sprite data */
void hh_clear_sprite();

/** @brief send data to BAM memory from binary from shop */
void hh_setup_Shop();
