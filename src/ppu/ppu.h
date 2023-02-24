#pragma once

#include "types.h"

void hh_ppu_vblank_interrupt();
void hh_ppu_init();

void hh_ppu_update_foreground(unsigned index, hh_s_ppu_loc_fam_entry e);
void hh_ppu_update_background(unsigned index, hh_s_ppu_loc_bam_entry e);
void hh_ppu_update_sprite(unsigned tilemap_index, hh_s_ppu_loc_sprite sprite);
void hh_ppu_update_aux(hh_s_ppu_loc_aux aux);
