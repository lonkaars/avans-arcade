#include "game_over.h"
#include "ui.h"

void hh_show_game_over_screen(){
	int index=0;
	char* Game = "Game";
	char* Over = "Over";
	hh_ui_show_char(&index,Game,(vec2){.x = 88, .y = 50});
	hh_ui_show_char(&index,Over,(vec2){.x = 168, .y = 50});
	char* Press = "Press";
	char* buttons = "buttons";
	char* A = "A";
	char* B = "B";
	char* and = "and";
	
	hh_ui_show_char(&index,Press,(vec2){.x = HH_PPU_SPRITE_WIDTH*4, .y = HH_PPU_SPRITE_HEIGHT*5});
	hh_ui_show_char(&index,buttons,(vec2){.x = HH_PPU_SPRITE_WIDTH*10, .y = HH_PPU_SPRITE_HEIGHT*5});
	hh_ui_show_char(&index,A,(vec2){.x = HH_PPU_SPRITE_WIDTH*6, .y = HH_PPU_SPRITE_HEIGHT*7});
	hh_ui_show_char(&index,and,(vec2){.x = HH_PPU_SPRITE_WIDTH*8, .y = HH_PPU_SPRITE_HEIGHT*7});
	hh_ui_show_char(&index,B,(vec2){.x = HH_PPU_SPRITE_WIDTH*12, .y = HH_PPU_SPRITE_HEIGHT*7});
}

void hh_game_over(hh_e_game_state* hh_game_state){
	static hh_e_game_over_h hh_e_states_game_over = hh_e_game_over_show;

	switch (hh_e_states_game_over)
	{
	case hh_e_game_over_show:
		hh_clear_screen();
		hh_clear_sprite();
		hh_show_game_over_screen();
		hh_e_states_game_over = hh_e_game_over_input;
		break;
	case hh_e_game_over_input:
		if(g_hh_controller_p1.button_primary && g_hh_controller_p1.button_secondary){
			hh_e_states_game_over = hh_e_game_over_end;
		}
		break;
	case hh_e_game_over_end:
		hh_e_states_game_over = hh_e_game_over_show;
		*hh_game_state = hh_e_state_high_score;
		break;
	default:
		hh_e_states_game_over = hh_e_game_over_show;
		break;
	}
}


