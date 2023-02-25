# src (game source files)

- the game can be built for the stm32 with fpga, or for desktop (windows and
  linux tested), by editing the `PLATFORM` variable in `makefile`
- stm-specific sources should be in the stm32 folder (setup/configuration code
  using HAL or FreeRTOS), or in `stm32.c` in any subfolder (when used as
  interface implementation)
- stm-specific sources should be declared in `stm32.mk`
- desktop-specific sources should be declared in `ds.mk`
- other source files (both platforms) should be declared in `makefile`
- for small sections of platform specific code, `#ifdef` macro's can be used.
  either `HH_TARGET_STM32` or `HH_TARGET_DESKTOP` is defined.

## directory structure

|name|description|
|apu|APU interface functions|
|lib|libraries (stm system headers, HAL, FreeRTOS)|
|ppusim|PPU simulation code (desktop only)|
|ppu|PPU interface functions|
|static|static game assets (binary files)|
|stm32|STM32 specific setup (HAL, FreeRTOS)|
