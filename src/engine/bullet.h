#pragma once
#include "player_controller.h"
#include "engine/sprite_controller.h"
#include "types.h"
#include "input.h"

#define hh_ppu_bullet_fg_offset_idx 10
hh_entity* hh_init_bullets(int size);

static void hh_shoot_bullet(vec2 playerPos, vec_cor cam_pos, hh_entity*);

static void hh_update_bullet(hh_entity* , vec_cor );

static void hh_draw_bullet(hh_entity, int);

void hh_multiple_bullets(vec2 player, vec_cor cam_pos, hh_entity* bullets);
