#include <stm32f0xx_hal.h>

#include "main.h"
#include "ppu/ppu.h"

void hh_ppu_load_tilemap() {}

void hh_loop() {
	// TODO: call hh_ppu_vblank_interrupt on interrupt instead of manually
	while (1) {
		hh_ppu_vblank_interrupt();
		HAL_Delay(16);
	}
}
