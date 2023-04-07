#include <SDL2/SDL.h>
#include <pthread.h>
#include <stdio.h>

#include "ppu/consts.h"
#include "ppusim/pixel.h"
#include "ppusim/sim.h"
#include "ppusim/work.h"
#include "ppusim/mem.h"
#include "ppu/internals.h"

pthread_t *g_hh_ppusim_threads;
unsigned g_hh_ppusim_core_count;
hh_s_ppusim_screen g_hh_ppusim_screen;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"

void *hh_ppusim_draw_thread(void *arg) {
	unsigned core = (unsigned long)arg;
	for (unsigned y = core; y < HH_PPU_SCREEN_HEIGHT; y += g_hh_ppusim_core_count)
		for (unsigned x = 0; x < HH_PPU_SCREEN_WIDTH; x++) hh_ppusim_pixel(g_hh_ppusim_screen[y][x], x, y);
	return NULL;
}

void hh_ppusim_draw_frame(SDL_Renderer *renderer) {
	for (unsigned core = 0; core < g_hh_ppusim_core_count; core++) pthread_create(&g_hh_ppusim_threads[core], NULL, hh_ppusim_draw_thread, (void *)(unsigned long)core);
	for (unsigned core = 0; core < g_hh_ppusim_core_count; core++) pthread_join(g_hh_ppusim_threads[core], NULL);

	for (unsigned x = 0; x < HH_PPU_SCREEN_WIDTH; x++) {
		for (unsigned y = 0; y < HH_PPU_SCREEN_HEIGHT; y++) {
			SDL_SetRenderDrawColor(renderer, g_hh_ppusim_screen[y][x][0], g_hh_ppusim_screen[y][x][1], g_hh_ppusim_screen[y][x][2], 255);
			SDL_RenderFillRect(renderer, &(SDL_Rect){
											 .x = x * HH_PPUSIM_UPSCALE_FACTOR,
											 .y = y * HH_PPUSIM_UPSCALE_FACTOR,
											 .w = HH_PPUSIM_UPSCALE_FACTOR,
											 .h = HH_PPUSIM_UPSCALE_FACTOR,
										 });
		}
	}
}

#pragma GCC diagnostic pop
