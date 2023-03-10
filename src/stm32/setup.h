#pragma once

#include <stm32f0xx_hal_spi.h>
#include <stm32f0xx_hal_uart.h>
#include <stm32f0xx_hal_tim.h>
#include <stm32f0xx_hal_gpio.h>

#define HH_IO_SPI_PINS (GPIO_PIN_5 | GPIO_PIN_7)
#define HH_IO_SPI_PORT GPIOA

extern UART_HandleTypeDef huart2; // NOLINT
extern GPIO_InitTypeDef spi_gpio; // NOLINT
extern SPI_HandleTypeDef hspi1; // NOLINT
extern TIM_HandleTypeDef htim3; // NOLINT

// required HAL setup functions
void HAL_MspInit(); // NOLINT
void HAL_UART_MspInit(UART_HandleTypeDef *huart); // NOLINT
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi); // NOLINT
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi); // NOLINT
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base); // NOLINT
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base); // NOLINT
void HardFault_Handler(); // NOLINT
void SysTick_Handler(); // NOLINT
