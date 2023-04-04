#include "engine/entity.h"
#include "input.h"
#include "engine/animator.h"
#include "game_loop/gameplay.h"


void enemy_ai(hh_entity* entity, hh_entity* player, int cord_x_begin, int cord_x_end, int cord_y);
void enemy_move_x(hh_entity* entity, int cord_x_begin, int cord_x_end);
