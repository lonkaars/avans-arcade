#include <stdlib.h>

#include "ppu/consts.h"
#include "ppu/internals.h"
#include "ppu/types.h"

bool g_hh_ppu_vram_buffer_swap = false;
uint8_t g_hh_ppu_vram_buffer_raw[HH_PPU_COMMAND_BUFFER_SIZE * 2] = { 0 };
uint8_t* g_hh_ppu_vram_buffer_ptr = g_hh_ppu_vram_buffer_raw;
size_t g_hh_ppu_vram_buffer_size = 0;

bool hh_ppu_vram_valid_address(hh_ppu_addr_t addr) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
	if ((addr >= HH_PPU_VRAM_TMM_OFFSET) && (addr < (HH_PPU_VRAM_TMM_OFFSET + HH_PPU_VRAM_TMM_SIZE))) return true;
	if ((addr >= HH_PPU_VRAM_BAM_OFFSET) && (addr < (HH_PPU_VRAM_BAM_OFFSET + HH_PPU_VRAM_BAM_SIZE))) return true;
	if ((addr >= HH_PPU_VRAM_FAM_OFFSET) && (addr < (HH_PPU_VRAM_FAM_OFFSET + HH_PPU_VRAM_FAM_SIZE))) return true;
	if ((addr >= HH_PPU_VRAM_PAL_OFFSET) && (addr < (HH_PPU_VRAM_PAL_OFFSET + HH_PPU_VRAM_PAL_SIZE))) return true;
	if ((addr >= HH_PPU_VRAM_AUX_OFFSET) && (addr < (HH_PPU_VRAM_AUX_OFFSET + HH_PPU_VRAM_AUX_SIZE))) return true;
#pragma GCC diagnostic pop
	return false;
}

void hh_ppu_update_aux(hh_s_ppu_loc_aux aux) {
	hh_s_ppu_vram_data a = hh_ppu_2nat_aux(aux);
	a.offset			 = HH_PPU_VRAM_AUX_OFFSET;
	hh_ppu_vram_write(a);
	free(a.data);
}

void hh_ppu_vram_write(hh_s_ppu_vram_data data) {
	for (unsigned i = 0; i < data.size; i++) {
		hh_ppu_addr_t ppu_addr = data.offset + i;
		hh_ppu_data_t ppu_data = data.data[i];
		hh_ppu_vram_buffer((uint8_t[4]) {
			(ppu_addr >> 8) & 0xff,
			(ppu_addr >> 0) & 0xff,
			(ppu_data >> 8) & 0xff,
			(ppu_data >> 0) & 0xff,
		});
	}
}

hh_s_ppu_vram_data hh_ppu_2nat_bam(hh_s_ppu_loc_bam_entry e) {
	hh_ppu_data_t *data = malloc(sizeof(hh_ppu_data_t) * HH_PPU_VRAM_BAM_ENTRY_SIZE);

	data[0] = HH_RESIZE(e.tilemap_index, 9, 0) << 0 | HH_RESIZE(e.palette_index, 2, 0) << 10 | e.vertical_flip << 13 | e.horizontal_flip << 14;

	hh_s_ppu_vram_data out = {.data = data, .size = HH_PPU_VRAM_BAM_ENTRY_SIZE};
	return out;
}

hh_s_ppu_vram_data hh_ppu_2nat_fam(hh_s_ppu_loc_fam_entry e) {
	hh_ppu_data_t *data = malloc(sizeof(hh_ppu_data_t) * HH_PPU_VRAM_FAM_ENTRY_SIZE);

	e.position_x += 16;
	e.position_y += 16;
	data[0] = HH_RESIZE(e.tilemap_index, 9, 0) << 0 | HH_RESIZE(e.palette_index, 2, 0) << 10 | HH_RESIZE(e.position_y, 2, 0) << 13;
	data[1] = HH_RESIZE(e.position_y, 7, 3) << 0 | HH_RESIZE(e.position_x, 8, 0) << 5 | e.vertical_flip << 14 | e.horizontal_flip << 15;

	hh_s_ppu_vram_data out = {.data = data, .size = HH_PPU_VRAM_FAM_ENTRY_SIZE};
	return out;
}

hh_s_ppu_vram_data hh_ppu_2nat_aux(hh_s_ppu_loc_aux aux) {
	hh_ppu_data_t *data = malloc(sizeof(hh_ppu_data_t) * HH_PPU_VRAM_AUX_SIZE);

	data[0] = HH_RESIZE(aux.bg_shift_y, 7, 0) << 0 | HH_RESIZE(aux.bg_shift_x, 7, 0) << 8;
	data[1] = HH_RESIZE(aux.bg_shift_x, 8, 8) << 0 | aux.fg_fetch << 1 | aux.sysreset << 2;

	hh_s_ppu_vram_data out = {.data = data, .size = HH_PPU_VRAM_AUX_SIZE};
	return out;
}

hh_s_ppu_vram_data hh_ppu_2nat_sprite(const hh_ppu_loc_sprite_data_t sprite_data) {
	hh_ppu_data_t *data = malloc(sizeof(hh_ppu_data_t) * HH_PPU_VRAM_TMM_SPRITE_SIZE);

	for (unsigned i = 0; i < HH_PPU_SPRITE_WIDTH * HH_PPU_SPRITE_HEIGHT; i++) {
		unsigned word  = i / 5;
		unsigned pixel = i % 5;
		if (pixel == 0) data[word] = 0;
		data[word] |= HH_RESIZE(sprite_data[i], 2, 0) << pixel * 3;
	}

	hh_s_ppu_vram_data out = {.data = data, .size = HH_PPU_VRAM_TMM_SPRITE_SIZE};
	return out;
}

hh_s_ppu_vram_data hh_ppu_2nat_color(hh_ppu_rgb_color_t rgb) {
	hh_ppu_data_t *data = malloc(sizeof(hh_ppu_data_t) * HH_PPU_VRAM_PAL_ENTRY_SIZE);

	data[0] = HH_RESIZE(rgb[0], 3, 0) << 8 | HH_RESIZE(rgb[1], 3, 0) << 4 | HH_RESIZE(rgb[2], 3, 0) << 0;

	hh_s_ppu_vram_data out = {.data = data, .size = HH_PPU_VRAM_PAL_ENTRY_SIZE};
	return out;
}

void hh_ppu_vram_buffer(uint8_t data[4]) {
	size_t head = g_hh_ppu_vram_buffer_size;
	g_hh_ppu_vram_buffer_ptr[head+0] = data[0]; // NOTE: this only works because sizeof(buffer) is divisible by 4
	g_hh_ppu_vram_buffer_ptr[head+1] = data[1];
	g_hh_ppu_vram_buffer_ptr[head+2] = data[2];
	g_hh_ppu_vram_buffer_ptr[head+3] = data[3];
	g_hh_ppu_vram_buffer_size += 4;
}

void hh_ppu_vram_flush() {
	hh_ppu_vram_buffer((uint8_t[4]){ 0xff, 0xff, 0xff, 0xff });
	hh_ppu_vram_dwrite(g_hh_ppu_vram_buffer_ptr, g_hh_ppu_vram_buffer_size);
	g_hh_ppu_vram_buffer_size = 0;
	g_hh_ppu_vram_buffer_ptr = g_hh_ppu_vram_buffer_raw + g_hh_ppu_vram_buffer_swap * HH_PPU_COMMAND_BUFFER_SIZE;
	g_hh_ppu_vram_buffer_swap = !g_hh_ppu_vram_buffer_swap;
}

