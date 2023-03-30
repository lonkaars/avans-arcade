#include <stdlib.h>

#include "main.h"
#include "../test/ppu-stm-integration-demo/data.h"
#include "ppu/internals.h"

bool g_hh_run = true;

bool g_hh_test_complete = false;

void hh_ppu_vblank_interrupt() {
#ifdef HH_TARGET_DESKTOP
	if (g_hh_test_complete) return;
#endif
	// // uint8_t test[4] = { 0x0f, 0x0f, 0xf0, 0xf0 };
	// uint8_t* test = malloc(4);
	// test[0] = 0x00;
	// test[1] = 0x00;
	// test[2] = 0x00;
	// test[3] = 0x00;
	// while (1)
	// 	hh_ppu_vram_dwrite(test, 4);
	// return;
	if (1) {
		hh_ppu_vram_dwrite((uint8_t*) HH_PPUINTDEMO_ARR, HH_PPUINTDEMO_LENGTH);
	}
	if (0) {
		for (size_t i = 0; i < HH_PPUINTDEMO_LENGTH; i += 4) {
			if (i+4 > HH_PPUINTDEMO_LENGTH) break;
			uint8_t test[4] = {
				HH_PPUINTDEMO_ARR[i+0],
				HH_PPUINTDEMO_ARR[i+1],
				HH_PPUINTDEMO_ARR[i+2],
				HH_PPUINTDEMO_ARR[i+3],
			};
			hh_ppu_vram_dwrite(test, 4);
		}
	}
	g_hh_test_complete = true;
}

int main() {
	hh_setup();
	hh_loop();
	hh_exit();
	return 0;
}

