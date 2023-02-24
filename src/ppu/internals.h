#pragma once

#include <stdbool.h>

#include "ppu/types.h"

typedef struct {
	hh_ppu_addr_t offset;
	hh_ppu_addr_t size;
	hh_ppu_data_t* data;
} hh_s_ppu_vram_data;

/** @brief check if `addr` is a valid PPU address */
bool hh_ppu_vram_valid_address(hh_ppu_addr_t addr);
/** @brief direct write vram word (platform-specific implementation) */
void hh_ppu_vram_dwrite(hh_ppu_addr_t addr, hh_ppu_data_t data);
/** @brief write data block into vram */
void hh_ppu_vram_write(hh_s_ppu_vram_data data);

hh_s_ppu_vram_data hh_ppu_2nat_bam(hh_s_ppu_loc_bam_entry);
hh_s_ppu_vram_data hh_ppu_2nat_fam(hh_s_ppu_loc_fam_entry);
hh_s_ppu_vram_data hh_ppu_2nat_aux(hh_s_ppu_loc_aux);
hh_s_ppu_vram_data hh_ppu_2nat_sprite(hh_ppu_loc_sprite_data_t);
hh_s_ppu_vram_data hh_ppu_2nat_color(hh_ppu_rgb_color_t);

