#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>

#define UPSCALE_FACTOR 3
#define WIN_HEIGHT 240
#define WIN_WIDTH 320
#define FPS 60

static void pixel(SDL_Renderer* r, unsigned px, unsigned py, unsigned cr, unsigned cg, unsigned cb) {
		SDL_SetRenderDrawColor(r, cr, cg, cb, 255);
		SDL_RenderFillRect(r, &(SDL_Rect) {.x = px * UPSCALE_FACTOR, .y = py * UPSCALE_FACTOR, .w = UPSCALE_FACTOR, .h = UPSCALE_FACTOR});
}

int main() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	SDL_Window *window = SDL_CreateWindow("sdl2 test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH * UPSCALE_FACTOR, WIN_HEIGHT * UPSCALE_FACTOR, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Event e;
	unsigned long frame = 0;
	while (true) {
		uint32_t start = SDL_GetTicks();
		frame++;
		while (SDL_PollEvent(&e)) if (e.type == SDL_QUIT) exit(0);
		
		SDL_RenderClear(renderer);

		for (unsigned i = 0; i < WIN_WIDTH; i++) {
			for (unsigned j = 0; j < WIN_HEIGHT; j++) {
				pixel(renderer, i, j, (unsigned)(sqrt(i * 20) + frame) % 255, (i * j + frame) % 255, (j * 20) % 255);
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);

		uint32_t end = SDL_GetTicks();
		int wait_for = 1000 / FPS - (end - start);
		if (wait_for > 0) SDL_Delay(wait_for);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
