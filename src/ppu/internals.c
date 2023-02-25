#include <stdlib.h>

#include "ppu/types.h"
#include "ppu/internals.h"

bool hh_ppu_vram_valid_address(hh_ppu_addr_t addr) {
	#warning unimlemented 
	(void) addr; // compiler bruh
	return true;
}

void hh_ppu_vram_write(hh_s_ppu_vram_data data) {
	for (unsigned i = 0; i < data.size; i++)
		hh_ppu_vram_dwrite(data.offset + i, data.data[i]);
}

hh_s_ppu_vram_data hh_ppu_2nat_bam(hh_s_ppu_loc_bam_entry e) {
	hh_ppu_data_t* data = malloc(sizeof(hh_ppu_data_t) * HH_PPU_VRAM_BAM_ENTRY_SIZE);

	data[0] = HH_RESIZE(e.tilemap_index, 9, 0) << 0 |
	          HH_RESIZE(e.palette_index, 2, 0) << 10 |
	          e.vertical_flip << 13 |
	          e.horizontal_flip << 14;

	hh_s_ppu_vram_data out = {
		.data = data,
		.size = HH_PPU_VRAM_FAM_ENTRY_SIZE
	};
	return out;
}

hh_s_ppu_vram_data hh_ppu_2nat_fam(hh_s_ppu_loc_fam_entry e) {
	hh_ppu_data_t* data = malloc(sizeof(hh_ppu_data_t) * HH_PPU_VRAM_FAM_ENTRY_SIZE);

	e.position_x += 16;
	e.position_y += 16;
	data[0] = HH_RESIZE(e.tilemap_index, 9, 0) << 0 |
	          HH_RESIZE(e.palette_index, 2, 0) << 10 |
	          HH_RESIZE(e.position_y, 2, 0) << 13;
	data[1] = HH_RESIZE(e.position_y, 7, 3) << 0 |
	          HH_RESIZE(e.position_x, 8, 0) << 5 |
	          e.vertical_flip << 14 |
	          e.horizontal_flip << 15;

	hh_s_ppu_vram_data out = {
		.data = data,
		.size = HH_PPU_VRAM_FAM_ENTRY_SIZE
	};
	return out;
}

hh_s_ppu_vram_data hh_ppu_2nat_aux(hh_s_ppu_loc_aux aux) {
	hh_ppu_data_t* data = malloc(sizeof(hh_ppu_data_t) * HH_PPU_VRAM_AUX_SIZE);

	data[0] = HH_RESIZE(aux.bg_shift_y, 7, 0) << 0 |
	          HH_RESIZE(aux.bg_shift_x, 7, 0) << 8;
	data[1] = HH_RESIZE(aux.bg_shift_x, 8, 8) << 0 |
	          aux.fg_fetch << 1 |
	          aux.sysreset << 2;

	hh_s_ppu_vram_data out = {
		.data = data,
		.size = HH_PPU_VRAM_AUX_SIZE
	};
	return out;
}

hh_s_ppu_vram_data hh_ppu_2nat_sprite(const hh_ppu_loc_sprite_data_t sprite_data) {
	hh_ppu_data_t* data = malloc(sizeof(hh_ppu_data_t) * HH_PPU_VRAM_TMM_SPRITE_SIZE);

	for (unsigned i = 0; i < HH_PPU_SPRITE_WIDTH * HH_PPU_SPRITE_HEIGHT; i++) {
		unsigned word = i / 5;
		unsigned pixel = i % 5;
		if (pixel == 0) data[word] = 0;
		data[word] |= HH_RESIZE(sprite_data[i], 2, 0) << pixel * 3;
	}

	hh_s_ppu_vram_data out = {
		.data = data,
		.size = HH_PPU_VRAM_TMM_SPRITE_SIZE
	};
	return out;
}

hh_s_ppu_vram_data hh_ppu_2nat_color(hh_ppu_rgb_color_t rgb) {
	hh_ppu_data_t* data = malloc(sizeof(hh_ppu_data_t) * HH_PPU_VRAM_PAL_ENTRY_SIZE);

	data[0] = HH_RESIZE(rgb[0], 3, 0) << 8 |
	          HH_RESIZE(rgb[1], 3, 0) << 4 |
	          HH_RESIZE(rgb[2], 3, 0) << 0;

	hh_s_ppu_vram_data out = {
		.data = data,
		.size = HH_PPU_VRAM_PAL_ENTRY_SIZE
	};
	return out;
}
