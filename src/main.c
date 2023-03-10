#include <stdlib.h>

#include "main.h"
#include "demo.h"
#include "input.h"

bool g_hh_run = true;

int main() {
	hh_setup();
	hh_loop();
	hh_exit();
	return 0;
}

void hh_ppu_vblank_interrupt() {
	static unsigned long frame = 0;
	hh_input_read();
	hh_demo_loop(frame);
	frame++;
}

