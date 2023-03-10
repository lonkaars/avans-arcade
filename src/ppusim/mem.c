#include <stdio.h>
#include <stdlib.h>

#include "ppu/internals.h"
#include "ppusim/mem.h"

hh_ppu_data_t *g_hh_ppusim_vram = NULL;

void hh_ppu_vram_dwrite(hh_ppu_addr_t addr, hh_ppu_data_t data) {
	if (!hh_ppu_vram_valid_address(addr)) return;
	// printf("ppu[0x%04x] = %04x\n", addr, data);
	g_hh_ppusim_vram[addr] = data;
}
