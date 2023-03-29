#include "main.h"
#include "ppu/ppu.h"

void hh_setup() {
	hh_ppu_init();
}

void hh_exit() {
	g_hh_run = false;

	hh_ppu_deinit();
}
