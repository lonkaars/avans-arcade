#include "engine/enemy.h"
#include "global.h"

void hh_multiple_enemies(hh_entity* enemies, hh_entity* player, vec_cor cam_pos, int total_enemies){
	for(int i=0; i < total_enemies;i++){
			enemies[i].is_hit = false;
			if(enemies[i].hp > 0){
				hh_update_enemy(&enemies[i] , *player, cam_pos);
				*player = hh_enemy_collision(*player, enemies[i]);
			}
		}

}

void hh_enemy_death_check(hh_entity* enemy){
	enemy->hp = enemy->hp - g_hh_player_dmg;

	if(enemy->hp <= 0){
		enemy->pos = (vec2){-16, -16};
	}
}

void hh_solve_hitted_enemies(hh_entity* enemies, int total_enemies){
	for(int i = 0; i < total_enemies; i++){
		if(enemies[i].is_hit){
			hh_enemy_death_check(&enemies[i]);
		}
	}
}

void hh_boss_battle(hh_entity* boss, hh_entity* player,hh_entity* bullets, int total_bullets, vec_cor cam_pos){
	boss->is_hit=false;
	hh_update_enemy(boss , *player, cam_pos);
	*player = hh_enemy_collision(*player, *boss);

	for(int i = 0; i < total_bullets; i++){
		if(!bullets[i].is_grounded){ 				
			if(hh_distance_circles(bullets[i].pos,boss->pos,bullets[i].radius,boss->radius)){
				boss->is_hit=true;
				hh_bullet_death(&bullets[i]);
			}
		}
	}

	if(boss->is_hit){
		hh_enemy_death_check(boss);
	}
}
