#include <SDL2/SDL.h>
#include <pthread.h>
#include <stdio.h>

#include "ppusim/work.h"
#include "ppusim/sim.h"
#include "ppu/consts.h"
#include "ppusim/pixel.h"

pthread_t g_hh_ppusim_scanline_threads[HH_PPU_SCREEN_HEIGHT];
hh_s_ppusim_screen g_hh_ppusim_screen;

void* hh_ppusim_draw_scanline(void* scanline_context) {
	unsigned scanline = (unsigned long) scanline_context;
	for (unsigned x = 0; x < HH_PPU_SCREEN_WIDTH; x++)
		hh_ppusim_pixel(g_hh_ppusim_screen[scanline][x], x, scanline);
	return NULL;
}

void hh_ppusim_draw_frame(SDL_Renderer* renderer) {
	for (unsigned y = 0; y < HH_PPU_SCREEN_HEIGHT; y++)
		pthread_create(&g_hh_ppusim_scanline_threads[y], NULL, hh_ppusim_draw_scanline, (void*)(unsigned long)y);
	for (unsigned y = 0; y < HH_PPU_SCREEN_HEIGHT; y++)
		pthread_join(g_hh_ppusim_scanline_threads[y], NULL);
	
	for (unsigned x = 0; x < HH_PPU_SCREEN_WIDTH; x++) {
		for (unsigned y = 0; y < HH_PPU_SCREEN_HEIGHT; y++) {
			SDL_SetRenderDrawColor(renderer, g_hh_ppusim_screen[y][x][0], g_hh_ppusim_screen[y][x][1], g_hh_ppusim_screen[y][x][2], 255);
			SDL_RenderFillRect(renderer, &(SDL_Rect) {
				.x = x * HH_PPUSIM_UPSCALE_FACTOR,
				.y = y * HH_PPUSIM_UPSCALE_FACTOR,
				.w = HH_PPUSIM_UPSCALE_FACTOR,
				.h = HH_PPUSIM_UPSCALE_FACTOR,
			});
		}
	}
}

