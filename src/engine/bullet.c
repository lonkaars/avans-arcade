#include "bullet.h"
int bullets_size=0;
static int current_bullet=0;


hh_entity* hh_init_bullets(int size) {
    bullets_size = size;
    hh_entity* all_bullets = malloc(size * sizeof(hh_entity));
    hh_entity bullet = {
        .speed = 1,
        .is_grounded = true,
        .is_hit = false,
        .radius = 4,
        .pos = (vec2){-16,-16},
        .vel = (vec2){0,0},
		  .size = (vec2) { 13,16},
        .render = {
            .frame0 = 84,
            .palette = 3,
            .fam = (hh_s_ppu_loc_fam_entry){
                .horizontal_flip = false,
                .vertical_flip = false,
                .palette_index = 7,
                .tilemap_index = 84,
            }
        }
    };
    for (int i = 0; i < size; i++) {
        all_bullets[i] = bullet;
    }
    return all_bullets;
}
bool rising_edge(bool signal, bool* prev) {
    bool edge = false;
    if (signal && !(*prev)) {
        edge = true;
    }
    *prev = signal;
    return edge;
}
bool prev_signal = false;
void hh_shoot_bullet(vec2 player, vec_cor cam_pos, hh_entity* bullet){
	vec2 temp;
	if(rising_edge(g_hh_controller_p1.button_secondary,&prev_signal) && bullet->is_grounded){
			bullet->is_grounded=false;
			bullet->pos = player;
			current_bullet = (current_bullet + 1) % bullets_size;
	}

}

void hh_update_bullet(hh_entity* bullet, vec_cor cam_pos){
	if(hh_background_collision_bulllet(*bullet)){
		bullet->is_grounded=true;
		bullet->pos.x=-16;
		bullet->pos.y=-16;
		// printf("x %d y %d\n",(bullet->pos.x-cam_pos.x),(bullet->pos.y-cam_pos.y));
	}
	else{
		bullet->pos.x += bullet->speed;	
	}
	

	// update bullet sprite on ppu
	bullet->render.fam.position_x = (bullet->pos.x - cam_pos.x);
	bullet->render.fam.position_y = (bullet->pos.y - cam_pos.y);

}
void hh_draw_bullet(hh_entity bullet,int number){
	// hh_s_ppu_loc_fam_entry temp = bullet.render.fam;
	// hh_ppu_update_foreground(hh_ppu_bullet_fg_offset_idx + number,temp);
	
}
void hh_multiple_bullets(vec2 player, vec_cor cam_pos, hh_entity* bullets){

	hh_shoot_bullet(player,cam_pos, &bullets[current_bullet]);
	for(int i=0; i < bullets_size;i++){
		if(!bullets[i].is_grounded){
			hh_update_bullet(&bullets[i] , cam_pos);
			hh_update_sprite(hh_ppu_bullet_fg_offset_idx + i, bullets[i], cam_pos);
		}

	}
}
