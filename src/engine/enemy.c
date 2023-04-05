#include "engine/enemy.h"

void hh_update_enemy(hh_entity* enemy, vec_cor cam_pos){
	//Bjorn functions


}

void hh_multiple_enemies( vec_cor cam_pos, hh_entity* enemies, int total_enemies){
	for(int i=0; i < total_enemies;i++){
			hh_update_enemy(&enemies[i] , cam_pos);
		}

}

void hh_enemy_death_check(hh_entity* enemy){
	if(enemy->hp == 0){
		enemy->is_hit=false;
		enemy->pos = (vec2){-16, -16};
	}
	else{
		enemy->hp--;
	}
}
void hh_solve_hitted_enemies(hh_entity* enemies, int total_enemies){
	for(int i = 0; i < total_enemies; i++){
		if(enemies[i].is_hit){
			hh_enemy_death_check(&enemies[i]);
		}
	}
}
