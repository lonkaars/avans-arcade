#pragma once

#include <SDL2/SDL.h>
#include <pthread.h>

#include "ppu/consts.h"

/** @brief logical core count, used for multithreading in `hh_ppusim_draw_frame` */
extern unsigned g_hh_ppusim_core_count;
/** @brief thread handles, used for multithreading in `hh_ppusim_draw_frame` */
extern pthread_t* g_hh_ppusim_threads;

/** @brief ppusim color (24-bit RGB) */
typedef uint8_t hh_s_ppusim_color[3];
/** @brief ppusim screen scanline */
typedef hh_s_ppusim_color hh_s_ppusim_scanline[HH_PPU_SCREEN_WIDTH];
/** @brief ppusim complete screen buffer */
typedef hh_s_ppusim_scanline hh_s_ppusim_screen[HH_PPU_SCREEN_HEIGHT];

/** @brief draw `core`'s work portion to `hh_s_ppusim_screen` */
void* hh_ppusim_draw_thread(void* core);

/** @brief spawn threads to draw frame, join threads and draw to `SDL_Renderer` */
void hh_ppusim_draw_frame(SDL_Renderer*);

