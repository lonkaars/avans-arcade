#pragma once
#include "player_controller.h"
#include "engine/sprite_controller.h"
#include "types.h"
#include "input.h"

/** @brief updates a single enemy locations TODO: Bjorn sets functions in here*/
void hh_update_enemy(hh_entity* , vec_cor );

/** @brief calculates all the given enemies positions*/
void hh_multiple_enemies( vec_cor cam_pos, hh_entity* enemies, int total_enemies);

/** @brief checks if the enemy has zero hp else it takes hp from the enemy */
void hh_enemy_death_check(hh_entity* enemy);

/** @brieg all the given enemies get controlled if there hit and than calculates the dmg */
void hh_solve_hitted_enemies(hh_entity* enemies, int total_enemies);

