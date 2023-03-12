#pragma once

#include <stdint.h>

#include "engine/maths.h"

typedef struct {
    vec2 pos, vec;
    bool is_grounded;
    int8_t hp;
    //armor/block?
}hh_entity;

/// @brief detect for collision enity and eviroment
/// @param pos1 position of environment tile to be checked
/// @param pos2 position entity
/// @return true if collision between enity and environment
bool hh_collision(vec2* pos1, vec2* pos2);

/// @brief solve collisions
/// @param environment position
/// @param entity position
/// @return solved new entity position
void hh_solve_collision(vec2* pos_environment, hh_entity* entity);
