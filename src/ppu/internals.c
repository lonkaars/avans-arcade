#include <stdlib.h>

#include "ppu/types.h"
#include "ppu/internals.h"

/** @brief generate bitmask with `n` bits set to one from LSB */
#define HH_MASK(n) ((1 << (n))-1)
/** @brief resize `in` to `upper downto lower` like in vhdl */
#define HH_RESIZE(in, upper, lower) ((((hh_ppu_data_t)(in)) & HH_MASK(upper+1-lower)) >> lower)

bool hh_ppu_vram_valid_address(hh_ppu_addr_t addr) {
	(void) addr; // compiler bruh
	return true; // TODO
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
	data[1] = HH_RESIZE(e.position_y, 4, 0) << 0 |
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

hh_s_ppu_vram_data hh_ppu_2nat_sprite(hh_ppu_loc_sprite_data_t sprite_data) {
	hh_ppu_data_t* data = malloc(sizeof(hh_ppu_data_t) * HH_PPU_VRAM_TMM_SPRITE_SIZE);

	for (unsigned x = 0; x < HH_PPU_SPRITE_WIDTH; x++) {
		for (unsigned y = 0; y < HH_PPU_SPRITE_HEIGHT; y++) {
			unsigned i = y * HH_PPU_SPRITE_WIDTH + x;
			unsigned word = i / 5;
			unsigned pixel = i % 5;
			data[word] |= HH_RESIZE(sprite_data[i], 2, 0) << pixel * 3;
		}
	}

	hh_s_ppu_vram_data out = {
		.data = data,
		.size = HH_PPU_VRAM_TMM_SPRITE_SIZE
	};
	return out;
}

hh_s_ppu_vram_data hh_ppu_2nat_color(hh_ppu_rgb_color_t rgb) {
	hh_ppu_data_t* data = malloc(sizeof(hh_ppu_data_t) * HH_PPU_VRAM_PAL_ENTRY_SIZE);

	data[0] = HH_RESIZE(rgb[0], 3, 0) << 0 |
	          HH_RESIZE(rgb[1], 3, 0) << 4 |
	          HH_RESIZE(rgb[2], 3, 0) << 8;

	hh_s_ppu_vram_data out = {
		.data = data,
		.size = HH_PPU_VRAM_PAL_ENTRY_SIZE
	};
	return out;
}
