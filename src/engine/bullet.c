#include "bullet.h"
int bullets_size=0;
static int current_bullet=0;

hh_entity* all_bullets=NULL;
hh_entity* hh_init_bullets(int size) {
    if (all_bullets != NULL) {
        free(all_bullets);
    }
    bullets_size = size;
    all_bullets = malloc(size * sizeof(hh_entity));
    hh_entity bullet = {
        .speed = 5,
        .is_grounded = true,
        .is_hit = false,
        .radius = 4,
        .pos = (vec2){-16,-16},
        .vel = (vec2){0,0},
        .size = (vec2) { 13,16},
        .render = {
            .frame0 = HH_TM_BULLET_OFFSET,
            .palette = 3,
            .fam = (hh_s_ppu_loc_fam_entry){
                .horizontal_flip = false,
                .vertical_flip = false,
                .palette_index = 7,
                .tilemap_index = HH_TM_BULLET_OFFSET,
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

void hh_handle_bullet_direction(hh_entity* bullet, hh_entity player){
	bullet->vel = (vec2){0,0};
	bullet->render.fam.horizontal_flip = false;
	bullet->render.fam.vertical_flip = false;
	bullet->render.fam.tilemap_index = HH_TM_BULLET_OFFSET;
	if (g_hh_controller_p1.dpad_left) {
		if (g_hh_controller_p1.dpad_up) {
			bullet->vel.x = -1;
			bullet->vel.y = -1;
			bullet->render.fam.tilemap_index = HH_TM_BULLET_OFFSET+2;
			bullet->render.fam.vertical_flip = true;
			bullet->render.fam.horizontal_flip = true;
		} else if (g_hh_controller_p1.dpad_down) {
			bullet->vel.x = -1;
			bullet->vel.y = 1;
			bullet->render.fam.tilemap_index = HH_TM_BULLET_OFFSET+2;
			bullet->render.fam.vertical_flip = false;
			bullet->render.fam.horizontal_flip = true;
		} else {
			bullet->render.fam.tilemap_index = HH_TM_BULLET_OFFSET;
			bullet->render.fam.vertical_flip = false;
			bullet->render.fam.horizontal_flip = true;
			bullet->vel.x = -1;
		}
	}else if (g_hh_controller_p1.dpad_right) {
		if (g_hh_controller_p1.dpad_up) {
			bullet->vel.x = 1;
			bullet->vel.y = -1;
			bullet->render.fam.tilemap_index = HH_TM_BULLET_OFFSET+2;
			bullet->render.fam.vertical_flip = true;
			bullet->render.fam.horizontal_flip = false;
		} else if (g_hh_controller_p1.dpad_down) {
			bullet->vel.x = 1;
			bullet->vel.y = 1;
			bullet->render.fam.tilemap_index = HH_TM_BULLET_OFFSET+2;
			bullet->render.fam.vertical_flip = false;
			bullet->render.fam.horizontal_flip = false;
		} else {
			bullet->vel.x = 1;
			bullet->render.fam.horizontal_flip = false;
		}
		// shooting up
	} else if (g_hh_controller_p1.dpad_up){
		bullet->render.fam.tilemap_index = HH_TM_BULLET_OFFSET+1;
		bullet->render.fam.vertical_flip = true;
		bullet->render.fam.horizontal_flip = false;
		bullet->vel.y = -1;
		// shooting down
	} else if (g_hh_controller_p1.dpad_down){
		bullet->render.fam.tilemap_index = HH_TM_BULLET_OFFSET+1;
		bullet->render.fam.vertical_flip = false;
		bullet->render.fam.horizontal_flip = false;
		bullet->vel.y = 1;
	}
	else{
		bullet->vel.x = (player.render.fam.horizontal_flip == true) ? -1 : 1 ;
		bullet->render.fam.horizontal_flip = player.render.fam.horizontal_flip;
	}
}

void hh_shoot_bullet(hh_entity player, hh_entity* bullet){
	vec2 temp;
	if(rising_edge(g_hh_controller_p1.button_secondary,&prev_signal) && bullet->is_grounded){
			bullet->is_grounded=false;
			// start position bullet
			bullet->pos.x=player.pos.x;
			bullet->pos.y=player.pos.y+10;
			// TODO: fix the travel distance for the bullet
			bullet->start.x=player.pos.x;
			bullet->start.y=player.pos.y+10;
			current_bullet = (current_bullet + 1) % bullets_size;
			hh_handle_bullet_direction(bullet,player);
	}

}

void hh_update_bullet(hh_entity* bullet){
	
	if(hh_background_collision_bulllet(*bullet)){
		hh_bullet_death(bullet);
	}
	else{
		// TODO: change idea for maximum pixel movement from bullet
		bullet->pos.x += bullet->vel.x * bullet->speed;    
		bullet->pos.y += bullet->vel.y * bullet->speed;
		int dx = bullet->pos.x - bullet->start.x; 
		int dy = bullet->pos.y - bullet->start.y; 
		int distance = abs(dx) + abs(dy);
		if (distance >= hh_bullet_travel_distance) { 
			hh_bullet_death(bullet); 
		}
	}
}

void hh_multiple_bullets(hh_entity player, hh_entity* bullets){

	hh_shoot_bullet(player, &bullets[current_bullet]);
	for(int i=0; i < bullets_size;i++){
		if(!bullets[i].is_grounded){
			hh_update_bullet(&bullets[i]);
		}
	}
}

void hh_bullet_death(hh_entity* bullet){
	bullet->is_grounded=true;
	bullet->pos.x= -16;
	bullet->pos.y= -16;
}
