#include "shop.h"


void hh_shop(hh_e_game_state* hh_game_state){
	static hh_e_shop_states hh_e_shop = hh_e_shop_show;
	
	switch (hh_e_shop)
	{
	case hh_e_shop_show:
		hh_clear_screen();
		hh_clear_sprite();
		// todo: make function to show shop
		//hh_setup_shop();
		hh_e_shop = hh_e_shop_input;
		break;
	case hh_e_shop_input:
		// todo: make it so that you can choose between shop
		if(g_hh_controller_p1.button_primary){
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
