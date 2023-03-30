#include <stdio.h>
#include <stdlib.h>

#include "ppu/internals.h"
#include "ppusim/mem.h"

hh_ppu_data_t *g_hh_ppusim_vram = NULL;

void hh_ppu_vram_dwrite(uint8_t* data, size_t size) {
	for (size_t i = 0; i < size; i += 4) {
		if (i+4 > size) break;
		hh_ppu_addr_t ppu_addr = (data[i+0] << 8) | (data[i+1] << 0);
		hh_ppu_data_t ppu_data = (data[i+2] << 8) | (data[i+3] << 0);
		// printf("%04x: %04x\n", ppu_addr, ppu_data);
		if (!hh_ppu_vram_valid_address(ppu_addr)) continue;
		g_hh_ppusim_vram[ppu_addr] = ppu_data;
	}
}

