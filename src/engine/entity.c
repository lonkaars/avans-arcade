#include <stdbool.h>

#include "engine/entity.h"
#include "engine/maths.h"

#ifdef HH_TARGET_DESKTOP
#include <stdio.h>
#endif

/*
    PLAYER: (pos on X)
    ,___,
    |   |
    | X |
    |___|

*/

bool hh_collision(vec_cor pos1, vec2 pos2){
    if (pos2.x == CLAMP(pos2.x, pos1.x, pos1.x+16)){// hit x
        return true;
    }

    if (pos2.y == CLAMP(pos2.y, pos1.y, pos1.y+16)){// hit y
        return true;
    }
    return false;
}

void hh_solve_collision(vec2 pos_environment, hh_entity* entity){
	if (!hh_collision(pos_environment,entity->pos))
		return;

	printf("BONK!/n");
	// if (entity->vel.y > 0){
	// 	entity->pos.y = MAX(entity->pos.y,pos_environment.y);
	// 	entity->vel.y = 0;
	// } else {
	// 	entity->pos.y = MIN(entity->pos.y,pos_environment.y);
	// 	entity->vel.y = 0;
	// }
	// if (entity->vel.x <= 0){
	// 	entity->pos.x = MIN(entity->pos.x,pos_environment.x-16);
	// 	entity->vel.x = 0;
	// } else {
	// 	entity->pos.x = MAX(entity->pos.x,pos_environment.x+16);
	// 	entity->vel.x = 0;
	// } 
}

