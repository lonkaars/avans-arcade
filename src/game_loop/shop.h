#pragma once

#include "input.h"
#include "engine/draw_screen.h"
#include "engine/level_const.h"


#include <stdint.h>
#include <stdbool.h>

typedef enum {
	hh_e_shop_show,
	hh_e_shop_main,
	hh_e_shop_end,
} hh_e_shop_states;

/** @brief amount of upgrade types */
#define HH_SHOP_UPG_COUNT 5
/** @brief count of visible upgrades in shop */
#define HH_SHOP_UPG_DISPLAY 4
/** @brief all possible upgrades */
typedef enum {
	hh_e_upg_jump,
	hh_e_upg_speed,
	hh_e_upg_damage,
	hh_e_upg_heal,
	hh_e_upg_max_health,
} hh_e_upgrades;

/** @brief init */
void hh_shop_init(hh_e_upgrades* in);
/** @brief deals with displayed entity rendering */
void hh_shop_display(uint8_t selected, hh_e_upgrades* upgrades);
/** @brief moves 'cursor' through selection field */
void hh_shift_selected(uint8_t* pos, bool dir, uint8_t min, uint8_t max);

void hh_shop(hh_e_game_state* ,hh_level_entity* , int);
