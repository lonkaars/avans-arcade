#include <stm32f0xx_hal_gpio.h>

#include "input.h"
#include "stm32/consts.h"

hh_s_gamepad g_hh_controller_p1 = { 0 };
hh_s_gamepad g_hh_controller_p2 = { 0 };

void hh_input_read() {
	g_hh_controller_p1.dpad_up = HAL_GPIO_ReadPin(HH_IO_GP1_UP_PORT, HH_IO_GP1_UP_PIN);
	g_hh_controller_p1.dpad_down = HAL_GPIO_ReadPin(HH_IO_GP1_DOWN_PORT, HH_IO_GP1_DOWN_PIN);
	g_hh_controller_p1.dpad_left = HAL_GPIO_ReadPin(HH_IO_GP1_LEFT_PORT, HH_IO_GP1_LEFT_PIN);
	g_hh_controller_p1.dpad_right = HAL_GPIO_ReadPin(HH_IO_GP1_RIGHT_PORT, HH_IO_GP1_RIGHT_PIN);
	g_hh_controller_p1.button_primary = HAL_GPIO_ReadPin(HH_IO_GP1_PRIMARY_PORT, HH_IO_GP1_PRIMARY_PIN);
	g_hh_controller_p1.button_secondary = HAL_GPIO_ReadPin(HH_IO_GP1_SECONDARY_PORT, HH_IO_GP1_SECONDARY_PIN);

	g_hh_controller_p2.dpad_up = HAL_GPIO_ReadPin(HH_IO_GP2_UP_PORT, HH_IO_GP2_UP_PIN);
	g_hh_controller_p2.dpad_down = HAL_GPIO_ReadPin(HH_IO_GP2_DOWN_PORT, HH_IO_GP2_DOWN_PIN);
	g_hh_controller_p2.dpad_left = HAL_GPIO_ReadPin(HH_IO_GP2_LEFT_PORT, HH_IO_GP2_LEFT_PIN);
	g_hh_controller_p2.dpad_right = HAL_GPIO_ReadPin(HH_IO_GP2_RIGHT_PORT, HH_IO_GP2_RIGHT_PIN);
	g_hh_controller_p2.button_primary = HAL_GPIO_ReadPin(HH_IO_GP2_PRIMARY_PORT, HH_IO_GP2_PRIMARY_PIN);
	g_hh_controller_p2.button_secondary = HAL_GPIO_ReadPin(HH_IO_GP2_SECONDARY_PORT, HH_IO_GP2_SECONDARY_PIN);
}
