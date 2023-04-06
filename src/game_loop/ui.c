#include "game_loop/ui.h"

void hh_ui_show_hp(int* idx, hh_entity* player, uint8_t max_hp, vec_cor cam_pos) {
	for (int i = 0; i < max_hp; i++) {
		int tilemap_idx = HH_TM_HUD_HEART_OFFSET + (player->hp > i?0:1),
			palette_idx = HH_PAL_IDX_HUD;
		hh_show_quad(idx, &(hh_s_rendering){
			.frame0 = tilemap_idx,
			.palette = palette_idx,
			.fam = (hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false, .vertical_flip = false,
				.palette_index = palette_idx,
				.tilemap_index = tilemap_idx,
				.position_x = 8 + i*15, .position_y = 8
			}
		});
	}
}
void hh_ui_show_char(int* idx, char* str, vec2 pos) {
	int i = 0;
	int tilemap_idx,
		palette_idx = 6;
	while (str[i] != '\0') {
		if(str[i] >= 'A' && str[i] <= 'Z'){
			tilemap_idx = HH_TM_FONT_OFFSET + str[i]-'A'+10;
		} else if (str[i] >= 'a' && str[i] <= 'z') {
			tilemap_idx = HH_TM_FONT_OFFSET + str[i]-'a'+10;
		} else if (str[i] >= '0' && str[i] <= '9'){
			tilemap_idx = HH_TM_FONT_OFFSET + str[i]-48;
		} else {
			return;
		}
		hh_show_quad(idx, &(hh_s_rendering){
			.frame0 = tilemap_idx,
			.palette = palette_idx,
			.fam = (hh_s_ppu_loc_fam_entry){
				.horizontal_flip = false, .vertical_flip = false,
				.palette_index = palette_idx,
				.tilemap_index = tilemap_idx,
				.position_x = pos.x + i*15, .position_y = pos.y
			}
		});
		i++;
	}
		
}

void itoa(char *c, int i) {
	c[0] = i + '0';
	c[1] = '\0';
}

