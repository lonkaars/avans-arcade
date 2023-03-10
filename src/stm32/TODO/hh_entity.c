<<<<<<< HEAD
#include "maths.h"
#include "hh_entity.h"

bool hh_collision(const vec2& environment, const vec2& entity){
    
}
=======
#include <stdbool.h>

#include "hh_entity.h"
#include "maths.h"

/*
    PLAYER: (pos on X)
    ,___,
    |   |
    | X |
    |___|

*/

bool hh_collision(vec2* pos1, vec2* pos2){
    if (pos2->x == CLAMP(pos2->x,pos1->x,pos1->x+1.0f)){// hit x
        return true;
    }

    if (pos2->y == CLAMP(pos2->y,pos1->y,pos1->y+0.99f)){// hit y
        return true;
    }
    return false;
}

void hh_solve_collision(vec2* pos_environment, hh_entity* entity){
    if (entity->vec.x > 0.0f){
        entity->pos.x = MIN(entity->pos.x,pos_environment->x-1.0f);
        entity->vec.x = 0.0f;
    } else if (entity->vec.x < 0.0f){
        entity->pos.x = MAX(entity->pos.x,pos_environment->x+1.0f);
        entity->vec.x = 0.0f;
    } else if (entity->vec.y > 0.0f){
        entity->pos.x = MIN(entity->pos.x,pos_environment->x-1.0f);
        entity->vec.x = 0.0f;
    } else if (entity->vec.y < 0.0f){
        entity->pos.x = MAX(entity->pos.x,pos_environment->x+1.0f);
        entity->vec.x = 0.0f;
    }
}

>>>>>>> 458d620a4ae17c42e97413a49db6c1c5f53393e5
