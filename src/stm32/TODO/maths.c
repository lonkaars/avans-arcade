#include "maths.h"

#include <math.h>

float clamp( float x, float min, float max ){
    if (x > max)
        return max;
    else if (x < min)
        return min;
    else
        return x;
}
