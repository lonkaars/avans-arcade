#include <stdlib.h>
#include <stm32f0xx_hal.h>

#include "main.h"
#include "stm32/setup.h"
#include "../test/ppu-stm-integration-demo/data.h"

bool g_hh_run = true;

int main() {
	hh_setup();

	while (1) {
		for (unsigned long i = 0; i < HH_PPUINTDEMO_LENGTH; i++) {
			uint16_t addr = HH_PPUINTDEMO_ADDR[i];
			uint16_t data = HH_PPUINTDEMO_DATA[i];

			uint8_t spi_data[4] = {
				(addr & 0xff00) >> 8,
				(addr & 0x00ff) >> 0,
				(data & 0xff00) >> 8,
				(data & 0x00ff) >> 0,
			};

			HAL_SPI_Transmit(&hspi1, spi_data, 4, HAL_MAX_DELAY);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, true);
			// HAL_Delay(1);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, false);
			// HAL_Delay(1);
		}
		HAL_Delay(1e3);
	}

	hh_exit();
	return 0;
}

