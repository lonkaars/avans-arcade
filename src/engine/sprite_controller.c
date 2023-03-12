#include <stdint.h>

#include "engine/sprite_controller.h"
// #include "engine/maths.h"
#include "ppu/types.h"
#include "ppu/consts.h"

uint8_t hh_get_palette(uint16_t tile_idx) {
	return hh_g_sprite_palette[tile_idx];
}
