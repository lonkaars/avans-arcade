#include "shop.h"

hh_e_upgrades hh_shop(hh_e_game_state* hh_game_state, hh_g_all_levels* levels, int rng_seed){
	static hh_e_shop_states hh_e_shop = hh_e_shop_show;
	static hh_e_upgrades upgrades[HH_SHOP_UPG_DISPLAY] = {0};
	static uint8_t selected = 0;
	static bool pressed_LR = false;

	switch (hh_e_shop)
	{
	case hh_e_shop_show:
		// hh_clear_screen();
		// hh_clear_sprite();
		// TODO: make function to show shop
		//hh_setup_shop();
		hh_setup_screen(levels->shop);
		hh_shop_upg_init(&upgrades, rng_seed);
		selected = HH_SHOP_UPG_DISPLAY/2;
		hh_shop_display(selected, &upgrades);
		int idx = 16;
		// hh_ui_show_char(&idx,"abyz09",(vec2){32,32});
		char* c[3];
		itoa(c,levels->current_level);
		hh_ui_show_char(&idx,c,(vec2){304-16-8,0});

		hh_e_shop = hh_e_shop_main;
		break;
	case hh_e_shop_main:
		if(g_hh_controller_p1.dpad_left || g_hh_controller_p1.dpad_right){
			if (!pressed_LR) {
				hh_shift_selected(&selected,(g_hh_controller_p1.dpad_right?1:0),0,HH_SHOP_UPG_DISPLAY-1);
				hh_shop_display(selected, &upgrades);
			}
			pressed_LR = true;
		} else {
			pressed_LR = false;
		}
		if(g_hh_controller_p1.button_primary){
			//apply selected upgrade
			// hh_e_shop = hh_e_shop_end;
			return upgrades[selected];
		}
		if(g_hh_controller_p1.button_secondary){//Quick exit
			hh_e_shop = hh_e_shop_end;
		}
		break;
	case hh_e_shop_end: // delay?
		hh_e_shop = hh_e_shop_show;
		*hh_game_state = hh_e_state_gameplay;
		break;
	default:
		hh_e_shop = hh_e_shop_show;
		break;
	}
	return hh_e_NULL;
}

uint8_t hh_shop_translate_upgrades(hh_e_upgrades upg) {
	return HH_TM_UPGRADES_OFFSET+upg;
}

void hh_shop_upg_init(hh_e_upgrades* in, int seed) {
	srand(seed);
	for (int i = 0; i < HH_SHOP_UPG_DISPLAY; i++) {
		hh_e_upgrades rng = rand()%(HH_SHOP_UPG_COUNT);
		while (rng == in[i-1]){
			rng = rand()%(HH_SHOP_UPG_COUNT);
		}
		in[i] = rng;
	}
}

void hh_shop_display(uint8_t selected, hh_e_upgrades* upgrades) {
	const vec_cor start = {104,144+16};
	const uint8_t up = 8,
					space = 24+8;

	for (int i = 0; i < HH_SHOP_UPG_DISPLAY; i++) {
		hh_ppu_update_foreground(i,
		(hh_s_ppu_loc_fam_entry){
			.horizontal_flip = false, .vertical_flip = false,
			.position_x = i*space+start.x, .position_y = start.y + (i==selected?-up:0),
			.palette_index = HH_PAL_IDX_UPGRADE,
			.tilemap_index = hh_shop_translate_upgrades(upgrades[i])
		});
	}
}


