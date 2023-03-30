#pragma once


// every function call for drawing the screen goes here.
#include "engine/maths.h"
#include "ppu/ppu.h"
#include "engine/level_const.h"
#include "engine/sprite_controller.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "engine/camera.h"

#define hh_max_x_size 40
#define hh_max_y_size 30

/** @brief return a single tile from world binary */
uint8_t hh_world_to_tile(vec2 pos);

/** @brief shift to level if viewport changed position */
vec_cor hh_draw_screen(vec2 player);

/** @brief send data to BAM memory from binary level */
void hh_setup_screen(hh_level_entity currentlevel);
/** @brief updates screen based on view and maybe player position if it needs to turn back*/
void hh_update_screen(vec2 view, vec2 );

/** @brief send black screen to background memory */
void hh_clear_screen();

/** @brief clears all sprite data */
void hh_clear_sprite();
/** @brief send data to BAM memory from binary from shop */
void hh_setup_Shop();
