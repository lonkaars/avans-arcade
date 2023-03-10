#pragma once

// #include <math.h>

typedef struct {
    u_int32_t x,y;//fixed point at decimal 7lsb (world positions in pixels (with fixed decimal point))
} vec2;

typedef vec2 vec_cen;//centered
typedef vec2 vec_cor;//left upper corner

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(N,LOWER,UPPER) (MIN(MAX(LOWER, N), UPPER))
