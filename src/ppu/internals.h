#pragma once

#include <stdbool.h>

#include "types.h"

typedef struct {
	hh_ppu_addr_t offset;
	hh_ppu_addr_t size;
	hh_ppu_data_t* data;
} hh_s_ppu_vram_data;

bool hh_ppu_vram_valid_address(hh_ppu_addr_t addr);
void hh_ppu_vram_write(hh_ppu_addr_t addr, hh_ppu_data_t data);
void hh_ppu_vram_memcpy(void* a, hh_s_ppu_vram_data b);

hh_s_ppu_vram_data* hh_ppu_2nat_bam(hh_s_ppu_loc_bam_entry);
hh_s_ppu_vram_data* hh_ppu_2nat_fam(hh_s_ppu_loc_fam_entry);
hh_s_ppu_vram_data* hh_ppu_2nat_aux(hh_s_ppu_loc_aux);
hh_s_ppu_vram_data* hh_ppu_2nat_sprite(hh_ppu_loc_sprite_data_t);
hh_s_ppu_vram_data* hh_ppu_2nat_pal(hh_ppu_loc_palette_data_t);

