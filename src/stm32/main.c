#include <stm32f0xx_hal.h>

#include "main.h"
#include "ppu/ppu.h"

void hh_ppu_load_tilemap() {}

void hh_loop() {
	HAL_Delay(1e3);
	hh_ppu_vblank_interrupt();
}
