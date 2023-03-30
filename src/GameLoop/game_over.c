#include "game_over.h"


void hh_game_over(hh_e_GameState* hh_game_state){
	static hh_e_game_over hh_e_states_game_over = hh_e_game_over_SHOW;

	switch (hh_e_states_game_over)
	{
	case hh_e_game_over_SHOW:
		hh_clear_screen();
		hh_clear_sprite();
		// TODO: make function to show game over
		hh_e_states_game_over = hh_e_game_over_Input;
		break;
	case hh_e_game_over_Input:
		if(g_hh_controller_p1.button_primary){
			hh_e_states_game_over = hh_e_game_over_END;
		}
		break;
	case hh_e_game_over_END:
		hh_e_states_game_over = hh_e_game_over_SHOW;
		*hh_game_state = hh_e_STATE_GameOver;
		break;
	default:
		hh_e_states_game_over = hh_e_game_over_SHOW;
		break;
	}
}
