#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_spi.h>
#include <stm32f0xx_hal_uart.h>
#include <stm32f0xx_hal_gpio.h>
#include <FreeRTOS.h>
#include <task.h>

#include "main.h"
#include "setup.h"
#include "demo.h"
#include "ppu/ppu.h"
#include "stm32/consts.h"

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
	.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2,
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
	hh_demo_setup();
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

static void gpio_init(GPIO_TypeDef* port, uint16_t pin, uint32_t mode, uint32_t pull) {
	HAL_GPIO_Init(port, &(GPIO_InitTypeDef) {
		.Pin = pin,
		.Mode = mode,
		.Pull = pull,
		.Speed = GPIO_SPEED_FREQ_HIGH,
	});
}

void hh_io_gpio_setup() {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

	// SPI reset line
	gpio_init(HH_IO_SPI_SR_PORT, HH_IO_SPI_SR_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);

	// PPU HBLANK/VBLANK
	gpio_init(HH_IO_PPU_HBLANK_PORT, HH_IO_PPU_HBLANK_PIN, GPIO_MODE_INPUT, GPIO_NOPULL);
	gpio_init(HH_IO_PPU_VBLANK_PORT, HH_IO_PPU_VBLANK_PIN, GPIO_MODE_IT_RISING, GPIO_NOPULL);
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

	// gamepad 1
	gpio_init(HH_IO_GP1_UP_PORT, HH_IO_GP1_UP_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	gpio_init(HH_IO_GP1_DOWN_PORT, HH_IO_GP1_DOWN_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	gpio_init(HH_IO_GP1_LEFT_PORT, HH_IO_GP1_LEFT_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	gpio_init(HH_IO_GP1_RIGHT_PORT, HH_IO_GP1_RIGHT_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	gpio_init(HH_IO_GP1_PRIMARY_PORT, HH_IO_GP1_PRIMARY_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	gpio_init(HH_IO_GP1_SECONDARY_PORT, HH_IO_GP1_SECONDARY_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN);

	// gamepad 2
	gpio_init(HH_IO_GP2_UP_PORT, HH_IO_GP2_UP_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	gpio_init(HH_IO_GP2_DOWN_PORT, HH_IO_GP2_DOWN_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	gpio_init(HH_IO_GP2_LEFT_PORT, HH_IO_GP2_LEFT_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	gpio_init(HH_IO_GP2_RIGHT_PORT, HH_IO_GP2_RIGHT_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	gpio_init(HH_IO_GP2_PRIMARY_PORT, HH_IO_GP2_PRIMARY_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN);
	gpio_init(HH_IO_GP2_SECONDARY_PORT, HH_IO_GP2_SECONDARY_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN);
}

void HAL_MspInit() {
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
	HAL_NVIC_SetPriority(PendSV_IRQn, 3, 0);
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
	__HAL_RCC_GPIOA_CLK_DISABLE();

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

void EXTI4_15_IRQHandler() {
	HAL_GPIO_EXTI_IRQHandler(HH_IO_PPU_HBLANK_PIN);
	HAL_GPIO_EXTI_IRQHandler(HH_IO_PPU_VBLANK_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == HH_IO_PPU_HBLANK_PIN) {
		g_hh_hcount++;
	}
	if (GPIO_Pin == HH_IO_PPU_VBLANK_PIN) {
		g_hh_hcount = 0;
		g_hh_vcount++;
		hh_ppu_vblank_interrupt();
	}
}
