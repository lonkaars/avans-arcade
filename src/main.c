#include <stdlib.h>

#include "main.h"
#include "ppu/ppu.h"
#include "demo.h"

bool g_hh_run = true;

int main() {
	hh_setup();
	hh_loop();
	hh_exit();
	return 0;
}

void hh_ppu_vblank_interrupt() {
	static unsigned long frame = 0;
	frame++;

	hh_demo_loop(frame);
}

void hh_setup() {
	hh_ppu_init();

	hh_demo_setup();
}

void hh_exit() {
	g_hh_run = false;

	hh_ppu_deinit();
}
