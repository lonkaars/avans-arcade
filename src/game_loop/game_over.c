#include "game_over.h"


void hh_game_over(hh_e_game_state* hh_game_state){
	static hh_e_game_over hh_e_states_game_over = hh_e_game_over_show;

	switch (hh_e_states_game_over)
	{
	case hh_e_game_over_show:
		hh_clear_screen();
		hh_clear_sprite();
		// todo: make function to show game over
		hh_e_states_game_over = hh_e_game_over_input;
		break;
	case hh_e_game_over_input:
		if(g_hh_controller_p1.button_primary){
			hh_e_states_game_over = hh_e_game_over_end;
		}
		break;
	case hh_e_game_over_end:
		hh_e_states_game_over = hh_e_game_over_show;
		*hh_game_state = hh_e_state_game_over;
		break;
	default:
		hh_e_states_game_over = hh_e_game_over_show;
		break;
	}
}
