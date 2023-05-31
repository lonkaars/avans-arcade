#pragma once
#include "player_controller.h"
#include "engine/sprite_controller.h"
#include "engine/enemy_ai.h"
#include "input.h"


/** @brief calculates all the given enemies positions*/
void hh_multiple_enemies(hh_entity* enemies, hh_entity* player, vec_cor cam_pos, int total_enemies);

/** @brief checks if the enemy has zero hp else it takes hp from the enemy */
void hh_enemy_death_check(hh_entity* enemy);

/** @brieg all the given enemies get controlled if there hit and than calculates the dmg */
void hh_solve_hitted_enemies(hh_entity* enemies, int total_enemies);

void hh_boss_battle(hh_entity* boss, hh_entity* player,hh_entity* bullets, int total_bullets, vec_cor cam_pos);
