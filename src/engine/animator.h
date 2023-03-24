#pragma once
#include <stdint.h>

#include "ppu/types.h"
#include "engine/types.h"
#include "engine/entity.h"

/** @brief flashes sprite white, also needs to be called next frame */
void hh_animate_hit(hh_s_rendering*, bool hit);
/** @brief updates current animation frame */
void hh_animate(hh_s_rendering*, hh_idx_t start, hh_idx_t end, uint8_t step);

/** @brief passively updates sprite*/
void hh_update_sprite(hh_entity* in);

