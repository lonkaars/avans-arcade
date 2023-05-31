#include <math.h>

#include "demo.h"
#include "input.h"
#include "entity.h"
#include "ppu/ppu.h"
#include "tilemap.h"
#include "ppu/internals.h"

#include "engine/maths.h"
#include "engine/camera.h"
#include "engine/entity.h"
#include "engine/draw_screen.h"
#include "engine/player_controller.h"
#include "engine/sprite_controller.h"
#include "engine/level_const.h"

#include "game_loop/starting_screen.h"
#include "game_loop/high_score.h"
#include "game_loop/gameplay.h"
#include "game_loop/game_over.h"
#include "game_loop/shop.h"

#include <stdbool.h>

hh_g_all_levels hh_game;

uint16_t g_hh_pos_x = 1000; // 0b0000 0001 0011 0110
uint16_t g_hh_pos_y;
uint8_t g_hh_left  = 0;
uint8_t g_hh_right = 0;
uint8_t g_hh_up	  = 0;
uint8_t g_hh_down  = 0;
uint8_t g_hh_pos_x_bit[2];
uint8_t g_hh_pos_y_bit[2];
uint8_t g_hh_data_send[3];
int g_hh_tile_size = 8;
int g_hh_tile_y;

typedef struct {
	vec2 pos;
	uint8_t idx;
}hh_s_tiles;

int points = 0;
int* ptr_points = &points;

hh_e_game_state hh_game_states;
 hh_entity hh_g_player, hh_g_player_new;
 hh_e_upgrades upg;
void hh_demo_setup() {
	for (unsigned int i = 0; i < HH_PPU_VRAM_TMM_SPRITE_SIZE * HH_TM_SIZE; i++) {
		hh_ppu_addr_t ppu_addr = HH_PPU_VRAM_TMM_OFFSET + i;
		hh_ppu_data_t ppu_data = HH_PPU_TILEMAP_RAW[i];
		hh_ppu_vram_dwrite((uint8_t[4]) {
			(ppu_addr >> 8) & 0xff,
			(ppu_addr >> 0) & 0xff,
			(ppu_data >> 8) & 0xff,
			(ppu_data >> 0) & 0xff,
		}, 4);
	}
	hh_setup_palettes();
	hh_game = hh_init_game_levels();
}

void hh_demo_loop(unsigned long frame) {
	static uint64_t hh_rng_seed = 0;
	hh_rng_seed++;

	
	switch (hh_game_states)
	{
	case hh_e_state_starting_screen:
		bool ret = hh_show_starting_screen();
		if(ret){
			hh_game_states = hh_e_state_shop;
		}
		points = 0;	
		break;
	case hh_e_state_shop:
		hh_clear_screen();
		hh_clear_sprite();
		// upg = hh_shop(&hh_game_states, &hh_game, hh_rng_seed, points);
		break;
	// case hh_e_state_gameplay:
	// 	hh_gameplay(&hh_game, &hh_game_states, upg, &ptr_points);

	// 	break;
	// case hh_e_state_game_over:
	// 	hh_game_over(&hh_game_states);
	// 	break;
	// case hh_e_state_high_score:
	// 	// todo:
	// 	// fucntion: show all previously scored points
	// 	// function: button pressed goto starting screen
	// 	hh_high_score(&hh_game_states);
	// 	// hh_game_states = hh_e_state_starting_screen;
	// 	break;
	default:
			hh_game_states = hh_e_state_starting_screen;
		break;
	}
}

// void send_data(uint8_t address, uint16_t data) {
// 	uint8_t bit_data[3];
// 	bit_data[2] = data & 0xff;
// 	bit_data[1] = (data >> 8);
// 	bit_data[0] = address; // first byte is address
//
// 	hal_gpio_write_pin(gpioa, gpio_pin_9, gpio_pin_reset);
// 	hal_spi_transmit(&hspi1, bit_data, 3, 100); //2*8 bit data
// 	hal_gpio_write_pin(gpioa, gpio_pin_9, gpio_pin_set);
// }







