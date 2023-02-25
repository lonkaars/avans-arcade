#pragma once

#include <SDL2/SDL.h>

#include "ppu/consts.h"

extern unsigned g_hh_ppusim_core_count;
extern pthread_t* g_hh_ppusim_threads;

typedef uint8_t hh_s_ppusim_color[3];
typedef hh_s_ppusim_color hh_s_ppusim_scanline[HH_PPU_SCREEN_WIDTH];
typedef hh_s_ppusim_scanline hh_s_ppusim_screen[HH_PPU_SCREEN_HEIGHT];

void* hh_ppusim_draw_thread(void*);

void hh_ppusim_draw_frame(SDL_Renderer*);

