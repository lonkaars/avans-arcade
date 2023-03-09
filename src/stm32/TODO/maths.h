#include <math.h>

typedef struct vec2{
    float x,y;
};

/// @brief clamps x between min and max
/// @param x value to clamp
/// @param min 
/// @param max 
float clamp( float& x, float &min, float &max );
