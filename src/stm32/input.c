#include <stm32f0xx_hal_gpio.h>

#include "input.h"

hh_s_gamepad g_hh_controller_p1 = { 0 };
hh_s_gamepad g_hh_controller_p2 = { 0 };

void hh_input_read() {
	g_hh_controller_p1.dpad_left = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
	g_hh_controller_p1.dpad_right = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
	g_hh_controller_p1.dpad_down = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6);
	g_hh_controller_p1.dpad_up = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
}
