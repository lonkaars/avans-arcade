#include <stm32f0xx_hal.h>

#include "ppu/ppu.h"
#include "ppu/internals.h"
#include "stm32/setup.h"

void hh_ppu_init() {}
void hh_ppu_deinit() {}

void hh_ppu_vram_dwrite(hh_ppu_addr_t addr, hh_ppu_data_t data) {
	// if (!hh_ppu_vram_valid_address(addr)) return;

	uint8_t spi_data[4] = {
		(addr & 0xff00) >> 8,
		(addr & 0x00ff) >> 0,
		(data & 0xff00) >> 8,
		(data & 0x00ff) >> 0,
	};

	HAL_SPI_Transmit(&hspi1, spi_data, 4, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, true);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, false);
}
