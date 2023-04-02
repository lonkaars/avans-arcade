#pragma once
#include "player_controller.h"
#include "engine/sprite_controller.h"
#include "input.h"


void hh_shoot_bullet(vec2 playerPos, vec_cor cam_pos, hh_entity*);

void hh_update_bullet(hh_entity* , vec_cor );

void hh_draw_bullet(hh_entity);
