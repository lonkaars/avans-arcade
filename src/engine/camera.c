#include "engine/camera.h"

#include "ppu/consts.h"


vec_cor hh_update_camera(vec_cen new, vec2 min, vec2 max){

	new = vec_cen2cor(new,(vec2){.x=20,.y=30});
	static vec_cor old;

	old.x = CLAMP(new.x,min.x,max.x);
	old.y = CLAMP(new.y,min.y,max.y);

	return old;
}

