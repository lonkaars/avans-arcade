CC := arm-none-eabi-gcc
LD := arm-none-eabi-gcc
OC := arm-none-eabi-objcopy
OS := arm-none-eabi-size

SHARED_FLAGS += -DSTM32F091xC
# SHARED_FLAGS += -Wno-register
SHARED_FLAGS += -Wa,--defsym,CALL_ARM_SYSTEM_INIT=1
SHARED_FLAGS += -I./lib/STM32-base-STM32Cube/HAL/STM32F0xx/inc
SHARED_FLAGS += -I./lib/STM32-base-STM32Cube/HAL/STM32F0xx/inc/Legacy
SHARED_FLAGS += -I./lib/STM32-base-STM32Cube/CMSIS/ARM/inc
SHARED_FLAGS += -I./lib/STM32-base-STM32Cube/CMSIS/STM32F0xx/inc
SHARED_FLAGS += -I./lib/STM32-base/startup
SHARED_FLAGS += -I./lib/FreeRTOS-Kernel/include
SHARED_FLAGS += -I./lib/FreeRTOS-Kernel/portable/GCC/ARM_CM0/
SHARED_FLAGS += -I./stm32
ifeq ($(HOST),GNU/Linux)
SHARED_FLAGS += -I/usr/arm-none-eabi/include/
endif
SHARED_FLAGS += -ffunction-sections
SHARED_FLAGS += -fdata-sections
SHARED_FLAGS += -Wl,--gc-sections
SHARED_FLAGS += -mlittle-endian
SHARED_FLAGS += -mthumb
SHARED_FLAGS += -specs=nosys.specs
SHARED_FLAGS += -fno-exceptions
SHARED_FLAGS += -fno-unwind-tables
SHARED_FLAGS += -Wl,-L./lib/STM32-base/linker,-T./lib/STM32-base/linker/STM32F0xx/STM32F091xC.ld
SHARED_FLAGS += -mcpu=cortex-m0
SHARED_FLAGS += -march=armv6-m

STM_SRCS += lib/STM32-base/startup/STM32F0xx/STM32F091xC.s
STM_SRCS += lib/STM32-base-STM32Cube/CMSIS/STM32F0xx/src/system_stm32f0xx.c
STM_SRCS += lib/FreeRTOS-Kernel/croutine.c \
            lib/FreeRTOS-Kernel/event_groups.c \
            lib/FreeRTOS-Kernel/list.c \
            lib/FreeRTOS-Kernel/queue.c \
            lib/FreeRTOS-Kernel/stream_buffer.c \
            lib/FreeRTOS-Kernel/tasks.c \
            lib/FreeRTOS-Kernel/timers.c \
            lib/FreeRTOS-Kernel/portable/GCC/ARM_CM0/port.c \
            lib/FreeRTOS-Kernel/portable/MemMang/heap_4.c
STM_SRCS += lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_rcc.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_rcc_ex.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_i2c.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_i2c_ex.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_gpio.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_dma.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_cortex.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_pwr.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_pwr_ex.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_flash.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_flash_ex.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_tim.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_tim_ex.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_uart.c \
            lib/STM32-base-STM32Cube/HAL/STM32F0xx/src/stm32f0xx_hal_uart_ex.c
STM_SRCS += stm32/idle_task_static_memory.c \
						stm32/main.c

