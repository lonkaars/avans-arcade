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


void enemy_ai(hh_entity* entity, hh_entity player, int cord_x_begin, int cord_x_end, int cord_y)
{
	int num = 32;

	switch (entity->ai_type)
	{

		case 1:
			enemy_move_x(entity, cord_x_begin, cord_x_end);

		break;

//		case 2:

//		 	enemy_move_x(entity, cord_x_begin, cord_x_end);
//			if( (entity->pos.x - player.pos.x) < 35 && (entity->pos.x - player.pos.x) > -35 )
//			{
//				int a = entity->pos.x - player.pos.x;
		//		printf("%d\n", a);
//					if(g_hh_controller_p1.button_primary && entity->is_grounded == true)
//					{
//						entity->vel.y = -10;
//						entity->is_grounded = false;
//					} 
//			}
//			if (entity->vel.y < 6 && entity->is_grounded == false){
//				entity->vel.y += 1; //gravity
//			}
//		break;


	}


	


  
}
