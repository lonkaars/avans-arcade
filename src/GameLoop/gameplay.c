#include "gameplay.h"



void hh_gameplay(hh_g_all_levels game, hh_e_GameState* hh_game_state){
	static hh_e_gameplay gameplay = hh_e_setup_screen;

	switch (gameplay)
	{
	case hh_e_setup_screen:
		hh_setup_screen(game.level[game.currentLevel]);
		gameplay = hh_e_play_level;
		break;
	case hh_e_play_level:
		// TODO: here come all the different functions for the gameplay
		hh_player_actions();



		if(game.level[game.currentLevel].hh_level_completed){
			gameplay = hh_e_level_complete;
		}
		break;
	case hh_e_level_complete:
		if(game.currentLevel < 3){
			game.currentLevel++;
			gameplay = hh_e_setup_screen;
		}
		else {
         gameplay = hh_e_game_over;
      }
		break;
	case hh_e_game_over:
		// TODO make reset levels
		hh_reset_levels();
		gameplay = hh_e_setup_screen;
		*hh_game_state = 	hh_e_STATE_GameOver;
		break;
	default:
		break;
	}
	
}
void hh_reset_levels(){}
