#include <stdlib.h>

#include "main.h"
#include "demo.h"
#include "ppu/ppu.h"
#include "input.h"

#ifdef HH_TARGET_DESKTOP
#include <stdio.h>
#include "../test/ppu-stm-integration-demo/data.h"
#include "ppu/internals.h"
#endif

bool g_hh_run = true;
volatile unsigned long g_hh_vcount;
volatile unsigned short g_hh_hcount;

void hh_ppu_vblank_interrupt() {
	static unsigned long frame = 0;
	static bool done = false;
#ifdef HH_TARGET_STM32
	hh_ppu_flush();
	hh_input_read();
	hh_demo_loop(frame);
#endif
#ifdef HH_TARGET_DESKTOP
	//hh_ppu_vram_dwrite((uint8_t*) HH_PPUINTDEMO_ARR, HH_PPUINTDEMO_LENGTH);
	for (unsigned int limit = 0; limit < 1000; limit++) {
		if (frame * 4 > HH_PPUINTDEMO_LENGTH) {
			if (!done) printf("done\n");
			done = true;
			return;
		}
		hh_ppu_vram_dwrite((uint8_t*) HH_PPUINTDEMO_ARR + frame * 4, 4);
		uint8_t* d = HH_PPUINTDEMO_ARR + frame * 4;
		if (d[0] == 0xff && d[1] == 0xff && d[2] == 0xff && d[3] == 0xff) break; // run all commands until flush
		else frame++;
	}
#endif
	frame++;
}

int main() {
	hh_setup();
	hh_loop();
	hh_exit();
	return 0;
}

