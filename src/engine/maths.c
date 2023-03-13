#include "engine/maths.h"

vec2 vec_add(vec2 a, vec2 b){
	return (vec2){a.x + b.x, a.y + b.y};
}

vec_cor vec_cen2cor(vec_cen in, vec2 halfDistance){
	return (vec_cor){
		.x = in.x - halfDistance.x,
		.y = in.y - halfDistance.y,
	};
}

vec_cen vec_cor2cen(vec_cor in, vec2 halfDistance){
	return (vec_cen){
		.x = in.x + halfDistance.x,
		.y = in.y + halfDistance.y,
	};
}
