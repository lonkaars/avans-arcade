#include <stdlib.h>

#include "main.h"
#include "../test/ppu-stm-integration-demo/data.h"
#include "ppu/internals.h"

bool g_hh_run = true;

void hh_ppu_vblank_interrupt() {
	for (unsigned long i = 0; i < HH_PPUINTDEMO_LENGTH; i++) {
		uint16_t addr = HH_PPUINTDEMO_ADDR[i];
		uint16_t data = HH_PPUINTDEMO_DATA[i];
		hh_ppu_vram_dwrite(addr, data);
	}
}

int main() {
	hh_setup();
	hh_loop();
	hh_exit();
	return 0;
}

