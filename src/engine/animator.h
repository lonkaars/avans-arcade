#pragma once
#include <stdint.h>

#include "ppu/types.h"
#include "engine/types.h"
#include "engine/entity.h"

/** @brief flashes sprite white, also needs to be called next frame */
void hh_animate_hit(hh_s_rendering*, bool hit);
/** @brief updates current animation frame */
void hh_animate(hh_s_rendering*, uint16_t start, uint16_t end, uint8_t step);

/** @brief passively updates sprite*/
uint16_t hh_update_sprite(uint16_t idx, hh_entity* in, vec_cor cam);
