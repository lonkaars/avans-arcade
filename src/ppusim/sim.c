#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "main.h"
#include "ppu/ppu.h"
#include "ppusim/mem.h"
#include "ppusim/sim.h"
#include "ppusim/work.h"

SDL_Window *g_hh_window = NULL;
SDL_Renderer *g_hh_renderer = NULL;

void hh_ppu_init() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	g_hh_window = SDL_CreateWindow("ppusim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, HH_PPU_SCREEN_WIDTH * HH_PPUSIM_UPSCALE_FACTOR, HH_PPU_SCREEN_HEIGHT * HH_PPUSIM_UPSCALE_FACTOR, SDL_WINDOW_SHOWN);
	g_hh_renderer = SDL_CreateRenderer(g_hh_window, -1, SDL_RENDERER_ACCELERATED);

	g_hh_ppusim_core_count = SDL_GetCPUCount();
	g_hh_ppusim_threads = malloc(sizeof(pthread_t) * g_hh_ppusim_core_count);

	g_hh_ppusim_vram = malloc(sizeof(hh_ppu_data_t) * 0xffff);
	memset(g_hh_ppusim_vram, 0x0000, 0xffff);
}

void hh_ppu_deinit() {
	free(g_hh_ppusim_threads);
	free(g_hh_ppusim_vram);

	SDL_DestroyRenderer(g_hh_renderer);
	SDL_DestroyWindow(g_hh_window);
	SDL_Quit();
}

void hh_loop() {
	SDL_Event e;
	while (g_hh_run) {
		uint32_t start = SDL_GetTicks();
		while (SDL_PollEvent(&e)) if (e.type == SDL_QUIT) exit(0);
		
		hh_ppu_vblank_interrupt();

		SDL_RenderClear(g_hh_renderer);
		hh_ppusim_draw_frame(g_hh_renderer);
		SDL_SetRenderDrawColor(g_hh_renderer, 0, 0, 0, 255);
		SDL_RenderPresent(g_hh_renderer);

		uint32_t end = SDL_GetTicks();
		int wait_for = 1000 / HH_PPUSIM_FRAMERATE - (end - start);
		if (wait_for > 0) SDL_Delay(wait_for);
	}
}
