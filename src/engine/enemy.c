#include "engine/enemy.h"


void hh_multiple_enemies(hh_entity* enemies, hh_entity player, vec_cor cam_pos, int total_enemies){
	for(int i=0; i < total_enemies;i++){
			hh_update_enemy(&enemies[i] , player, cam_pos);
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
