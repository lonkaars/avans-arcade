#include <math.h>

#include "demo.h"
#include "ppu/ppu.h"
#include "input.h"
#include "entity.h"

#define HH_DEMO_BALL_COUNT 1
hh_s_ppu_loc_fam_entry g_hh_demo_balls[HH_DEMO_BALL_COUNT];

hh_s_entity_player g_hh_player_1 = {
	.pos_x = 31000, //0b0000 0001 0011 0110
	.pos_y = 21000,
	.radius = 8,
	.speed = 1,
	.direction_x = 1,
	.rotation = 8,
	.in_air = false,
};

void playerMovement();

uint16_t pos_x; //0b0000 0001 0011 0110
uint16_t pos_y;
uint8_t left = 0;
uint8_t right = 0;
uint8_t up = 0;
uint8_t down = 0;
uint8_t pos_x_bit[2];
uint8_t pos_y_bit[2];
uint8_t data_send[3];
int tileX;
int tileY;

void hh_demo_setup() {
	// load sprites
	hh_ppu_update_sprite(0, HH_DBG_SPRITE_BALL);
	hh_ppu_update_sprite(1, HH_DBG_SPRITE_CHECKERBOARD);

	// background pattern
	hh_ppu_update_color(0, 1, (hh_ppu_rgb_color_t) {0x4, 0x4, 0x4});
	for (unsigned i = 0; i < HH_PPU_BG_CANVAS_TILES_H * HH_PPU_BG_CANVAS_TILES_V; i++) {
		hh_ppu_update_background(i, (hh_s_ppu_loc_bam_entry) {
			.horizontal_flip = false,
			.vertical_flip = false,
			.palette_index = 0,
			.tilemap_index = 1,
		});
	}

	// cool colors
	hh_ppu_update_color(1, 1, (hh_ppu_rgb_color_t) {0xf, 0x0, 0xf});
	hh_ppu_update_color(2, 1, (hh_ppu_rgb_color_t) {0xf, 0xf, 0xf});
	hh_ppu_update_color(3, 1, (hh_ppu_rgb_color_t) {0xf, 0x0, 0x0});
	hh_ppu_update_color(4, 1, (hh_ppu_rgb_color_t) {0x0, 0xf, 0xf});
	hh_ppu_update_color(5, 1, (hh_ppu_rgb_color_t) {0x0, 0x0, 0xf});

	// balls
	for (unsigned i = 0; i < HH_DEMO_BALL_COUNT; i++) {
		g_hh_demo_balls[i].horizontal_flip = false;
		g_hh_demo_balls[i].vertical_flip = false;
		g_hh_demo_balls[i].palette_index = i+1;
		g_hh_demo_balls[i].tilemap_index = 0;
	}
}

void hh_demo_loop(unsigned long frame) {
	playerMovement();

	//adjust map size
	pos_x = g_hh_player_1.pos_x / 100;
	pos_y = g_hh_player_1.pos_y / 100;
	
	// input testing (no hitbox stuff)
	// pos_x += (-1 * g_hh_controller_p1.dpad_left) + (1 * g_hh_controller_p1.dpad_right); // -1 = L || 1 == R
	// pos_y += (-1 * g_hh_controller_p1.dpad_up) + (1 * g_hh_controller_p1.dpad_down); // -1 = D || 1 == U

	// update player sprite on ppu
	g_hh_demo_balls[0].position_x = pos_x;
	g_hh_demo_balls[0].position_y = pos_y;
	hh_ppu_update_foreground(0, g_hh_demo_balls[0]);


	// set background pattern position
	hh_ppu_update_aux((hh_s_ppu_loc_aux) {
		.bg_shift_x = (frame / 2) % HH_PPU_SPRITE_WIDTH,
		.bg_shift_y = (frame / 8) % HH_PPU_SPRITE_HEIGHT,
		.fg_fetch = 0,
		.sysreset = 0,
	});
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

void playerMovement() {
	int8_t directionX = (-1 * g_hh_controller_p1.dpad_left) + (1 * g_hh_controller_p1.dpad_right); // -1 = L || 1 == R
	int8_t directionY = (-1 * g_hh_controller_p1.dpad_down) + (1 * g_hh_controller_p1.dpad_up); // -1 = D || 1 == U

	uint8_t i,j;
	uint8_t rotation = 0; // 0-7

	//rotation calc
	for(i = -1; i < 2;i++)
	{
		for(j = -1; j < 2; j++)
		{
			if(directionX == i)
			{
				if(directionY == j)
				{
					if(i != 0 && j != 0) //dont update when player idle
					{
						g_hh_player_1.rotation = rotation;
					}
				}
			}
		rotation++;
		}
	}
	//direction calc
	if(directionX != 0) //update direction if player is not idle
	{
		g_hh_player_1.direction_x = directionX;
	}
	//collision map x-axis

	//tile calc including radius and direction for background coliision

	uint16_t tileColX;
	uint16_t tileColY = ( g_hh_player_1.pos_y / 100) / 16; ;

	// remaining space between grid and exact
	uint8_t modTileX;
	uint8_t modTileY;

	if(g_hh_player_1.in_air == false && directionX != 0)
	{
		if(directionX == 1)
		{
			tileColX = ( ( g_hh_player_1.pos_x / 100) + g_hh_player_1.radius ) / 20;
			modTileX = ( g_hh_player_1.pos_x + ( 100 * g_hh_player_1.radius ) ) % 2000;
		}
		else if(directionX == -1)
		{
			tileColX = ( ( g_hh_player_1.pos_x / 100) - g_hh_player_1.radius ) / 20;
			modTileX = ( g_hh_player_1.pos_x - ( 100 * g_hh_player_1.radius ) ) % 2000;
		}

			if(HH_DEMO_HITBOX_TILEMAP[tileColY][tileColX + directionX] != 1)
			{
				g_hh_player_1.pos_x = g_hh_player_1.pos_x + (directionX * g_hh_player_1.speed); // NEW x set
			}

			else if(HH_DEMO_HITBOX_TILEMAP[tileColY][tileColX + directionX] == 1)
			{
				if(modTileX < g_hh_player_1.speed)
				{
					g_hh_player_1.pos_x = g_hh_player_1.pos_x + (directionX * modTileX); // NEW x set
				}
				else
				{
					g_hh_player_1.pos_x = g_hh_player_1.pos_x + (directionX * g_hh_player_1.speed); // NEW x set
				}
			}

	}
	else //if in air different all borders have to be checked
	{

	}

	//collision map floor (y-axis) (falling)
	// if falling no jump press (implement)
	/*
	tileColY = (( g_hh_player_1.pos_y / 100) + g_hh_player_1.radius) / 16; //bottom of player box
	modTileY = 1;
	if(HH_DEMO_HITBOX_TILEMAP[tileColY+1][tileColX] != 1) //rework after jumping
	{
		g_hh_player_1.pos_y = g_hh_player_1.pos_y + 5 ;// NEW y set //makew var gravity
		//playerStat = falling; //for later use of graphics/sound
	}
	*/
	//else if(HH_DEMO_HITBOX_TILEMAP[])





}
