#include "startingScreen.h"
#include "input.h"
#include "engine/title_screen.h"
#include "engine/draw_screen.h"
// #include "engine/player_controller.h"

bool hh_show_startingScreen(){
	static hh_e_screenStates hh_e_startingScreen = hh_e_STATE_SHOW;
	
	switch (hh_e_startingScreen)
	{
	case hh_e_STATE_SHOW:
		hh_clear_screen();
		hh_init_title_screen();
		hh_e_startingScreen = hh_e_STATE_Input;
		return false;
		break;
	case hh_e_STATE_Input:
		if(g_hh_controller_p1.button_primary){
			hh_e_startingScreen = hh_e_STATE_END;
		}
		break;
	case hh_e_STATE_END:
		hh_e_startingScreen = hh_e_STATE_SHOW;
		return true;
		break;
	default:
		hh_e_startingScreen = hh_e_STATE_SHOW;
		break;
	}
	return false;
}
