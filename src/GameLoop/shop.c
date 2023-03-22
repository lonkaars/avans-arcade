#include "shop.h"


bool hh_show_Shop(){
	static hh_e_ShopStates hh_e_Shop = hh_e_STATE_SHOW;
	
	switch (hh_e_Shop)
	{
	case hh_e_STATE_SHOW:
		//hh_clear_screen();

		//hh_setup_shop();
		hh_e_Shop = hh_e_STATE_Input;
		return false;
		break;
	case hh_e_STATE_Input:
		if(g_hh_controller_p1.button_primary){
			hh_e_Shop = hh_e_STATE_END;
		}
		break;
	case hh_e_STATE_END:
		hh_e_Shop = hh_e_STATE_SHOW;
		return true;
		break;
	default:
		hh_e_Shop = hh_e_STATE_SHOW;
		break;
	}
	return false;
}
