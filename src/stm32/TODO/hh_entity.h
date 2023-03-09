#include "maths.h"

typedef struct hh_entity
{
    vec2 pos, direction;
    int8_t hp;
    //armor/block?
};

/// @brief detect for collision enity and eviroment
/// @param environment position of tile to be checked
/// @param entity position entity
/// @return true if collision between enity and environment
bool hh_collision(const vec2& environment, const vec2& entity);

/// @brief solve collisions
/// @param environment position
/// @param entity position
/// @return solved new entity position
vec2 hh_solve_collision(const vec2& environment, const vec2& entity);
