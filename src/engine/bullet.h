#pragma once
#include "player_controller.h"
#include "engine/sprite_controller.h"
#include "types.h"
#include "input.h"

hh_entity* hh_init_bullets(int size);

/** @brief checks if player hit button and sets the activate bullet*/
void hh_shoot_bullet(vec2 playerPos, hh_entity*);

/** @brief updates single bullet position*/
void hh_update_bullet(hh_entity* );

/** @brief calculates all the bullets positions and which to shoot */
void hh_multiple_bullets(vec2 player, hh_entity* bullets);

void hh_bullet_death(hh_entity* bullet);
