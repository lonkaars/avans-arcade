#include <math.h>

#include "demo.h"
#include "entity.h"
#include "input.h"
#include "ppu/ppu.h"

#define HH_DEMO_BALL_COUNT 2
hh_s_ppu_loc_fam_entry g_hh_demo_balls[HH_DEMO_BALL_COUNT];

hh_s_entity_player g_hh_player_1 = {
	.pos_x		 = 11000, 
	.pos_y		 = 5000,
	.radius		 = 4,
	.speed		 = 100,
	.air_speed   = 80,
	.direction_x = 1,
	.accelY 		 = 0,
	.rotation	 = 8,
	.in_air		 = false,
};

hh_s_entity_enemy g_hh_enemy_1 = {
	.pos_x		 = 16000, 
	.pos_y		 = 20000,
	.radius		 = 4,
	.speed		 = 100,
	.direction_x = 1,
	.accelY 		 = 0,
	.in_air		 = false,
};

void hh_player_movement();

int hh_background_collision_x(hh_s_entity_player tempEntity);
hh_s_entity_player hh_background_collision_y_player(hh_s_entity_player tempEntity);

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

void hh_demo_setup() {
	// load sprites
	hh_ppu_update_sprite(0, HH_DBG_SPRITE_BALL);
	hh_ppu_update_sprite(1, HH_DBG_SPRITE_CHECKERBOARD);

	// background pattern
	hh_ppu_update_color(0, 1, (hh_ppu_rgb_color_t){0x4, 0x4, 0x4});
	for (unsigned i = 0; i < HH_PPU_BG_CANVAS_TILES_H * HH_PPU_BG_CANVAS_TILES_V; i++) {
		hh_ppu_update_background(i, (hh_s_ppu_loc_bam_entry){
										.horizontal_flip = false,
										.vertical_flip	 = false,
										.palette_index	 = 0,
										.tilemap_index	 = 1,
									});
	}

	// cool colors
	hh_ppu_update_color(1, 1, (hh_ppu_rgb_color_t){0xf, 0x0, 0xf});
	hh_ppu_update_color(2, 1, (hh_ppu_rgb_color_t){0xf, 0xf, 0xf});
	hh_ppu_update_color(3, 1, (hh_ppu_rgb_color_t){0xf, 0x0, 0x0});
	hh_ppu_update_color(4, 1, (hh_ppu_rgb_color_t){0x0, 0xf, 0xf});
	hh_ppu_update_color(5, 1, (hh_ppu_rgb_color_t){0x0, 0x0, 0xf});

	// balls
	for (unsigned i = 0; i < HH_DEMO_BALL_COUNT; i++) {
		g_hh_demo_balls[i].horizontal_flip = false;
		g_hh_demo_balls[i].vertical_flip   = false;
		g_hh_demo_balls[i].palette_index   = i + 1;
		g_hh_demo_balls[i].tilemap_index   = 0;
	}
}

