#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "ppu/types.h"
#include "engine/maths.h"
#include "engine/types.h"
#include "engine/animator.h"
#include "engine/sprite_controller.h"

// #include "engine/animator.h"

// TODO: make a sprite update function (and required data structs?)

/// @brief detect for collision enity and eviroment
/// @param pos1 position of environment tile to be checked
/// @param pos2 position entity
/// @return true if collision between enity and environment
bool hh_collision(vec2 pos1, vec2 pos2);

/// @brief solve collisions
/// @param environment position
/// @param entity position
/// @return solved new entity position
void hh_solve_collision(vec2 pos_environment, hh_entity* entity);

/// @brief solve collision of entity with background tiles
/// @param temp_old_entity old data of entity 
/// @param temp_new_entity new data of entity where it wants to go to
/// @return updated new entity where it actually can go to
hh_entity hh_background_collision(hh_entity temp_old_entity, hh_entity temp_new_entity);

/// @brief solve collision of player with enemy 
/// @param temp_player data of player 
/// @param temp_enemy data of enemy
/// @return updated player with new stats if hitted with enemy
hh_entity hh_enemy_collision(hh_entity temp_player, hh_entity temp_enemy);

/// @brief calculate if circles (entity) hit each other 
/// @param object_1 position of first object (entity)
/// @param object_2 position of second object (entity)
/// @param radius_1 radius of first object (entity)
/// @param radius_2 radius of second object (entity)
/// @return true if objects collids
bool hh_distance_circles(vec2 object_1, vec2 object_2, int radius_1, int radius_2);

/** @brief jumps a entity. TODO: input jumping how high and no magic number*/  
void hh_jump_entity(hh_entity* );
/** @brief looks if the player is grounded and if not it sets the position down */
void hh_gravity_entity(hh_entity* );
/** @brief if object is hit bounches in specific direction and moves the object */
void hh_hit_entity(hh_entity* object_1, int8_t* hit_timer, int8_t* direction);

/** @brief specific bullet background collisions detection */
bool hh_background_collision_bulllet (hh_entity temp_old_entity);
/** @brief checks any entity has a hit on each othe*/
void hh_check_all_collisions(hh_entity* player, hh_entity* enemies, int total_enemies, hh_entity* bullets, int total_bullets, vec_cor cam_pos);
