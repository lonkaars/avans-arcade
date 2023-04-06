#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_spi.h>
#include <stm32f0xx_hal_uart.h>
#include <stm32f0xx_hal_gpio.h>
#include <FreeRTOS.h>
#include <task.h>

#include "main.h"
#include "setup.h"
#include "ppu/ppu.h"
#include "tilemap.h"

// hex(0x0803_ffff - 0xd000 + 1) (stm32f091rc rm0091 table 5, flash memory organization)
uint32_t* g_hh_tilemap_rom = 0x08033000;

UART_HandleTypeDef huart2 = {
	.Instance = USART2,
	.Init.BaudRate = 115200,
	.Init.WordLength = UART_WORDLENGTH_8B,
	.Init.StopBits = UART_STOPBITS_1,
	.Init.Parity = UART_PARITY_NONE,
	.Init.Mode = UART_MODE_TX_RX,
	.Init.HwFlowCtl = UART_HWCONTROL_NONE,
	.Init.OverSampling = UART_OVERSAMPLING_16,
	.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE,
	.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT,
};

GPIO_InitTypeDef spi_gpio = {
	.Pin = HH_IO_SPI_PINS,
	.Mode = GPIO_MODE_AF_PP,
	.Pull = GPIO_NOPULL,
	.Speed = GPIO_SPEED_FREQ_HIGH,
	.Alternate = GPIO_AF0_SPI1,
};

SPI_HandleTypeDef hspi1 = {
	.Instance = SPI1,
	.Init.Mode = SPI_MODE_MASTER,
	.Init.Direction = SPI_DIRECTION_1LINE,
	.Init.DataSize = SPI_DATASIZE_8BIT,
	.Init.CLKPolarity = SPI_POLARITY_LOW,
	.Init.CLKPhase = SPI_PHASE_1EDGE,
	.Init.NSS = SPI_NSS_SOFT,
	.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16,
	.Init.FirstBit = SPI_FIRSTBIT_MSB,
	.Init.TIMode = SPI_TIMODE_DISABLE,
	.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE,
	.Init.CRCPolynomial = 7,
	.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE,
	.Init.NSSPMode = SPI_NSS_PULSE_DISABLE,
};

TIM_HandleTypeDef htim3 = {
	.Instance = TIM3,
	.Init.Prescaler = 7999,
	.Init.CounterMode = TIM_COUNTERMODE_UP,
	.Init.Period = 65535,
	.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
	.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE,
};

static void hh_io_spi_setup();
static void hh_io_tim_setup();
static void hh_io_usart2_setup();
static void hh_io_gpio_setup();
static void hh_io_clock_setup();
static void hh_io_setup_error_handler();

void hh_setup() {
	HAL_Init();

	hh_io_clock_setup();
	hh_io_usart2_setup();
	hh_io_gpio_setup();
	hh_io_spi_setup();
	hh_io_tim_setup();

	hh_ppu_init();
}

void hh_exit() {
	hh_ppu_deinit();

	HAL_DeInit();
}

void hh_io_clock_setup() {
	if (HAL_OK != HAL_RCC_OscConfig(&(RCC_OscInitTypeDef){
		.OscillatorType = RCC_OSCILLATORTYPE_HSI,
		.HSIState = RCC_HSI_ON,
		.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT,
		.PLL.PLLState = RCC_PLL_ON,
	})) return hh_io_setup_error_handler();

	// cpu, ahb & apb clocks
	if (HAL_OK != HAL_RCC_ClockConfig(&(RCC_ClkInitTypeDef){
		.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1,
		.SYSCLKSource = RCC_SYSCLKSOURCE_HSI,
		.AHBCLKDivider = RCC_SYSCLK_DIV1,
		.APB1CLKDivider = RCC_HCLK_DIV1,
	}, FLASH_LATENCY_1)) return hh_io_setup_error_handler();

	// usart2 clock (usb serial)
	if (HAL_RCCEx_PeriphCLKConfig(&(RCC_PeriphCLKInitTypeDef){
		.PeriphClockSelection = RCC_PERIPHCLK_USART2,
		.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1,
		.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI,
	}) != HAL_OK) return hh_io_setup_error_handler();
}

void hh_io_spi_setup() {
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
		return hh_io_setup_error_handler();
}

void hh_io_usart2_setup() {
	if (HAL_UART_Init(&huart2) != HAL_OK)
		return hh_io_setup_error_handler();
}

void hh_io_tim_setup() {
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
		return hh_io_setup_error_handler();

	if (HAL_TIM_ConfigClockSource(&htim3, &(TIM_ClockConfigTypeDef) {
		.ClockSource = TIM_CLOCKSOURCE_INTERNAL
	}) != HAL_OK) return hh_io_setup_error_handler();

	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &(TIM_MasterConfigTypeDef) {
		.MasterOutputTrigger = TIM_TRGO_RESET,
		.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE,
	}) != HAL_OK) return hh_io_setup_error_handler();
}

void hh_io_gpio_setup() {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
		.Pin = GPIO_PIN_9,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_LOW,
	});
	HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
		.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8,
		.Mode = GPIO_MODE_INPUT,
		.Pull = GPIO_PULLDOWN,
	});
}

void HAL_MspInit() {
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi) {
  if(hspi->Instance != SPI1) return;

	__HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init(HH_IO_SPI_PORT, &spi_gpio);
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi) {
  if(hspi->Instance != SPI1) return;

	__HAL_RCC_SPI1_CLK_DISABLE();

	HAL_GPIO_DeInit(HH_IO_SPI_PORT, HH_IO_SPI_PINS);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base) {
  if(htim_base->Instance != TIM3) return;

	__HAL_RCC_TIM3_CLK_ENABLE();
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base) {
  if(htim_base->Instance != TIM3) return;

	__HAL_RCC_TIM3_CLK_DISABLE();
}

void SysTick_Handler() {
	HAL_IncTick();
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
		xPortSysTickHandler();
}

void HardFault_Handler() {
	for(;;);
}

void hh_io_setup_error_handler() {
	__disable_irq();
	while (1);
}
