#pragma once
#include "player_controller.h"
#include "engine/sprite_controller.h"
#include "input.h"


void shootBullet(vec2 playerPos, vec_cor cam_pos, hh_entity*);

void updateBullet(hh_entity* , vec_cor, vec2 );

void drawBullet(hh_entity);
