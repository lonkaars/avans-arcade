#include "starting_screen.h""
#include "input.h"
#include "engine/title_screen.h"
#include "engine/draw_screen.h"
// #include "engine/player_controller.h"

bool hh_show_starting_screen(){
	static hh_e_screen_states hh_e_starting_screen = hh_e_state_show;
	
	switch (hh_e_starting_screen)
	{
	case hh_e_state_show:
		hh_clear_screen();
		hh_init_title_screen();
		hh_e_starting_screen = hh_e_state_input;
		return false;
		break;
	case hh_e_state_input:
		if(g_hh_controller_p1.button_primary){
			hh_e_starting_screen = hh_e_state_end;
		}
		break;
	case hh_e_state_end:
		hh_e_starting_screen = hh_e_state_show;
		return true;
		break;
	default:
		hh_e_starting_screen = hh_e_state_show;
		break;
	}
	return false;
}
