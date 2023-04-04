#include <stdlib.h>

#include "main.h"
#include "demo.h"
#include "ppu/ppu.h"
#include "ppu/internals.h"

bool g_hh_run = true;
volatile unsigned long g_hh_vcount;
volatile unsigned short g_hh_hcount;

void hh_ppu_vblank_interrupt() {
	static unsigned long frame = 0;
	hh_demo_loop(frame++);
	hh_ppu_vram_flush();
}

int main() {
	hh_setup();
	hh_loop();
	hh_exit();
	return 0;
}

