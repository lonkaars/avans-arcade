#pragma once

#include "stdlib.h"
#include "stdio.h"
#include "ppu/consts.h"
#include "entity.h"
// #include <stdint.h>

typedef struct 
{
	int pos_x;
	int pos_y;
}cameraPos;


cameraPos playerCamera(uint16_t X, uint16_t Y);
