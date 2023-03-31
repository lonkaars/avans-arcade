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
	hh_ppu_vram_dwrite((uint8_t*) HH_PPUINTDEMO_ARR, HH_PPUINTDEMO_LENGTH);
	g_hh_test_complete = true;
}

int main() {
	hh_setup();
	hh_loop();
	hh_exit();
	return 0;
}