void hh_demo_loop(unsigned long frame) {
	hh_player_movement();
	// input testing (no hitbox stuff)
	// g_hh_player_1.pos_x  += ((-1 * g_hh_controller_p1.dpad_left) + (1 * g_hh_controller_p1.dpad_right)) * g_hh_player_1.speed; // -1 = L || 1 == R
	g_hh_player_1.pos_y  += ((-1 * g_hh_controller_p1.dpad_up) + (1 * g_hh_controller_p1.dpad_down)) * g_hh_player_1.speed; // -1 = D || 1 == U


	// adjust map size
	g_hh_pos_x = g_hh_player_1.pos_x / 100;
	g_hh_pos_y = g_hh_player_1.pos_y / 100;


	// update player sprite on ppu
	g_hh_demo_balls[0].position_x = g_hh_pos_x;
	g_hh_demo_balls[0].position_y = g_hh_pos_y;

	g_hh_demo_balls[1].position_x = 220;
	g_hh_demo_balls[1].position_y = 220;
	hh_ppu_update_foreground(0, g_hh_demo_balls[0]);
	hh_ppu_update_foreground(1, g_hh_demo_balls[1]);

	// set background pattern position
	hh_ppu_update_aux((hh_s_ppu_loc_aux){
		.bg_shift_x = (frame / 2) % HH_PPU_SPRITE_WIDTH,
		.bg_shift_y = (frame / 8) % HH_PPU_SPRITE_HEIGHT,
		.fg_fetch	= 0,
		.sysreset	= 0,
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

void hh_player_movement() {
	int8_t directionX = (-1 * g_hh_controller_p1.dpad_left) + (1 * g_hh_controller_p1.dpad_right); // -1 = L || 1 == R
	int8_t directionY = (-1 * g_hh_controller_p1.dpad_up) + (1 * g_hh_controller_p1.dpad_down);	   // -1 = D || 1 == U

	if(g_hh_controller_p1.button_primary == 1 && g_hh_player_1.accelY == 0 && g_hh_player_1.in_air == false)
	{
		g_hh_player_1.accelY = 25;
	}


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
	// X-axis calc
	if (directionX != 0) // update direction and position if player is not idle 
	{
		g_hh_player_1.direction_x = directionX;
		g_hh_player_1.pos_x = hh_background_collision_x(g_hh_player_1);
	}

	
	g_hh_player_1 = hh_background_collision_y_player(g_hh_player_1);

}


int hh_background_collision_x(hh_s_entity_player tempEntity){
	// tile calc including radius and direction for background coliision

	uint16_t tileColX;
	uint16_t tileColY = (tempEntity.pos_y / 100) / 8;
	;

	// remaining space between grid and exact
	uint8_t modTileX;


		
	if (tempEntity.direction_x == 1) {
		tileColX = ((tempEntity.pos_x / 100) + tempEntity.radius) / 8;
		modTileX = 800 -((tempEntity.pos_x + (100 * tempEntity.radius)) % 800);
	} else if (tempEntity.direction_x == -1) {
		tileColX = ((tempEntity.pos_x / 100) - tempEntity.radius )/ 8;
		modTileX = (tempEntity.pos_x - (100 * tempEntity.radius)) % 800;
	}
	
	tileColY = (( tempEntity.pos_y / 100) + tempEntity.radius) / 8; //bottom of player box
	
	if (HH_DEMO_HITBOX_TILEMAP[tileColY][tileColX + tempEntity.direction_x] != 1) {
		tileColY = (( tempEntity.pos_y / 100) - tempEntity.radius) / 8; //bottom of player box
		if (HH_DEMO_HITBOX_TILEMAP[tileColY][tileColX + tempEntity.direction_x] != 1) {
			if(tempEntity.in_air== true){
				tempEntity.pos_x= tempEntity.pos_x + (tempEntity.direction_x * tempEntity.air_speed); // 80% of max speed if in air
			} else {
				tempEntity.pos_x= tempEntity.pos_x + (tempEntity.direction_x * tempEntity.speed); // 80% of max speed if in air
			}
		} else {
			if(tempEntity.in_air == true){
				if (modTileX < tempEntity.air_speed) {
					tempEntity.pos_x = tempEntity.pos_x + (tempEntity.direction_x * modTileX); // NEW x set
				} else {
					tempEntity.pos_x= tempEntity.pos_x + (tempEntity.direction_x * tempEntity.air_speed); //
				}
			} else {
				if (modTileX < g_hh_player_1.speed) {
					tempEntity.pos_x= tempEntity.pos_x + (tempEntity.direction_x * modTileX); // NEW x set
				} else {
					tempEntity.pos_x = tempEntity.pos_x + (tempEntity.direction_x * tempEntity.speed); // 
				}
			}
		}
	} else if (HH_DEMO_HITBOX_TILEMAP[tileColY][tileColX + tempEntity.direction_x] == 1) {
		if(tempEntity.in_air == true){
			if (modTileX < tempEntity.air_speed) {
				tempEntity.pos_x = tempEntity.pos_x + (tempEntity.direction_x * modTileX); // NEW x set
			} else {
				tempEntity.pos_x = tempEntity.pos_x + (tempEntity.direction_x * tempEntity.air_speed); //
			}
		} else {
			if (modTileX < g_hh_player_1.speed) {
				tempEntity.pos_x = tempEntity.pos_x + (tempEntity.direction_x * modTileX); // NEW x set
			} else {
				tempEntity.pos_x= tempEntity.pos_x + (tempEntity.direction_x * tempEntity.speed); // 
			}
		}
	}

	
	return tempEntity.pos_x;

}

hh_s_entity_player hh_background_collision_y_player(hh_s_entity_player tempEntity){

	uint16_t tileColX;
	uint16_t tileColY;
	

	// remaining space between grid and exact
		uint8_t modTileY;

	if(g_hh_controller_p1.button_primary == 1 && tempEntity.accelY == 0 && tempEntity.in_air == false)
	{
//		tempEntity.accelY = 25;
	}
	
	tileColY = (( tempEntity.pos_y / 100) + tempEntity.radius) / 8; //bottom of player box
	tileColX = ((tempEntity.pos_x / 100) + tempEntity.radius) / 8; //right of player
	modTileY = ((tempEntity.pos_y + (100 * tempEntity.radius)) % 800); //bottom of box

	//rework arfter jumping
	if(tempEntity.accelY <= 0){ //falling
		if(HH_DEMO_HITBOX_TILEMAP[tileColY+1][tileColX] != 1) {//check bottom right first
			tileColX = ((tempEntity.pos_x / 100) - tempEntity.radius) / 8;
			if(HH_DEMO_HITBOX_TILEMAP[tileColY+1][tileColX] != 1)	{
				tempEntity.in_air = true;
				tempEntity.pos_y = tempEntity.pos_y - (tempEntity.accelY * 8) ;// NEW y set 
				if (tempEntity.accelY > -35){
					tempEntity.accelY--;
				}
			}
			else	{
				if(modTileY < tempEntity.accelY * -8) {
					tempEntity.pos_y = tempEntity.pos_y + (modTileY) ;// NEW y set 
				} else {
					tempEntity.pos_y = tempEntity.pos_y - (tempEntity.accelY * 8) ;// NEW y set 
				}
				tempEntity.in_air = false;
				tempEntity.accelY = 0;
			}
		}
		else	{
			if(modTileY < tempEntity.accelY * -8) {
				tempEntity.pos_y = tempEntity.pos_y + (modTileY) ;// NEW y set 
			} else {
				tempEntity.pos_y = tempEntity.pos_y - (tempEntity.accelY * 8) ;// NEW y set 
			}
			tempEntity.in_air = false;
			tempEntity.accelY = 0;
		}
	}
	else{ //jumping
		tileColY = (( tempEntity.pos_y / 100) - tempEntity.radius) / 8; //top of player box
		modTileY = 800 -((tempEntity.pos_y + (100 * tempEntity.radius)) % 800); //top of box
			if(HH_DEMO_HITBOX_TILEMAP[tileColY-1][tileColX] != 1) {//check bottom right first
				tileColX = ((tempEntity.pos_x / 100) - tempEntity.radius) / 8;
				if(HH_DEMO_HITBOX_TILEMAP[tileColY-1][tileColX] != 1)	{
					tempEntity.pos_y = tempEntity.pos_y - (tempEntity.accelY * 8) ;// NEW y set 
					tempEntity.in_air = true;
					tempEntity.accelY--;
				}	else	{
				if(modTileY < tempEntity.accelY * -8) {
					tempEntity.pos_y = tempEntity.pos_y - (modTileY) ;// NEW y set 
				} else {
					tempEntity.pos_y = tempEntity.pos_y - (tempEntity.accelY * 8) ;
				}
				tempEntity.accelY = 0;
				}
			}
			else	{
				if(modTileY < tempEntity.accelY * -8) {
					tempEntity.pos_y = tempEntity.pos_y - (modTileY) ;// NEW y set //makew var gravity
				} else {
					tempEntity.pos_y = tempEntity.pos_y - (tempEntity.accelY * 8) ;// NEW y set //makew var gravity
				}
				tempEntity.accelY = 0;
			}
		
	}

	return tempEntity;
}
