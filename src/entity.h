#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint16_t pos_x;
	uint16_t pos_y;
	uint8_t radius;
	uint8_t rotation; //45 degrees steps 0 == right 2 == down 4 == left 6 == up
	uint8_t direction_x; //direction where its looking at in case no input;
	int8_t speed;	//10 default L/R MODifier
	bool in_air;
} hh_s_entity_player;

