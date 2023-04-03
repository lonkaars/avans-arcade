#include "gameplay.h"

// player struct


void hh_gameplay(hh_g_all_levels game, hh_e_game_state* hh_game_state){
	static hh_e_gameplay gameplay = hh_e_setup_screen;

	switch (gameplay)
	{
	case hh_e_setup_screen:
		hh_setup_screen(game.level[game.current_level]);
		gameplay = hh_e_play_level;
		break;
	case hh_e_play_level:
		// todo: here come all the different functions for the gameplay
		hh_player_actions();





		if(game.level[game.current_level].hh_level_completed){
			gameplay = hh_e_level_complete;
		}
		break;
	case hh_e_level_complete:
		if(game.current_level < 3){
			game.current_level++;
			gameplay = hh_e_setup_screen;
		}
		else {
         gameplay = hh_e_game_over;
      }
		break;
	case hh_e_game_over:
		// todo make reset levels
		hh_reset_levels();
		gameplay = hh_e_setup_screen;
		*hh_game_state = 	hh_e_state_game_over;
		break;
	default:
		break;
	}
	
}
void hh_reset_levels(){}
