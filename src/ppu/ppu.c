#include <stdlib.h>

#include "ppu/ppu.h"
#include "ppu/internals.h"
#include "ppu/consts.h"

void hh_ppu_update_foreground(unsigned index, hh_s_ppu_loc_fam_entry e) {
	hh_s_ppu_vram_data s = hh_ppu_2nat_fam(e);
	s.offset = HH_PPU_VRAM_FAM_OFFSET + HH_PPU_VRAM_FAM_ENTRY_SIZE * index;
	hh_ppu_vram_write(s);
	free(s.data);
}

void hh_ppu_update_background(unsigned index, hh_s_ppu_loc_bam_entry e) {
	hh_s_ppu_vram_data s = hh_ppu_2nat_bam(e);
	s.offset = HH_PPU_VRAM_BAM_OFFSET + HH_PPU_VRAM_BAM_ENTRY_SIZE * index;
	hh_ppu_vram_write(s);
	free(s.data);
}

void hh_ppu_update_sprite(unsigned tilemap_index, hh_s_ppu_loc_sprite sprite) {
	hh_s_ppu_vram_data s = hh_ppu_2nat_sprite(sprite);
	s.offset = HH_PPU_VRAM_TMM_OFFSET + HH_PPU_VRAM_TMM_SPRITE_SIZE * tilemap_index;
	hh_ppu_vram_write(s);
	free(s.data);
}

void hh_ppu_update_aux(hh_s_ppu_loc_aux aux) {
	hh_s_ppu_vram_data a = hh_ppu_2nat_aux(aux);
	a.offset = HH_PPU_VRAM_AUX_OFFSET;
	hh_ppu_vram_write(a);
	free(a.data);
}

void hh_ppu_update_palette_table(hh_ppu_loc_palette_table_t table) {
	for(unsigned i = 0; i < HH_PPU_PALETTE_COUNT; i++)
		hh_ppu_update_palette(i, table[i]);
}

void hh_ppu_update_palette(unsigned palette_index, hh_ppu_loc_palette_data_t palette) {
	for(unsigned i = 0; i < HH_PPU_PALETTE_COLOR_COUNT; i++)
		hh_ppu_update_color(palette_index, i, palette[i]);
}

void hh_ppu_update_color(unsigned palette_index, unsigned color_index, hh_ppu_rgb_color_t color) {
	hh_s_ppu_vram_data c = hh_ppu_2nat_color(color);
	c.offset = HH_PPU_VRAM_PAL_OFFSET +
		palette_index * HH_PPU_VRAM_PAL_ENTRY_SIZE * HH_PPU_PALETTE_COLOR_COUNT +
		color_index * HH_PPU_VRAM_PAL_ENTRY_SIZE;
	hh_ppu_vram_write(c);
	free(c.data);
}
