#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "ppu/ppu.h"
#include "ppusim/sim.h"

SDL_Window *g_hh_window = NULL;
SDL_Renderer *g_hh_renderer = NULL;

static void pixel(SDL_Renderer* r, unsigned px, unsigned py, unsigned cr, unsigned cg, unsigned cb) {
	SDL_SetRenderDrawColor(r, cr, cg, cb, 255);
	SDL_RenderFillRect(r, &(SDL_Rect) {
		.x = px * HH_PPUSIM_UPSCALE_FACTOR,
		.y = py * HH_PPUSIM_UPSCALE_FACTOR,
		.w = HH_PPUSIM_UPSCALE_FACTOR,
		.h = HH_PPUSIM_UPSCALE_FACTOR
	});
}

void hh_ppu_init() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	g_hh_window = SDL_CreateWindow("ppusim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, HH_PPU_SCREEN_WIDTH * HH_PPUSIM_UPSCALE_FACTOR, HH_PPU_SCREEN_HEIGHT * HH_PPUSIM_UPSCALE_FACTOR, SDL_WINDOW_SHOWN);
	g_hh_renderer = SDL_CreateRenderer(g_hh_window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Event e;
	unsigned long frame = 0;
	while (true) {
		uint32_t start = SDL_GetTicks();
		frame++;
		while (SDL_PollEvent(&e)) if (e.type == SDL_QUIT) exit(0);
		
		SDL_RenderClear(g_hh_renderer);

		for (unsigned i = 0; i < HH_PPU_SCREEN_WIDTH; i++) {
			for (unsigned j = 0; j < HH_PPU_SCREEN_HEIGHT; j++) {
				pixel(g_hh_renderer, i, j, (unsigned)(sqrt(i * 20) + frame) % 255, (i * j + frame) % 255, (j * 20) % 255);
			}
		}

		SDL_SetRenderDrawColor(g_hh_renderer, 0, 0, 0, 255);
		SDL_RenderPresent(g_hh_renderer);

		uint32_t end = SDL_GetTicks();
		int wait_for = 1000 / HH_PPUSIM_FRAMERATE - (end - start);
		if (wait_for > 0) SDL_Delay(wait_for);
	}
}

void hh_ppu_deinit() {
	SDL_DestroyRenderer(g_hh_renderer);
	SDL_DestroyWindow(g_hh_window);
	SDL_Quit();
}
