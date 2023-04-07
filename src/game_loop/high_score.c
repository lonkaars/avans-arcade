#include "high_score.h"

#include <string.h>

#define HH_KB_MAX_X 9
#define HH_KB_MAX_Y 4

void hh_high_score(hh_e_game_state* hh_game_state) {
	static hh_e_score_states hh_e_score = hh_e_score_show;
	// static uint8_t selected[HH_KB_MAX_X][HH_KB_MAX_Y] = {0};
	static char* name = "";
	static hh_s_controller_exp con1;
	static uint8_t selected_x = 0,
						selected_y = 0;

	hh_update_controller_exp(&con1);

	switch (hh_e_score)
	{
	case hh_e_score_show:
		hh_clear_screen();
		hh_clear_sprite();
		name = "";
		if (!hh_controller_exp_pressed(con1)){
			hh_e_score = hh_e_score_main;
		}
		break;
	case hh_e_score_main:
		if (con1.rising.dpad_left || con1.rising.dpad_right){
			hh_shift_selected(&selected_x,con1.pressed.dpad_right,0,HH_KB_MAX_X-1);
		}
		if (con1.rising.dpad_up || con1.rising.dpad_down){
			hh_shift_selected(&selected_y,con1.pressed.dpad_down,0,HH_KB_MAX_Y-1);
		}
		if (con1.rising.button_primary) {
			int str_idx = selected_x + selected_y*HH_KB_MAX_X;
			int len = strlen(name);
			if (str_idx < 10) {
				name[len] = str_idx + '0';
			} else {
				name[len] = str_idx + 'a';
			}
			name[len+1] = '\0';
		}
		int idx = 37;
		hh_ui_show_char(&idx,name,(vec2){16,16});
		hh_kb_display(selected_x + selected_y*HH_KB_MAX_X);

		
		break;
	case hh_e_score_end:
		hh_e_score = hh_e_score_show;
		*hh_game_state = hh_e_state_gameplay;
		break;	
	default:
		hh_e_score = hh_e_score_show;
		break;
	}

}

bool hh_controller_exp_pressed(hh_s_controller_exp con1) {
	return (con1.pressed.button_primary || con1.pressed.button_secondary || con1.pressed.dpad_down || con1.pressed.dpad_up || con1.pressed.dpad_left || con1.pressed.dpad_right);
}

void hh_update_controller_exp(hh_s_controller_exp* in) {

	in->falling.button_primary = (in->pressed.button_primary && !g_hh_controller_p1.button_primary);
	in->falling.button_secondary = (in->pressed.button_secondary && !g_hh_controller_p1.button_secondary);
	in->falling.dpad_right = (in->pressed.dpad_right && !g_hh_controller_p1.dpad_right);
	in->falling.dpad_left = (in->pressed.dpad_left && !g_hh_controller_p1.dpad_left);
	in->falling.dpad_down = (in->pressed.dpad_down && !g_hh_controller_p1.dpad_down);
	in->falling.dpad_up = (in->pressed.dpad_up && !g_hh_controller_p1.dpad_up);

	in->rising.button_primary = (!in->pressed.button_primary && g_hh_controller_p1.button_primary);
	in->rising.button_secondary = (!in->pressed.button_secondary && g_hh_controller_p1.button_secondary);
	in->rising.dpad_right = (!in->pressed.dpad_right && g_hh_controller_p1.dpad_right);
	in->rising.dpad_left = (!in->pressed.dpad_left && g_hh_controller_p1.dpad_left);
	in->rising.dpad_down = (!in->pressed.dpad_down && g_hh_controller_p1.dpad_down);
	in->rising.dpad_up = (!in->pressed.dpad_up && g_hh_controller_p1.dpad_up);
	
	in->pressed = g_hh_controller_p1;
}

void hh_kb_display(uint8_t sel) {
	const vec_cor start = {(320-16*HH_KB_MAX_X)/2,120};
	const uint8_t space = 16;
	int counter = 0;
	for (int y = 0; y < HH_KB_MAX_Y; y++) {
		for (int x = 0; x < HH_KB_MAX_X; x++) {
			hh_ppu_update_foreground(counter++,
			(hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false, .vertical_flip = false,
				.position_x = x*space+start.x,
				.position_y = y*space+start.y,
				.palette_index = (counter == sel ? 6 : 4),
				.tilemap_index = HH_TM_FONT_OFFSET + counter
			});
		}
		
	}
	
}

