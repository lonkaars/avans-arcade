#include <SDL2/SDL.h>

#include "ppusim/sim.h"

static void hh_ppusim_draw(SDL_Renderer* r, unsigned px, unsigned py, unsigned cr, unsigned cg, unsigned cb) {
	SDL_SetRenderDrawColor(r, cr, cg, cb, 255);
	SDL_RenderFillRect(r, &(SDL_Rect) {
		.x = px * HH_PPUSIM_UPSCALE_FACTOR,
		.y = py * HH_PPUSIM_UPSCALE_FACTOR,
		.w = HH_PPUSIM_UPSCALE_FACTOR,
		.h = HH_PPUSIM_UPSCALE_FACTOR
	});
}

void hh_ppusim_pixel(SDL_Renderer* r, unsigned x, unsigned y) {
	// loop through foreground sprites to get highest priority cidx
	// get background cidx
	// check if palette color cidx of foreground is 0
	// lookup color from PAL
	// hh_ppusim_draw
}

