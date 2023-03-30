#include "shop.h"


void hh_Shop(hh_e_GameState* hh_game_state){
	static hh_e_ShopStates hh_e_Shop = hh_e_shop_SHOW;
	
	switch (hh_e_Shop)
	{
	case hh_e_shop_SHOW:
		hh_clear_screen();
		hh_clear_sprite();
		// TODO: make function to show shop
		//hh_setup_shop();
		hh_e_Shop = hh_e_shop_Input;
		break;
	case hh_e_shop_Input:
		// TODO: make it so that you can choose between shop
		if(g_hh_controller_p1.button_primary){
			hh_e_Shop = hh_e_shop_END;
		}
		break;
	case hh_e_shop_END:
		hh_e_Shop = hh_e_shop_SHOW;
		*hh_game_state = hh_e_STATE_Gameplay;
		break;
	default:
		hh_e_Shop = hh_e_shop_SHOW;
		break;
	}
}
