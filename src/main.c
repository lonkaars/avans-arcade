#include <stdlib.h>

#include "main.h"
#include "demo.h"
#include "ppu/internals.h"
#include "ppu/ppu.h"
#include "input.h"

#include "../test/ppu-stm-integration-demo/data.h"
#ifdef HH_TARGET_DESKTOP
#include <stdio.h>
#endif

bool g_hh_run = true;
volatile unsigned long g_hh_vcount;
volatile unsigned short g_hh_hcount;

void hh_ppu_vblank_interrupt() {
	static unsigned long frame = 0;

	// hh_ppu_flush();
	// hh_input_read();
	// hh_demo_loop(frame);

	size_t offset = frame * 4;
	if (offset >= HH_PPUINTDEMO_LENGTH) return;
	uint8_t* d = HH_PPUINTDEMO_ARR + offset;
	hh_ppu_vram_buffer(d);
	hh_ppu_vram_flush();

	frame++;
}

int main() {
	hh_setup();
	hh_loop();
	hh_exit();
	return 0;
}

