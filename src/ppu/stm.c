#include <stm32f0xx_hal.h>

#include "ppu/ppu.h"
#include "ppu/internals.h"
#include "stm32/setup.h"

void hh_ppu_init() {
	hh_ppu_update_aux((hh_s_ppu_loc_aux) { .sysreset = 1 });
	hh_ppu_vram_buffer((uint8_t[4]) { 0xff, 0xff, 0xff, 0xff });
	hh_ppu_flush();
	HAL_Delay(10);
}

void hh_ppu_deinit() {}

void hh_ppu_vram_dwrite(uint8_t* data, size_t size) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(&hspi1, data, size);
}

