#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_uart.h>
#include <stm32f0xx.h>

void error_handler() {
	__disable_irq();
	for(;;);
}

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

void HAL_MspInit() {
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
	if (huart->Instance != USART2) return;

	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &(GPIO_InitTypeDef) {
		.Pin = GPIO_PIN_2 | GPIO_PIN_3,
		.Mode = GPIO_MODE_AF_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_LOW,
		.Alternate = GPIO_AF1_USART2,
	});
}

const uint32_t* BIG_DATA_PTR = (uint32_t*) 0x0803F800;

void setup() {
	if (HAL_UART_Init(&huart2) != HAL_OK)
		return error_handler();
}

int main() {
	setup();

	HAL_UART_Transmit(&huart2, (uint8_t*) "[", 1, 100);
	for (unsigned int i = 0; i < 4; i++) {
		uint32_t word = BIG_DATA_PTR[i];
		uint8_t* bytes = (uint8_t*) &word;
		HAL_UART_Transmit(&huart2, bytes, 4, 100);
	}
	HAL_UART_Transmit(&huart2, (uint8_t*) "]", 1, 100);

	while(1);
}
