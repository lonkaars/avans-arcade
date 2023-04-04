#include "engine/Enemy_AI.h"



//entity pointer van maken 
//vec2

void enemy_move_x(hh_entity* entity, int cord_x_begin, int cord_x_end)
{
			if(entity->pos.x == cord_x_begin)
			{
				entity->vel.x = 1;
			}
			else if (entity->pos.x == cord_x_end)
			{
				entity->vel.x = -1;
			}
}


void enemy_ai(hh_entity* entity, hh_entity *player, int cord_x_begin, int cord_x_end, int cord_y)
{
	int num = 32;
	static hh_entity entity_new = {0};
	entity_new = *entity;

	switch (entity->ai_type)
	{

		case 1:
		
			enemy_move_x(entity, cord_x_begin, cord_x_end);
			
		break;

		case 2:

		 	enemy_move_x(entity, cord_x_begin, cord_x_end);
			if( (entity->pos.x - player->pos.x) < 35 && (entity->pos.x - player->pos.x) > -35 )
			{
					if(g_hh_controller_p1.button_primary)
					{
						hh_jump_entity(entity);
					} 
			}
		break;

		case 3:

		enemy_move_x(entity, cord_x_begin, cord_x_end);
		if((player->pos.x - entity->pos.x) > -10  && (player->pos.x - entity->pos.x) < 10)
		{
			entity->vel.y = 5;
		}
		else if(entity->is_grounded == true && entity->pos.y > 5)
		{
			entity->vel.y = -1;
		}
		else if(entity->pos.y == 5)
		{
			entity->vel.y = 0;
		}

		break;



	}

	if(entity->ai_type != 3){
	hh_gravity_entity(entity);
}
		entity_new.vel = (vec2){
		.x = entity->vel.x,
		.y = entity->vel.y,
	};

		entity_new.pos = (vec2){
		.x = entity->pos.x + entity_new.vel.x,
		.y = entity->pos.y + entity_new.vel.y,
	};

	*entity = hh_background_collision ( *entity, entity_new);
  
}
