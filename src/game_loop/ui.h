#pragma once

#include "static/tilemap.h"
#include "engine/player_controller.h"

#include "ppu/types.h"

#include "engine/types.h"
#include "engine/entity.h"
#include "engine/animator.h"
#include "engine/player_controller.h"


// extern hh_g_max_hp;

void hh_ui_show_hp(int* idx, hh_entity* player, uint8_t max_hp, vec_cor cam_pos);

void hh_ui_show_char(int* idx, char* str, vec2 pos);

/** @brief converts char* [0] to i and [1]='\0' */
void itoa(char *c, int i);

/** @brief moves 'cursor' through selection field */
void hh_shift_selected(uint8_t *pos, bool dir, uint8_t min, uint8_t max);
