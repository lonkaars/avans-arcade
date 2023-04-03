#include "shop.h"
#include "engine/maths.h"
#include "ppu/ppu.h"

void hh_shop(hh_e_game_state* hh_game_state){
	static hh_e_shop_states hh_e_shop = hh_e_shop_show;
	static hh_e_upgrades upgrades[HH_SHOP_UPG_DISPLAY] = {0};
	static uint8_t selected = 0;

	switch (hh_e_shop)
	{
	case hh_e_shop_show:
		hh_clear_screen();
		hh_clear_sprite();
		// TODO: make function to show shop
		//hh_setup_shop();
		hh_shop_init(&upgrades);
		selected = HH_SHOP_UPG_DISPLAY/2;
		// hh_shop_display(selected, &upgrades);
		hh_e_shop = hh_e_shop_main;
		break;
	case hh_e_shop_main:
		if(g_hh_controller_p1.dpad_left || g_hh_controller_p1.dpad_right){
			hh_shift_selected(&selected,(g_hh_controller_p1.dpad_right?1:0),0,HH_SHOP_UPG_DISPLAY);
			// hh_shop_display(selected, &upgrades);
		}
		// if(g_hh_controller_p1.button_primary){
		// 	//apply selected upgrade
		// 	hh_e_shop = hh_e_shop_end;
		// }
		if(g_hh_controller_p1.button_secondary){//Quick exit
			hh_e_shop = hh_e_shop_end;
		}
		break;
	case hh_e_shop_end:
		hh_e_shop = hh_e_shop_show;
		*hh_game_state = hh_e_state_gameplay;
		break;
	default:
		hh_e_shop = hh_e_shop_show;
		break;
	}
}

void hh_shop_init(hh_e_upgrades* in) {
	for (int i = 0; i < HH_SHOP_UPG_DISPLAY; i++) {
		in[i] = i%HH_SHOP_UPG_COUNT;
	}
}

void hh_shop_display(uint8_t selected, hh_e_upgrades* upgrades) {
	const vec_cor start = {48,16};
	const uint8_t up = 8,
					space = 24+8;

	for (int i = 0; i < HH_SHOP_UPG_DISPLAY; i++) {
		hh_ppu_update_foreground(i+16,
		(hh_s_ppu_loc_fam_entry){
			.horizontal_flip = false, .vertical_flip = false,
			.position_x = i*space+start.x, .position_y = start.y + (i==selected?up:0),
			.palette_index = 7,
			.tilemap_index = 8
		});
	}
}

void hh_shift_selected(uint8_t* pos, bool dir, uint8_t min, uint8_t max) {
	if (dir) {
		pos = CLAMP(++pos,min,max);
	} else {
		if (pos > 0) {
			pos--;
		}
	}
}

