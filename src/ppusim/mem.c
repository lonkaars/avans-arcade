#include <stdio.h>
#include <stdlib.h>

#include "ppu/internals.h"
#include "ppusim/mem.h"

hh_ppu_data_t *g_hh_ppusim_vram = NULL;

#define HH_DBG_PPUSIM_PRINT_MEM

static void hh_ppu_dbg_memprint(hh_ppu_addr_t addr, hh_ppu_data_t data) {
	int area_i = ((addr >= HH_PPU_VRAM_TMM_OFFSET) && (addr < (HH_PPU_VRAM_TMM_OFFSET + HH_PPU_VRAM_TMM_SIZE))) ? 0 :
	             ((addr >= HH_PPU_VRAM_BAM_OFFSET) && (addr < (HH_PPU_VRAM_BAM_OFFSET + HH_PPU_VRAM_BAM_SIZE))) ? 1 :
	             ((addr >= HH_PPU_VRAM_FAM_OFFSET) && (addr < (HH_PPU_VRAM_FAM_OFFSET + HH_PPU_VRAM_FAM_SIZE))) ? 2 :
	             ((addr >= HH_PPU_VRAM_PAL_OFFSET) && (addr < (HH_PPU_VRAM_PAL_OFFSET + HH_PPU_VRAM_PAL_SIZE))) ? 3 :
	             ((addr >= HH_PPU_VRAM_AUX_OFFSET) && (addr < (HH_PPU_VRAM_AUX_OFFSET + HH_PPU_VRAM_AUX_SIZE))) ? 4 : -1;
	char* area_str = (area_i == 0) ? "TMM" :
	                 (area_i == 1) ? "BAM" :
	                 (area_i == 2) ? "FAM" :
	                 (area_i == 3) ? "PAL" :
	                 (area_i == 4) ? "AUX" : "???";
	printf("[%s] %04x: %04x", area_str, addr, data);

	switch (area_i) {
		case 0: {
			unsigned short i = addr - HH_PPU_VRAM_TMM_OFFSET;
			printf(" (tile[%04i] data)", i / 52);
			break;
		}
		case 1: {
			unsigned short i = addr - HH_PPU_VRAM_BAM_OFFSET;
			printf(" (bam tile [%02i, %02i] attributes)", i % 40, i / 40);
			break;
		}
		case 2: {
			unsigned short i = addr - HH_PPU_VRAM_FAM_OFFSET;
			printf(" (fam[%03i] attributes)", i/2);
			break;
		}
		case 3: {
			unsigned short i = addr - HH_PPU_VRAM_PAL_OFFSET;
			printf(" (pal[%02i] = #%x%x%x%x%x%x)", i, (data >> 0) & 0xf, (data >> 0) & 0xf, 
					(data >> 4) & 0xf, (data >> 4) & 0xf, 
					(data >> 8) & 0xf, (data >> 8) & 0xf);
			break;
		}
		case 4: {

			if (addr == 0xde01 && ((data & (1 << 2)) > 0)) printf(" (+sysreset)");
			break;
		}

		default: { break; }
	}

	if (addr == 0xffff && data == 0xffff) printf(" (flush)");

	printf("\n");
}

void hh_ppu_vram_dwrite(uint8_t* data, size_t size) {
	for (size_t i = 0; i < size; i += 4) {
		if (i+4 > size) break;
		hh_ppu_addr_t ppu_addr = (data[i+0] << 8) | (data[i+1] << 0);
		hh_ppu_data_t ppu_data = (data[i+2] << 8) | (data[i+3] << 0);
#ifdef HH_DBG_PPUSIM_PRINT_MEM
		hh_ppu_dbg_memprint(ppu_addr, ppu_data);
#endif
		if (!hh_ppu_vram_valid_address(ppu_addr)) continue;
		g_hh_ppusim_vram[ppu_addr] = ppu_data;
	}
}

