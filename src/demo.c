#include <math.h>

#include "demo.h"
#include "input.h"
#include "entity.h"
#include "ppu/ppu.h"

#include "engine/maths.h"
#include "engine/camera.h"
#include "engine/entity.h"
#include "engine/draw_screen.h"
#include "engine/player_controller.h"
#include "engine/sprite_controller.h"


#include "GameLoop/startingScreen.h"
#include "GameLoop/shop.h"
// states
typedef enum {
	hh_e_STATE_startingScreen,
	hh_e_STATE_Shop,
	hh_e_STATE_Gameplay,
	hh_e_STATE_GameOver,
	hh_e_STATE_HighScore
} hh_e_GameState;
hh_e_GameState hh_gameStates;

hh_s_entity_player g_hh_player_1 = {
	.pos_x		 = 31000, // 0b0000 0001 0011 0110
	.pos_y		 = 21000,
	.radius		 = 8,
	.speed		 = 100,
	.direction_x = 1,
	.rotation	 = 8,
	.in_air		 = false,
};

void hh_player_movement();

uint16_t g_hh_pos_x; // 0b0000 0001 0011 0110
uint16_t g_hh_pos_y;
uint8_t g_hh_left  = 0;
uint8_t g_hh_right = 0;
uint8_t g_hh_up	  = 0;
uint8_t g_hh_down  = 0;
uint8_t g_hh_pos_x_bit[2];
uint8_t g_hh_pos_y_bit[2];
uint8_t g_hh_data_send[3];
int g_hh_tile_x;
int g_hh_tile_y;

typedef struct {
	vec2 pos;
	uint8_t idx;
}hh_s_tiles;


hh_entity hh_g_player, hh_g_player_new;
void hh_demo_setup() {	
	hh_setup_palettes();

	//hh_setup_screen();
}
void hh_demo_loop(unsigned long frame) {

	// hh_player_movement();
	switch (hh_gameStates)
	{
	case hh_e_STATE_startingScreen:

		if(hh_show_startingScreen()){
			hh_gameStates = hh_e_STATE_Shop;
		}	
		break;
	case hh_e_STATE_Shop:
		// TODO:

		if(hh_show_Shop()){
			hh_gameStates = hh_e_STATE_Gameplay;
		}
		// function: new level is chosen goto level
		break;
	case hh_e_STATE_Gameplay:
		static int run_test=0;
		if(!run_test){
			hh_setup_screen();
			run_test=1;

		}
		else{
			hh_player_actions();
		}
		// TODO:
		// function: if level complete goto shop
		// function: if player is dead goto game over
		break;
	case hh_e_STATE_GameOver:
		// TODO:
		// function: show game over screen
		// function: after time goto high score
		break;
	case hh_e_STATE_HighScore:
		// TODO:
		// fucntion: show all previously scored points
		// function: button pressed goto starting screen
		break;
	default:
			hh_gameStates = hh_e_STATE_startingScreen;
		break;
	}
	

}

// void sendData(uint8_t address, uint16_t data) {
// 	uint8_t bitData[3];
// 	bitData[2] = data & 0xff;
// 	bitData[1] = (data >> 8);
// 	bitData[0] = address; // first byte is address
//
// 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
// 	HAL_SPI_Transmit(&hspi1, bitData, 3, 100); //2*8 bit data
// 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
// }

void hh_player_movement() {
	int8_t directionX = (-1 * g_hh_controller_p1.dpad_left) + (1 * g_hh_controller_p1.dpad_right); // -1 = L || 1 == R
	int8_t directionY = (-1 * g_hh_controller_p1.dpad_up) + (1 * g_hh_controller_p1.dpad_down);	   // -1 = D || 1 == U

	uint8_t i, j;
	uint8_t rotation = 0; // 0-7

	// rotation calc
	for (i = -1; i < 2; i++) {
		for (j = -1; j < 2; j++) {
			if (directionX == i) {
				if (directionY == j) {
					if (i != 0 && j != 0) // dont update when player idle
					{
						g_hh_player_1.rotation = rotation;
					}
				}
			}
			rotation++;
		}
	}
	// direction calc
	if (directionX != 0) // update direction if player is not idle
	{
		g_hh_player_1.direction_x = directionX;
	}
	// collision map x-axis

	// tile calc including radius and direction for background coliision

	uint16_t tileColX;
	uint16_t tileColY = (g_hh_player_1.pos_y / 100) / 16;
	;

	// remaining space between grid and exact
	uint8_t modTileX;
	uint8_t modTileY;

	if (g_hh_player_1.in_air == false && directionX != 0) {
		if (directionX == 1) {
			tileColX = ((g_hh_player_1.pos_x / 100) + g_hh_player_1.radius) / 16;
			modTileX = (g_hh_player_1.pos_x + (100 * g_hh_player_1.radius)) % 1600;
		} else if (directionX == -1) {
			tileColX = ((g_hh_player_1.pos_x / 100) - g_hh_player_1.radius) / 16;
			modTileX = (g_hh_player_1.pos_x - (100 * g_hh_player_1.radius)) % 1600;
		}

		if (HH_DEMO_HITBOX_TILEMAP[tileColY][tileColX + directionX] != 1) {
			g_hh_player_1.pos_x = g_hh_player_1.pos_x + (directionX * g_hh_player_1.speed); // NEW x set
		}

		else if (HH_DEMO_HITBOX_TILEMAP[tileColY][tileColX + directionX] == 1) {
			if (modTileX < g_hh_player_1.speed) {
				g_hh_player_1.pos_x = g_hh_player_1.pos_x + (directionX * modTileX); // NEW x set
			} else {
				g_hh_player_1.pos_x = g_hh_player_1.pos_x + (directionX * g_hh_player_1.speed); // NEW x set
			}
		}

	} else // if in air different all borders have to be checked
	{
	}


	if(directionY != 0)
	{
	//	g_hh_player_1.pos_y = g_hh_player_1.pos_y + (directionY * g_hh_player_1.speed * 2); // NEW x set
	}
	// collision map floor (y-axis) (falling)
	//  if falling no jump press (implement)
	/*
	tileColY = (( g_hh_player_1.pos_y / 100) + g_hh_player_1.radius) / 16; //bottom of player box
	modTileY = 1;
	if(HH_DEMO_HITBOX_TILEMAP[tileColY+1][tileColX] != 1) //rework after jumping
	{
		g_hh_player_1.pos_y = g_hh_player_1.pos_y + 5 ;// NEW y set //makew var gravity
		//playerStat = falling; //for later use of graphics/sound
	}
	*/
	// else if(HH_DEMO_HITBOX_TILEMAP[])
}
